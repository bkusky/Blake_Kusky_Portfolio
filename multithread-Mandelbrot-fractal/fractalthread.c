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

// thread arguments struct
struct thread_args {
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	int curr;
	int tot;
	int maxiter;
};

// lock
pthread_mutex_t lock;

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
	struct thread_args *t = args;

	int i,j;

	int width = gfx_xsize();
	int height = gfx_ysize();
	int slice = height/t->tot;
	int y_end;

	// compute slices in pixels	
	if (t->curr == (t->tot - 1)) { // if last thread
		y_end = height ;
	} else {
		y_end = slice * (t->curr + 1);
	}
	
	int y_start = slice * t->curr;
	// For every pixel i,j, in the image...

	for(j = y_start; j<y_end; j++) {
		for(i = 0; i < width; i++) {

			// Scale from pixels i,j to coordinates x,y
			double x = t->xmin + i*(t->xmax-t->xmin)/width;
			double y = t->ymin + j*(t->ymax-t->ymin)/slice - (t->curr*(t->ymax-t->ymin));

			// Compute the iterations at x,y
			int iter = compute_point(x,y,t->maxiter);

			// Convert a iteration number to an RGB color.
			// (Change this bit to get more interesting colors.)
			int gray = 255 * iter / t->maxiter;
			// locking 
			pthread_mutex_lock(&lock);
			gfx_color(gray,gray,gray);

			// Plot the point on the screen.
			gfx_point(i,j);
			pthread_mutex_unlock(&lock);
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
	int width = 640;
	int height = 480;
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
		// Wait for a key or mouse click.
		int c = gfx_wait();
		double xrange = xmax - xmin;
		double yrange = ymax - ymin;

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

		// define slice for each thread and array holding each thread
		double slice = (ymax - ymin) / num_threads;
		pthread_t * thr_array[num_threads];

		for (int i = 0; i < num_threads; i++) {
			// allocating memory for thread and thread args
			struct thread_args * targs = (struct thread_args *) malloc(sizeof(struct thread_args));
			pthread_t *t = (pthread_t *) malloc(sizeof(pthread_t));
			
			// compute slices
			targs->xmin = xmin;
			targs->xmax = xmax;
			targs->ymin = i * slice + ymin;
			targs->ymax = targs->ymin + slice;

			targs->curr = i;
			targs->tot = num_threads;
			targs->maxiter = maxiter;

			// add thread to array
			thr_array[i] = t;

			// create thread
			if (pthread_create(t, NULL, compute_image, targs) != 0) {
				printf("fractalthread: failed to create thread\n");
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
