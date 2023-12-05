all: fractal fthread ftask

fractal: fractal.c gfx.c
	gcc fractal.c gfx.c -pthread -g -Wall --std=c99 -lX11 -lm -o fractal

fthread: fractalthread.c gfx.c
	gcc fractalthread.c gfx.c -pthread -g -Wall -std=c99 -lX11 -lm -o fractalthread

ftask: fractaltask.c gfx.c
	gcc fractaltask.c gfx.c -pthread -g -Wall -std=c99 -lX11 -lm -o fractaltask
	

clean:
	rm fractal fractalthread fractaltask

