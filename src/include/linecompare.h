#pragma once
#include <fstream>
#include <iostream>
#include <vector>

#include "file_manager.h"
using std::string;
using std::vector;
class LineCompare {
  friend class Analyze;

 public:
  LineCompare() {}
  ~LineCompare() {}

 private:
  int GetLineInfo(const string& reference_file, const FileData& peer_file);
  int GetMatchPercentage(FileData& ref, FileData& check);
  static string NormalizeString(const string& str);
  static inline bool IsForbidden(const char symbol);
  static inline bool IsPreProcessing(const string& str);
};