/**
 * @file UTF8Utils.h
 * @brief Funkcje pomocnicze do konwersji miedzy UTF-8 a codepointami Unicode.
 */

#ifndef UTF8_UTILS_H
#define UTF8_UTILS_H

#include <vector>
#include <string>
#include <cstdint>

 /**
  * @brief Konwertuje string UTF-8 na wektor codepointow Unicode.
  * @param utf8 String zakodowany w UTF-8.
  * @return Wektor codepointow (uint32_t).
  */
std::vector<uint32_t> utf8_to_codepoints(const std::string& utf8);

/**
 * @brief Konwertuje wektor codepointow Unicode na string UTF-8.
 * @param codepoints Wektor codepointow do konwersji.
 * @return String zakodowany w UTF-8.
 */
std::string codepoints_to_utf8(const std::vector<uint32_t>& codepoints);

#endif