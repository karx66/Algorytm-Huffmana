/**
 * @file Node.h
 * @brief Definicje klas wezlow drzewa Huffmana.
 *
 * Plik zawiera abstrakcyjna klase bazowa Node oraz klasy pochodne
 * LeafNode (lisc) i Branch (galaz) wykorzystywane do budowy drzewa Huffmana.
 * Implementacja polimorfizmu poprzez metody wirtualne.
 */
#ifndef NODE_H
#define NODE_H
#include <memory>
#include <cstdint>
 /**
  * @class Node
  * @brief Abstrakcyjna klasa bazowa reprezentujaca wezel drzewa Huffmana.
  *
  * Klasa stanowi interfejs dla wezlow drzewa Huffmana. 
  * Definiuje metody czysto wirtualne isLeaf() i getSymbol() ktore musza byc zaimplementowane przez klasy pochodne.
  * Wykorzystuje polimorfizm do obslugi roznych typow wezlow.
  */
class Node {
protected:
	int frequency;    // w klasie i w klasach dziedziczacych
public:
	/**
	 * @brief Konstruktor klasy Node.
	 * @param freq Czestotliwosc wystepowania znaku/wezla.
	 */
	Node(int freq);
	/**
	 * @brief Wirtualny destruktor.
	 */
	virtual ~Node() = default;
	/**
	 * @brief Sprawdza czy wezel jest lisciem.
	 * @return true jesli wezel jest lisciem, false jesli nie jest.
	 */
	virtual bool isLeaf() const = 0;
	/**
	 * @brief Pobiera symbol (codepoint UTF-8) przechowywany w wezle.
	 * @return Codepoint znaku jako uint32_t.
	 * @throws std::logic_error Gdy wywolane na wezle niebedacym lisciem.
	 */
	virtual uint32_t getSymbol() const = 0;
	/**
	 * @brief Pobiera czestotliwosc wezla.
	 * @return Czestotliwosc wystepowania jako int.
	 */
	int getFrequency() const;
};
/**
 * @class LeafNode
 * @brief Klasa reprezentujaca lisc drzewa Huffmana.
 *
 * Lisc przechowuje pojedynczy znak (codepoint UTF-8) oraz jego czestotliwosc wystepowania w tekscie.
 * Dziedziczy po klasie Node.
 */
class LeafNode : public Node {  // dziedziczenie public Node
private:
	uint32_t symbol;
public:
	/**
	 * @brief Konstruktor liscia.
	 * @param sym Codepoint znaku (UTF-8).
	 * @param freq Czestotliwosc wystepowania znaku.
	 */
	LeafNode(uint32_t sym, int freq);
	/**
	 * @brief Sprawdza czy wezel jest lisciem.
	 * @return Zawsze true dla LeafNode.
	 */
	bool isLeaf() const override;
	/**
	 * @brief Pobiera symbol przechowywany w lisciu.
	 * @return Codepoint znaku jako uint32_t.
	 */
	uint32_t getSymbol() const override;
};
/**
 * @class Branch
 * @brief Klasa reprezentujaca galaz drzewa Huffmana.
 *
 * Galaz laczy dwa wezly potomne (lewy i prawy) i przechowuje sume ich czestotliwosci.
 * Uzywana do budowy struktury drzewa Huffmana.
 * Lewe dziecko odpowiada bitowi 0, prawe dziecko bitowi 1.
 */
class Branch : public Node {
public:
	std::unique_ptr<Node> left;  // lewo = 0
	std::unique_ptr<Node> right;  // prawo = 1
	/**
	 * @brief Konstruktor galezi.
	 * @param l Wskaznik na lewe dziecko (unique_ptr).
	 * @param r Wskaznik na prawe dziecko (unique_ptr).
	 */
	Branch(std::unique_ptr<Node> l, std::unique_ptr<Node> r);
	/**
	 * @brief Sprawdza czy wezel jest lisciem.
	 * @return Dla branch zawsze bedzie false.
	 */
	bool isLeaf() const override;
	/**
	 * @brief Proba pobrania symbolu z galezi.
	 * @return Nie zwraca wartosci - zawsze rzuca wyjatek.
	 * @throws std::logic_error Zawsze, poniewaz galaz nie przechowuje symbolu.
	 */
	uint32_t getSymbol() const override;
};
#endif