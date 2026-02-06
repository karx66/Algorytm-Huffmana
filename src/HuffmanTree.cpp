/**
 * @file HuffmanTree.cpp
 * @brief Implementacja klasy HuffmanTree.
 */

#include "HuffmanTree.h"
#include <queue>
#include <stdexcept>
#include <vector>

HuffmanTree::HuffmanTree() : root(nullptr) {}  //ustawiam korzen na pusty


void HuffmanTree::build(const std::vector<uint32_t>& codepoints)
{

	if (codepoints.empty()) {
		throw std::runtime_error("Tekst nie moze byc pusty!");
	}

	std::map<uint32_t, int> frequency;
	for (uint32_t cp : codepoints) {
		frequency[cp]++;
	}

	auto compare = [](Node* left, Node* right) {
		return left->getFrequency() > right->getFrequency(); // > zeby byl najmniejszy na gorze
		};

	std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> p_q(compare);   //min heap

	for (const auto& pair : frequency) {
		p_q.push(new LeafNode(pair.first, pair.second));  //klucz-codepoint, wartosc-czestosc
	}

	while (p_q.size() > 1) {   // dopoki wiecej niz jeden wezel jest w kolejce
		Node* left = p_q.top();
		p_q.pop();
		// bierze lewy bierze prawy tworzymy rodzica z tych dwoch i potem rodzic bedzie sie laczyc z nastepnym lisciem w kolejce
		Node* right = p_q.top();
		p_q.pop();

		Node* parent = new Branch(
			std::unique_ptr<Node>(left),    //inteligentne wskazniki
			std::unique_ptr<Node>(right)
		);
		p_q.push(parent);
	}

	if (!p_q.empty()) {
		root.reset(p_q.top());
		p_q.pop();
	}

	generateCodes();
}


void HuffmanTree::generateCodes() {
	codes.clear();
	if (root) {
		generateCodesHelper(root.get(), "");
	}
}


void HuffmanTree::generateCodesHelper(Node* node, const std::string& code) {
	if (!node) {
		return;
	}

	if (node->isLeaf()) {
		codes[node->getSymbol()] = code.empty() ? "0" : code;  // blokada gdy kod sklada sie tylko z jednego znaku. Czy kod jest pusty jesli tak daj 0 jesli nie podaj np. [a] = "1" 
		return;
	}

	Branch* internal = dynamic_cast<Branch*>(node);   //jest node jest branch to internal wskazuje na branch jesli nie daje nullptr
	if (internal) {
		if (internal->left) {
			generateCodesHelper(internal->left.get(), code + "0");
		}
		if (internal->right) {
			generateCodesHelper(internal->right.get(), code + "1");
		}
	}
}


std::string HuffmanTree::encode(const std::vector<uint32_t>& codepoints) const {
	std::string result;   // tu kody bitowe

	for (uint32_t cp : codepoints) {          // cp = 97 ('a') itd...
		auto it = codes.find(cp);     //szukanie kodu dla danego znaku w slowniku. Klucz to 97 a wartosc to 11
		if (it != codes.end()) {
			result += it->second;  //jezeli koniec dodaj to co masz do ci¹gu
		}
		else {
			throw std::runtime_error("Znak nie znajduje siê w drzewie!");
		}
	}

	return result;

}


std::vector<uint32_t> HuffmanTree::decode(const std::string& bits) const {
	if (!root) {
		throw std::runtime_error("Drzewo jest puste!");
	}

	std::vector<uint32_t> result;  // tu odkodowane codepointsy
	Node* current = root.get();    // zaczynamy od korzenia, current to aktualne miejsce podczas przechodzenia przez drzewo

	for (char bit : bits) {
		if (bit != '0' && bit != '1') {
			throw std::runtime_error("Nieprawidlowy bit! Musi byc 1 badz 0");
		}

		if (current->isLeaf()) {
			result.push_back(current->getSymbol());  //zwracamy codepoint z liscia i dodajemy go do wektora
			current = root.get();       // RESET wracamy do korzenia
		}

		Branch* internal = dynamic_cast<Branch*>(current);  //jesli current jest branch to internal wskazuje na branch   robimy to by miec dostep do left right
		if (internal) {
			if (bit == '0') {
				current = internal->left.get();
			}
			else {
				current = internal->right.get();
			}

			if (!current) {
				throw std::runtime_error("Nieprawidlowa sciezka!");
			}
		}
	}

	if (current && current->isLeaf()) {
		result.push_back(current->getSymbol());  // dodaj ostatni znak do wyniku
	}
	else if (current != root.get()) {
		throw std::runtime_error("Niepelny kod!");
	}


	return result;
}


std::map<uint32_t, std::string> HuffmanTree::getCodes() const {
	return codes;
}


bool HuffmanTree::isEmpty() const {
	return root == nullptr;
}