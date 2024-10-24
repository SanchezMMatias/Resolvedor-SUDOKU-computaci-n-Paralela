#include "SudokuBoard.hpp"
#include "SudokuTest.hpp"
#include "SudokuSolver.hpp"
#include "SudokuSolver_SequentialBacktracking.hpp"
#include "SudokuSolver_SequentialBruteForce.hpp"
#include "SudokuSolver_ParallelBruteForce.hpp"
#include "SudokuSolver_SequentialDLX.hpp"
#include "SudokuSolver_ParallelDLX.hpp"
#include "SudokuSolver_SequentialForwardChecking.hpp"


#include "termcolor.hpp"
#include <windows.h>
#include <iostream>
#include <chrono>
#include <omp.h>
#include <memory>
#include <fstream>  // Para manejar archivos

#define PRINT_TIME 1

using namespace std;

void mostrarMenu() {
    cout << "Opciones disponibles para <MODO>:\n";
    cout << "0: modo secuencial con algoritmo de backtracking\n";
    cout << "1: modo secuencial con algoritmo de fuerza bruta\n";
    cout << "2: modo paralelo con algoritmo de fuerza bruta\n";
    cout << "3: modo secuencial con algoritmo DLX\n";
    cout << "4: modo paralelo con algoritmo DLX\n";
    cout << "5: modo secuencial con algoritmo de chequeo hacia adelante\n";
}

// Función para mostrar el submenú de selección de tamaño y dificultad
string seleccionarCaso() {
    int tamano, dificultad;

    cout << "Selecciona el tamaño del Sudoku:\n";
    cout << "1: 9x9\n";
    cout << "2: 16x16\n";
    cout << "3: 25x25\n";
    cout << "Selecciona una opción: ";
    cin >> tamano;

    cout << "Selecciona la dificultad del Sudoku:\n";
    cout << "1: Easy\n";
    cout << "2: Medium\n";
    cout << "3: Hard\n";
    cout << "Selecciona una opción: ";
    cin >> dificultad;

    string archivo;
    switch (tamano) {
        case 1: archivo = "9x9_"; break;
        case 2: archivo = "16x16_"; break;
        case 3: archivo = "25x25_"; break;
        default: archivo = "9x9_"; break;  // Valor por defecto
    }

    switch (dificultad) {
        case 1: archivo += "easy"; break;
        case 2: archivo += "medium"; break;
        case 3: archivo += "hard"; break;
        default: archivo += "easy"; break;  // Valor por defecto
    }

    return "..\\Test_Cases\\" + archivo + ".txt";  // Ruta correcta a la carpeta
}

// Función para verificar si el archivo existe
bool archivoExiste(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    return archivo.good();
}

unique_ptr<SudokuSolver> CreateSudokuSolver(MODES mode, SudokuBoard& board) {
    switch (mode) {
        case MODES::SEQUENTIAL_BACKTRACKING:
            return make_unique<SudokuSolver_SequentialBacktracking>(board);
            
        case MODES::SEQUENTIAL_BRUTEFORCE:
            return make_unique<SudokuSolver_SequentialBruteForce>(board);
        
        case MODES::PARALLEL_BRUTEFORCE:
            return std::make_unique<SudokuSolver_ParallelBruteForce>(board);
       
        case MODES::SEQUENTIAL_DANCINGLINKS:
            return std::make_unique<SudokuSolver_SequentialDLX>(board);

		case MODES::PARALLEL_DANCINGLINKS:
            return std::make_unique<SudokuSolver_ParallelDLX>(board);
 
		case MODES::SEQUENTIAL_FORWARDCHECKING:
            return std::make_unique<SudokuSolver_SequentialForwardChecking>(board);
		default:
            cerr << termcolor::red << "Available options for <MODE>: " << "\n";
            cerr << "    - 0: sequential mode with backtracking algorithm" << "\n";
            cerr << "	 - 1: sequential mode with brute force algorithm" << "\n";
            cerr << "	 - 2: parallel mode with brute force algorithm" << "\n";
			cerr << "		- 3: sequential mode with DLX algorithm" << "\n";
			cerr << "		- 4: parallel mode with DLX algorithm" << "\n";
			cerr << "		- 5: sequential mode with forward checking algorithm" << "\n";
			cerr << "Please try again." << termcolor::reset << "\n";
			
            exit(-1);
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    int choice;

    while (true) {
        cout << "\n"
             << R"(
███████╗██╗   ██╗██████╗  ██████╗ ██╗  ██╗██╗   ██╗    ███████╗ ██████╗ ██╗    ██╗   ██╗███████╗██████╗ 
██╔════╝██║   ██║██╔══██╗██╔═══██╗██║ ██╔╝██║   ██║    ██╔════╝██╔═══██╗██║    ██║   ██║██╔════╝██╔══██╗
███████╗██║   ██║██║  ██║██║   ██║█████╔╝ ██║   ██║    ███████╗██║   ██║██║    ██║   ██║█████╗  ██████╔╝
╚════██║██║   ██║██║  ██║██║   ██║██╔═██╗ ██║   ██║    ╚════██║██║   ██║██║    ╚██╗ ██╔╝██╔══╝  ██╔══██╗
███████║╚██████╔╝██████╔╝╚██████╔╝██║  ██╗╚██████╔╝    ███████║╚██████╔╝███████╗╚████╔╝ ███████╗██║  ██║
╚══════╝ ╚═════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═╝ ╚═════╝     ╚══════╝ ╚═════╝ ╚══════╝ ╚═══╝  ╚══════╝╚═╝  ╚═╝
             )"
             << "\n"
             << "developed by Matías Sánchez"
             << "\n\n\n";

        cout << termcolor::red << "Menú Principal\n" << termcolor::reset;
        mostrarMenu();
       cout << "Selecciona una opción: ";
cin >> choice;
 if (choice >= 0 && choice <= 5) {  // Maneja todas las opciones entre 0 y 5
            string archivo = seleccionarCaso();  // Selección del archivo
            cout << "Intentando abrir el archivo: " << archivo << endl;
            cout << "Ruta completa del archivo: " << archivo << endl;

            if (!archivoExiste(archivo)) {
                cout << termcolor::red << "Error abriendo el archivo: " << archivo << "\n" << termcolor::reset;
                continue;
            }

            auto board = SudokuBoard(archivo);  // Carga el archivo seleccionado
            SudokuTest::testBoard(board);

            MODES mode = static_cast<MODES>(choice);  // Asignar modo basado en opción elegida
            auto solver = CreateSudokuSolver(mode, board);
            solver->solve();

            if (solver->get_status()) {
                cout << termcolor::green << "Sudoku resuelto exitosamente:\n" << termcolor::reset;

                // Obtiene la solución resuelta y la imprime
                SudokuBoard solution = solver->get_solution();
                cout << solution;  // Asegúrate de que SudokuBoard tiene sobrecarga de << para imprimir
            } else {
                cout << termcolor::red << "No se pudo resolver el Sudoku.\n" << termcolor::reset;
            }
        } else {
            cout << termcolor::yellow << "Opción aún no implementada.\n" << termcolor::reset;
        }

        cout << "¿Quieres salir del programa? (s/n): ";
        char exitChoice;
        cin >> exitChoice;
        if (exitChoice == 's' || exitChoice == 'S') {
            cout << "Saliendo del programa..." << endl;
            break;
        }
    }

    return 0;
}
