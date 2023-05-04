#pragma once
#include <fstream>
#include <iostream>
#include <vector>

#include "file_manager.h"
#include "peer_file_data.h"
using Analyze::PeerFileData;
using std::string;
using std::vector;
namespace LineCompare {
int GetLineInfo(const string& reference_file, const FileData& peer_file);
int GetLineInfo(const string& reference_file, const PeerFileData& peer_file);
int GetMatchPercentage(const FileData& ref, const FileData& check);
void PrepareFilesData(FilesData& data);
void PrepareFileData(FileData& data);
string NormalizeString(const string& str);
inline bool IsForbidden(const char symbol);
inline bool IsPreProcessing(const string& str);
};  // namespace LineCompare