/**
 * @file Node.cpp
 * @brief Implementacja klas wezlow drzewa Huffmana.
 *
 * Plik zawiera implementacje metod klas Node, LeafNode oraz Branch zdefiniowanych w Node.h.
 */
#include "Node.h"
#include <stdexcept>

Node::Node(int freq) : frequency(freq) {}

int Node::getFrequency() const {
	return frequency;
}

LeafNode::LeafNode(uint32_t sym, int freq) : Node(freq), symbol(sym) {}

bool LeafNode::isLeaf() const {
	return true;             // zawsze bedzie true 
}

uint32_t LeafNode::getSymbol() const {
	return symbol;
}

Branch::Branch(std::unique_ptr<Node> l, std::unique_ptr<Node> r) : Node(l->getFrequency() + r->getFrequency()), left(std::move(l)), right(std::move(r)) {}

bool Branch::isLeaf() const {
	return false;      // zawsze bedzie false bo to galazka
}

uint32_t Branch::getSymbol() const {
	throw std::logic_error("Galaz nie ma znaku!");
}