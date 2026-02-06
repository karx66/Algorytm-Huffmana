/**
 * @file UTF8Utils.cpp
 * @brief Implementacja funkcji konwersji UTF-8 i codepointow Unicode.
 */

#include "UTF8Utils.h"

 
std::vector<uint32_t> utf8_to_codepoints(const std::string& utf8) {
	std::vector<uint32_t> result;

	for (size_t i = 0; i < utf8.length(); ) {   //utf8 ma bajty o roznych dlugosciach dlatego musze zrobic bez i++
		uint32_t codepoint = 0;
		unsigned char c = utf8[i]; //c=current byte 

		if ((c & 0x80) == 0) {  //sprawdzamy najstarszy bit, jesli najstarszy to 0 to jest to ASCII - 1 bajtowy znak
			codepoint = c;
			i += 1;     //iteracja przeskakuje na nastepny znak
		}
		else if ((c & 0xE0) == 0xC0) {     //jesli pierwsze 3 bity to 110 - 2 bajtowy znak
			if (i + 1 >= utf8.length()) break;   //zabezpieczenie 

			codepoint = ((c & 0x1F) << 6) | (utf8[i + 1] & 0x3F);  //usuwamy ANDem prefiks pierwszego bajtu przesuwamy w lewo i bierzemy kolejny i robimy tak samo
			i += 2;
		}
		else if ((c & 0xF0) == 0xE0) {  //jesli pierwsze 4 bity to 1110 to jest to 3 bajtowy znak

			if (i + 2 >= utf8.length()) break;

			codepoint = ((c & 0x0F) << 12) | ((utf8[i + 1] & 0x3F) << 6) | (utf8[i + 2] & 0x3F);
			i += 3;
		}
		else if ((c & 0xF8) == 0xF0) {   //jesli pierwsze 5 bitow to 11110 to jest to 4 bitowy znak
			if (i + 3 >= utf8.length()) break;

			codepoint = ((c & 0x07) << 18) | ((utf8[i + 1] & 0x3F) << 12) | ((utf8[i + 2] & 0x3F) << 6) | (utf8[i + 3] & 0x3F);
			i += 4;
		}
		else {
			i += 1;   //nieprawidlowy utf8 pomin bajt idz dalej
			continue;
		}

		result.push_back(codepoint);

	}

	return result;
}


std::string codepoints_to_utf8(const std::vector<uint32_t>& codepoints) {
	std::string result;

	for (uint32_t codepoint : codepoints) {
		if (codepoint <= 0x7F) {      //0-127
			//1 bajt (ASCII)
			result += static_cast<char>(codepoint);  //dodaj do stringa
		}
		else if (codepoint <= 0x7FF) {  //128-2047 
			//2 bajty
			result += static_cast<char>(0xC0 | (codepoint >> 6));
			result += static_cast<char>(0x80 | (codepoint & 0x3F));
		}
		else if (codepoint <= 0xFFFF) {
			//3 bajty
			result += static_cast<char>(0xE0 | (codepoint >> 12));
			result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
			result += static_cast<char>(0x80 | (codepoint & 0x3F));
		}
		else if (codepoint <= 0x10FFFF) {  //max unicode
			//4 bajty
			result += static_cast<char>(0xF0 | (codepoint >> 18));
			result += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
			result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
			result += static_cast<char>(0x80 | (codepoint & 0x3F));
		}
	}

	return result;

}