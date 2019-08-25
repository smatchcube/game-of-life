#include "grid.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Initialize a Grid object
void Initialize(Grid* pgrid, unsigned height, unsigned width)
{
	pgrid->height = height;
	pgrid->width = width;
	pgrid->states = (state **) malloc((width+2) * sizeof(state *));
	for (unsigned i = 0; i < width + 2; ++i)
		pgrid->states[i] = calloc(width+2, sizeof(state));
	pgrid->future_states = (state **) malloc((width+2) * sizeof(state *));
	for (unsigned i = 0; i < width + 2; ++i)
		pgrid->future_states[i] = calloc(width+2, sizeof(state));
}
	

// Update an initialized grid according to the classic rules
void Update(Grid* pgrid)
{
	unsigned x, y, live_count;
	unsigned width = pgrid->width;
	unsigned height = pgrid->height;
	state ** states = pgrid->states;
	state ** future_states = pgrid->future_states;
	state ** temp;
	for (x = 1; x <= width; ++x) {
		for (y = 1; y <= height; ++y) {
			live_count =
				  states[x-1][y-1]
				+ states[x-1][y]
				+ states[x-1][y+1]
				+ states[x][y-1]
				+ states[x][y+1]
				+ states[x+1][y-1]
				+ states[x+1][y]
				+ states[x+1][y+1];
			if (states[x][y] == live) {
				if (live_count < 2 || live_count > 3)
					future_states[x][y] = dead;
				else
					future_states[x][y] = live;
			} else if (live_count == 3) {
				future_states[x][y] = live;
			} else {
				future_states[x][y] = dead;
			}
		}
	}
	// swap grids
	temp = states;
	pgrid->states = future_states;
	pgrid->future_states = temp;
}

// Switch state of a specific cell, leave unchanged if
// state requested is already set
void Set(Grid* pgrid, state my_state, unsigned x, unsigned y)
{
	pgrid->states[x+1][y+1] = my_state;
}


// Return the state of a specific cell
bool Is_Alive(Grid* pgrid, unsigned x, unsigned y)
{
	return (pgrid->states[x+1][y+1] == live);
}

void Destroy(Grid* pgrid)
{
	for (unsigned i = 0; i < pgrid->width + 2; ++i) {
		free(pgrid->states[i]);
		free(pgrid->future_states[i]);
	}
	free(pgrid->states);
	free(pgrid->future_states);
}
