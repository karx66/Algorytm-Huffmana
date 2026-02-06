/**
 * @file main.cpp
 * @brief Glowny plik programu do kompresji i dekompresji plikow algorytmem Huffmana.
 *
 * Program konsolowy umozliwiajacy kompresje plikow tekstowych przy uzyciu algorytmu Huffmana.
 * Obsluguje kodowanie UTF-8 oraz rozne tryby dzialania dostepne przez parametry linii polecen.
  */

#include "HuffmanCoder.h"
#include <iostream>
#include <string>
#include <cstring>

 /**
  * @brief Wyswietla pomoc dotyczaca uzycia programu.
  *
  * Funkcja wypisuje na standardowe wyjscie informacje o dostepnych trybach dzialania, parametrach, opcjach oraz przyklady uzycia programu.
  * Wywolywana gdy program uruchomiony bez argumentow lub z flaga -h.
  */
void showHelp() {
    std::cout << "\n\n";
    std::cout << "      ALGORYTM HUFFMANA" << std::endl;
    std::cout << "\n\n";

    std::cout << "UZYCIE: " << std::endl;
    std::cout << "  .\projekt_huffman.exe [TRYBY/PARAMETRY/OPCJE...]\n\n";

    std::cout << "TRYBY DZIALANIA: " << std::endl;
    std::cout << "  -e, (encode)          Koduj plik tekstowy" << std::endl;
    std::cout << "  -d, (decode)          Odkoduj plik .huf" << std::endl;
    std::cout << "  -a, (analyze)         Analiza\n\n";

    std::cout << "PARAMETRY: " << std::endl;
    std::cout << "  -i, (input file)      Plik wejsciowy" << std::endl;
    std::cout << "  -o, (output file)     Plik wyjsciowy\n\n";

    std::cout << "OPCJE:" << std::endl;
    std::cout << "  -stats,  (stats)           Pokaz statystyki\n";
    std::cout << "  -h, (help)                Wyswietl pomoc\n\n";

    std::cout << "PRZYKLADY:\n\n";
    std::cout << "  Kodowanie:" << std::endl;
    std::cout << "    .\projekt_huffman.exe -e -i plik.txt -o skompresowane.huf" << std::endl;
    std::cout << "    .\projekt_huffman.exe -e -stats -i tekst.txt -o compressed.huf\n\n";

    std::cout << "  Dekodowanie:\n";
    std::cout << "    .\projekt_huffman.exe -d -i skompresowane.huf -o zdekodowane.txt" << std::endl;
    std::cout << "    .\projekt_huffman.exe -d -stats -i skompresowane.huf -o zdekodowane.txt\n\n";

    std::cout << "  Analiza:\n";
    std::cout << "    .\projekt_huffman -a -i plik.txt\n\n";

}

/**
 * @brief Glowna funkcja programu.
 *
 * Funkcja parsuje argumenty linii polecen i wykonuje odpowiednia operacje na podstawie przekazanych flag.
 *
 * Obslugiwane flagi:
 * - '-e' - tryb kodowania (encode)
 * - '-d' - tryb dekodowania (decode)
 * - '-a' - tryb analizy (analyze)
 * - '-i <plik>' - plik wejsciowy (input)
 * - '-o <plik>' - plik wyjsciowy (output)
 * - '-stats' - wyswietl statystyki kompresji
 * - '-h' - wyswietl pomoc
 *
 * @param arg_count Liczba argumentow przekazanych do programu (argc).
 * @param arg_vector Tablica wskaznikow do argumentow (argv).
 *
 * @exception std::exception Przechwytywane sa wyjatki standardowe, a komunikat bledu wypisywany jest w konsoli.      
 */
int main(int arg_count, char* arg_vector[]) {  //tablica zawiera wszystkie slowa z komendy

    if (arg_count == 1) {
        showHelp();
        return 0;
    }

    std::string mode;
    std::string inputFile;
    std::string outputFile;
    bool showStats = false;

    for (int i = 1; i < arg_count; i++) {
        std::string flag = arg_vector[i];

        if (flag == "-e") {
            mode = "encode";
        }
        else if (flag == "-d") {
            mode = "decode";
        }
        else if (flag == "-a") {
            mode = "analyze";
        }
        else if (flag == "-i") {
            if (i + 1 < arg_count) {     //wymagana nazwa pliku
                inputFile = arg_vector[++i];      // przypisuje do inputFile ten argument z nazwa pliku
            }
            else {
                std::cerr << "Blad: Brak nazwy pliku po " << flag << std::endl;
                return 1;
            }
        }
        else if (flag == "-o") {
            if (i + 1 < arg_count) {
                outputFile = arg_vector[++i];
            }
            else {
                std::cerr << "Blad: Brak nazwy pliku po " << flag << std::endl;
                return 1;
            }
        }
        else if (flag == "-stats") {
            showStats = true;
        }
        else if (flag == "-h") {
            showHelp();
            return 0;
        }
        else {
            std::cerr << "Blad: Nieznana opcja: " << flag << std::endl;
            std::cerr << "Wpisz -h by zobaczyc dostepne opcje." << std::endl;
        }
    }

    if (mode.empty()) {
        std::cerr << "Blad: Nie okreslono trybu dzialania" << std::endl;
        return 1;
    }
    if (inputFile.empty()) {
        std::cerr << "Blad: Nie podano pliku wejsciowego" << std::endl;
        return 1;
    }
    if ((mode == "encode" || mode == "decode") && outputFile.empty()) {
        std::cerr << "Blad: Nie podano pliku wyjsciowego" << std::endl;
        return 1;
    }

    try {
        HuffmanCoder coder;
        if (mode == "encode") {
            coder.encodeFile(inputFile, outputFile, showStats);
        }
        else if (mode == "decode") {
            coder.decodeFile(inputFile, outputFile, showStats);
        }
        else if (mode == "analyze") {
            coder.analyzeFile(inputFile);
        }
        return 0;
    }
    catch (const std::exception& error) {
        std::cerr << "Blad: " << error.what() << std::endl;
        return 1;
    }

}