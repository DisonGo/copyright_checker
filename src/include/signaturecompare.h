#pragma once

#include <fstream>
#include <iostream>
#include <vector>

class SignatureCompare {
  friend class Analyze;

 public:
  SignatureCompare() {}
  ~SignatureCompare() {}

 private:
  int GetSignatureInfo(const std::string& reference_file,
                       std::vector<std::string> checked_file);
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
};

/*  === === === === === === ===
    SignatureCheck functions.
=== === === === === === === ===  */

int SignatureCompare::GetSignatureInfo(const std::string& reference_file,
                                       std::vector<std::string> checked_file) {
  if (reference_file.empty()) return 1;
  std::ifstream ref(reference_file);

  if (!ref.is_open()) {
    std::cout << "Reference file is not exist.\n";
    return 0;
  }

  std::vector<std::string> reference = SignatureNormalize(ref);
  SignatureNormalize(checked_file);
  RemoveVariables(reference);
  RemoveVariables(checked_file);

  if (reference.size() == 0 || checked_file.size() == 0) return 0;
  return static_cast<int>(GetMatchedPercentage(reference, checked_file));
}

double SignatureCompare::GetMatchedPercentage(std::vector<std::string>& ref,
                                              std::vector<std::string>& copy) {
  size_t all_count{};
  size_t match_count{};
  for (size_t i = 0; i < ref.size(); i++) {
    for (size_t j = 0; j < copy.size(); j++) {
      if (ref[i].compare(copy[j]) == 0) {
        match_count++;
        break;
      }
    }
    all_count++;
  }

  if (ref.size() > copy.size()) all_count += ref.size() - copy.size();

  return (100.0 / all_count * match_count);
}

std::vector<std::string> SignatureCompare::SignatureNormalize(
    std::ifstream& input_file) {
  std::vector<std::string> strings;

  std::string buffer;
  while (std::getline(input_file, buffer)) {
    RemoveExtra(buffer);
    strings.push_back(buffer);
  }
  return strings;
}

void SignatureCompare::SignatureNormalize(
    std::vector<std::string>& input_string) {
  for (auto& line : input_string) RemoveExtra(line);
}

void SignatureCompare::RemoveExtra(std::string& str) {
  std::string result_string;

  for (size_t i = 0; i < str.size(); i++) {
    if (str[i] != ' ')
      result_string.push_back(str[i]);
    else if (result_string.empty())
      continue;
    else if (i < str.size() - 1 && str[i + 1] != ' ')
      result_string.push_back(str[i]);
  }

  RemoveQuotes(result_string);

  str = result_string;
}

void SignatureCompare::RemoveQuotes(std::string& str) {
  size_t res = str.find('"', 0);
  if (res == std::string::npos) return;
  res++;
  while (res != str.size() && str[res] != '"') str.erase(res, 1);
}

void SignatureCompare::RemoveVariables(std::vector<std::string>& strings) {
  std::vector<std::string> var_types = {
      "int*",         "float*",        "char*",
      "double*",      "int",           "char",
      "float",        "double",        "void",
      "enum",         "struct",        "size_t",
      "short",        "long double",   "unsigned",
      "unsigned int", "unsigned char", "unsigned long long",
      "signed",       "signed int",    "signed char"};

  std::vector<std::string> variables;

  std::string buffer;
  size_t find_result{};
  for (size_t i = 0; i < strings.size(); i++) {
    for (size_t j = 0; j < var_types.size(); j++) {
      find_result = strings[i].find(var_types[j], 0);
      if (find_result != std::string::npos) {
        find_result += var_types[j].size() + 1;
        ReadVariableName(strings[i], buffer, find_result);
        if (buffer.size()) variables.push_back(buffer);
      }
    }
  }
  RemoveVariableFromString(strings, variables);
}

void SignatureCompare::ReadVariableName(std::string& from, std::string& buffer,
                                        size_t& pos) {
  if (from[pos] == ')') return;
  size_t from_size = from.size();
  buffer.clear();
  for (; pos < from_size; pos++) {
    char current = from[pos];
    if (IsEndOfName(current)) break;
    buffer.push_back(current);
  }
}

inline bool SignatureCompare::IsEndOfName(const char& current) {
  return (current == ' ' || current == '=' || current == '(' || current == ',');
}

void SignatureCompare::RemoveVariableFromString(
    std::vector<std::string>& str, std::vector<std::string>& variable_names) {
  size_t find_iterator{};
  for (size_t i = 0; i < str.size(); i++) {
    for (size_t j = 0; j < variable_names.size(); j++) {
      find_iterator = str[i].find(variable_names[j], 0);
      if (find_iterator != std::string::npos) {
        str[i].erase(find_iterator, variable_names[j].size());
        j = 0;
      }
    }
  }
}
