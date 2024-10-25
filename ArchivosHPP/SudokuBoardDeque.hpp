#ifndef SUDOKUBOARDDEQUE_HPP
#define SUDOKUBOARDDEQUE_HPP

#include <iostream>     
#include <iterator>     
#include <deque>        
#include "SudokuBoard.hpp"  

class SudokuBoardDeque {
public:
    SudokuBoardDeque() = default;  // Constructor por defecto

    // Constructor de copia de la clase SudokuBoardDeque: construye un nuevo SudokuBoardDeque guardando el tablero como su primer elemento
    SudokuBoardDeque(const SudokuBoard& board);

    std::deque<SudokuBoard> boardDeque;  // Deque que almacena tableros de Sudoku

    // Obtiene el número total de tableros en el deque
    int size() const;

    // Obtiene el primer tablero en el deque
    SudokuBoard& front();

    // Obtiene el último tablero en el deque
    SudokuBoard& back();

    // Obtiene el tablero en la posición i del deque
    SudokuBoard& operator[] (int i);

    // Elimina el primer tablero en el deque y libera la memoria
    void pop_front();

    // Elimina el último tablero en el deque y libera la memoria
    void pop_back();

    // Empuja un tablero al frente del deque
    void push_front(SudokuBoard& board);

    // Empuja un tablero al final del deque
    void push_back(SudokuBoard& board);

    // Destructor de la clase SudokuBoardDeque: elimina los tableros dentro del deque uno por uno
    virtual ~SudokuBoardDeque();
};

#endif // SUDOKUBOARDDEQUE_HPP
