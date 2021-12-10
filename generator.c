#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "generator.h"

/*
 * Given a direction, get its opposite
 *
 * Parameters:
 *	- dir: direction
 *
 * Returns:
 *	- the opposite direction to dir
 */
Direction get_opposite_dir(Direction dir) {
	if (dir == NORTH) {
		return SOUTH;
	}
	if (dir == SOUTH) {
		return NORTH;
	}
	if (dir == EAST) {
		return WEST;
	} else {
		return EAST;
	}
}

/*
 * Given an array of the four directions, randomizes the order of the directions
 *
 * Parameters:
 *	- directions: an array of Direction enums of size 4
 *
 * Returns:
 *	- nothing - the array should be shuffled in place
 */
void shuffle_array(Direction directions[]) {
	for (int i = 0; i < 4; i++) {
		int random_int = (rand() % (4-i)) + i;
		int tmp;
		tmp = directions[i];
		directions[i] = directions[random_int];
		directions[random_int] = tmp;
	}
}

/*
 * Recursive function for stepping through a maze and setting its connections/walls using
 * the drunken_walk algorithm
 *
 * Parameters:
 * 	- row: row of the current room
 *	- col: column of the current room
 *	- num_rows: number of rows in the maze
 *	- num_cols: number of columns in the maze
 *	- maze: a 2D array of maze_room structs representing your maze
 * Returns:
 *	- nothing - the state of your maze should be saved in the maze array being passed
 *	  in (make sure to use pointers correctly!).
 */
void drunken_walk(int row, int col, int num_rows, int num_cols,
	struct maze_room maze[num_rows][num_cols]) {
	  struct maze_room *room;
	  room = &maze[row][col];
	  room->visited = 1;
	  Direction dir[4] = {NORTH, SOUTH, EAST, WEST};
	  shuffle_array(dir);
	  for (int i = 0; i < 4; i++) {
	  	int direct;
	  	direct = dir[i];
		  if (get_neighbor(num_rows, num_cols, maze, room, direct) == NULL) {
			  room->connections[direct] = 1;
		  } else {
			  struct maze_room neighbor = *get_neighbor(num_rows, num_cols, maze, room, direct);
			  if (neighbor.visited != 1) {
				  room->connections[direct] = 0;
				  drunken_walk(neighbor.row, neighbor.col, num_rows, num_cols, maze);
		  	} else {
				  int op_dir;
				  op_dir = get_opposite_dir(direct);
				  int con;
				  con = neighbor.connections[op_dir];
				  if (con == 0 || con == 1) {
					  room->connections[direct] = con;
				  } else {
					  room->connections[direct] = 1;
				  }
			  }
		  }
	  }
}

/*
 * Represents a maze_room struct as a hexadecimal number based on its connections
 *
 * Parameters:
 *	- room: a struct maze_room to be converted to a hexadecimal number
 *
 * Returns:
 *	- the hexadecimal representation of room
 */
int encode_room(struct maze_room room) {
	int dec;
	dec = room.connections[EAST] * 8 + room.connections[WEST] * 4 +
	  room.connections[SOUTH] * 2 + room.connections[NORTH] * 1;
	return dec;
}

/*
 * Represents a maze_room array as a hexadecimal array based on its connections
 *
 * Parameters:
 *	- num_rows: number of rows in the maze
 *	- num_cols: number of columns in the maze
 *	- maze: a 2D array of maze_room structs representing the maze
 *	- result: a 2D array of hexadecimal numbers representing the maze
 *
 * Returns:
 *	- nothing - each maze_room in the maze should be represented
 *	  as a hexadecimal number  and put in nums at the corresponding index
 */
void encode_maze(int num_rows, int num_cols, struct maze_room maze[num_rows][num_cols],
	int result[num_rows][num_cols]) {
	  for (int i = 0; i < num_rows; i++) {
	  	for (int j = 0; j < num_cols; j++) {
	  		int hex;
			  hex = encode_room(maze[i][j]);
			  result[i][j] = hex;
		  }
	  }
}

/*
 * Main function
 *
 * Parameters:
 *	- argc: the number of command line arguments - for this function 4
 *	- **argv: a pointer to the first element in the command line
 *            arguments array - for this function:
 *		      ["generator", <output file>, <number of rows>, <number of columns>]
 *
 * Returns:
 *	- 0 if program exits correctly, 1 if there is an error
 */
int main(int argc, char **argv) {
	srand(time(NULL));
	char *file_name;
	int num_rows;
	int num_cols;
	if (argc != 4) {
		printf("Incorrect number of arguments.\n");
		printf("./generator <output file> <number of rows> <number of columns>\n");
		return 1;
	} else {
		file_name = argv[1];
		num_rows = atoi(argv[2]);
		num_cols = atoi(argv[3]);
	}
	if (num_rows <= 0 || num_cols <= 0) {
		fprintf(stderr, "Number of rows and columns must be greater than 0.\n");
		return 1;
	}
	struct maze_room maze[num_rows][num_cols];
	for (int i = 0; i < num_rows; i++) {
		for (int j = 0; j < num_cols; j++) {
			for (int k = 0; k < 4; k++) {
			  maze[i][j].connections[k] = -1;
			}
			maze[i][j].visited = 0;
			maze[i][j].row = i;
			maze[i][j].col = j;
		}
	}
	drunken_walk(0, 0, num_rows, num_cols, maze);
	int result[num_rows][num_cols];
	encode_maze(num_rows, num_cols, maze, result);
	FILE *file;
	file = fopen(file_name, "w");
	if (file == NULL) {
		fprintf(stderr, "File did not open successfully.");
		return 1;
	}
	for (int i = 0; i < num_rows; i++) {
		for (int j = 0; j < num_cols; j++) {
			int pr1 = fprintf(file, "%1x", result[i][j]);
			if (pr1 < 0) {
				fprintf(stderr, "File did not print successfully.");
				return 1;
			}
		}
		int pr2 = fprintf(file, "\n");
		if (pr2 < 0) {
			fprintf(stderr, "File did not print successfully.");
			return 1;
		}
	}
	int clo = fclose(file);
	if (clo == EOF) {
		fprintf(stderr, "File did not close sucessfully.");
		return 1;
	}
	return 0;
}
