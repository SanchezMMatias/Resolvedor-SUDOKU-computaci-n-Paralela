#ifndef SUDOKUSOLVER_SEQUENTIALDANCINGLINKS_HPP
#define SUDOKUSOLVER_SEQUENTIALDANCINGLINKS_HPP

#include "SudokuBoard.hpp"   
#include "SudokuSolver.hpp"  
#include "Node.hpp"          
#include <vector>            

class SudokuSolver_SequentialDLX : public SudokuSolver {
private:
    CoverMatrix _coverMatrix;         // Matriz de cobertura para el Sudoku
    ColumnNode* _header;              // Nodo cabecera de la lista de DLX
    std::vector<DancingNode*> _answer; // Vector de nodos que conforman la solución
    int _numberOfColumns;             // _BOARD_SIZE * _BOARD_SIZE * _MAX_VALUE
    int _numberOfRows;                // _BOARD_SIZE * _BOARD_SIZE * _NUM_CONSTRAINTS

public:
    // Constructor que inicializa el solucionador de Sudoku secuencial con Dancing Links
    SudokuSolver_SequentialDLX(SudokuBoard& board, bool print_message=true);

    // Crea la lista cuádruplemente enlazada para representar la matriz de cobertura
    ColumnNode* createDLXList(CoverMatrix& coverMatrix);

    // Convierte la lista cuádruplemente enlazada al equivalente del tablero de Sudoku resuelto
    SudokuBoard convertToSudokuGrid(std::vector<DancingNode*> answer);

    // Resuelve el tablero de Sudoku dado usando el algoritmo de Dancing Links secuencial
    virtual void solve() override { solve_kernel(0); }

    // Implementación del Algoritmo X (https://en.wikipedia.org/wiki/Knuth%27s_Algorithm_X) y
    // uso de Dancing Links en la lista cuádruplemente enlazada para resolver el problema de cobertura exacta
    void solve_kernel(int k);

    // Selecciona el nodo de columna usando una heurística
    ColumnNode* selectColumnNodeHeuristic(ColumnNode* c, int k);
};

#endif // SUDOKUSOLVER_SEQUENTIALDANCINGLINKS_HPP
