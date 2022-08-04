#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>

#define WIDTH 1000
#define HEIGHT 1000

//map a variable's value in a specific range
double map(double in, double minIn, double maxIn, double minOut, double maxOut){
	return minOut+(in-minIn)*((maxOut-minOut)/(maxIn-minIn));
}

//check if a point in the complex plane is in the mandelbrot set
int mandelbrot(double a, double b, int maxIterations){
	//local variable declaration
	int n = 0;
	double za = 0;
	double zb = 0;

	//iteration loop
	while(n<maxIterations){
		//printf("Iteration: %d - za=%f zb=%f\n", n, za, zb);
		if((za*za + zb*zb)>4){
			break;
		}
		double temp = (za*za - zb*zb) + a;
		zb = (2*za*zb) + b;
		za = temp;
		n++;
	}
	return n;
}

int main(int argc, char* argv[]){


	//SDL initialization stuff
	SDL_Window* win;
	SDL_Renderer* rend;
	if(SDL_Init(SDL_INIT_VIDEO)<0){ printf("error in SDL initialization!\n"); return -1; }
	win = SDL_CreateWindow("Mandelbrot Set.", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if(win<0 || rend<0){ printf("error in window or renderer initialization!\n"); return -1;}

	//variables
	bool running = true;
	bool rendernext = true;
	SDL_Event event;
	int result;

	double posX = -2;
	double posY = 2;
	double zoom = 4;

	//main loop
	while(running){
		//event handeling
		while(SDL_PollEvent(&event)!=0){
			if(event.type==SDL_QUIT){
				running = false;
			}
			if(event.type==SDL_KEYDOWN){
				rendernext=true;
				switch(event.key.keysym.sym){
					case SDLK_w:
						posY+=0.01;
						break;
					case SDLK_a:
						posX-=0.01;
						break;
					case SDLK_s:
						posY-=0.01;
						break;
					case SDLK_d:
						posX+=0.01;
						break;
					case SDLK_o:
						zoom-=(zoom/4);
						break;
					case SDLK_l:
						zoom+=(zoom/4);
						break;
				}
				printf("posX: %f posY: %f   zoom: %f\n", posX, posY, zoom);
			}
		}
		//render the fractal
		if(rendernext){
			for(int x=0;x<WIDTH;x++){
				for(int y=0;y<HEIGHT;y++){
					result = map(mandelbrot(map(x,0,WIDTH,posX,posX+zoom), map(y,0,HEIGHT,posY,posY-zoom), atoi(argv[1])), 0, atol(argv[1]), 0, 255);
					SDL_SetRenderDrawColor(rend,result, result, result, 255);
					SDL_RenderDrawPoint(rend, x,y);
				}
			}
			SDL_RenderPresent(rend);
			rendernext = false;
		}
	}

	SDL_Quit();

	return 0;
}