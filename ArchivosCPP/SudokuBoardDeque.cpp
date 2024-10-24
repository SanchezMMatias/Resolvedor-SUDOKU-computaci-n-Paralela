
// Definir una cola de tableros para ayudar a la implementación de la versión OpenMP del solucionador de Sudoku.

#include <vector>
#include <random>
#include <algorithm>
#include "SudokuBoardDeque.hpp"

// Constructor que inicializa el deque con un tablero inicial
SudokuBoardDeque::SudokuBoardDeque(const SudokuBoard& board) 
{
    boardDeque.push_back(board);  // Agrega el tablero al final del deque
}

// Destructor que limpia el deque al eliminar los elementos del frente
SudokuBoardDeque::~SudokuBoardDeque() 
{
    while (boardDeque.size() > 0) 
    {
        boardDeque.pop_front();  // Elimina los elementos desde el frente
    }
}

// Función que retorna el tamaño del deque
int SudokuBoardDeque::size() const 
{
    return boardDeque.size();
}

// Función que retorna una referencia al primer tablero del deque
SudokuBoard& SudokuBoardDeque::front() 
{
    return boardDeque.front();
}

// Función que retorna una referencia al último tablero del deque
SudokuBoard& SudokuBoardDeque::back() 
{
    return boardDeque.back();
}

// Sobrecarga del operador [] para acceder a los elementos del deque por índice
SudokuBoard& SudokuBoardDeque::operator[] (int i) 
{
    return boardDeque[i];
}

// Función que elimina el primer tablero del deque
void SudokuBoardDeque::pop_front() 
{
    boardDeque.pop_front();
}

// Función que elimina el último tablero del deque
void SudokuBoardDeque::pop_back() 
{
    boardDeque.pop_back();
}

// Función que inserta un tablero al frente del deque
void SudokuBoardDeque::push_front(SudokuBoard& board) 
{
    boardDeque.push_front(board);
}

// Función que inserta un tablero al final del deque
void SudokuBoardDeque::push_back(SudokuBoard& board) 
{
    boardDeque.push_back(board);
}
