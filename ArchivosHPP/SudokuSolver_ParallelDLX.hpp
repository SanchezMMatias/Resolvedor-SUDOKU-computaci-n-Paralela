#ifndef SUDOKUSOLVER_PARALLELDANCINGLINKS_HPP
#define SUDOKUSOLVER_PARALLELDANCINGLINKS_HPP

#include "SudokuBoard.hpp"   
#include "SudokuSolver.hpp"  
#include "Node.hpp"          
#include <vector>           

class SudokuSolver_ParallelDLX : public SudokuSolver {
private:
    SudokuBoard _originalBoard;       // Tablero original de Sudoku
    CoverMatrix _coverMatrix;         // Matriz de cobertura para el Sudoku
    ColumnNode* _header;              // Nodo cabecera de la lista de DLX
    std::vector<DancingNode*> _answer; // Vector de nodos que conforman la solución
    int _numberOfColumns;             // _BOARD_SIZE * _BOARD_SIZE * _MAX_VALUE
    int _numberOfRows;                // _BOARD_SIZE * _BOARD_SIZE * _NUM_CONSTRAINTS

public:
    // Constructor que inicializa el solucionador de Sudoku paralelo con Dancing Links
    SudokuSolver_ParallelDLX(SudokuBoard& board, bool print_message=true);

    // Crea la lista de DLX a partir de la matriz de cobertura
    ColumnNode* createDLXList(CoverMatrix& coverMatrix);

    // Convierte la solución en forma de nodos de DLX a un tablero de Sudoku
    SudokuBoard convertToSudokuGrid(std::vector<DancingNode*> answer);

    // Resuelve el tablero de Sudoku dado usando el algoritmo de Dancing Links en paralelo
    virtual void solve() override { solve_kernel(0); }

    // Función de kernel para resolver el Sudoku
    void solve_kernel(int k);

    // Selecciona el nodo de columna usando una heurística
    ColumnNode* selectColumnNodeHeuristic(ColumnNode* c, int k);
};

#endif // SUDOKUSOLVER_PARALLELDANCINGLINKS_HPP
