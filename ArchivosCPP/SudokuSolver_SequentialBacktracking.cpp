#include "SudokuSolver_SequentialBacktracking.hpp"  // Incluir encabezado específico para el solucionador secuencial
#include <iostream>                                 // Incluir biblioteca de entrada/salida estándar

// Constructor del solucionador de Sudoku secuencial usando el algoritmo de backtracking
SudokuSolver_SequentialBacktracking::SudokuSolver_SequentialBacktracking(SudokuBoard& board, bool print_message /*=true*/)
: SudokuSolver(board){
    _mode = MODES::SEQUENTIAL_BACKTRACKING;
    if (print_message){
        std::cout << "\nSequential Sudoku solver using backtracking algorithm starts, please wait...\n";
    }
}

// Función para resolver el Sudoku usando backtracking
bool SudokuSolver_SequentialBacktracking::solve_kernel(){
    if (_solved) { return _solved; }  // Si el Sudoku ya está resuelto, retornar el estado resuelto
    if (_mode == MODES::SEQUENTIAL_BACKTRACKING) {
        show_progress_bar(_board, _recursionDepth);  // Mostrar barra de progreso
    }
    if (checkIfAllFilled(_board)) {  // Caso base: si todas las celdas están llenas
        _solved = true;
        _solution = _board;  // Guardar solución
        return _solved;
    }
    Position empty_cell_pos = find_empty(_board);  // Encontrar celda vacía
    int row = empty_cell_pos.first;
    int col = empty_cell_pos.second;
    for (int num = _board.get_min_value(); num <= _board.get_max_value(); ++num){
        if (isValid(_board, num, empty_cell_pos)){  // Verificar si el número es válido en la posición
            _board.set_board_data(row, col, num);  // Asignar número a la celda
            if (isUnique(_board, num, empty_cell_pos)) { 
                num = _board.get_board_size() + 1;  // Forzar salida del bucle si el número es único
            }
            if (solve_kernel()) {  // Intentar resolver la siguiente celda recursivamente
                _solved = true;
                return _solved;
            } else {
                _board.set_board_data(row, col, _board.get_empty_cell_value());  // Retroceder si no se resuelve
            }
        }
    }
    _recursionDepth++;  // Incrementar profundidad de recursión
    _solved = false;  // Si ninguno de los valores resuelve el Sudoku, marcar como no resuelto
    return _solved;
}
