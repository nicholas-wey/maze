#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "solver.h"

/*
 * Given a pointer to a maze_room, set its connections in all four directions based
 * on the hex value passed in.
 *
 * For example:
 *
 * 		create_room_connections(&maze[3][4], 0xb)
 *
 * 0xb is 1011 in binary, which means we have walls on all sides except the WEST.
 * This means that every connection in maze[3][4] should be set to 1, except the WEST
 * connection, which should be set to 0.
 *
 * See the handout for more details about our hexadecimal representation, as well as
 * examples on how to use bit masks to extract information from this hexadecimal
 * representation.
 *
 * Parameters:
 *	- room: pointer to the current room
 *	- hex: hexadecimal integer (between 0-15 inclusive) that represents the connections
 *		   in all four directions from the given room.
 *
 * Returns:
 *	- nothing. The connections should be saved in the maze_room struct pointed to by
 *	  the parameter (make sure to use pointers correctly!).
 */
void create_room_connections(struct maze_room *room, unsigned int hex) {
	int east = hex & 8;
	if (east != 0) {
		room->connections[EAST] = 1;
	} else {
		room->connections[EAST] = 0;
	}
	int west = hex & 4;
	if (west != 0) {
		room->connections[WEST] = 1;
	} else {
		room->connections[WEST] = 0;
	}
	int south = hex & 2;
	if (south != 0) {
		room->connections[SOUTH] = 1;
	} else {
		room->connections[SOUTH] = 0;
	}
	int north = hex & 1;
	if (north != 0) {
		room->connections[NORTH] = 1;
	} else {
		room->connections[NORTH] = 0;
	}
}

/*
 * Recursive depth-first search algorithm for solving your maze.
 * This function should also print out either every visited room as it goes, or
 * the final pruned solution, depending on whether the FULL macro is set.
 *
 * Note that you may need to add extra parameters to this function's signature,
 * depending on how you choose to store the pruned solution.
 *
 * See handout for more details, as well as a pseudocode implementation.
 *
 * Parameters:
 *	- row: row of the current room
 *	- col: column of the current room
 *	- goal_row: row of the goal room
 *	- goal_col: col of the goal room
 *	- num_rows: number of rows in the maze
 *	- num_cols: number of columns in the maze
 *	- maze: a 2d array of maze_room structs representing your maze
 *	- file: the file to write the solution to
 *
 * Returns:
 *	- 1 if the current branch finds a valid solution, 0 if no branches are valid.
 */
int dfs(int row, int col, int goal_row, int goal_col, int num_rows, int num_cols,
		struct maze_room maze[num_rows][num_cols], FILE *file) {
			struct maze_room *room;
			room = &maze[row][col];
			#ifdef FULL
			int pr1 = fprintf(file, "%d, %d\n", row, col);
			if (pr1 < 0) {
				fprintf(stderr, "File did not print successfully.");
				exit(1);
			}
			#endif
			if (row == goal_row && col == goal_col) {
				return 1;
			} else {
				room->visited = 1;
				Direction dir[4] = {NORTH, SOUTH, EAST, WEST};
				for (int i = 0; i < 4; i++) {
					int direct = dir[i];
					if (get_neighbor(num_rows, num_cols, maze, room, direct) != NULL) {
						struct maze_room *neighbor;
						neighbor = get_neighbor(num_rows, num_cols, maze, room, direct);
						if (room->connections[direct] == 0 && neighbor->visited == 0) {
							room->next = neighbor;
							if (dfs(neighbor->row, neighbor->col, goal_row, goal_col, num_rows, num_cols, maze, file)) {
								return 1;
							} else {
								#ifdef FULL
								int pr5 = fprintf(file, "%d, %d\n", row, col);
								if (pr5 < 0) {
									fprintf(stderr, "File did not print successfully.");
									exit(1);
								}
								#endif
							}
						}
					}
				}
			}
			return 0;
}

/*
 * Main function
 *
 * Parameters:
 *	- argc: the number of command line arguments - for this function 9
 *	- **argv: a pointer to the first element in the command line
 *            arguments array - for this function:
 *		      ["solver", <input maze file>, <number of rows>, <number of columns>
 *				 <output path file>, <starting row>, <starting column>,
 *				 <ending row>, <ending column>]
 *
 * Returns:
 *	- 0 if program exits correctly, 1 if there is an error
 */
int main(int argc, char **argv) {
	int start_row, start_col, num_rows, num_cols, goal_row, goal_col;
	char *maze_file;
	char *path_file;
	if (argc != 9) {
		printf("Incorrect number of arguments.\n");
		printf("./solver <input maze file> <number of rows> <number of columns>");
		printf(" <output path file> <starting row> <starting column>");
		printf(" <ending row> <ending column>\n");
		return 1;
	} else {
		maze_file = argv[1];
		num_rows = atoi(argv[2]);
		num_cols = atoi(argv[3]);
		path_file = argv[4];
		start_row = atoi(argv[5]);
		start_col = atoi(argv[6]);
		goal_row = atoi(argv[7]);
		goal_col = atoi(argv[8]);
	}
	if (num_rows <= 0 || num_cols <= 0) {
		fprintf(stderr, "Number of rows and columns must be greater than 0.\n");
		return 1;
	}
	if (!is_in_range(goal_row, goal_col, num_rows, num_cols)) {
		fprintf(stderr, "Goal location not in range.\n");
		return 1;
	}
	if (!is_in_range(start_row, start_col, num_rows, num_cols)) {
		fprintf(stderr, "Start location not in range.\n");
		return 1;
	}
	FILE *file;
	file = fopen(maze_file, "r");
	if (file == NULL) {
		fprintf(stderr, "File did not open successfully.");
		return 1;
	}
	struct maze_room maze[num_rows][num_cols];
	for (int i = 0; i < num_rows; i++) {
		for(int j = 0; j < num_cols; j++) {
			unsigned int dec;
			int fsf = fscanf(file, "%1x", &dec);
			if (fsf == EOF) {
				fprintf(stderr, "File did not scan successfully.");
				return 1;
			}
			struct maze_room *room;
			room = &maze[i][j];
			create_room_connections(room, dec);
			maze[i][j].visited = 0;
			maze[i][j].row = i;
			maze[i][j].col = j;
		}
	}
	int clo = fclose(file);
	if (clo == EOF) {
		fprintf(stderr, "Maze file did not close successfully.");
		return 1;
	}
	FILE *file2;
	file2 = fopen(path_file, "w");
	if (file2 == NULL) {
		fprintf(stderr, "File did not open successfully.");
		return 1;
	}
	#ifdef FULL
	int pr6 = fprintf(file2, "%s\n", "FULL");
	if (pr6 < 0) {
		fprintf(stderr, "File did not print successfully.");
		return 1;
	}
	#endif
	int ret = dfs(start_row, start_col, goal_row, goal_col, num_rows, num_cols, maze, file2);
	if (ret == 0) {
		fprintf(stderr, "No solution found.");
		return 1;
	}
	#ifndef FULL
	int pr2 = fprintf(file2, "%s\n", "PRUNED");
	if (pr2 < 0) {
		fprintf(stderr, "File did not print successfully.");
		return 1;
	}
	int i = start_row;
	int j = start_col;
	struct maze_room cur_room = maze[i][j];
	while(i != goal_row || j != goal_col) {
		int pr3 = fprintf(file2, "%d, %d\n", i, j);
		if (pr3 < 0) {
			fprintf(stderr, "File did not print successfully.");
			return 1;
		}
		i = cur_room.next->row;
		j = cur_room.next->col;
		cur_room = maze[i][j];
	}
	int pr4 = fprintf(file2, "%d, %d\n", goal_row, goal_col);
	if (pr4 < 0) {
		fprintf(stderr, "File did not print successfully.");
		return 1;
	}
	#endif
	int clo2 = fclose(file2);
	if (clo2 == EOF) {
		fprintf(stderr, "Solution file did not close successfully.");
		return 1;
	}
	return 0;
}
