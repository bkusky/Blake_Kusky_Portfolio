// Blake Kusky
// project.c
// Snake game written in C with XQuartz graphics framework. 

#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "gfx.h"

typedef struct { // Struct that has the x and y direction of first node on snake
	int x_dir;
	int y_dir;
} B_dir ;

typedef struct { // Struct that has x, y, previous x, and previous y, position of a node
	int x;
	int y;
	int x_prev;
	int y_prev;
} Snake;

int start_screen(int wd, int ht);
void end_screen(int wd, int ht);
void draw_border(int x1, int y1, int x2, int y2);
void draw_block(int xc, int yc, int size, char color);
int x_polar(int rad, float angle);
int y_polar(int rad, float angle);
bool border_hit_check(int x, int y, B_dir dir, int x_size, int y_size);
bool valid_move(B_dir dir, int move);
void spawn_apple(int wd, int ht, int *apx, int *apy, Snake sn[], int count);
bool hit_apple(int sx, int sy, int ax, int ay);
void draw_snake(int ix, int iy, Snake sn[], B_dir dir, int s_sz);
bool snake_hit_check(int ix, int iy, Snake sn[], int s_sz);
void end(int *pxv, int *pyv, int *prun);
void disp_score(int wd, int count);

int main()
{
	int run = 1;
	char c;
	int wd = 500, ht = 600;
	int xp = wd/2, yp = ht - 20;
	int xv = 0, yv = -10;
	const int bsize = 10;
	bool hit;
	B_dir direction;
	direction.x_dir = 0;
	direction.y_dir = -1;
	int pausetime;
	int move;
	int board_size = (wd/10) * (wd/10);
	Snake s_arr[board_size]; // array of structs with data about each node
	int num_sn[board_size];
	memset(num_sn, 0, board_size*sizeof(int));
	int count = 1;
	int ax;
	int ay;
	bool s_check;
	int diff;

	gfx_open(wd+5, ht+5, "SNAKE");
	diff = start_screen(wd, ht); 
	if (diff == 1) { // difficulty settings
		pausetime = 300000;
	} else if (diff == 2) {
		pausetime = 100000;
	} else if (diff == 3) {
		pausetime = 50000;
	}

	gfx_clear_color(255, 255, 255); // makes board white
	gfx_clear();

	
	draw_border(0, ht-500, wd, ht-500);
	spawn_apple(wd, ht, &ax, &ay, s_arr, count);

	while (run) {
		gfx_clear();
		draw_border(0, ht-505, wd, ht-505);
		
		hit = border_hit_check(xp, yp, direction, wd, ht); // checks if first node hits a wall

		if (hit == true) { // if hit wall then end game
			end(&xv, &yv, &run);
			direction.x_dir = 3;
			direction.y_dir = 3;
		}
		disp_score(wd, count); // updates the scoreboard

		draw_block(xp, yp, bsize, 'b'); // updates first node position
		gfx_flush;
		usleep(pausetime);
		s_arr[0].x_prev = xp; // records first nodes previous x and y position
		s_arr[0].y_prev = yp;
		xp += xv; // updates position by velocity
		yp += yv;
		s_arr[0].x = xp; // records first nodes current x and y position
		s_arr[0].y = yp;
		
		draw_block(ax, ay, 10, 'r'); // draws apple at current ax and ay
		if (hit_apple(xp, yp, ax, ay)) {
			count++;
			spawn_apple(wd, ht, &ax, &ay, s_arr, count); // spawns new apple at random location
			num_sn[count-1] = 1;
		}

		s_check = snake_hit_check(xp, yp, s_arr, count); // checks if first node hits any other node
		if (s_check) { // if hit snake then end game
			end(&xv, &yv, &run);
			direction.x_dir = 3;
			direction.y_dir = 3;
		}


		draw_snake(xp, yp, s_arr, direction, count); // draws the rest of the snake

		if (gfx_event_waiting()) { // WSAD controls
			c = gfx_wait();
			if (c == 'w') { // w
				move = 1;
				if (valid_move(direction, move)) { // checks if valid move
					xv = 0;
					yv = -10;
					direction.y_dir = -1;
					direction.x_dir = 0;
				}
			} else if (c == 's') { // s
				move = 2;
				if (valid_move(direction, move)) { // checks if valid move
					xv = 0;
					yv = 10;
					direction.y_dir = 1;
					direction.x_dir = 0;
				}
			} else if (c == 'd') { // d
				move = 3;
				if (valid_move(direction, move)) { // checks if valid move
					xv = 10;
					yv = 0;
					direction.x_dir = 1;
					direction.y_dir = 0;
				}
			} else if (c == 'a') { // a
				move = 4;
				if (valid_move(direction, move)) { // checks if valid move
					xv = -10;
					yv = 0;
					direction.x_dir = -1;
					direction.y_dir = 0;
				}
			}
		}
	}

	end_screen(wd, ht); // displays end screen



	return 0;
}

void draw_border(int x1, int y1, int x2, int y2) { // draws top border
	gfx_color(0, 0, 0);
	gfx_line(x1, y1, x2, y2);
}

void draw_block(int xc, int yc, int size, char c) {
	// Draws a block using polar coordinates at x and y with color c
	float angle = M_PI / 2;
	float offset = M_PI / 4;
	int rad = size / 2;
	int x1 = xc + x_polar(rad, offset);
	int y1 = yc + y_polar(rad, offset);
	int x2 = xc + x_polar(rad, angle + offset);
	int y2 = yc + y_polar(rad, angle + offset);

	int i;
	if (c == 'b') {
		gfx_color(0, 0, 255);
	} else if (c == 'r') {
		gfx_color(255, 0, 0);
	}
	
	for (i = 0; i <= 4; i++) {
		gfx_line(x1, y1, x2, y2);
		x1 = x2;
		y1 = y2;

		x2 = xc + x_polar(rad, offset + (i+1) * angle);
		y2 = yc + y_polar(rad, offset + (i+1) * angle);
	}
}



int x_polar(int rad, float angle) { // x polar to cart
	return rad * cos(angle);
}

int y_polar(int rad, float angle) { // y polar to cart
	return rad * sin(angle);
}

bool border_hit_check(int x, int y, B_dir dir, int x_size, int y_size) {
	// checks if snake hits a border
	bool check = false;
	if (x <= 9 && dir.x_dir == -1) { // if direction is into the border and position is within range
		check = true;
	} else if (x >= x_size - 9 && dir.x_dir == 1) {
		check = true;
	}
	if (y <= 109 && dir.y_dir == -1) {
		check = true;
	} else if (y >= y_size - 9 && dir.y_dir == 1) {
		check = true;
	}
	return check;
}

bool valid_move(B_dir dir, int move) {
	// While snake is moving to the right, this function does not allow the user
	// hit 's'. 
	// In general, it does not let user instantly turn snake around. It would
	// be a stupid way to lose the game.
	bool check = true;
	switch (move) {
		case 1:
			if (dir.y_dir == 1 || dir.y_dir == 3) {
				check = false;
			}
			break;
		case 2:
			if (dir.y_dir == -1 || dir.y_dir == 3) {
				check = false;
			}
			break;
		case 3:
			if (dir.x_dir == -1 || dir.x_dir == 3) {
				check = false;
			}
			break;
		case 4:
			if (dir.x_dir == 1 || dir.x_dir == 3) {
				check = false;
			}
			break;
	}
	return check;
}

void spawn_apple(int wd, int ht, int *apx, int *apy, Snake sn[], int count) {
	// Spawns apple at a random location
	srand(time(0));
	int xlower = 5;
	int xupper = wd-10;
	int yupper = ht-10;
	int ylower = 105;
	int x;
	int y;
	int i;
	int run = 1;

	while (run) {
		while (1) { // makes sure that the random location is on an interger divisible by 10
			x = (rand() % (xupper-xlower + 1)) + xlower;
			y = (rand() % (yupper-ylower + 1)) + ylower;
			if (x % 10 == 0 && y % 10 == 0) {
				break;
			}
		}
		for (i = 0; i < count; i++) { // makes sure the apple does not spawn within the snake
			run = 0;
			if (sn[i].x == x && sn[i].y == y) {
				run = 1;
				break;
			}
		}
	}
	draw_block(x, y, 10, 'r');
	*apx = x;
	*apy = y;
}

void draw_snake(int ix, int iy, Snake sn[], B_dir dir, int s_sz) {
	// draws the rest of the snake based off the first node
	int i;
	s_sz += 2;
	
	for (i = 1; i < s_sz - 1; i++) {
		// updates every previous nodes position based off the position of the node
		// in front of it.
		sn[i].x = sn[i-1].x_prev;			
		sn[i].y = sn[i-1].y_prev;
	}
	
	for (i = 0; i < s_sz; i++) {
		// updates every nodes previous position so that it can be used later
		sn[i].x_prev = sn[i].x;
		sn[i].y_prev = sn[i].y;
		draw_block(sn[i].x, sn[i].y, 10, 'b'); // draws the blocks that make up the snake
	}
}

	
bool hit_apple(int sx, int sy, int ax, int ay) {
	// checks if the first node on snake is in the same position as the apple
	if (ax == sx && ay == sy) {
		return true;
	} else {
		return false;
	}
}

bool snake_hit_check(int ix, int iy, Snake sn[], int s_sz) {
	// checks if position of first node of snake is the same as
	// any other nodes position.
	int i;
	bool check = false;
	for (i = 1; i < s_sz; i++) {
		if (ix == sn[i].x && iy == sn[i].y) {
			check = true;
			break;
		}
	}
	return check;
}

void end(int *pxv, int *pyv, int *prun) {
	// stops all motion
	*pxv = 0;
	*pyv = 0;
	*prun = 0;
	usleep(3000000);
}

void disp_score(int wd, int count) {
	// displays the score board with current count
	char str[4];
	gfx_text(wd/2 - 50, 50, "Score: ");
	sprintf(str, "%d", count);
	gfx_text(wd / 2, 50, str);
}

int start_screen(int wd, int ht) {
	// Displays start screen
	char c;
	int run = 1;
	int diff;
	while (run) {
		gfx_text(wd/2 - 50, ht/2 - 60, "Welcome to Snake!");
		gfx_text(wd/2 - 50, ht/2 - 30, "Eat the apple to grow");
		gfx_text(wd/2 - 50, ht/2, "Controls WSAD");
		gfx_text(wd/2 - 50, ht/2 + 30, "Press 'e' for easy mode");
		gfx_text(wd/2 - 50, ht/2 + 60, "Press 'r' for regular mode");
		gfx_text(wd/2 - 50, ht/2 + 90, "Press 'h' for hard mode");
		if (gfx_event_waiting()) {
			c = gfx_wait();
			if (c == 'e') {
				diff = 1;
				run = 0;
			} else if (c == 'r') {
				diff = 2;
				run = 0;
			} else if (c == 'h') {
				diff = 3;
				run = 0;
			}
		}
	}
	return diff;
}

void end_screen(int wd, int ht) {
	// Displays end screen
	char c;
	gfx_clear_color(0, 0, 0);
	gfx_clear();
	gfx_color(255, 255, 255);
	while (c != 'q') {
		gfx_text(wd/2 - 50, ht/2, "Thanks for Playing!");
		gfx_text(wd/2 - 50, ht/2 - 30, "Press 'q' to quit");
		if (gfx_event_waiting()) {
			c = gfx_wait();
		}
	}
}
