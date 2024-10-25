#ifndef SUDOKUBOARD_HPP
#define SUDOKUBOARD_HPP

#include <vector>   
#include <string>   
#include <iostream> 
#include <variant>  
#include <set>      

// Definir alias para los tipos de datos usados en el tablero y matrices de cobertura y estado
using Board = std::vector<std::vector<int>>;            // Tamaño: _BOARD_SIZE * _BOARD_SIZE
using CoverMatrix = std::vector<std::vector<int>>;      // Tamaño: (_BOARD_SIZE * _BOARD_SIZE * _MAX_VALUE) * (_BOARD_SIZE * _BOARD_SIZE * _NUM_CONSTRAINTS)
using MultiType = std::variant<int, std::set<int>>;     // Puede ser un int o un set de int
using StateMatrix = std::vector<std::vector<MultiType>>;// Tamaño: _BOARD_SIZE * _BOARD_SIZE

class SudokuBoard {
    friend class SudokuSolver; // Permitir acceso a la clase SudokuSolver
    friend class SudokuTest;   // Permitir acceso a la clase SudokuTest

private:
    Board _board_data;    // Datos del tablero
    int _BOX_SIZE;        // Tamaño de la caja (subgrilla)
    int _BOARD_SIZE;      // Tamaño del tablero
    int _MIN_VALUE = 1;   // Valor mínimo permitido en el tablero
    int _MAX_VALUE = _BOARD_SIZE; // Valor máximo permitido en el tablero
    int _NUM_CONSTRAINTS = 4;     // 4 restricciones: celda, fila, columna, caja
    int _INIT_NUM_EMPTY_CELLS;    // Número inicial de celdas vacías
    int _EMPTY_CELL_VALUE = 0;    // Valor de celda vacía
    std::string _EMPTY_CELL_CHARACTER = ".";  // Caracter de celda vacía
    int _COVER_MATRIX_START_INDEX = 1;        // Índice de inicio para la matriz de cobertura

public:
    // Devuelve un vector 2D leyendo de un archivo que contiene el tablero de Sudoku inicial en formato separado por espacios
    // (las celdas vacías están representadas por 0)
    const Board read_input(const std::string& filename);
    
    // Escribe la solución en un archivo de texto (solution.txt)
    friend void write_output(const SudokuBoard& solutionBoard);
    
    SudokuBoard() = default;   // Constructor por defecto
    SudokuBoard(const std::string& filename);  // Constructor que inicializa desde un archivo
    SudokuBoard(const SudokuBoard& anotherSudokuBoard);  // Constructor de copia

    // Funciones para establecer y obtener los datos del tablero
    void set_board_data(int row, int col, int num) { _board_data[row][col] = num; }
    int get_board_data(int row, int col) const { return _board_data[row][col]; }
    Board get_board_data() const { return _board_data; }
    int at(int row, int col) const { return _board_data[row][col]; }
    
    // Funciones para obtener las dimensiones y otros valores del tablero
    int get_box_size() const { return _BOX_SIZE; }
    int get_board_size() const { return _BOARD_SIZE; }
    int get_min_value() const { return _MIN_VALUE; }
    int get_max_value() const { return _MAX_VALUE; }
    int get_init_num_empty_cells() const { return _INIT_NUM_EMPTY_CELLS; }
    int get_empty_cell_value() const { return _EMPTY_CELL_VALUE; }
    std::string get_empty_cell_character() const { return _EMPTY_CELL_CHARACTER; }
    
    int get_num_total_cells() const;
    int get_num_empty_cells() const;
    
    std::vector<int> getNumbersInRow(int indexOfRows) const;
    std::vector<int> getNumbersInCol(int indexOfColumns) const;

    // Operador de asignación de copia
    SudokuBoard& operator= (const SudokuBoard& another_sudokuboard);

    // Imprime el tablero de Sudoku de entrada
    friend std::ostream& operator<< (std::ostream &out, const SudokuBoard& board);

    // Imprime el tablero de Sudoku de salida
    friend void print_board(const SudokuBoard& board);

    // Transforma el problema de Sudoku en una instancia del problema de cobertura exacta, es decir,
    // modela una cuadrícula de Sudoku en forma de matriz de cobertura con tamaño:
    // (_BOARD_SIZE * _BOARD_SIZE * _MAX_VALUE) * (BOARD_SIZE * _BOARD_SIZE * _NUM_CONSTRAINTS)
    int indexInCoverMatrix(int row, int col, int num);
    int createBoxConstraints(CoverMatrix& coverMatrix, int header);
    int createColumnConstraints(CoverMatrix& coverMatrix, int header);
    int createRowConstraints(CoverMatrix& coverMatrix, int header);
    int createCellConstraints(CoverMatrix& coverMatrix, int header);
    
    void createCoverMatrix(CoverMatrix& coverMatrix);
    void convertToCoverMatrix(CoverMatrix& coverMatrix);

    // Agrega información de estado al tablero de Sudoku
    // reemplazando la celda vacía, es decir, 0, con un conjunto de valores posibles
    void createStateMatrix(StateMatrix& stateMatrix);
    void convertToStateMatrix(StateMatrix& stateMatrix);
};

#endif // SUDOKUBOARD_HPP
