#pragma once
#include <fstream>
#include <iostream>
#include <vector>

#include "file_manager.h"
#define NO_MATCH std::string::npos

using std::string;
using std::vector;
class SignatureCompare {
  friend class Analyze;

 public:
  SignatureCompare() {}
  ~SignatureCompare() {}

  static void RemoveVariables(FileData& strings);

 private:
  bool IsCommentary{};
  int GetSignatureInfo(const string& reference_file, FileData checked_file);
  double GetMatchedPercentage(FileData& ref, FileData& copy);
  static FileData SignatureNormalize(std::ifstream& path);
  static string RemoveExtra(const string& str);
  static void SignatureNormalize(FileData& data);
  static void RemoveQuotes(string& str);
  static void ReadVariableName(string& from, string& buffer, size_t& pos);
  static inline bool IsEndOfName(const char& current);
  static void RemoveVariableFromString(FileData& str,
                                       vector<string>& variable_names);
};
