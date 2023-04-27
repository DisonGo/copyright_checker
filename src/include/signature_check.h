#ifndef SRC_INCLUDE_SIGNATURE_CHECK_H
#define SRC_INCLUDE_SIGNATURE_CHECK_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int GetSignatureInfo(const std::string& reference_file, std::vector<std::string> checked_file);
double GetMatchedPercentage(std::vector<std::string>& ref,
                            std::vector<std::string>& copy);
std::vector<std::string> SignatureNormalize(std::ifstream& input_file);
void RemoveExtra(std::string& str);
void SignatureNormalize(std::vector<std::string>& input_string);
void RemoveQuotes(std::string& str);
void RemoveVariables(std::vector<std::string>& strings);
void ReadVariableName(std::string& from, std::string& buffer, size_t& pos);
inline bool IsEndOfName(const char& current);
void RemoveVariableFromString(std::vector<std::string>& str,
                              std::vector<std::string>& variable_names);

#endif  //  SRC_INCLUDE_SIGNATURE_CHECK_H