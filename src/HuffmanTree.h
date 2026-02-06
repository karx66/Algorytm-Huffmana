/**
 * @file HuffmanTree.h
 * @brief Definicja klasy drzewa Huffmana.
 */

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "Node.h"
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <cstdint>

 /**
  * @class HuffmanTree
  * @brief Klasa reprezentujaca drzewo Huffmana do kompresji i dekompresji danych.
  */
class HuffmanTree {
private:
	std::unique_ptr<Node> root;
	std::map<uint32_t, std::string> codes;

	/**
	 * @brief Rekurencyjna funkcja pomocnicza do generowania kodow bitowych.
	 * @param node Wskaznik na aktualny wezel.
	 * @param code Dotychczasowy kod bitowy (w lewo 0, w prawo 1).
	 */
	void generateCodesHelper(Node* node, const std::string& code); //funkcja rekurencyjna do generowania kodow w prawo 1 w lewo 0

public:
	/**
	 * @brief Konstruktor domyslny inicjalizujacy puste drzewo.
	 */
	HuffmanTree();

	/**
	 * @brief Buduje drzewo Huffmana na podstawie wektora codepointow.
	 * @param codepoints Wektor codepointow UTF-8 do analizy czestotliwosci.
	 * @throws std::runtime_error Gdy wektor codepointow jest pusty.
	 */
	void build(const std::vector<uint32_t>& codepoints);

	/**
	 * @brief Generuje kody bitowe dla wszystkich znakow w drzewie.
	 */
	void generateCodes();

	/**
	 * @brief Koduje wektor codepointow na ciag bitow.
	 * @param codepoints Wektor codepointow do zakodowania.
	 * @return Ciag bitow jako string (np. "01101").
	 * @throws std::runtime_error Gdy znak nie istnieje w drzewie.
	 */
	std::string encode(const std::vector<uint32_t>& codepoints) const;

	/**
	 * @brief Dekoduje ciag bitow na wektor codepointow.
	 * @param bits Ciag bitow do odkodowania.
	 * @return Wektor odkodowanych codepointow UTF-8.
	 * @throws std::runtime_error Gdy drzewo jest puste, bit jest nieprawidlowy lub sciezka jest niepelna.
	 */
	std::vector<uint32_t> decode(const std::string& bits) const;

	/**
	 * @brief Zwraca mape kodow Huffmana (codepoint -> kod bitowy).
	 * @return Mapa przypisujaca codepointom ich kody bitowe.
	 */
	std::map<uint32_t, std::string> getCodes() const;

	/**
	 * @brief Sprawdza czy drzewo jest puste.
	 * @return true jesli drzewo nie ma korzenia, false w przeciwnym przypadku.
	 */
	bool isEmpty() const;
};

#endif