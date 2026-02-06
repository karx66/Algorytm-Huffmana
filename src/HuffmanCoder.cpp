/**
 * @file HuffmanCoder.cpp
 * @brief Implementacja klasy HuffmanCoder do kompresji i dekompresji plikow.
 */

#include "HuffmanCoder.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

HuffmanCoder::HuffmanCoder() : data(false) {}

std::string HuffmanCoder::readFile(const std::string& filename) {
	std::ifstream file(filename);

	if (!file) {
		throw std::runtime_error("Nie mozna otworzyc pliku: " + filename);
	}

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	file.close();

	if (content.empty()) {
		throw std::runtime_error("Zawartosc pliku jest pusta: " + filename);
	}

	return content;
}

void HuffmanCoder::writeFile(const std::string& filename, const std::string& content) {
	std::ofstream file(filename);

	if (!file) {
		throw std::runtime_error("Nie mozna uwtorzyc pliku: " + filename);
	}

	file << content;
	file.close();
}

void HuffmanCoder::writeBinaryFile(const std::string& filename, const std::string& bits) {
	std::ofstream file(filename, std::ios::binary);

	if (!file) {
		throw std::runtime_error("Nie mozna utworzyc pliku binarnego: " + filename);
	}

	unsigned char bajt = 0;    // unsigned char bo chcemy tylko liczby dodatnie
	int licznik = 0;
	size_t totalBits = bits.length();

	file.write(reinterpret_cast<const char*>(&totalBits), sizeof(totalBits));

	for (char bit : bits) {
		bajt = (bajt << 1) | (bit - '0');  //zamieniaimy tekstowa cyfre na wartosc bitowa i wrzucamy ja do ciagu
		licznik++;
		if (licznik == 8) {
			file.write(reinterpret_cast<const char*>(&bajt), 1);   //zapisujemy jeden pelny ciag 8 bitow jako jeden bajt i idziemy dalej
			bajt = 0;
			licznik = 0;
		}
	}

	if (licznik > 0) {
		bajt = bajt << (8 - licznik);       //zabezpieczenie by bity przesunac do lewej
		file.write(reinterpret_cast<const char*>(&bajt), 1);
	}

	file.close();
}

std::string HuffmanCoder::readBinaryFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary);

	if (!file) {
		throw std::runtime_error("Nie mozna otworzyc pliku binarnego: " + filename);
	}

	size_t totalBits = 0;
	file.read(reinterpret_cast<char*>(&totalBits), sizeof(totalBits));

	std::string bits;
	bits.reserve(totalBits);    //przygotowanie miejsca na duza liczbe znakow

	unsigned char bajt;
	size_t bitsRead = 0;

	while (file.read(reinterpret_cast<char*>(&bajt), 1) && bitsRead < totalBits) {   //pobieramy po jednym bajcie i potem gdy jest padding to przerywamy
		for (int i = 7; i >= 0 && bitsRead < totalBits; i--) {   //robimy iteracje w tyl zeby dostac bity w odpowiedniej kolejnosci
			int obecnyBit = (bajt >> i) & 1;  //przesuwamy bit do prawej strony, sprawdzamy za pomoca AND czy to jest 1
			if (obecnyBit == 1) {
				bits += '1';
			}
			else {
				bits += '0';
			}
			bitsRead++;
		}
	}
	
	file.close();
	return bits;
}

void HuffmanCoder::DisplayStatsEncode(int originalSize, int compressedBits) {
	int originalBits = originalSize * 8;
	double compressionStats = 100.0 * (originalBits - compressedBits) / originalBits;

	std::cout << "=== STATYSTYKI ===" << std::endl;
	std::cout << "Rozmiar przed:  " << originalBits << " bitow" << std::endl;
	std::cout << "Rozmiar po:  " << compressedBits << " bitow" << std::endl;
	std::cout << "Wykonana kompresja:  " << std::fixed << std::setprecision(2) << compressionStats << "%" << std::endl;

}

void HuffmanCoder::DisplayStatsDecode(int compressedBits, int decodedSize) {
	int decodedBits = decodedSize * 8;

	std::cout << "=== STATYSTYKI ===" << std::endl;
	std::cout << "Rozmiar przed:  " << compressedBits << " bitow skompresowanych" << std::endl;
	std::cout << "Rozmiar po:  " << decodedBits << " bitow odkodowanych" << std::endl;
}

void HuffmanCoder::encodeFile(const std::string& inputFile, const std::string& outputFile, bool showStats) {
	try {
		std::string utf8 = readFile(inputFile);
		std::vector<uint32_t> codepoints = utf8_to_codepoints(utf8);
		lastCodepoints = codepoints;

		if (codepoints.empty()) {
			throw std::runtime_error("Nieprawidlowe znaki!");
		}

		tree.build(codepoints);
		lastEncodedBits = tree.encode(codepoints);
		data = true;

		std::ostringstream output;

		auto codes = tree.getCodes();
		output << "ZLICZONE_KODY: " << codes.size() << std::endl;
		output << "=== KODY ===" << std::endl;

		for (const auto& pair : codes) {
			uint32_t cp = pair.first;
			std::vector<uint32_t> single = { cp };  // musi byc wektor nie pojedyncza liczba
			std::string utf8Char = codepoints_to_utf8(single);

			// Wyjątki
			if (cp == '\n') {
				output << "[NEWLINE] " << pair.second << std::endl;
			}
			else if (cp == '\t') {
				output << "[TAB] " << pair.second << std::endl;
			}
			else if (cp == ' ') {
				output << "[SPACE] " << pair.second << std::endl;
			}
			else if (cp < 32 || cp == 127) {
				output << "[CTRL_" << cp << "] " << pair.second << std::endl;
			}
			else {
				output << utf8Char << " " << pair.second << std::endl;
			}
		}

		writeFile(outputFile, output.str());

		std::string binaryFile = outputFile.substr(0, outputFile.find_last_of('.')) + ".bin";
		writeBinaryFile(binaryFile, lastEncodedBits);

		std::cout << "Plik zostal zakodowany" << std::endl;
		std::cout << "Slownik zapisano do: " << outputFile << std::endl;
		std::cout << "Dane binarne zapisano do: " << binaryFile << std::endl;

		if (showStats) {
			DisplayStatsEncode(utf8.length(), lastEncodedBits.length());
		}
	}
	catch (const std::exception& error) {
		throw std::runtime_error("Blad w kodowaniu: " + std::string(error.what()));  //lapie wszystkie inne errory przez co na poczatku dodajemy blad kodowania a potem faktyczny error
	}

}

void HuffmanCoder::decodeFile(const std::string& inputFile, const std::string& outputFile, bool showStats) {
	try {
		std::ifstream file(inputFile);

		if (!file) {
			throw std::runtime_error("Nie mozna otworzyc pliku: " + inputFile);
		}

		std::string line;

		std::getline(file, line);
		if (line.find("ZLICZONE_KODY:") == std::string::npos) {  //npos to "no position" czyli gdy nie znaleziona danej pozycji
			throw std::runtime_error("Nieprawidlowy format! Plik nie jest zakodowanym plikiem .huf. ");
		}

		int codesCount = std::stoi(line.substr(line.find(':') + 2)); // do odczytania ile jest kodow do przejscia petla (dochodzi do : daje +2 i odczytuje liczbe kodow)
		std::getline(file, line);
		std::map<std::string, uint32_t> reverseCodes;     //w dekodowaniu robimy map na odwrot jak w encode dlatego robimy reverse

		for (int i = 0; i < codesCount; i++) {
			std::getline(file, line);

			size_t space = line.find(' ');    //zdefiniuj spacje
			std::string symbol = line.substr(0, space);   //zdefiniuj symbol 
			std::string code = line.substr(space + 1);    //zdefiniuj kod

			if (symbol == "[NEWLINE]") {
				reverseCodes[code] = '\n';
			}
			else if (symbol == "[TAB]") {
				reverseCodes[code] = '\t';
			}
			else if (symbol == "[SPACE]") {
				reverseCodes[code] = ' ';
			}
			else if (symbol.substr(0, 6) == "[CTRL_") {
				std::string number_string = symbol.substr(6, symbol.length() - 7);  //wycinamy tylko liczbe z ctrl
				uint32_t codepoint = std::stoi(number_string);   //zamieniamy stringa liczbowego na faktyczne liczbe w int
				reverseCodes[code] = codepoint;
			}
			else {
				std::vector<uint32_t> codepoints = utf8_to_codepoints(symbol);
				if (!codepoints.empty()) {
					reverseCodes[code] = codepoints[0];
				}
			}
		}

		file.close();

		std::string binaryFile = inputFile.substr(0, inputFile.find_last_of('.')) + ".bin";
		std::string encodedBits = readBinaryFile(binaryFile);

		std::vector<uint32_t> decodedCodepoints;
		std::string buffer;

		for (char bit : encodedBits) {
			buffer += bit;

			auto dictionary = reverseCodes.find(buffer); //szukaj w slowniku bit/bity z bufora
			if (dictionary != reverseCodes.end()) {  //gdy znaleziono w slowniku wskazujemy na druga czesc czyli nasz codepoint
				decodedCodepoints.push_back(dictionary->second);
				buffer.clear();
			}
		}


		std::string decodedText = codepoints_to_utf8(decodedCodepoints);    //konwertujemy z powrotem na utf
		writeFile(outputFile, decodedText);

		std::cout << "Plik odkodowano pomyslnie" << std::endl;

		if (showStats) {
			DisplayStatsDecode(encodedBits.length(), decodedText.length());
		}


	}
	catch (const std::exception& error) {
		throw std::runtime_error("Blad w dekodowaniu: " + std::string(error.what()));
	}
}

void HuffmanCoder::analyzeFile(const std::string& inputFile) {
	try {
		std::string utf8Text = readFile(inputFile);
		std::vector<uint32_t> codepoints = utf8_to_codepoints(utf8Text);
		tree.build(codepoints);
		std::string encoded = tree.encode(codepoints);

		int originalBits = utf8Text.length() * 8;

		std::cout << "=== ANALIZA ===" << std::endl;
		std::cout << "Rozmiar pliku:  " << originalBits << " bitow" << std::endl;
		std::cout << "Liczba znakow:  " << codepoints.size() << " znakow" << std::endl;
		std::cout << "Uniklanych znakow:  " << tree.getCodes().size() << std::endl;


		int compressedBits = encoded.length();
		double compressionStats = 100.0 * (originalBits - compressedBits) / originalBits;

		std::cout << "Przewidywana kompresja: " << std::fixed << std::setprecision(2) << compressionStats << "%" << std::endl;

	}
	catch (const std::exception& error) {
		throw std::runtime_error("Blad w analizie: " + std::string(error.what()));
	}
}