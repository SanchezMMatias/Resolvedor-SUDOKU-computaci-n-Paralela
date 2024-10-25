#ifndef SUDOKUSOLVER_PARALLELBRUTEFORCE_HPP
#define SUDOKUSOLVER_PARALLELBRUTEFORCE_HPP

#include "SudokuBoard.hpp"     
#include "SudokuSolver.hpp"       
#include "SudokuBoardDeque.hpp"  

// Clase SudokuSolver_ParallelBruteForce que hereda de SudokuSolver
class SudokuSolver_ParallelBruteForce : public SudokuSolver {
private:
    SudokuBoardDeque _board_deque;   // Deque para almacenar los tableros de Sudoku

public:
    // Constructor que inicializa el solucionador de Sudoku paralelo de fuerza bruta
    SudokuSolver_ParallelBruteForce(SudokuBoard& board, bool print_message=true);

    // Divide un problema de Sudoku en varios subproblemas más simples y los empuja al final del deque de tableros
    void bootstrap();
    void bootstrap(SudokuBoardDeque& boardDeque, int indexOfRows);

    // Resuelve el tablero de Sudoku dado usando el algoritmo de fuerza bruta paralela
    virtual void solve() override {
        /* Elige uno de los siguientes kernels para ejecutar */
        solve_kernel_1();
        // solve_kernel_2();
        // solve_bruteforce_par(_board, 0, 0);
    }

    void solve_kernel_1();  // Definición de kernel de resolución 1
    void solve_kernel_2();  // Definición de kernel de resolución 2
    void solve_bruteforce_seq(SudokuBoard& board, int row, int col);  // Resolución secuencial de fuerza bruta
    void solve_bruteforce_par(SudokuBoard& board, int row, int col);  // Resolución paralela de fuerza bruta
};

#endif // SUDOKUSOLVER_PARALLELBRUTEFORCE_HPP
