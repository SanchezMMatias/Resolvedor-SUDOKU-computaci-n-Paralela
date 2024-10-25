#ifndef SUDOKUSOLVER_SEQUENTIALBACKTRACKING_HPP
#define SUDOKUSOLVER_SEQUENTIALBACKTRACKING_HPP

#include "SudokuBoard.hpp"   
#include "SudokuSolver.hpp"  

// Clase SudokuSolver_SequentialBacktracking que hereda de SudokuSolver
class SudokuSolver_SequentialBacktracking : public SudokuSolver {
public:
    // Constructor que inicializa el solucionador de Sudoku con backtracking secuencial
    SudokuSolver_SequentialBacktracking(SudokuBoard& board, bool print_message=true);

    // Resuelve el tablero de Sudoku dado usando el algoritmo de backtracking secuencial
    virtual void solve() { solve_kernel(); }

    // Función kernel que implementa el algoritmo de backtracking secuencial
    bool solve_kernel();
};

#endif // SUDOKUSOLVER_SEQUENTIALBACKTRACKING_HPP
