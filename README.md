# Huffman Compression

Obiektowa implementacja algorytmu bezstratnej kompresji Huffmana w C++ z obsługą Unicode.

## Funkcje
- Kodowanie plików tekstowych do formatu .huf
- Dekodowanie plików .huf do tekstu
- Analiza przewidywanej kompresji
- Obsługa wszystkich znaków Unicode (UTF-8)
- Zapis binarny (prawdziwa kompresja)

## Użycie

Kodowanie:
```
.\huffman.exe -e -i plik.txt -o zakodowany.huf
```

Dekodowanie:
```
.\huffman.exe -d -i zakodowany.huf -o odkodowany.txt
```

Analiza:
```
.\huffman.exe -a -i plik.txt
```

## Technologie
- C++17
- Programowanie obiektowe (polimorfizm, dziedziczenie, enkapsulacja)
- Kontenery STL (map, vector, priority_queue)
- Inteligentne wskaźniki (unique_ptr)
- Wyrażenia lambda
- Semantyka przeniesienia (std::move)
