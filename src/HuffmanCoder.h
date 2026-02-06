/**
 * @file HuffmanCoder.h
 * @brief Definicja klasy HuffmanCoder do kompresji i dekompresji plikow.
 */
#ifndef HUFFMAN_CODER_H
#define HUFFMAN_CODER_H
#include "HuffmanTree.h"
#include "UTF8Utils.h"
#include <string>
#include <vector>
#include <cstdint>
 /**
  * @class HuffmanCoder
  * @brief Klasa obslugujaca kodowanie i dekodowanie plikow algorytmem Huffmana.
  */
class HuffmanCoder {
private:
	HuffmanTree tree;
	std::vector<uint32_t> lastCodepoints;
	std::string lastEncodedBits;
	bool data;
	/**
	 * @brief Wczytuje zawartosc pliku tekstowego.
	 * @param filename Sciezka do pliku.
	 * @return Zawartosc pliku jako string.
	 * @throws std::runtime_error Gdy nie mozna otworzyc pliku lub plik jest pusty.
	 */
	std::string readFile(const std::string& filename);
	/**
	 * @brief Zapisuje tekst do pliku.
	 * @param filename Sciezka do pliku wyjsciowego.
	 * @param content Tresc do zapisania.
	 * @throws std::runtime_error Gdy nie mozna utworzyc pliku.
	 */
	void writeFile(const std::string& filename, const std::string& content);
	/**
	 * @brief Zapisuje zakodowane bity do pliku binarnego.
	 * @param filename Sciezka do pliku wyjsciowego (.bin).
	 * @param bits Ciag bitow do zapisania (jako string zer i jedynek).
	 * @throws std::runtime_error Gdy nie mozna utworzyc pliku binarnego.
	 */
	void writeBinaryFile(const std::string& filename, const std::string& bits);
	/**
	 * @brief Odczytuje zakodowane bity z pliku binarnego.
	 * @param filename Sciezka do pliku binarnego (.bin).
	 * @return Ciag bitow jako string zer i jedynek.
	 * @throws std::runtime_error Gdy nie mozna otworzyc pliku binarnego.
	 */
	std::string readBinaryFile(const std::string& filename);
	/**
	 * @brief Wyswietla statystyki po kodowaniu.
	 * @param originalSize Rozmiar oryginalnego pliku w bajtach.
	 * @param compressedBits Liczba bitow po kompresji.
	 */
	void DisplayStatsEncode(int originalSize, int compressedBits);
	/**
	 * @brief Wyswietla statystyki po dekodowaniu.
	 * @param compressedBits Liczba bitow skompresowanych.
	 * @param decodedSize Rozmiar odkodowanego pliku w bajtach.
	 */
	void DisplayStatsDecode(int compressedBits, int decodedSize);
public:
	/**
	 * @brief Konstruktor domyslny.
	 */
	HuffmanCoder();
	/**
	 * @brief Koduje plik tekstowy algorytmem Huffmana.
	 * @param inputFile Sciezka do pliku wejsciowego.
	 * @param outputFile Sciezka do pliku wyjsciowego (.huf).
	 * @param show_stats wyswietlenie statystyk kompresji (domyslnie false).
	 * @throws std::runtime_error Gdy wystapi blad podczas kodowania.
	 */
	void encodeFile(const std::string& inputFile, const std::string& outputFile, bool show_stats = false);  // bool stats dajemy false zeby domyslnie ebz flagi bylo bez statystyk
	/**
	 * @brief Dekoduje plik .huf do pliku tekstowego.
	 * @param inputFile Sciezka do pliku .huf.
	 * @param outputFile Sciezka do pliku wyjsciowego.
	 * @param show_stats wyswietlenie statystyk kompresji (domyslnie false).
	 * @throws std::runtime_error Gdy wystapi blad podczas dekodowania lub nieprawidlowy format pliku.
	 */
	void decodeFile(const std::string& inputFile, const std::string& outputFile, bool show_stats = false);
	/**
	 * @brief Analizuje plik i wyswietla przewidywana kompresje.
	 * @param inputFile Sciezka do pliku do analizy.
	 * @throws std::runtime_error Gdy wystapi blad podczas analizy.
	 */
	void analyzeFile(const std::string& inputFile);
};
#endif