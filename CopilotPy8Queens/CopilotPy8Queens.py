import numpy as np
import time

# Function to check if a position is safe for the queen
def is_safe(board, row, col):
    for i in range(col):
        if board[row][i] == 1:
            return False

    for i, j in zip(range(row, -1, -1), range(col, -1, -1)):
        if board[i][j] == 1:
            return False

    for i, j in zip(range(row, len(board), 1), range(col, -1, -1)):
        if board[i][j] == 1:
            return False
    return True
 
# Function to solve the N-Queens problem using backtracking
def solve_nqueens(board, col, solutions):
    if col >= len(board):
        solutions.append(np.copy(board))
        return
    for i in range(len(board)):
        if is_safe(board, i, col):
            board[i][col] = 1
            solve_nqueens(board, col + 1, solutions)
            board[i][col] = 0
 
# Function to print the solution
def print_solution(board):
    for row in board:
        print(" ".join(str(x) for x in row))
    print()
 
# Main function to solve the 8-Queens problem and measure time
def solve_8queens():
    N = 8
    board = np.zeros((N, N), dtype=int)
    solutions = []
 
    start_time = time.time()
    solve_nqueens(board, 0, solutions)
    end_time = time.time()
 
    elapsed_time = end_time - start_time
    num_solutions = len(solutions)
 
    print(f"Time taken: {elapsed_time} seconds")
    print(f"Number of solutions found: {num_solutions}")
 
    # Print the first three solutions
    for i in range(min(3, num_solutions)):
        print(f"Solution {i + 1}:")
        print_solution(solutions[i])
 
# Solve the 8-Queens problem and print the results
solve_8queens()

"""
Time taken: 0.03397417068481445 seconds
Number of solutions found: 92
Solution 1:
1 0 0 0 0 0 0 0
0 0 0 0 0 0 1 0
0 0 0 0 1 0 0 0
0 0 0 0 0 0 0 1
0 1 0 0 0 0 0 0
0 0 0 1 0 0 0 0
0 0 0 0 0 1 0 0
0 0 1 0 0 0 0 0

Solution 2:
1 0 0 0 0 0 0 0
0 0 0 0 0 0 1 0
0 0 0 1 0 0 0 0
0 0 0 0 0 1 0 0
0 0 0 0 0 0 0 1
0 1 0 0 0 0 0 0
0 0 0 0 1 0 0 0
0 0 1 0 0 0 0 0

Solution 3:
1 0 0 0 0 0 0 0
0 0 0 0 0 1 0 0
0 0 0 0 0 0 0 1
0 0 1 0 0 0 0 0
0 0 0 0 0 0 1 0
0 0 0 1 0 0 0 0
0 1 0 0 0 0 0 0
0 0 0 0 1 0 0 0

Press any key to continue . . .

"""