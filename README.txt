README:

Project: Maze

----------------------------------------------------------------------------------------------

Note:

- I used the Atom text editor for this assignment. For looking over code (especially checking
style), it may be a good idea to open my files with Atom. From my experience n the past,
opening Atom-edited files in Sublime (or other text editors) can sometimes change newlines and
spacing.

----------------------------------------------------------------------------------------------

Unresolved Bugs:

- There are no unresolved bugs that I am aware of.

----------------------------------------------------------------------------------------------

Debugging Help:

- I received debugging help from the CS33 TA’s.

----------------------------------------------------------------------------------------------

How to Compile and Run:

- Navigate to the maze directory and open terminal. First, run: "make" on the command line.
This will compile all the relevant programs in the maze directory.

- To run the generator, run: "./generator <maze_file> <num_rows> <num_cols>" on the command
line where maze_file is the name of a file for the maze output to be written to, num_rows is
the number of rows in the maze, and num_cols is the number of columns in the maze.

- To run the solver with a "PRUNED" solution (i.e. no backtracking from start to end
locations), run: "./solver <maze_file> <num_rows> <num_cols> <solution_file> <start_row>
<start_col> <goal_row> <goal_col>" on the command line. maze_file is the name the maze file
outputted by the generator, num_rows is the number of rows in the maze, and num_cols is the
number of columns in the maze, solution_file is the name of a file for the solution output
to be written to, start_row is the 0-indexed row of the starting location in the maze,
start_col is the 0-indexed column of the starting location in the maze, goal_row is the
0-indexed row of the ending location in the maze, and goal_col is the 0-indexed column of
the ending location in the maze.

- To run the solver with a "FULL" solution (i.e. including backtracking from start to end),
run "./solver_full <maze_file> <num_rows> <num_cols> <solution_file> <start_row> <start_col>
<goal_row> <goal_col>" on the command line. All arguments are the same as with the "PRUNED"
solution.

- Again, the maze file and solution file outputs can be found in the maze directory after
running generator.c and solver.c, respectively.

----------------------------------------------------------------------------------------------

General Overview:

- The program has two main parts, the maze generator (generator.c) and the maze solver
(solver.c), which both share a couple methods found in common.c.

- GENERATOR: The maze generator takes in 3 arguments on the command line needed to build a
maze: a file to write to, an integer representing the number of rows, and an integer
representing the number columns. In main(), the generator first instantiates and initializes a
2D array of maze_room structs. Each struct has an integer row field, a integer column field,
a integer visited field (initially 0), a next field (a maze_room pointer), and an four-integer
array representing the room’s connections (1 for wall and 0 for open) in each direction
(initially all -1). Then the generator calls drunken_walk(), which starts at location (0,0)
and recursively traverses the maze array randomly, modifying (most importantly) the
connections array in each new maze_room it visits through the drunken walk algorithm, until
connections and walls are formed in each direction for each room, and a maze representation
is created after each room is visited. In drunken_walk(), get_neighbor() (which gets the
neighbor of a room in a certain direction and comes from common.c), shuffle_array() (which
shuffles a 4-direction array), and get_opppsite_dir() (which gets an opposite direction given
a certain direction) are all called at various points as described in the drunken walk
algorithm pseudocode. Next in main(), encode_maze() is called on an int array the same
dimensions as the maze. For each room in the maze array, encode_maze() calls encode_room, a
method that returns a integer hexadecimal number representing the connections of a
maze_room, which is then passed into the same-size 2-D integer array. And finally the result
int array is read off onto a maze .txt file as output (to be scanned by the solver). The
open file is closed and main() exits, and the maze .txt file can be found in the maze directory.

- SOLVER: The maze solver takes in multiple arguments on the command line, a maze .txt
file and its dimensions, a solution .txt named file to write to, and integers representing
starting and ending row and column locations for the solution path. In main(), the solver
first re-creates the maze data structure from the inputted maze .txt file, instantiating and
initializing a 2D array of maze_room structs. In doing so, the connections from the maze
file are read in and set for each room in the maze via create_room_connections(), as well
as the rows, columns, and the visited field. Then dfs() (depth-first search) is called,
which performs the depth-first search solver algorithm on the 2-D maze array with a given
start and end location in order to find a solution path. If the start (or current) location
is not the goal location, dfs() calls get_neighbor in order to recursively move along the
2-D array in one of the four directions, somewhat like in the generator. If a dead-end is
hit, the next direction from the last dfs() call is chosen, again performing the same steps
and recursion until the goal location or a dead-end is found. If ./solve_full is called,
dfs() records every room visited with backtracking until a solution is found, and all of this
information (i.e. the "FULL" solution) is printed onto solve text file. If ./solve is called,
a path without backtracking (i.e. the "PRUNED" solution) is recorded in main() after dfs()
executes by following next pointers from the start location room to the end location room.
These are set every time a valid new room is visited to create the non-backtracked path.
The open files are closed and main() exits, and the solve .txt file can be found in the maze
directory.

- ERROR CHECKING: Both maze generator and maze solver perform error checking mostly in main(),
for things like bad user input rows and columns, dfs() not finding a solution, or if C library
functions such as fprinf() or fscanf() do not execute properly. When such errors happen, the
program exits and an informative message is printed to stderr.

- OTHER FILES: common.h, generator.h, and solver.h are header files defining the methods in
common.c, generator.c, and solver.c, respectively. common.h also defines Direction enums to
represent the four directions (for ease of use in the other programs) and defines the maze_room
struct (referenced above). Also, in common.c, besides get_neighbor() there is is_in_range().
is_in_range() determines whether or not a room is valid within a maze, and is called by
get_neighbor() as well as in the main() methods in solver.c and generator.c for error checking.
