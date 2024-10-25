#include "SudokuSolver_SequentialDLX.hpp"  // Incluir encabezado específico para el solucionador secuencial usando "dancing links"
#include <iostream>                         // Incluir biblioteca de entrada/salida estándar

// Constructor del solucionador de Sudoku secuencial usando el algoritmo de "dancing links"
SudokuSolver_SequentialDLX::SudokuSolver_SequentialDLX(SudokuBoard& board, bool print_message /*=true*/)
: SudokuSolver(board){
    _mode = MODES::SEQUENTIAL_DANCINGLINKS;
    if (print_message){
        std::cout << "\n Resolviendo usando secuencial DLX algoritmo porfavor espere mientras se ejecuta...\n";
    }
    board.createCoverMatrix(_coverMatrix);           // Crear la matriz de cobertura
    board.convertToCoverMatrix(_coverMatrix);        // Convertir el tablero a matriz de cobertura
    _numberOfRows = _coverMatrix.size();             // Obtener el número de filas
    _numberOfColumns = _coverMatrix[0].size();       // Obtener el número de columnas
    _header = createDLXList(_coverMatrix);           // Crear la lista de "dancing links"
}

// Función para crear la lista de "dancing links" desde la matriz de cobertura
ColumnNode* SudokuSolver_SequentialDLX::createDLXList(CoverMatrix& coverMatrix){
    // Si declaramos headerNode como una variable local (en la "pila"), se limpiará de la memoria cuando finalice la llamada a la función
    // ---> Para mantener su vida útil, creamos headerNode en el "montón" para que exista todo el tiempo
    //      a menos que liberemos explícitamente la memoria asociada
    ColumnNode* headerNode = new ColumnNode("header"); // Nodo cabecera creado en el heap para mantener vida útil
    std::vector<ColumnNode*> columnNodes;
    for (int i = 0; i < _numberOfColumns; ++i){
        ColumnNode* n = new ColumnNode(std::to_string(i));
        columnNodes.push_back(n);
        headerNode = (ColumnNode*) headerNode->linkRight(n); // Enlazar nodos columna a la derecha
    }
    headerNode = headerNode->right->column;
    for (int i = 0; i < _numberOfRows; ++i){
        DancingNode* prev = NULL;
        for (int j = 0; j < _numberOfColumns; ++j){
            if (coverMatrix[i][j] == 1){
                ColumnNode* col = columnNodes[j];
                DancingNode* newNode = new DancingNode(col); // Crear nodo de "dancing links"
                if (prev == NULL) { prev = newNode; }
                col->top->linkDown(newNode); // Enlazar nodos hacia abajo
                prev = prev->linkRight(newNode); // Enlazar nodos a la derecha
                col->size++;
            }
        }
    }
    headerNode->size = _numberOfColumns; // Asignar tamaño de columnas al nodo cabecera
    return headerNode;
}

// Convertir la solución de la lista de "dancing nodes" a un tablero de Sudoku
SudokuBoard SudokuSolver_SequentialDLX::convertToSudokuGrid(std::vector<DancingNode*> answer){
    SudokuBoard tmpBoard = SudokuBoard(_board);
    for (DancingNode* n : answer){
        DancingNode* rcNode = n;
        int min = std::stoi(rcNode->column->name);
        for (DancingNode* tmp = n->right; tmp != n; tmp = tmp->right){
            int val = std::stoi(tmp->column->name);
            if (val < min){
                min = val;
                rcNode = tmp;
            }
        }
        int ans1 = std::stoi(rcNode->column->name);
        int ans2 = std::stoi(rcNode->right->column->name);
        int r = ans1 / _board.get_board_size();
        int c = ans1 % _board.get_board_size();
        int num = (ans2 % _board.get_board_size()) + 1;
        tmpBoard.set_board_data(r, c, num);
    }
    return tmpBoard;
}

// Núcleo del algoritmo de "dancing links" para resolver el Sudoku
void SudokuSolver_SequentialDLX::solve_kernel(int k){
    if (_header->right == _header){
        _solved = true;
        _solution = convertToSudokuGrid(_answer); // Convertir y almacenar solución si se encuentra
        return;
    } else {
        ColumnNode* c = (ColumnNode*) _header->right;
        c = selectColumnNodeHeuristic(c, k);
        c->cover(); // Cubrir la columna seleccionada
        for (DancingNode* r = c->bottom; r != c; r = r->bottom){
            _answer.push_back(r);
            for (DancingNode* j = r->right; j != r; j = j->right){
                j->column->cover(); // Cubrir nodos en la fila
            }
            solve_kernel(k + 1);
            if (_solved) { return; }
            r = _answer[_answer.size() - 1];
            _answer.pop_back();
            c = r->column;
            for (DancingNode* j = r->left; j != r; j = j->left){
                j->column->uncover(); // Descubrir nodos en la fila
            }
        }
        c->uncover(); // Descubrir la columna
    }
}

// Selección heurística de la columna a cubrir
ColumnNode* SudokuSolver_SequentialDLX::selectColumnNodeHeuristic(ColumnNode* c, int k){
    if (k == 0){
        c = (ColumnNode*) c->right; // Comenzar con una restricción diferente en cada hilo
    } else {
        // Luego elegir la restricción con menos opciones para satisfacer
        for (ColumnNode* temp = (ColumnNode*) c->right; temp != _header; temp = (ColumnNode*) temp->right){
            if (temp->size < c->size) c = temp;
        }
    }
    return c;
}
