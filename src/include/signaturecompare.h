#pragma once
#include <fstream>
#include <iostream>
#include <set>
#include <stack>
#include <unordered_set>
#include <vector>

#include "file_manager.h"

typedef std::unordered_set<std::string> UniqVarNames;

using std::string;
using std::vector;

namespace SignatureCompare {
UniqVarNames GetTypedefNames(const std::vector<std::string>& lines);
void RemoveVariableFromFileData(FileData& data,
                                const UniqVarNames& variable_names);
inline string GetTypedefNameFromLine(const string& line);
void RemoveVariables(FileData& data);
int GetSignatureInfo(const string& reference_file, FileData checked_file);
double GetMatchedPercentage(FileData& ref, FileData& copy);
FileData SignatureNormalize(std::ifstream& path);
string RemoveExtra(const string& str);
void SignatureNormalize(FileData& data);
void RemoveQuotes(string& str);
void ReadVariableName(const string& from, string& buffer, size_t& pos);
inline bool IsEndOfName(const char& current);
};  // namespace SignatureCompare
