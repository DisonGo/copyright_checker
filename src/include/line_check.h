#ifndef SRC_INCLUDE_LINE_CHECK_H
#define SRC_INCLUDE_LINE_CHECK_H

#include <iostream>
#include <string>
#include <fstream>
#include <regex>

int GetMatchPercentage(std::vector<std::string>& ref, std::vector<std::string>& check);
int GetLineInfo(std::string reference_file, std::string checked_file);

#endif  //  SRC_INCLUDE_LINE_CHECK_H