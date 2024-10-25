#ifndef SUDOKUSOLVER_HPP
#define SUDOKUSOLVER_HPP

#include "SudokuBoard.hpp"   
#include <utility>           
using Position = std::pair<int, int>;   // Definir alias para la posición como un par de enteros

// Enumeración de los modos de solución del Sudoku
enum class MODES {
    SEQUENTIAL_BACKTRACKING,    // Modo secuencial usando algoritmo de backtracking
    SEQUENTIAL_BRUTEFORCE,      // Modo secuencial usando algoritmo de fuerza bruta
    PARALLEL_BRUTEFORCE,        // Modo paralelo (OpenMP) usando algoritmo de fuerza bruta
    SEQUENTIAL_DANCINGLINKS,    // Modo secuencial usando algoritmo de "dancing links"
    PARALLEL_DANCINGLINKS,      // Modo paralelo (OpenMP) usando algoritmo de "dancing links"
    SEQUENTIAL_FORWARDCHECKING  // Modo secuencial usando algoritmo de forward checking
};

// Clase base SudokuSolver para resolver Sudokus
class SudokuSolver {
protected:
    SudokuBoard _board;               // Tablero de Sudoku a resolver
    bool _solved = false;             // Indicador de si el Sudoku ha sido resuelto
    SudokuBoard _solution;            // Solución del Sudoku
    int _recursionDepth = 0;          // Profundidad de la recursión
    int _current_num_empty_cells;     // Número actual de celdas vacías
    MODES _mode;                      // Modo de solución

public:
    SudokuSolver(SudokuBoard& board); // Constructor

    // Verifica si el tablero de Sudoku está completamente lleno
    bool checkIfAllFilled(const SudokuBoard& board) const;

    // Verifica si una fila específica está completamente llena
    bool checkIfRowFilled(const SudokuBoard& board, int indexOfRows) const;

    // Encuentra la primera celda vacía en el tablero de Sudoku
    const std::pair<int, int> find_empty(const SudokuBoard& board);

    // Encuentra la primera celda vacía desde una fila específica
    int find_empty_from_row(const SudokuBoard& board, int indexOfRows) const;

    // Verifica si una posición específica en el tablero está vacía
    bool isEmpty(const SudokuBoard& board, int i, int j) const;

    // Verifica si un número ya existe en la fila dada
    bool isValidRow(const SudokuBoard& board, int num, Position pos) const;

    // Verifica si un número ya existe en la columna dada
    bool isValidColumn(const SudokuBoard& board, int num, Position pos) const;

    // Verifica si un número ya existe en la caja (subgrilla) dada
    bool isValidBox(const SudokuBoard& board, int num, Position pos) const;

    // Verifica si un número dado puede insertarse en una posición específica
    bool isValid(const SudokuBoard& board, int num, Position pos) const;

    // Verifica la unicidad de un número válido dado viendo la ocurrencia de pares de ese número
    // dentro de las filas y columnas vecinas en la misma caja
    bool isUnique(const SudokuBoard& board, int num, Position pos) const;

    // Función virtual pura para resolver el Sudoku (debe ser implementada por las clases derivadas)
    virtual void solve() = 0;

    // Configura el modo de solución
    void set_mode(MODES mode) { _mode = mode; }

    // Obtiene el estado de si el Sudoku ha sido resuelto
    bool get_status() const { return _solved; }

    // Obtiene la solución del Sudoku
    SudokuBoard get_solution() const { return _solution; }

    // Muestra una barra de progreso
    void show_progress_bar(SudokuBoard& board, int recursionDepth, int interval=5);

    // Destructor virtual por defecto
    virtual ~SudokuSolver() = default;
};

#endif // SUDOKUSOLVER_HPP
