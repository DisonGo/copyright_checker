#pragma once

#include <fstream>
#include <iostream>
#include <vector>
using std::string;
using std::vector;
class LineCompare {
  friend class Analyze;

 public:
  LineCompare() {}
  ~LineCompare() {}

 private:
  int GetLineInfo(const string& reference_file,
                  const vector<string>& peer_file);
  int GetMatchPercentage(vector<string>& ref, vector<string>& check);
  string NormalizeString(const string& str);
  inline bool IsForbidden(const char symbol);
  inline bool IsPreProcessing(const string& str);
};