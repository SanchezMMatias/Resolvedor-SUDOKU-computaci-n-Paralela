#ifndef HELPER_HPP
#define HELPER_HPP

// =========================================================================
#include <iomanip>    
#include <sstream>    
#include <string>     

// Operador para repetir una cadena un número dado de veces
inline std::string operator* (const std::string& str, size_t times) {
    std::stringstream stream;
    for (size_t i = 0; i < times; i++) stream << str;
    return stream.str();
}

// =========================================================================
#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define BAR_WIDTH 70

// Función para imprimir una barra de progreso (versión 1)
inline void printProgressBar1(double percentage) {
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * BAR_WIDTH);
    int rpad = BAR_WIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}

// Función para imprimir una barra de progreso (versión 2)
inline void printProgressBar2(double percentage) {
    std::cout << "[";
    int pos = BAR_WIDTH * percentage;
    for (int i = 0; i < BAR_WIDTH; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(percentage * 100.0) << " %\r";
    std::cout.flush();
}

// =========================================================================
#include <random>     // Incluir biblioteca para generación aleatoria
#include <iterator>   // Incluir biblioteca para iteradores

// Selector aleatorio de un elemento de un contenedor
template <typename RandomGenerator = std::default_random_engine>
struct random_selector {
    // En la mayoría de las plataformas, probablemente se quiera usar std::random_device("/dev/urandom")()
    random_selector(RandomGenerator g = RandomGenerator(std::random_device()())) : gen(g) { }

    // Función para seleccionar un iterador aleatorio entre dos iteradores
    template <typename Iter>
    Iter select(Iter start, Iter end) {
        std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
        std::advance(start, dis(gen));
        return start;
    }

    // Función de conveniencia
    template <typename Iter>
    Iter operator() (Iter start, Iter end) {
        return select(start, end);
    }

    // Función de conveniencia que funciona con cualquier contenedor con begin() y end() razonables,
    // y devuelve una referencia al tipo de valor
    template <typename Container>
    auto operator() (const Container& c) -> decltype(*begin(c))& {
        return *select(begin(c), end(c));
    }

private:
    RandomGenerator gen;  // Generador aleatorio
};

// =========================================================================
#endif   // HELPER_HPP
