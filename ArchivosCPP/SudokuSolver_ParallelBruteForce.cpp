#include "SudokuSolver_ParallelBruteForce.hpp" 
#include "SudokuSolver_SequentialBruteForce.hpp" 
#include "termcolor.hpp"  
#include <iostream> 
#include <vector>  
#include <omp.h>   

// Constructor para inicializar el solucionador paralelo
SudokuSolver_ParallelBruteForce::SudokuSolver_ParallelBruteForce(SudokuBoard& board, bool print_message /*=true*/)
	: SudokuSolver(board) // Inicializa la clase base SudokuSolver
{
	_mode = MODES::PARALLEL_BRUTEFORCE; // Establece el modo a fuerza bruta paralela
	if (print_message) // Si se debe imprimir un mensaje
	{
		std::cout << "\n" << "Resolviendo el sudoku usando algoritmo fuerza bruta en paralelo porfavor espere mientras se ejecuta..." << "\n"; // Mensaje informativo
	}
}

// Método de inicialización (bootstrap) para llenar las celdas vacías
void SudokuSolver_ParallelBruteForce::bootstrap()
{
	// Si no hay tableros de inicio en la cola, retorna
    if (_board_deque.size() == 0) { return; }

	SudokuBoard board = _board_deque.front(); // Obtiene el primer tablero de la cola

	if (checkIfAllFilled(board)) { return; } // Verifica si el tablero ya está lleno

	Position empty_cell_pos = find_empty(board); // Encuentra la posición de la celda vacía

	int row = empty_cell_pos.first; // Fila de la celda vacía
	int col = empty_cell_pos.second; // Columna de la celda vacía

	// Rellena todas las posibles cifras en la celda vacía y añade el tablero correspondiente a la cola
	for (int num = board.get_min_value(); num <= board.get_max_value(); ++num)
	{
		if (isValid(board, num, empty_cell_pos)) // Verifica si el número es válido
		{
			board.set_board_data(row, col, num); // Establece el número en la celda
			_board_deque.push_back(board); // Agrega el tablero a la cola
		}
	}

	_board_deque.pop_front(); // Elimina el tablero procesado de la cola
}

// Método de inicialización para un índice específico de fila
void SudokuSolver_ParallelBruteForce::bootstrap(SudokuBoardDeque& boardDeque, int indexOfRows)
{
	// Si no hay tableros en la cola, retorna
    if (boardDeque.size() == 0) { return; }

	// Rellena las celdas vacías en la fila especificada
	while (!checkIfRowFilled(boardDeque.front(), indexOfRows))
	{
		SudokuBoard board = boardDeque.front(); // Obtiene el primer tablero de la cola

		int empty_cell_col_index = find_empty_from_row(board, indexOfRows); // Encuentra la celda vacía en la fila

		// Rellena todas las posibles cifras en la celda vacía y añade el tablero correspondiente a la cola
		for (int num = board.get_min_value(); num <= board.get_max_value(); ++num)
		{
			Position empty_cell_pos = std::make_pair(indexOfRows, empty_cell_col_index); // Crea la posición de la celda vacía

			if (isValid(board, num, empty_cell_pos)) // Verifica si el número es válido
			{
				board.set_board_data(indexOfRows, empty_cell_col_index, num); // Establece el número en la celda
				boardDeque.push_back(board); // Agrega el tablero a la cola
			}
		}

		boardDeque.pop_front(); // Elimina el tablero procesado de la cola
	}
}

// Método principal para resolver el Sudoku utilizando paralelismo
void SudokuSolver_ParallelBruteForce::solve_kernel_1()
{
	// Agrega el tablero inicial a la cola
	_board_deque.push_back(_board);

	// Asegura un nivel de inicialización (bootstrapping)
	int num_bootstraps = omp_get_num_threads(); // Obtiene el número de hilos disponibles
	#pragma omp parallel for schedule(static) default(none) shared(num_bootstraps) // Directiva para paralelismo
	for (int i = 0; i < num_bootstraps; ++i)
	{
		bootstrap(); // Llama al método bootstrap en paralelo
	}

	int numberOfBoards = _board_deque.size(); // Obtiene el número de tableros en la cola
    
	// Para depuración (comentado)
	// std::cout << "Number of Sudoku boards on the board deque: " << numberOfBoards << "\n";
	// for (int i = 0; i < numberOfBoards; ++i)
	// {
	// 	std::cout << "BOARD-" << i << "\n";
	// 	print_board(_board_deque[i]); // Imprime cada tablero
	// 	std::cout << "*****" << "\n";
	// }

	std::vector<SudokuSolver_SequentialBruteForce> solvers; // Vector de solucionadores secuenciales

	#pragma omp parallel for schedule(static) default(none) shared(numberOfBoards, solvers) // Directiva para paralelismo
    for (int indexOfBoard = 0; indexOfBoard < numberOfBoards; ++indexOfBoard)
	{
		solvers.push_back(SudokuSolver_SequentialBruteForce(_board_deque[indexOfBoard], false)); // Crea un solucionador secuencial para cada tablero
		
		// Nota: No se permite la instrucción break en OpenMP, todas las iteraciones deben procesarse.
		// La solución es establecer un flag como verdadero cuando se cumple la condición, y dejar las iteraciones restantes sin trabajo.
		if (_solved) { continue; } // Si ya se ha encontrado una solución, continúa con la siguiente iteración

		solvers[indexOfBoard].set_mode(MODES::PARALLEL_BRUTEFORCE); // Establece el modo del solucionador secuencial

        solvers[indexOfBoard].solve(); // Resuelve el tablero

		if (solvers[indexOfBoard].get_status() == true) // Si se ha encontrado una solución
		{
			_solved = true; // Marca como resuelto
			_solution = solvers[indexOfBoard].get_solution(); // Almacena la solución
		}
	}
}

// Método secundario para resolver el Sudoku utilizando paralelismo
void SudokuSolver_ParallelBruteForce::solve_kernel_2()
{
	std::vector<SudokuBoardDeque> groupOfBoardDeques(_board.get_board_size(), SudokuBoardDeque(_board)); // Crea un grupo de colas de tableros
	#pragma omp parallel default(none) shared(groupOfBoardDeques) // Directiva para paralelismo
	{	
		int SIZE = groupOfBoardDeques.size(); // Tamaño del grupo de colas

		#pragma omp for nowait schedule(static) // Directiva para paralelismo sin espera
		for (int i = 0; i < SIZE; ++i)
		{
			bootstrap(groupOfBoardDeques[i], i); // Inicializa cada cola con un índice específico
			_board_deque.boardDeque.insert(_board_deque.boardDeque.end(), // Inserta los tableros procesados en la cola principal
										   groupOfBoardDeques[i].boardDeque.begin(),
										   groupOfBoardDeques[i].boardDeque.end());
		}
	}

	int numberOfBoards = _board_deque.size(); // Obtiene el número de tableros en la cola
    
	// Para depuración (comentado)
	// std::cout << "Number of Sudoku boards on the board deque: " << numberOfBoards << "\n";
	// for (int i = 0; i < numberOfBoards; ++i)
	// {
	// 	std::cout << "BOARD-" << i << "\n";
	// 	print_board(_board_deque[i]); // Imprime cada tablero
	// 	std::cout << "*****" << "\n";
	// }

	std::vector<SudokuSolver_SequentialBruteForce> solvers; // Vector de solucionadores secuenciales

	#pragma omp parallel for schedule(static) default(none) shared(numberOfBoards, solvers) // Directiva para paralelismo
    for (int indexOfBoard = 0; indexOfBoard < numberOfBoards; ++indexOfBoard)
	{	
		solvers.push_back(SudokuSolver_SequentialBruteForce(_board_deque[indexOfBoard], false)); // Crea un solucionador secuencial para cada tablero

		// Nota: No se permite la instrucción break en OpenMP, todas las iteraciones deben procesarse.
		// La solución es establecer un flag como verdadero cuando se cumple la condición, y dejar las iteraciones restantes sin trabajo.
		if (_solved) { continue; } // Si ya se ha encontrado una solución, continúa con la siguiente iteración

        solvers[indexOfBoard].solve(); // Resuelve el tablero

		if (solvers[indexOfBoard].get_status() == true) // Si se ha encontrado una solución
		{
			_solved = true; // Marca como resuelto
			_solution = solvers[indexOfBoard].get_solution(); // Almacena la solución
		}
	}
}

// Método para resolver el Sudoku de forma secuencial
void SudokuSolver_ParallelBruteForce::solve_bruteforce_seq(SudokuBoard& board, int row, int col)
{
	if (_solved) { return; } // Si ya se ha encontrado una solución, retorna
	
	int BOARD_SIZE = board.get_board_size(); // Obtiene el tamaño del tablero

	int abs_index = row * BOARD_SIZE + col; // Calcula el índice absoluto de la celda

    if (abs_index >= board.get_num_total_cells()) // Si se han procesado todas las celdas
	{
		_solved = true; // Marca como resuelto
		_solution = board; // Almacena la solución
		return;
    }
    
	int row_next = (abs_index + 1) / BOARD_SIZE; // Calcula la fila de la siguiente celda
	int col_next = (abs_index + 1) % BOARD_SIZE; // Calcula la columna de la siguiente celda

	if (!isEmpty(board, row, col)) // Si la celda no está vacía
	{   
		solve_bruteforce_seq(board, row_next, col_next); // Llama recursivamente a la siguiente celda
    }
	else
	{
		// Rellena todas las posibles cifras
        for (int num = board.get_min_value(); num <= board.get_max_value(); ++num)
		{
			Position pos = std::make_pair(row, col); // Crea la posición de la celda

            if (isValid(board, num, pos)) // Verifica si el número es válido
			{
                board.set_board_data(row, col, num); // Establece el número en la celda

				if (isUnique(board, num, pos)) { num = BOARD_SIZE + 1; }   // Forzar a salir del bucle for

				// Intenta la siguiente celda recursivamente
                solve_bruteforce_seq(board, row_next, col_next);

				board.set_board_data(row, col, board.get_empty_cell_value()); // Restablece la celda a su valor vacío
            }
        }
    }

	_recursionDepth++; // Incrementa la profundidad de recursión
}

// Método para resolver el Sudoku de forma paralela
void SudokuSolver_ParallelBruteForce::solve_bruteforce_par(SudokuBoard& board, int row, int col)
{
	if (_solved) { return; } // Si ya se ha encontrado una solución, retorna
	
	int BOARD_SIZE = board.get_board_size(); // Obtiene el tamaño del tablero

	int abs_index = row * BOARD_SIZE + col; // Calcula el índice absoluto de la celda

    if (abs_index >= board.get_num_total_cells()) // Si se han procesado todas las celdas
	{
		_solved = true; // Marca como resuelto
		_solution = board; // Almacena la solución
		return;
    }
    
	int row_next = (abs_index + 1) / BOARD_SIZE; // Calcula la fila de la siguiente celda
	int col_next = (abs_index + 1) % BOARD_SIZE; // Calcula la columna de la siguiente celda

	if (!isEmpty(board, row, col)) // Si la celda no está vacía
	{   
		solve_bruteforce_par(board, row_next, col_next); // Llama recursivamente a la siguiente celda
    }
	else
	{
		// Rellena todas las posibles cifras
        for (int num = board.get_min_value(); num <= board.get_max_value(); ++num)
		{
			Position pos = std::make_pair(row, col); // Crea la posición de la celda

            if (isValid(board, num, pos)) // Verifica si el número es válido 
			{
				// Evita crear nuevas tareas si estamos demasiado profundos en la recursión.
				// Alternativamente, podemos usar la cláusula final en la directiva #pragma omp task
				// Si no establecemos un umbral, la carga de trabajo de una sola tarea es demasiado pequeña
				// y el costo de crear tareas se vuelve significativo en comparación con la carga de trabajo.
				if (_recursionDepth > BOARD_SIZE)
				{
					board.set_board_data(row, col, num); // Establece el número en la celda

					if (isUnique(board, num, pos)) { num = BOARD_SIZE + 1; }   // Forzar a salir del bucle for

					// Intenta la siguiente celda recursivamente
					solve_bruteforce_seq(board, row_next, col_next);
				}
				else
				{
					// Necesitamos trabajar sobre una copia del tablero de Sudoku
					SudokuBoard local_board(board); // Crea una copia del tablero
					local_board.set_board_data(row, col, num); // Establece el número en la celda de la copia

					if (isUnique(board, num, pos)) { num = BOARD_SIZE + 1; }   // Forzar a salir del bucle for

					// Intenta la siguiente celda recursivamente
					#pragma omp task default(none) firstprivate(local_board, row_next, col_next) // Crea una tarea paralela
					solve_bruteforce_par(local_board, row_next, col_next); // Llama a la función en paralelo

					// board.set_board_data(row, col, board.get_empty_cell_value()); // No es necesario ya que no modificamos nada en el tablero original
				}
            }
        }
    }
	
	_recursionDepth++; // Incrementa la profundidad de recursión
}
