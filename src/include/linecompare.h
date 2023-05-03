#pragma once
#include <fstream>
#include <iostream>
#include <vector>

#include "file_manager.h"
using std::string;
using std::vector;
namespace LineCompare {
int GetLineInfo(const string& reference_file, const FileData& peer_file);
int GetMatchPercentage(FileData& ref, FileData& check);
string NormalizeString(const string& str);
inline bool IsForbidden(const char symbol);
inline bool IsPreProcessing(const string& str);
};  // namespace LineCompare