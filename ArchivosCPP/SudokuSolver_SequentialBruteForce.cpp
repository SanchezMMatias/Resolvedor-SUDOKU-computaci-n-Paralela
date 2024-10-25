#include "SudokuSolver_SequentialBruteForce.hpp" 
#include <iostream>                               

// Constructor del solucionador de Sudoku secuencial usando el algoritmo de fuerza bruta
SudokuSolver_SequentialBruteForce::SudokuSolver_SequentialBruteForce(SudokuBoard& board, bool print_message /*=true*/)
: SudokuSolver(board){
    _mode = MODES::SEQUENTIAL_BRUTEFORCE;
    if (print_message){
        std::cout << "\nSequential Sudoku solver using brute force algorithm starts, please wait...\n";
    }
}

// Función para resolver el Sudoku usando el algoritmo de fuerza bruta
void SudokuSolver_SequentialBruteForce::solve_kernel(int row, int col){
    if (_solved) { return; }  // Si el Sudoku ya está resuelto, retornar
    if (_mode == MODES::SEQUENTIAL_BRUTEFORCE) { 
        show_progress_bar(_board, _recursionDepth);  // Mostrar barra de progreso
    }
    int BOARD_SIZE = _board.get_board_size();
    int abs_index = row * BOARD_SIZE + col;  // Calcular el índice absoluto
    if (abs_index >= _board.get_num_total_cells()){  // Caso base: si se ha llenado todo el tablero
        _solved = true;
        _solution = _board;  // Guardar solución
        return;
    }
    int row_next = (abs_index + 1) / BOARD_SIZE;  // Calcular la siguiente fila
    int col_next = (abs_index + 1) % BOARD_SIZE;  // Calcular la siguiente columna
    if (!isEmpty(_board, row, col)){  // Si la celda no está vacía, proceder a la siguiente celda
        solve_kernel(row_next, col_next);
    } else {
        // Rellenar con todos los números posibles
        for (int num = _board.get_min_value(); num <= _board.get_max_value(); ++num){
            Position pos = std::make_pair(row, col);
            if (isValid(_board, num, pos)){  // Verificar si el número es válido en la posición
                _board.set_board_data(row, col, num);
                if (isUnique(_board, num, pos)) { 
                    num = BOARD_SIZE + 1;  // Forzar salida del bucle si el número es único
                }
                // Intentar resolver la siguiente celda recursivamente
                solve_kernel(row_next, col_next);
                _board.set_board_data(row, col, _board.get_empty_cell_value());  // Retroceder si no se resuelve
            }
        }
    }
    _recursionDepth++;  // Incrementar profundidad de recursión
}
