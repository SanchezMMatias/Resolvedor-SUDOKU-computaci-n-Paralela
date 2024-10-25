#include "SudokuSolver_SequentialForwardChecking.hpp"  
#include "helper.hpp"                                   
#include <algorithm>                                    
#include <vector>                                       

// Constructor del solucionador de Sudoku secuencial usando el algoritmo de "forward checking"
SudokuSolver_SequentialForwardChecking::SudokuSolver_SequentialForwardChecking(SudokuBoard& board, bool print_message /*=true*/)
: SudokuSolver(board){
    _mode = MODES::SEQUENTIAL_FORWARDCHECKING;
    if (print_message){
        std::cout << "\n Resolviendo usandoe el algoritmo de chequeo hacia adelante, porfavor espere mientras se ejecuta...\n";
    }
    board.createStateMatrix(_stateMatrix);           // Crear la matriz de estado
    board.convertToStateMatrix(_stateMatrix);        // Convertir el tablero a matriz de estado
}

// Paso de propagación del algoritmo de "forward checking"
bool SudokuSolver_SequentialForwardChecking::propagate_step(StateMatrix& stateMatrix){
    bool new_units = false;
    random_selector<> selector{ };  // Selector aleatorio
    int BOARD_SIZE = _board.get_board_size();
    int BOX_SIZE = _board.get_box_size();
    
    // Propagación de la regla de filas
    for (int i = 0; i < BOARD_SIZE; ++i){
        std::vector<MultiType> row = stateMatrix[i];
        std::set<int> setOfFilledValuesInRow;
        for (auto& e : row){
            if (std::holds_alternative<int>(e)){   // Si el elemento row[i] es de tipo int
                setOfFilledValuesInRow.insert(std::get<int>(e));
            }
        }
        for (int j = 0; j < BOARD_SIZE; ++j){
            if (std::holds_alternative<std::set<int>>(stateMatrix[i][j])){   // Si el elemento stateMatrix[i][j] es de tipo std::set<int>
                auto state = std::get<std::set<int>>(stateMatrix[i][j]);
                std::set<int> temp;
                std::set_difference(state.begin(), state.end(),
                                    setOfFilledValuesInRow.begin(), setOfFilledValuesInRow.end(),
                                    std::inserter(temp, temp.begin()));
                stateMatrix[i][j] = temp;
                if (state.size() == 1){
                    int val = selector(state);
                    state.erase(val);
                    stateMatrix[i][j] = val;
                    setOfFilledValuesInRow.insert(val);
                    new_units = true;
                }
            }
        }
    }
    
    // Propagación de la regla de columnas
    for (int j = 0; j < BOARD_SIZE; ++j){
        std::vector<MultiType> column;
        for (int i = 0; i < BOARD_SIZE; ++i){
            column.push_back(stateMatrix[i][j]);
        }
        std::set<int> setOfFilledValuesInCol;
        for (auto& e : column){
            if (std::holds_alternative<int>(e)){   // Si el elemento column[i] es de tipo int
                setOfFilledValuesInCol.insert(std::get<int>(e));
            }
        }
        for (int i = 0; i < BOARD_SIZE; ++i){
            if (std::holds_alternative<std::set<int>>(stateMatrix[i][j])){   // Si el elemento stateMatrix[i][j] es de tipo std::set<int>
                auto state = std::get<std::set<int>>(stateMatrix[i][j]);
                std::set<int> temp;
                std::set_difference(state.begin(), state.end(),
                                    setOfFilledValuesInCol.begin(), setOfFilledValuesInCol.end(),
                                    std::inserter(temp, temp.begin()));
                stateMatrix[i][j] = temp;
                if (state.size() == 1){
                    int val = selector(state);
                    state.erase(val);
                    stateMatrix[i][j] = val;
                    setOfFilledValuesInCol.insert(val);
                    new_units = true;
                }
            }
        }
    }
    
    // Propagación de la regla de cajas
    for (int i = 0; i < BOX_SIZE; ++i){
        for (int j = 0; j < BOX_SIZE; ++j){
            std::set<int> setOfFilledValuesInBox;
            for (int s = BOX_SIZE * i; s < BOX_SIZE * i + BOX_SIZE; ++s){
                for (int t = BOX_SIZE * j; t < BOX_SIZE * j + BOX_SIZE; ++t){
                    if (std::holds_alternative<int>(stateMatrix[i][j])){   // Si el elemento column[i] es de tipo int
                        setOfFilledValuesInBox.insert(std::get<int>(stateMatrix[i][j]));
                    }
                }
            }
            for (int s = BOX_SIZE * i; s < BOX_SIZE * i + BOX_SIZE; ++s){
                for (int t = BOX_SIZE * j; t < BOX_SIZE * j + BOX_SIZE; ++t){
                    if (std::holds_alternative<std::set<int>>(stateMatrix[i][j])){   // Si el elemento stateMatrix[i][j] es de tipo std::set<int>
                        auto state = std::get<std::set<int>>(stateMatrix[i][j]);
                        std::set<int> temp;
                        std::set_difference(state.begin(), state.end(),
                                            setOfFilledValuesInBox.begin(), setOfFilledValuesInBox.end(),
                                            std::inserter(temp, temp.begin()));
                        stateMatrix[i][j] = temp;
                        if (state.size() == 1){
                            int val = selector(state);
                            state.erase(val);
                            stateMatrix[i][j] = val;
                            setOfFilledValuesInBox.insert(val);
                            new_units = true;
                        }
                    }
                }
            }
        }
    }
    return new_units;
}

// Función para propagar restricciones en la matriz de estado
void SudokuSolver_SequentialForwardChecking::propagate(StateMatrix& stateMatrix){
    while (true){
        if (!propagate_step(stateMatrix)) { return; }
    }
}

// Función para comprobar si se ha completado la solución
bool SudokuSolver_SequentialForwardChecking::done(StateMatrix& stateMatrix){
    for (int i = 0; i < _board.get_board_size(); ++i){
        for (int j = 0; j < _board.get_board_size(); ++j){
            if (std::holds_alternative<std::set<int>>(stateMatrix[i][j])){   // Si el elemento stateMatrix[i][j] es de tipo std::set<int>
                return false;
            }
        }
    }
    return true;
}

// Convertir la matriz de estado a un tablero de Sudoku
SudokuBoard SudokuSolver_SequentialForwardChecking::convertToSudokuGrid(StateMatrix& stateMatrix){
    SudokuBoard tmpBoard = SudokuBoard(_board);
    for (int i = 0; i < _board.get_board_size(); ++i){
        for (int j = 0; j < _board.get_board_size(); ++j){
            if (std::holds_alternative<int>(stateMatrix[i][j])){
                tmpBoard.set_board_data(i, j, std::get<int>(stateMatrix[i][j]));
            }
        }
    }
    return tmpBoard;
}

// Núcleo del algoritmo de "forward checking" para resolver el Sudoku
void SudokuSolver_SequentialForwardChecking::solve_kernel(StateMatrix& stateMatrix){
    if (_solved) return;
    propagate(stateMatrix);
    if (done(stateMatrix)){
        _solved = true;
        _solution = convertToSudokuGrid(stateMatrix);
        return;
    } else {
        for (int i = 0; i < _board.get_board_size(); ++i){
            for (int j = 0; j < _board.get_board_size(); ++j){
                if (std::holds_alternative<std::set<int>>(stateMatrix[i][j])){   // Si el elemento stateMatrix[i][j] es de tipo std::set<int>
                    for (const auto& value : std::get<std::set<int>>(stateMatrix[i][j])){
                        StateMatrix newStateMatrix = stateMatrix;
                        newStateMatrix[i][j] = value;
                        solve_kernel(newStateMatrix);
                    }
                }
            }
        }
    }
}
