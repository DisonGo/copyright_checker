#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <stack>

#include "file_manager.h"

typedef std::unordered_set<std::string> UniqVarNames;

using std::string;
using std::vector;
class SignatureCompare {
  friend class Analyze;

 public:
  SignatureCompare() {}
  ~SignatureCompare() {}

  static void RemoveVariables(FileData& data);

 private:
  bool IsCommentary{};
  static int GetSignatureInfo(const string& reference_file,
                              FileData checked_file);
  static double GetMatchedPercentage(FileData& ref, FileData& copy);
  static FileData SignatureNormalize(std::ifstream& path);
  static string RemoveExtra(const string& str);
  static UniqVarNames GetTypedefNames(const std::vector<std::string>& lines);
  static void SignatureNormalize(FileData& data);
  static void RemoveQuotes(string& str);
  static void ReadVariableName(const string& from, string& buffer, size_t& pos);
  static inline bool IsEndOfName(const char& current);
  static void RemoveVariableFromFileData(FileData& data, const UniqVarNames& variable_names);
  static inline string GetTypedefNameFromLine(const string& line);
};
