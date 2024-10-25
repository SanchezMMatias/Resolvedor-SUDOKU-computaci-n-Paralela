#ifndef SUDOKUSOLVER_SEQUENTIALBRUTEFORCE_HPP
#define SUDOKUSOLVER_SEQUENTIALBRUTEFORCE_HPP

#include "SudokuBoard.hpp" 
#include "SudokuSolver.hpp" 

// Clase SudokuSolver_SequentialBruteForce que hereda de SudokuSolver
class SudokuSolver_SequentialBruteForce : public SudokuSolver {
public:
    // Constructor que inicializa el solucionador de Sudoku con el algoritmo de fuerza bruta secuencial
    SudokuSolver_SequentialBruteForce(SudokuBoard& board, bool print_message=true);

    // Resuelve el tablero de Sudoku dado usando el algoritmo de fuerza bruta secuencial
    virtual void solve() override { solve_kernel(0, 0); }

    // Función kernel que implementa el algoritmo de fuerza bruta secuencial
    void solve_kernel(int row, int col);
};

#endif // SUDOKUSOLVER_SEQUENTIALBRUTEFORCE_HPP
