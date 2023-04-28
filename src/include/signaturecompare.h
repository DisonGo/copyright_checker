#pragma once

#include <fstream>
#include <iostream>
#include <vector>
using std::string;
using std::vector;
class SignatureCompare {
  friend class Analyze;

 public:
  SignatureCompare() {}
  ~SignatureCompare() {}

 private:
  int GetSignatureInfo(const string& reference_file,
                       vector<string> checked_file);
  double GetMatchedPercentage(vector<string>& ref, vector<string>& copy);
  vector<string> SignatureNormalize(std::ifstream& input_file);
  void RemoveExtra(string& str);
  void SignatureNormalize(vector<string>& input_string);
  void RemoveQuotes(string& str);
  void RemoveVariables(vector<string>& strings);
  void ReadVariableName(string& from, string& buffer, size_t& pos);
  inline bool IsEndOfName(const char& current);
  void RemoveVariableFromString(vector<string>& str,
                                vector<string>& variable_names);
};
