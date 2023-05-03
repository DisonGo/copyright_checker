#pragma once
#include <fstream>
#include <iostream>
#include <vector>

#include "file_manager.h"

using std::string;
using std::vector;
namespace SignatureCompare {
void RemoveVariables(FileData& data);
int GetSignatureInfo(const string& reference_file, FileData checked_file);
double GetMatchedPercentage(FileData& ref, FileData& copy);
FileData SignatureNormalize(std::ifstream& path);
string RemoveExtra(const string& str);
void SignatureNormalize(FileData& data);
void RemoveQuotes(string& str);
void ReadVariableName(const string& from, string& buffer, size_t& pos);
inline bool IsEndOfName(const char& current);
void RemoveVariableFromFileData(FileData& data,
                                const vector<string>& variable_names);
};  // namespace SignatureCompare
