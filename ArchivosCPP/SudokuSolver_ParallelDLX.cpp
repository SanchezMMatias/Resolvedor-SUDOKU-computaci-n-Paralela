#include "SudokuSolver_ParallelDLX.hpp"
#include "termcolor.hpp"                 
#include <omp.h>                         

// Constructor del solucionador de Sudoku paralelo usando el algoritmo de "dancing links"
SudokuSolver_ParallelDLX::SudokuSolver_ParallelDLX(SudokuBoard& board, bool print_message /*=true*/)
: SudokuSolver(board){
    _mode = MODES::PARALLEL_DANCINGLINKS;
    if (print_message){
        std::cout << "\nParallel Sudoku solver using dancing links algorithm starts, please wait...\n";
    }
    board.createCoverMatrix(_coverMatrix);           // Crear la matriz de cobertura
    board.convertToCoverMatrix(_coverMatrix);        // Convertir el tablero a matriz de cobertura
    _numberOfRows = _coverMatrix.size();             // Obtener el número de filas
    _numberOfColumns = _coverMatrix[0].size();       // Obtener el número de columnas
    _header = createDLXList(_coverMatrix);           // Crear la lista de "dancing links"
}

// Función para crear la lista de "dancing links" desde la matriz de cobertura
ColumnNode* SudokuSolver_ParallelDLX::createDLXList(CoverMatrix& coverMatrix){
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
SudokuBoard SudokuSolver_ParallelDLX::convertToSudokuGrid(std::vector<DancingNode*> answer){
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
void SudokuSolver_ParallelDLX::solve_kernel(int k){
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
ColumnNode* SudokuSolver_ParallelDLX::selectColumnNodeHeuristic(ColumnNode* c, int k){
    if (k == 0){
        int id = omp_get_thread_num(); // Obtener el ID del hilo actual
        for (int i = 0; i < id; i++){
            c = (ColumnNode*) c->right;
        }
    } else {
        for (ColumnNode* temp = (ColumnNode*) c->right; temp != _header; temp = (ColumnNode*) temp->right){
            if (temp->size < c->size) c = temp; // Elegir columna con menos opciones disponibles
        }
    }
    return c;
}
