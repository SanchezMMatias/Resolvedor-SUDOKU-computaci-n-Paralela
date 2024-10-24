#include "SudokuBoard.hpp"
#include "helper.hpp"
#include "termcolor.hpp"
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <iomanip>  // Necesario para std::setw y formateo de salida

// Usamos el namespace std para evitar repetir std:: en cada función
using namespace std;

// Función para leer el tablero de Sudoku desde un archivo
const Board SudokuBoard::read_input(const string& filename)
{
    ifstream inputFile(filename);   // Abre el archivo de entrada

    if (!inputFile)  // Verifica si el archivo se abrió correctamente
    {
        // Muestra un mensaje de error si el archivo no se pudo abrir
        cerr << termcolor::red << "Error opening file " << filename
             << "! Please make sure the file specified exists." << termcolor::reset << "\n";
        exit(1);  // Finaliza el programa en caso de error
    }

    inputFile >> _BOARD_SIZE;  // Lee el tamaño del tablero desde el archivo
    _BOX_SIZE = sqrt(_BOARD_SIZE);  // Calcula el tamaño de las subcajas (raíz cuadrada de _BOARD_SIZE)

    // Inicializa el tablero con celdas vacías (valor 0)
    Board sudokuBoard(_BOARD_SIZE, vector<int>(_BOARD_SIZE, 0));

    int num_empty_cells = 0;  // Contador para el número de celdas vacías
    for (int row = 0; row < _BOARD_SIZE; ++row)  // Recorre las filas
    {
        for (int col = 0; col < _BOARD_SIZE; ++col)  // Recorre las columnas
        {
            int value;
            inputFile >> value;  // Lee el valor de la celda desde el archivo
            sudokuBoard[row][col] = value;  // Asigna el valor leído al tablero
            num_empty_cells += (value == 0);  // Incrementa el contador si la celda está vacía
        }
    }
    _INIT_NUM_EMPTY_CELLS = num_empty_cells;  // Guarda el número total de celdas vacías

    inputFile.close();   // Cierra el archivo

    return sudokuBoard;  // Devuelve el tablero leído
}

// Función para escribir la solución del Sudoku en un archivo de salida
void write_output(const SudokuBoard& solutionBoard)
{
    // Obtiene los datos del tablero de solución y su tamaño
    Board solution = solutionBoard.get_board_data();
    int BOARD_SIZE = solutionBoard.get_board_size();
    int BOX_SIZE = solutionBoard.get_box_size();

    ofstream outputFile("solution.txt");  // Crea un archivo de salida

    int digit = log10(BOARD_SIZE) + 1;  // Calcula el número de dígitos necesarios para la representación de los números

    for (int r = 0; r < BOARD_SIZE; ++r)  // Recorre las filas
    {
        for (int c = 0; c < BOARD_SIZE; ++c)  // Recorre las columnas
        {
            // Escribe el valor de la celda con el formato adecuado
            outputFile << setw(digit) << solution[r][c];

            if (c != BOARD_SIZE - 1)  // Añade un espacio entre valores, excepto al final de la fila
            {
                outputFile << " ";
            }

            if (c % BOX_SIZE == (BOX_SIZE - 1))  // Añade un espacio adicional después de cada subcaja
            {
                if (c != BOARD_SIZE - 1)
                {
                    outputFile << "  ";
                }
            }
        }

        if (r != BOARD_SIZE - 1)  // Añade una nueva línea después de cada fila, excepto al final del tablero
        {
            outputFile << "\n";
            if (r % BOX_SIZE == (BOX_SIZE - 1))  // Añade una línea adicional entre subcajas
            {
                outputFile << "\n";
            }
        }
    }

    outputFile.close();  // Cierra el archivo de salida
}

// Constructor de la clase SudokuBoard que carga el tablero desde un archivo
SudokuBoard::SudokuBoard(const string& filename)
    : _board_data(read_input(filename))  // Llama a la función read_input para inicializar el tablero
{
    // Muestra un mensaje indicando que se ha cargado el tablero
    cout << "Load the initial Sudoku board from "
         << termcolor::yellow << filename << termcolor::reset << "..." << "\n";
}

// Constructor de copia de la clase SudokuBoard
SudokuBoard::SudokuBoard(const SudokuBoard& anotherSudokuBoard)
    // Copia todos los atributos del objeto SudokuBoard original
    : _board_data(anotherSudokuBoard._board_data),
      _BOX_SIZE(anotherSudokuBoard._BOX_SIZE),
      _BOARD_SIZE(anotherSudokuBoard._BOARD_SIZE),
      _MIN_VALUE(anotherSudokuBoard._MIN_VALUE),
      _MAX_VALUE(anotherSudokuBoard._MAX_VALUE),
      _NUM_CONSTRAINTS(anotherSudokuBoard._NUM_CONSTRAINTS),
      _INIT_NUM_EMPTY_CELLS(anotherSudokuBoard._INIT_NUM_EMPTY_CELLS),
      _EMPTY_CELL_VALUE(anotherSudokuBoard._EMPTY_CELL_VALUE),
      _EMPTY_CELL_CHARACTER(anotherSudokuBoard._EMPTY_CELL_CHARACTER),
      _COVER_MATRIX_START_INDEX(anotherSudokuBoard._COVER_MATRIX_START_INDEX)
{ }

// Método para obtener el número total de celdas en el tablero
int SudokuBoard::get_num_total_cells() const
{
    return _BOARD_SIZE * _BOARD_SIZE;  // Calcula el número total de celdas (BOARD_SIZE^2)
}

// Método para obtener el número de celdas vacías en el tablero
int SudokuBoard::get_num_empty_cells() const
{
    int n = 0;  // Contador de celdas vacías
    for (int i = 0; i < _BOARD_SIZE; ++i)  // Recorre todas las filas
    {
        for (int j = 0; j < _BOARD_SIZE; ++j)  // Recorre todas las columnas
        {
            n += (this->at(i, j) == get_empty_cell_value());  // Incrementa el contador si la celda está vacía
        }
    }
    return n;  // Devuelve el número total de celdas vacías
}

// Método para obtener los números en una fila específica
vector<int> SudokuBoard::getNumbersInRow(int indexOfRows) const
{
    vector<int> numbersInRow;  // Almacena los números no vacíos de la fila

    for (int col = 0; col < _BOARD_SIZE; ++col)  // Recorre las columnas de la fila
    {
        int num = _board_data[indexOfRows][col];  // Obtiene el valor de la celda
        if (num == _EMPTY_CELL_VALUE) continue;  // Omite las celdas vacías
        numbersInRow.push_back(num);  // Añade el número a la lista si no está vacío
    }

    return numbersInRow;  // Devuelve los números encontrados en la fila
}

// Método para obtener los números en una columna específica
vector<int> SudokuBoard::getNumbersInCol(int indexOfColumns) const
{
    vector<int> numbersInCol;  // Almacena los números no vacíos de la columna

    for (int row = 0; row < _BOARD_SIZE; ++row)  // Recorre las filas de la columna
    {
        int num = _board_data[row][indexOfColumns];  // Obtiene el valor de la celda
        if (num == _EMPTY_CELL_VALUE) continue;  // Omite las celdas vacías
        numbersInCol.push_back(num);  // Añade el número a la lista si no está vacío
    }

    return numbersInCol;  // Devuelve los números encontrados en la columna
}

// Sobrecarga del operador de asignación para la clase SudokuBoard
SudokuBoard& SudokuBoard::operator= (const SudokuBoard& anotherSudokuBoard)
{
    if (this != &anotherSudokuBoard)  // Verifica que no sea autoasignación
    {
        // Copia todos los atributos del otro tablero de Sudoku
        _board_data = anotherSudokuBoard._board_data;
        _BOX_SIZE = anotherSudokuBoard._BOX_SIZE;
        _BOARD_SIZE = anotherSudokuBoard._BOARD_SIZE;
        _MIN_VALUE = anotherSudokuBoard._MIN_VALUE;
        _MAX_VALUE = anotherSudokuBoard._MAX_VALUE;
        _NUM_CONSTRAINTS = anotherSudokuBoard._NUM_CONSTRAINTS;
        _INIT_NUM_EMPTY_CELLS = anotherSudokuBoard._INIT_NUM_EMPTY_CELLS;
        _EMPTY_CELL_VALUE = anotherSudokuBoard._EMPTY_CELL_VALUE;
        _EMPTY_CELL_CHARACTER = anotherSudokuBoard._EMPTY_CELL_CHARACTER;
        _COVER_MATRIX_START_INDEX = anotherSudokuBoard._COVER_MATRIX_START_INDEX;
    }

    return *this;  // Devuelve el objeto actual
}

// Función para imprimir el tablero de Sudoku en consola
void print_board(const SudokuBoard& board)
{
    // Obtiene los datos del tablero
    Board grid = board._board_data;

    for (int i = 0; i < board._BOARD_SIZE; ++i)  // Recorre las filas
    {
        // Si es el inicio de una nueva subcaja horizontal, imprime una línea divisoria
        if (i % board._BOX_SIZE == 0 && i != 0)
        {
            // Crea la línea divisoria entre subcajas
            string s1 = "---";
            string s2 = s1 * board._BOX_SIZE + " + ";
            cout << s2 * (board._BOX_SIZE - 1) << s1 * board._BOX_SIZE << "\n";
        }

        for (int j = 0; j < board._BOARD_SIZE; ++j)  // Recorre las columnas
        {
            // Si es el inicio de una nueva subcaja vertical, imprime un separador
            if (j % board._BOX_SIZE == 0 && j != 0)
            {
                cout << "  | ";  // Separador entre subcajas
            }	

            // Imprime el último número de la fila
            if (j == board._BOARD_SIZE - 1)
            {
                cout << setfill(' ') << setw(2) << grid[i][j] << "\n";  // Sin espacio extra al final
            }
            // Para los bordes de las subcajas
            else if (j % board._BOX_SIZE == board._BOX_SIZE - 1)
            {
                cout << setfill(' ') << setw(2) << grid[i][j];
            }
            // Imprime los números separados por espacio
            else
            {
                cout << setfill(' ') << setw(2) << grid[i][j] << " ";
            }
        }
    }
}

// Sobrecarga del operador << para imprimir el tablero en cualquier flujo de salida (como cout o archivos)
ostream& operator<< (ostream &out, const SudokuBoard& board)
{
    // Obtiene los datos del tablero
    Board grid = board.get_board_data();
    int BOARD_SIZE = board.get_board_size();
    int BOX_SIZE = board.get_box_size();
    int EMPTY_CELL_VALUE = board.get_empty_cell_value();
    string EMPTY_CELL_CHARACTER = board.get_empty_cell_character();

    for (int i = 0; i < BOARD_SIZE; ++i)  // Recorre las filas
    {
        // Si es el inicio de una nueva subcaja horizontal, imprime una línea divisoria
        if (i % BOX_SIZE == 0 && i != 0)
        {
            // Crea la línea divisoria entre subcajas
            string s1 = "---";
            string s2 = s1 * BOX_SIZE + " + ";
            out << s2 * (BOX_SIZE - 1) << s1 * BOX_SIZE << "\n";
        }

        for (int j = 0; j < BOARD_SIZE; ++j)  // Recorre las columnas
        {
            // Si es el inicio de una nueva subcaja vertical, imprime un separador
            if (j % BOX_SIZE == 0 && j != 0)
            {
                out << "  | ";  // Separador entre subcajas
            }	

            // Si la celda está vacía, imprime el carácter especial para celdas vacías
            string forPrinting = (grid[i][j] == EMPTY_CELL_VALUE) ? EMPTY_CELL_CHARACTER : to_string(grid[i][j]);

            // Imprime el último número de la fila
            if (j == BOARD_SIZE - 1)
            {
                out << setfill(' ') << setw(2) << forPrinting << "\n";  // Sin espacio extra al final
            }
            // Para los bordes de las subcajas
            else if (j % BOX_SIZE == BOX_SIZE - 1)
            {
                out << setfill(' ') << setw(2) << forPrinting;
            }
            // Imprime los números separados por espacio
            else
            {
                out << setfill(' ') << setw(2) << forPrinting << " ";
            }
        }
    }

    return out;  // Retorna el flujo de salida para encadenar operaciones
}

// Función para calcular el índice en la matriz de cobertura basado en fila, columna y número
int SudokuBoard::indexInCoverMatrix(int row, int col, int num)
{
    // Fórmula para calcular la posición en la matriz de cobertura
    return (row - 1) * _BOARD_SIZE * _BOARD_SIZE + (col - 1) * _BOARD_SIZE + (num - 1);
}

// Función para crear restricciones de subcajas en la matriz de cobertura
int SudokuBoard::createBoxConstraints(CoverMatrix& coverMatrix, int header)
{
    // Recorre las subcajas (por bloques)
	for (int row = _COVER_MATRIX_START_INDEX; row <= _BOARD_SIZE; row += _BOX_SIZE)
    {
        for (int col = _COVER_MATRIX_START_INDEX; col <= _BOARD_SIZE; col += _BOX_SIZE)
        {
            // Recorre cada valor posible dentro de la subcaja
            for (int n = _COVER_MATRIX_START_INDEX; n <= _BOARD_SIZE; ++n, ++header)
            {
                // Recorre cada celda dentro de la subcaja
                for (int rowDelta = 0; rowDelta < _BOX_SIZE; ++rowDelta)
                {
                    for (int colDelta = 0; colDelta < _BOX_SIZE; ++colDelta)
                    {
                        // Calcula el índice y actualiza la matriz de cobertura
                        int index = indexInCoverMatrix(row + rowDelta, col + colDelta, n);
                        coverMatrix[index][header] = 1;
                    }
                }
            }
        }
    }

    return header;  // Retorna el siguiente encabezado
}

// Función para crear restricciones de columnas en la matriz de cobertura
int SudokuBoard::createColumnConstraints(CoverMatrix& coverMatrix, int header)
{
    // Recorre las columnas y valores posibles
	for (int col = _COVER_MATRIX_START_INDEX; col <= _BOARD_SIZE; ++col)
    {
        for (int n = _COVER_MATRIX_START_INDEX; n <= _BOARD_SIZE; ++n, ++header)
        {
            for (int row = _COVER_MATRIX_START_INDEX; row <= _BOARD_SIZE; ++row)
            {
                // Calcula el índice y actualiza la matriz de cobertura
                int index = indexInCoverMatrix(row, col, n);
                coverMatrix[index][header] = 1;
            }
        }
    }

    return header;
}

// Función para crear restricciones de filas en la matriz de cobertura
int SudokuBoard::createRowConstraints(CoverMatrix& coverMatrix, int header)
{
    // Recorre las filas y valores posibles
	for (int row = _COVER_MATRIX_START_INDEX; row <= _BOARD_SIZE; ++row)
    {
        for (int n = _COVER_MATRIX_START_INDEX; n <= _BOARD_SIZE; ++n, ++header)
        {
            for (int col = _COVER_MATRIX_START_INDEX; col <= _BOARD_SIZE; ++col)
            {
                // Calcula el índice y actualiza la matriz de cobertura
                int index = indexInCoverMatrix(row, col, n);
                coverMatrix[index][header] = 1;
            }
        }
    }

    return header;
}

// Función para crear restricciones de celdas en la matriz de cobertura
int SudokuBoard::createCellConstraints(CoverMatrix& coverMatrix, int header)
{
    // Recorre las celdas y valores posibles
	for (int row = _COVER_MATRIX_START_INDEX; row <= _BOARD_SIZE; ++row)
    {
        for (int col = _COVER_MATRIX_START_INDEX; col <= _BOARD_SIZE; ++col, ++header)
        {
            for (int n = _COVER_MATRIX_START_INDEX; n <= _BOARD_SIZE; ++n)
            {
                // Calcula el índice y actualiza la matriz de cobertura
                int index = indexInCoverMatrix(row, col, n);
                coverMatrix[index][header] = 1;
            }
        }
    }

    return header;
}

// Función que crea la matriz de cobertura combinando todas las restricciones
void SudokuBoard::createCoverMatrix(CoverMatrix& coverMatrix)
{
    // Calcula el tamaño de la matriz de cobertura
	int numberOfRows = _BOARD_SIZE * _BOARD_SIZE * _MAX_VALUE;
	int numberOfCols = _BOARD_SIZE * _BOARD_SIZE * _NUM_CONSTRAINTS;

    // Inicializa la matriz de cobertura con ceros
	coverMatrix.resize(numberOfRows, vector<int>(numberOfCols));

    // Llama a las funciones para agregar las restricciones
    int header = 0;
    header = createCellConstraints(coverMatrix, header);
    header = createRowConstraints(coverMatrix, header);
    header = createColumnConstraints(coverMatrix, header);
    createBoxConstraints(coverMatrix, header);
}

// Función que convierte el tablero a una matriz de cobertura, marcando celdas ya ocupadas
void SudokuBoard::convertToCoverMatrix(CoverMatrix& coverMatrix)
{	
    // Recorre cada celda del tablero
    for (int row = _COVER_MATRIX_START_INDEX; row <= _BOARD_SIZE; ++row)
    {
        for (int col = _COVER_MATRIX_START_INDEX; col <= _BOARD_SIZE; ++col)
        {
            int n = _board_data[row - 1][col - 1];

            // Si la celda no está vacía, desactiva los valores que no son válidos
            if (n != _EMPTY_CELL_VALUE)
            {
                for (int num = _MIN_VALUE; num <= _MAX_VALUE; ++num)
                {
                    if (num != n)
                    {
                        int index = indexInCoverMatrix(row, col, num);
                        for (int i = 0; i < _BOARD_SIZE * _BOARD_SIZE * _NUM_CONSTRAINTS; ++i)
                        {
                            coverMatrix[index][i] = 0;
                        }
                    }
                }
            }
        }
    }
}

// Función que crea la matriz de estado (posibilidades para cada celda)
void SudokuBoard::createStateMatrix(StateMatrix& stateMatrix)
{
    // Inicializa la matriz de estado con celdas vacías
	stateMatrix.resize(_BOARD_SIZE, vector<MultiType>(_BOARD_SIZE));
}

// Función que convierte el tablero en una matriz de estado con las posibles opciones para cada celda vacía
void SudokuBoard::convertToStateMatrix(StateMatrix& stateMatrix)
{
    // Crea un conjunto de posibles valores
	set<int> setOfPossibleValues;
	for (int num = _MIN_VALUE; num < _MAX_VALUE; ++num)
	{
		setOfPossibleValues.insert(num);
	}

    // Recorre el tablero
	for (int i = 0; i < _BOARD_SIZE; ++i)
	{
		for (int j = 0; j < _BOARD_SIZE; ++j)
		{
            // Si la celda está vacía, asigna el conjunto de valores posibles
			stateMatrix[i][j] = _board_data[i][j];
			if (get<int>(stateMatrix[i][j]) == _EMPTY_CELL_VALUE)
			{
				stateMatrix[i][j] = setOfPossibleValues;
			}
		}
	}
}

