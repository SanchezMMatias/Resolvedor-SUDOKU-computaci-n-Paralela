#ifndef NODE_HPP
#define NODE_HPP

#include <string>

// Forward declaration de la clase ColumnNode
class ColumnNode;

// Un objeto DancingNode se utiliza para modelar un nodo en la lista cuádruplemente enlazada para el problema de cobertura exacta.
class DancingNode {
public:
    DancingNode* left;   // Puntero al nodo de la izquierda
    DancingNode* right;  // Puntero al nodo de la derecha
    DancingNode* top;    // Puntero al nodo de arriba
    DancingNode* bottom; // Puntero al nodo de abajo
    ColumnNode* column;  // Puntero al nodo de la columna correspondiente

public:
    DancingNode();                    // Constructor por defecto
    DancingNode(ColumnNode* c);       // Constructor que inicializa con un nodo columna
    DancingNode* linkDown(DancingNode* node);   // Enlaza el nodo dado hacia abajo
    DancingNode* linkRight(DancingNode* node);  // Enlaza el nodo dado hacia la derecha
    void removeLeftRight();            // Elimina este nodo de la lista izquierda-derecha
    void reinsertLeftRight();          // Reinsertar este nodo en la lista izquierda-derecha
    void removeTopBottom();            // Elimina este nodo de la lista arriba-abajo
    void reinsertTopBottom();          // Reinsertar este nodo en la lista arriba-abajo
};

// Un objeto ColumnNode se utiliza para enlazar el nodo actual a la columna correspondiente en la matriz de cobertura.
class ColumnNode : public DancingNode {
public:
    int size;           // Tamaño de la columna (número de nodos en la columna)
    std::string name;   // Nombre de la columna

public:
    ColumnNode(std::string n);   // Constructor que inicializa con un nombre de columna
    void cover();                // Cubrir esta columna
    void uncover();              // Descubrir esta columna
};

#endif  // NODE_HPP
