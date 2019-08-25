#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>
#include "grid.h"
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

SDL_Window * window;
SDL_Renderer * renderer;

void display(Grid *);

int main(void)
{
	SDL_Log("Initializing SDL ...");
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
				"Unable to initialize SDL: %s", SDL_GetError());
		return EXIT_FAILURE;
	}
	SDL_Log("Initialization successfull");

	atexit(SDL_Quit); // Clean up SDL when exiting

	SDL_Log("Creating a window ...");
	window = SDL_CreateWindow(
			"Game of Life",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			0
	);
	if (!window) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
				"Unable to create a window: %s", SDL_GetError());
		return EXIT_FAILURE;
	}
	SDL_Log("Window created successfully");

	SDL_Log("Creating a renderer ...");
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
				"Unable to create a renderer: %s", SDL_GetError());
		SDL_DestroyWindow(window);
		return EXIT_FAILURE;
	}
	SDL_Log("Renderer created successfully");
	SDL_Log("Game starting");

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	Grid game;
	Initialize(&game, 100, 100);

	/* Random initial state */
	srand(time(0));
	for (unsigned x = 0; x < 100; ++x) {
		for (unsigned y = 0; y < 100; ++y)
			Set(&game, rand()%2 ? dead : live, x, y);
	}
	
	bool close_requested = false;
	SDL_Event event;
	while (!close_requested) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				close_requested = true;
		}
		Update(&game);
		display(&game);
		SDL_Delay(90);
	}
	Destroy(&game);

	SDL_Log("Exiting ...");
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return EXIT_SUCCESS;
}

void display(Grid * pgrid)
{
	SDL_Rect rect;
	rect.w = rect.h = 6;
	rect.x = 0; rect.y = 0;
	for (unsigned x = 0; x < 100; ++x) {
		rect.y = 0;
		for (unsigned y = 0; y < 100; ++y) {
			if (Is_Alive(pgrid, x, y)) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(renderer, &rect);
			} else { // cell is dead
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderFillRect(renderer, &rect);
			}
			rect.y += 6;
		}
		rect.y = 0;
		rect.x += 6;
	}
	SDL_RenderPresent(renderer);
}
