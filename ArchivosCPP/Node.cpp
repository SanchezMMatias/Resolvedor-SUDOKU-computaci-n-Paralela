#include "Node.hpp"

// Constructor por defecto: los punteros de un nodo se apuntan a sí mismos
DancingNode::DancingNode()
{
	left = this;
	right = this;
	top = this;
	bottom = this;
}

// Constructor que inicializa el nodo columna, reutilizando el constructor por defecto
DancingNode::DancingNode(ColumnNode* c)
	: DancingNode()
{
	column = c;
}

// Método que enlaza este nodo con otro en la dirección inferior
DancingNode* DancingNode::linkDown(DancingNode* node)
{
	node->bottom = bottom; // Conecta el nodo nuevo al nodo inferior
	node->bottom->top = node; // Ajusta el puntero superior del nodo inferior
	node->top = this; // Conecta el nodo nuevo con este nodo
	bottom = node; // Actualiza el nodo inferior
	return node;
}

// Método que enlaza este nodo con otro en la dirección derecha
DancingNode* DancingNode::linkRight(DancingNode* node)
{
	node->right = right; // Conecta el nodo nuevo al nodo derecho
	node->right->left = node; // Ajusta el puntero izquierdo del nodo derecho
	node->left = this; // Conecta el nodo nuevo con este nodo
	right = node; // Actualiza el nodo derecho
	return node;
}

// Elimina este nodo de la lista en las direcciones izquierda-derecha
void DancingNode::removeLeftRight()
{
	left->right = right; // El nodo a la izquierda apunta al nodo a la derecha
	right->left = left; // El nodo a la derecha apunta al nodo a la izquierda
}

// Reinserta este nodo en la lista en las direcciones izquierda-derecha
void DancingNode::reinsertLeftRight()
{
	left->right = this; // El nodo a la izquierda vuelve a apuntar a este nodo
	right->left = this; // El nodo a la derecha vuelve a apuntar a este nodo
}

// Elimina este nodo de la lista en las direcciones arriba-abajo
void DancingNode::removeTopBottom()
{
	top->bottom = bottom; // El nodo superior apunta al nodo inferior
	bottom->top = top; // El nodo inferior apunta al nodo superior
}

// Reinserta este nodo en la lista en las direcciones arriba-abajo
void DancingNode::reinsertTopBottom()
{
	top->bottom = this; // El nodo superior vuelve a apuntar a este nodo
	bottom->top = this; // El nodo inferior vuelve a apuntar a este nodo
}

// Constructor para un nodo columna, inicializa el nombre y columna, y establece el tamaño en 0
ColumnNode::ColumnNode(std::string n)
	: DancingNode()
{
	size = 0;
	name = n;
	column = this; // Apunta a sí mismo ya que es un nodo columna
}

// Método para cubrir una columna, removiendo todos los nodos en esa columna
void ColumnNode::cover()
{
	removeLeftRight(); // Elimina el nodo columna de la lista izquierda-derecha
	for (DancingNode* i = bottom; i != this; i = i->bottom) // Itera sobre los nodos de abajo hacia arriba
	{
		for (DancingNode* j = i->right; j != i; j = j->right) // Itera sobre los nodos de derecha a izquierda
		{
			j->removeTopBottom(); // Elimina el nodo de la lista arriba-abajo
			j->column->size--; // Disminuye el tamaño de la columna correspondiente
		}
	}
}

// Método para descubrir una columna, reinsertando todos los nodos en esa columna
void ColumnNode::uncover()
{
	for (DancingNode* i = top; i != this; i = i->top) // Itera sobre los nodos de arriba hacia abajo
	{
		for (DancingNode* j = i->left; j != i; j = j->left) // Itera sobre los nodos de izquierda a derecha
		{
			j->column->size++; // Aumenta el tamaño de la columna correspondiente
			j->reinsertTopBottom(); // Reinsere el nodo en la lista arriba-abajo
		}
	}
	reinsertLeftRight(); // Reinsere el nodo columna en la lista izquierda-derecha
}
