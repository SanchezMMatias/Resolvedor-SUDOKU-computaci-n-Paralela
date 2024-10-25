#ifndef SUDOKUSOLVER_SEQUENTIALFORWARDCHECKING_HPP
#define SUDOKUSOLVER_SEQUENTIALFORWARDCHECKING_HPP

#include "SudokuBoard.hpp"  
#include "SudokuSolver.hpp"  

class SudokuSolver_SequentialForwardChecking : public SudokuSolver {
private:
    StateMatrix _stateMatrix;  // Matriz de estado para el Sudoku

public:
    // Constructor que inicializa el solucionador de Sudoku con forward checking secuencial
    SudokuSolver_SequentialForwardChecking(SudokuBoard& board, bool print_message=true);

    // Convierte la matriz de estado al equivalente del tablero de Sudoku resuelto
    SudokuBoard convertToSudokuGrid(StateMatrix& stateMatrix);

    // Resuelve el tablero de Sudoku dado usando el algoritmo de forward checking secuencial
    virtual void solve() override { solve_kernel(_stateMatrix); }

    // Función kernel que implementa el algoritmo de forward checking secuencial
    void solve_kernel(StateMatrix& stateMatrix);

    /* 
     * Propaga un paso.
     * 
     * @return: valor booleano que indica si la propagación cambió el estado.
     */
    bool propagate_step(StateMatrix& stateMatrix);

    /* 
     * Propaga hasta que no cambiemos más el estado.
     */
    void propagate(StateMatrix& stateMatrix);

    /* 
     * Verifica si hemos alcanzado la solución.
     */
    bool done(StateMatrix& stateMatrix);
};

#endif // SUDOKUSOLVER_SEQUENTIALFORWARDCHECKING_HPP
