#include "SudokuTest.hpp" 

// Devuelve si las dimensiones del tablero de Sudoku son válidas.
bool SudokuTest::checkValidSizes(const SudokuBoard& board){
    int BOARD_SIZE = board._BOARD_SIZE;
    int ROW_SIZE = board._board_data.size();
    int COL_SIZE = board._board_data[0].size(); 
    if ((ROW_SIZE != BOARD_SIZE) || (COL_SIZE != BOARD_SIZE)){
        return false;
    }
    return true;
}

// Devuelve si todas las filas son válidas, es decir, si no contienen números duplicados.
bool SudokuTest::checkValidRows(const SudokuBoard& board){
    for (int row = 0; row < board._BOARD_SIZE; ++row){
        std::set<int> valuesInRow;
        for (int col = 0; col < board._BOARD_SIZE; ++col){
            int val = board._board_data[row][col];
            if (val == board._EMPTY_CELL_VALUE) { continue; }
            if (valuesInRow.find(val) != valuesInRow.end()) { return false; }
            valuesInRow.insert(val);
        }
    }
    return true;
}

// Devuelve si todas las columnas son válidas, es decir, si no contienen números duplicados.
bool SudokuTest::checkValidColumns(const SudokuBoard& board){
    for (int col = 0; col < board._BOARD_SIZE; ++col){
        std::set<int> valuesInCol;
        for (int row = 0; row < board._BOARD_SIZE; ++row){
            int val = board._board_data[row][col];
            if (val == board._EMPTY_CELL_VALUE) { continue; }
            if (valuesInCol.find(val) != valuesInCol.end()) { return false; }
            valuesInCol.insert(val);
        }
    }
    return true;
}

// Devuelve si todas las cajas (subcuadrículas) son válidas, es decir, si no contienen números duplicados.
bool SudokuTest::checkValidBoxes(const SudokuBoard& board){
    int BOX_SIZE = board._BOX_SIZE;
    for (int box_x = 0; box_x < BOX_SIZE; ++box_x){
        for (int box_y = 0; box_y < BOX_SIZE; ++box_y){
            std::set<int> valuesInBox;
            for (int m = 0; m < BOX_SIZE; ++m){
                for (int n = 0; n < BOX_SIZE; ++n){
                    int val = board._board_data[box_x * BOX_SIZE + m][box_y * BOX_SIZE + n];
                    if (val == board._EMPTY_CELL_VALUE) { continue; }
                    if (valuesInBox.find(val) != valuesInBox.end()) { return false; }
                    valuesInBox.insert(val);
                }
            }
        }
    }
    return true;
}
