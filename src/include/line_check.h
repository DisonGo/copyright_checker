#ifndef SRC_INCLUDE_LINE_CHECK_H
#define SRC_INCLUDE_LINE_CHECK_H

#include <fstream>
#include <iostream>
#include <string>

int GetMatchPercentage(std::vector<std::string>& ref,
                       std::vector<std::string>& check);
int GetLineInfo(const std::string& reference_file,
                const std::string& checked_file);
std::string NormalizeString(const std::string& str);
inline bool IsForbidden(const char symbol);
inline bool IsPreProcessing(const std::string& str);

#endif  //  SRC_INCLUDE_LINE_CHECK_H