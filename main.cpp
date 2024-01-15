#include "SDL.h"
#include <iostream>
#include <random>
#include <chrono>
#include <string> 
using namespace std;
int main(int argx, char* argv[])
{
	// -------------------------
	int displaysizex = 1920;
	int displaysizey = 1080;
	int xsize = 1920; // In Pixels
	int ysize = 1080; 
	int delay = 0; // In Ms
	int generationsize = 10000; // number of generations
	double framecap = 60.0; // The lower the framecap the greater the performance
	// -------------------------
	double surround;
	int up;
	int down;
	int left;
	int right;
	int oldgen = 0;
	int oldgen2 = 0;
	srand(time(NULL));
	random_device rd;
	mt19937 gen(rd());
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, displaysizex, displaysizey, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	double* out = new double[xsize*ysize];
	for (int y1 = 0; y1 < (xsize*ysize); y1++) {
		out[y1] = 0;
	}	
	for (int r = 0; r < round((ysize * xsize)*0.75); r++) {
		uniform_int_distribution<> distribution(0, (xsize*ysize)-1);
		out[distribution(gen)] = 1;
		continue;
	}
	SDL_RenderSetScale(renderer, floor(displaysizex /xsize), floor(displaysizey / ysize));
	auto start = chrono::high_resolution_clock::now();
	auto start2 = chrono::high_resolution_clock::now();
	for (int generation = 0; generation < generationsize; generation++) {
		double* neboard = new double[xsize * ysize];
		for (int y1 = 0; y1 < (xsize * ysize); y1++) {
			neboard[y1] = out[y1];
		}
		for (int y = 0; y < ysize; y++) {
			for (int x = 0; x < xsize; x++) {
					left = x - 1;
					right = x + 1;
					down = y + 1;
					up = y - 1;
					if (x == 0) {
						left = xsize - 1;
					}
					else if (x == xsize - 1) {
						right = 0;
					}
					if (y == 0) {
						up = ysize - 1;
					}
					else if (y == ysize - 1) {
						down = 0;
					}
					surround = out[(down * xsize) + right] + out[(down * xsize) + x] + out[(down * xsize) + left] + out[(y * xsize) + right] + out[(y * xsize) + left] + out[(up * xsize) + right] + out[(up * xsize) + x] + out[(up * xsize) + left];
					if (out[(y * xsize) + x] == 1)
					{
						if (surround < 2 || surround >3) {
							neboard[(y * xsize) + x] = 0;
						}
					}
					else {
						if (surround == 3) {
							neboard[(y * xsize) + x] = 1;
						}
					}
			}
		}
		for (int y1 = 0; y1 < (xsize * ysize); y1++) {
			out[y1] = neboard[y1];
		}
		delete[] neboard;
		auto finish = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed = finish - start;
		chrono::duration<double> elapsed2 = finish - start2;
		if (elapsed.count() > 1.0) {
			cout << to_string((generation-oldgen)/elapsed.count()) << " FPS\n";
			oldgen = generation;
			start = chrono::high_resolution_clock::now();
		}
		if (elapsed2.count() > (1.0/framecap)) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			for (int y2 = 0; y2 < ysize; y2++) {
				for (int x2 = 0; x2 < xsize; x2++) {
					SDL_SetRenderDrawColor(renderer, round(out[(y2 * xsize) + x2] * 255), round(out[(y2 * xsize) + x2] * 255), round(out[(y2 * xsize) + x2] * 255), 255);
					if (out[(y2 * xsize) + x2] > 0) { SDL_RenderDrawPoint(renderer, x2, y2); }
				}
			}
			SDL_RenderPresent(renderer);
			start2 = chrono::high_resolution_clock::now();
		}
		SDL_Delay(delay);
	}
	return 0;
}