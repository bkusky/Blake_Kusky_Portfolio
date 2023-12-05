/*
fractal.c - Sample Mandelbrot Fractal Display
Starting code for CSE 30341 Project 3.
*/

#include "gfx.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <complex.h>
#include <pthread.h>
#include <stdbool.h>

#define WD 640
#define HT 400
#define BSIZE 20


// thread arguments struct
struct task {
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	int curr;
	int tot;
	int maxiter;
};

int tasks[HT/BSIZE][WD/BSIZE] = {{0}};

// lock
pthread_mutex_t write_lock;
pthread_mutex_t draw_lock;

/*
Compute the number of iterations at point x, y
in the complex space, up to a maximum of maxiter.
Return the number of iterations at that point.

This example computes the Mandelbrot fractal:
z = z^2 + alpha

Where z is initially zero, and alpha is the location x + iy
in the complex plane.  Note that we are using the "complex"
numeric type in C, which has the special functions cabs()
and cpow() to compute the absolute values and powers of
complex values.
*/

static int compute_point( double x, double y, int max )
{
	double complex z = 0;
	double complex alpha = x + I*y;

	int iter = 0;

	while( cabs(z)<4 && iter < max ) {
		z = cpow(z,2) + alpha;
		iter++;
	}

	return iter;
}

/*
Compute an entire image, writing each point to the given bitmap.
Scale the image to the range (xmin-xmax,ymin-ymax).
*/

void *compute_image(void *args)
{
	// set args
	struct task *t = args;
	int xblock = WD / BSIZE;
	int yblock = HT / BSIZE;
		
	int i,j;

	int width = gfx_xsize();
	int height = gfx_ysize();
	int x_start, y_start, x_end, y_end;

	bool exit_main_loop;
	bool exit_sub_loop;

	while (1) {
		exit_sub_loop = false;
		exit_main_loop = false;
		// find next open spot
		for (j = 0; j < yblock; j++) {
			for (i = 0; i < xblock; i++) {
				pthread_mutex_lock(&write_lock);
				if ((j == yblock - 1) && (i == xblock - 1)) {
					if (tasks[j][i] == 1) {
						exit_main_loop = true;
					}
				}

				if (tasks[j][i] == 0) {
					// update block to done
					tasks[j][i] = 1;

					// compute x start and end
					x_start = i * xblock;
					if (i == xblock - 1) { 
						x_end = width - 1;
					} else {
						x_end = i * xblock + (xblock);
					}

					// compute y start and end
					y_start = j * yblock;
					if (j == yblock - 1) {
						y_end = height;
					} else {
						y_end = j * yblock + (yblock);
					}

					exit_sub_loop = true;
					pthread_mutex_unlock(&write_lock);
					break;
				}

				pthread_mutex_unlock(&write_lock);
			}
			if (exit_sub_loop) break;
		}

		if (exit_main_loop == true) {
			break;
		}
		// For every pixel i,j, in the image...
		for(j = y_start; j<y_end; j++) {
			for(i = x_start; i < x_end; i++) {

				// Scale from pixels i,j to coordinates x,y
				double x = t->xmin + i*(t->xmax-t->xmin)/width;
				double y = t->ymin + j*(t->ymax-t->ymin)/height;

				// Compute the iterations at x,y
				int iter = compute_point(x,y,t->maxiter);
	
				// Convert a iteration number to an RGB color.
				// (Change this bit to get more interesting colors.)
				int r = 255 * sin(log(iter)) / sin(log(t->maxiter));
				int g = 0 * sin(log(iter)) / sin(log(t->maxiter));
				int b = 0 * sin(log(iter)) / sin(log(t->maxiter));
				// locking 
				pthread_mutex_lock(&draw_lock);
				gfx_color(r,g,b);

				// Plot the point on the screen.
				gfx_point(i,j);
				pthread_mutex_unlock(&draw_lock);
			}
		}
	}
	return 0;
}

int main( int argc, char *argv[] )
{
	// The initial boundaries of the fractal image in x,y space.

	double xmin=-1.5;
	double xmax= 0.5;
	double ymin=-1.0;
	double ymax= 1.0;
	double xrange_n;
	double yrange_n;

	// Maximum number of iterations to compute.
	// Higher values take longer but have more detail.
	int maxiter=500;

	// Open a new window.
	int width = WD;
	int height = HT;
	gfx_open(width,height,"Mandelbrot Fractal");

	// Show the configuration, just in case you want to recreate it.
	printf("coordinates: %lf %lf %lf %lf\n",xmin,xmax,ymin,ymax);

	// Fill it with a dark blue initially.
	gfx_clear_color(0,0,255);
	gfx_clear();
	int num_threads = 1;

	// Display the fractal image
	//compute_image(xmin,xmax,ymin,ymax,maxiter);
	double move_factor = 0.5;

	while(1) {
		printf("Computing with %d threads\n", num_threads);
		//compute range
		double xrange = xmax - xmin;
		double yrange = ymax - ymin;
	
		// Wait for a key or mouse click.
		int c = gfx_wait();

		// Quit if q is pressed.
		switch (c) {
			case 'q':
				exit(0);
			case 'w':
				ymin -= move_factor;
				ymax -= move_factor;
				break;
			case 'a':
				xmax -= move_factor;
				xmin -= move_factor;
				break;
			case 's':
				ymin += move_factor;
				ymax += move_factor;
				break;
			case 'd':
				xmax += move_factor;
				xmin += move_factor;
				
				break;
			case 'i': ; // zoom in
				xrange_n = xrange * 0.625;
				yrange_n = yrange * 0.625;
				
				xmin += (xrange - xrange_n) * 0.5;
				xmax -= (xrange - xrange_n) * 0.5;
				ymin += (yrange - yrange_n) * 0.5;
				ymax -= (yrange - yrange_n) * 0.5;

				move_factor = move_factor * 0.625;
				break;
			case 'o': // zoom out
				xrange_n = xrange * 1.6;
				yrange_n = yrange * 1.6;
				
				xmin += (xrange - xrange_n) * 0.5;
				xmax -= (xrange - xrange_n) * 0.5;
				ymin += (yrange - yrange_n) * 0.5;
				ymax -= (yrange - yrange_n) * 0.5;
	
				move_factor = move_factor * 1.6;
				break;
			case 'r': // run with no change
				break;
			case 49: // 1 threads
				num_threads = 1;
				continue;
			case 50: // 2 threads
				num_threads = 2;
				continue;
			case 51: // 3 threads
				num_threads = 3;
				continue;
			case 52: // 4 threads
				num_threads = 4;
				continue;
			case 53: // 5 threads
				num_threads = 5;
				continue;
			case 54: // 6 threads
				num_threads = 6;
				continue;
			case 55: // 7 threads
				num_threads = 7;
				continue;
			case 56: // 8 threads
				num_threads = 8;
				continue;				

		}

		for (int j = 0; j < HT/20; j++) {
			for (int i = 0; i < WD/20; i++) {
				tasks[j][i] = 0;
			}
		}

		// array holding each thread
		pthread_t * thr_array[num_threads];

		for (int i = 0; i < num_threads; i++) {
			// allocating memory for thread and thread args
			struct task * targs = (struct task *) malloc(sizeof(struct task));
			pthread_t *t = (pthread_t *) malloc(sizeof(pthread_t));
			
			// compute slices
			targs->xmin = xmin;
			targs->xmax = xmax;

			targs->ymin = ymin;
			targs->ymax = ymax;

			targs->curr = i;
			targs->tot = num_threads;
			targs->maxiter = maxiter;

			// add thread to array
			thr_array[i] = t;

			// create thread
			if (pthread_create(t, NULL, compute_image, targs) != 0) {
				printf("fractaltask: failed to create thread\n");
				exit(1);
			}
		}
		// join threads
		for (int i = 0; i < num_threads; i++) {
			pthread_join(*thr_array[i], NULL);
		}
		// free allocated memory
		for (int i = 0; i < num_threads; i++) {
			free(thr_array[i]);
		}
	}

	return 0;
}
