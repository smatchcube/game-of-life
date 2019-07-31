#ifndef GRID_H
#define GRID_H
#include <stdbool.h>

typedef enum {dead, live} state;

typedef struct {
	unsigned width;
	unsigned height;
	state ** states;
	state ** future_states;
} Grid;

// Initialize a Grid object
void Initialize(Grid*, unsigned height, unsigned width);

// Update an initialized grid according to the classic rules
void Update(Grid*);

// Switch state of a specific cell, leave unchanged if
// state requested is already set
void Set(Grid*, state, unsigned x, unsigned y);

// Return the state of a specific cell
bool Is_Alive(Grid*, unsigned x, unsigned y);

// Free space allocated to a grid object
void Destroy(Grid*);

#endif
