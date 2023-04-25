#ifndef SRC_INCLUDE_SIGNATURE_CHECK_H
#define SRC_INCLUDE_SIGNATURE_CHECK_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int GetSignatureInfo(std::string reference_file, std::string checked_file);
double GetMatchedPercentage(std::vector<std::string>& ref, std::vector<std::string>& copy);
std::vector<std::string> SignatureNormalize(std::ifstream& input_file);
void RemoveExtra(std::string& str);
void RemoveQuotes(std::string& str);
void RemoveVariables(std::vector<std::string>& strings);
void RemoveVariableFromString(std::vector<std::string>& str, std::vector<std::string>& variable_names);

#endif  //  SRC_INCLUDE_SIGNATURE_CHECK_H