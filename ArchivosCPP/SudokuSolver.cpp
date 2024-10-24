#include "SudokuSolver.hpp"
#include "helper.hpp"
#include <algorithm>
#include <cmath>
#include <vector>
#include <thread>
#include <chrono>

// Constructor de SudokuSolver que inicializa el tablero.
SudokuSolver::SudokuSolver(SudokuBoard& board)
	: _board(board) // Inicializa el tablero asociado al solucionador
{ }

// Verifica si todas las celdas del tablero están llenas.
bool SudokuSolver::checkIfAllFilled(const SudokuBoard& board) const
{
    for (int i = 0; i < board.get_board_size(); ++i)
	{
        for (int j = 0; j < board.get_board_size(); ++j)
		{
            if (isEmpty(board, i, j)) // Comprueba si hay celdas vacías
			{
                return false; // Retorna falso si hay celdas vacías
			}
        }
    }
    return true; // Retorna verdadero si todas las celdas están llenas
}

// Verifica si una fila específica está completamente llena.
bool SudokuSolver::checkIfRowFilled(const SudokuBoard& board, int indexOfRows) const
{
	for (int j = 0; j < board.get_board_size(); ++j)
	{
		if (isEmpty(board, indexOfRows, j)) { return false; } // Retorna falso si hay celdas vacías en la fila
	}

    return true; // Retorna verdadero si la fila está llena
}

// Encuentra la primera celda vacía en el tablero y retorna su posición (fila, columna).
const std::pair<int, int> SudokuSolver::find_empty(const SudokuBoard& board)
{
	Position empty_cell; // Variable para almacenar la posición de la celda vacía
	bool stop = false;

	for (int i = 0; i < board.get_board_size(); ++i)
	{
		for (int j = 0; j < board.get_board_size(); ++j)
		{
			if (isEmpty(board, i, j))
			{
				empty_cell = std::make_pair(i, j); // Asigna la posición de la celda vacía
				stop = true;
				break; // Sale del bucle si se encuentra una celda vacía
			}
		}
		if (stop) { break; } // Sale del bucle exterior si se encontró una celda vacía
	}
	
	return empty_cell;  // Retorna la posición de la celda vacía (fila, col)
}

// Encuentra la primera celda vacía en una fila específica y retorna su índice de columna.
int SudokuSolver::find_empty_from_row(const SudokuBoard& board, int indexOfRows) const
{
	int empty_cell_col_index = -1;

	for (int j = 0; j < board.get_board_size(); ++j)
	{
		if (isEmpty(board, indexOfRows, j))
		{
			empty_cell_col_index = j; // Asigna el índice de columna de la celda vacía
			return empty_cell_col_index; // Retorna inmediatamente después de encontrarla
		}
	}

	return empty_cell_col_index; // Retorna -1 si no se encontró ninguna celda vacía
}

// Verifica si una celda está vacía.
bool SudokuSolver::isEmpty(const SudokuBoard& board, int i, int j) const
{
	return (board.at(i, j) == board._EMPTY_CELL_VALUE); // Retorna verdadero si la celda está vacía
}

// Verifica si un número se puede colocar en una fila específica.
bool SudokuSolver::isValidRow(const SudokuBoard& board, int num, Position pos) const
{
    for (int i = 0; i < board.get_board_size(); ++i)
	{
        if ( (i != pos.second) && (board.at(pos.first, i) == num) ) { return false; } // Retorna falso si el número ya existe en la fila
	}

	return true; // Retorna verdadero si el número se puede colocar en la fila
}

// Verifica si un número se puede colocar en una columna específica.
bool SudokuSolver::isValidColumn(const SudokuBoard& board, int num, Position pos) const
{
	for (int i = 0; i < board.get_board_size(); ++i)
	{
        if ( (i != pos.first) && (board.at(i, pos.second) == num) ) { return false; } // Retorna falso si el número ya existe en la columna
	}
	
	return true; // Retorna verdadero si el número se puede colocar en la columna
}

// Verifica si un número se puede colocar en el cuadro correspondiente.
bool SudokuSolver::isValidBox(const SudokuBoard& board, int num, Position pos) const
{
	int BOX_SIZE = board.get_box_size();

	int box_x = std::floor(pos.first / board.get_box_size());
    int box_y = std::floor(pos.second / board.get_box_size());

    for (int i = box_x * BOX_SIZE; i < box_x * BOX_SIZE + BOX_SIZE; ++i)
	{
        for (int j = box_y * BOX_SIZE; j < box_y * BOX_SIZE + BOX_SIZE; ++j)
		{
            if ( (i != pos.first && j != pos.second) && (board.at(i, j) == num) ) { return false; } // Retorna falso si el número ya existe en el cuadro
        }
    }

	return true; // Retorna verdadero si el número se puede colocar en el cuadro
}

// Verifica si un número se puede colocar en una posición específica (fila, columna).
bool SudokuSolver::isValid(const SudokuBoard& board, int num, Position pos) const
{
    return isValidRow(board, num, pos) && isValidColumn(board, num, pos) && isValidBox(board, num, pos); // Verifica las tres condiciones
}

// Verifica si un número es único en su cuadro, fila y columna.
bool SudokuSolver::isUnique(const SudokuBoard& board, int num, Position pos) const
{
	int local_row = pos.first % board.get_box_size();
	int local_col = pos.second % board.get_box_size();

	int box_x = std::floor(pos.first / board.get_box_size());
    int box_y = std::floor(pos.second / board.get_box_size());

	for (int i = ((local_row == 0) ? 1 : 0); i < board.get_box_size(); ++i)
	{
		if (i == local_row) { continue; } // Salta la fila actual
		std::vector<int> numbersInRow = board.getNumbersInRow(box_x * board.get_box_size() + i);
		if (std::find(numbersInRow.begin(), numbersInRow.end(), num) == numbersInRow.end())
		{
			return false; // Retorna falso si el número no está en la fila
		}
	}

	for (int j = ((local_col == 0) ? 1 : 0); j < board.get_box_size(); ++j)
	{
		if (j == local_col) { continue; } // Salta la columna actual
		std::vector<int> numbersInCol = board.getNumbersInCol(box_y * board.get_box_size() + j);
		if (std::find(numbersInCol.begin(), numbersInCol.end(), num) == numbersInCol.end())
		{
			return false; // Retorna falso si el número no está en la columna
		}
	}

	return true; // Retorna verdadero si el número es único
}

// Muestra una barra de progreso durante la solución del Sudoku.
void SudokuSolver::show_progress_bar(SudokuBoard& board, int _recursionDepth, int interval /*=5*/)
{
	if (_recursionDepth == 0)
	{
		_current_num_empty_cells = board.get_init_num_empty_cells();
		printProgressBar2(double(board.get_init_num_empty_cells() - _current_num_empty_cells) / board.get_init_num_empty_cells());
		std::this_thread::sleep_for(std::chrono::milliseconds(interval)); // Espera antes de mostrar el progreso
	} 
	else
	{
		if (board.get_num_empty_cells() < _current_num_empty_cells)
		{
			_current_num_empty_cells = board.get_num_empty_cells();
			printProgressBar2(double(board.get_init_num_empty_cells() - _current_num_empty_cells) / board.get_init_num_empty_cells());
			std::this_thread::sleep_for(std::chrono::milliseconds(interval)); // Espera antes de mostrar el progreso
		}
	}
}
