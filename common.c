#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

/*
 * Determines whether or not the room at [row][col] is a valid room within the maze with
 * dimensions num_rows x num_cols
 *
 * Parameters:
 *	- row: row of the current room
 *	- col: column of the current room
 *	- num_rows: number of rows in the maze
 *	- num_cols: number of columns in the maze
 *
 * Returns:
 *	- 0 if room is not in the maze, 1 if room is in the maze
 */
int is_in_range(int row, int col, int num_rows, int num_cols) {
	if ((0 <= row && row <= num_rows -1) && (0 <= col && col <= num_cols -1)) {
    return 1;
  } else {
		return 0;
	}
}

/*
 * Given a pointer to the room and a Direction to travel in, return a pointer to the room
 * that neighbors the current room on the given Direction.
 * For example: get_neighbor(&maze[3][4], EAST) should return &maze[3][5]
 *
 * Parameters:
 *	- num_rows: number of rows in the maze
 *	- num_cols: number of columns in the maze
 *	- room: pointer to the current room
 *	- dir: Direction to get the neighbor from
 *	- maze: a 2D array of maze_room structs representing your maze
* Returns:
 *	- pointer to the neighboring room
 */
struct maze_room *get_neighbor(int num_rows, int num_cols,
	struct maze_room maze[num_rows][num_cols], struct maze_room *room, Direction dir) {
		if (dir == NORTH) {
			if	(is_in_range(room->row-1, room->col, num_rows, num_cols)) {
				return &maze[room->row-1][room->col];
			} else {
				return NULL;
			}
		}
		if (dir == SOUTH) {
			if	(is_in_range(room->row+1, room->col, num_rows, num_cols)) {
				return &maze[room->row+1][room->col];
			} else {
				return NULL;
			}
		}
		if (dir == EAST) {
			if	(is_in_range(room->row, room->col+1, num_rows, num_cols)) {
				return &maze[room->row][room->col+1];
			} else {
				return NULL;
			}
		}
		if (dir == WEST) {
			if	(is_in_range(room->row, room->col-1, num_rows, num_cols)) {
				return &maze[room->row][room->col-1];
			} else {
				return NULL;
			}
		} return NULL;
}
