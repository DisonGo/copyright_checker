#pragma once

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "file_manager.h"
#include "logger.h"
#include "repo_manager.h"

typedef std::vector<std::pair<std::vector<std::string>, string>> FilesData;

/*  === === === === === === ===
    LineCompare class.
=== === === === === === === ===  */

class LineCompare {
  friend class Analyze;
 public:
  LineCompare() {}
  ~LineCompare() {}
 private:
  int GetLineInfo(const std::string& reference_file,
                  const std::vector<std::string>& peer_file);
  int GetMatchPercentage(std::vector<std::string>& ref,
                         std::vector<std::string>& check);
  std::string NormalizeString(const std::string& str);
  inline bool IsForbidden(const char symbol);
  inline bool IsPreProcessing(const std::string& str);
};


struct AnalyzeInfo {
  size_t id{};
  std::string git_link;
  size_t signature_info{};
  size_t line_info{};
  std::string file_path_ref;
  std::string file_path_review;
};


/*  === === === === === === ===
    SignatureCheck class.
=== === === === === === === ===  */

class SignatureCompare {
  friend class Analyze;
 public:
 SignatureCompare(){}
 ~SignatureCompare(){}
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


class Analyze {
 public:
  Analyze() {}
  ~Analyze() {}

  friend class LineCompare;
  friend class SignatureCompare;

  static void AnalyzeProject(const RepoPair& reference_path, FilesData& peer_files,
                      std::vector<AnalyzeInfo>& result, size_t id) {
    size_t signature{};
    size_t line{};
    FilePathArrays paths = FileManager::FindSourcesC(reference_path.second);
    for (auto& peer_file : peer_files) {
      for (auto& reference_files : paths.second) {
        signature = GetSignatureCompareInfo(reference_files, peer_file.first);
        line = GetLineCompareInfo(reference_files, peer_file.first);
        AnalyzeInfo current;
        current.id = id;
        current.git_link = reference_path.first;
        current.signature_info = signature;
        current.line_info = line;
        current.file_path_ref = reference_files;
        current.file_path_review = peer_file.second;
        result.push_back(current);
      }
    }
  }

 private:
  static size_t GetLineCompareInfo(std::string reference_file, std::vector<std::string> peer_file_data) {
    size_t result{};
    LineCompare obj;
    result = obj.GetLineInfo(reference_file, peer_file_data);
    return result;
  }
  static size_t GetSignatureCompareInfo(std::string reference_file, std::vector<std::string> peer_file_data) {
    size_t result{};
    SignatureCompare obj;
    result = obj.GetSignatureInfo(reference_file, peer_file_data);
    return result;
  }
};



/*  === === === === === === ===
    LineCompare functions.
=== === === === === === === ===  */

int LineCompare::GetLineInfo(const std::string& reference_file,
                             const std::vector<std::string>& peer_file) {
  if (reference_file.empty()) return 0;

  std::ifstream ref(reference_file);
  if (!ref.is_open()) {
    std::cout << "Reference file does not exist.\n";
    return 0;
  }

  std::vector<std::string> reference;
  std::vector<std::string> check;

  std::string buffer;
  while (std::getline(ref, buffer)) {
    if (buffer.size() > 0) {
      buffer = NormalizeString(buffer);
      if (buffer.size() > 0) reference.push_back(buffer);
    }
  }
  buffer.clear();
  for (auto& line : peer_file) {
    if (line.size() > 0) {
      buffer = NormalizeString(line);
      if (buffer.size() > 0) check.push_back(buffer);
    }
  }

  if (reference.size() == 0 || check.size() == 0) return 0;
  return GetMatchPercentage(reference, check);
}

int LineCompare::GetMatchPercentage(std::vector<std::string>& ref,
                                    std::vector<std::string>& check) {
  size_t all_count{};
  size_t matched_count{};

  size_t reference_size = ref.size();
  size_t check_size = check.size();
  for (size_t i = 0; i < reference_size; i++) {
    for (size_t j = 0; j < check_size; j++) {
      if (check[j].compare(ref[i]) == 0) {
        matched_count++;
        break;
      }
    }
    all_count++;
  }

  double result = (100.0 / static_cast<double>(all_count) *
                   static_cast<double>(matched_count));
  return static_cast<int>(result);
}

std::string LineCompare::NormalizeString(const std::string& str) {
  if (IsPreProcessing(str)) return "";
  std::string new_string;
  for (size_t i = 0; i < str.size(); i++) {
    if (!IsForbidden(str[i])) new_string.push_back(str[i]);
  }
  return new_string;
}

inline bool LineCompare::IsForbidden(const char symbol) {
  std::string forbiddeg_symbols = "(){} ;\t";
  if (forbiddeg_symbols.find(symbol, 0) != std::string::npos) return true;
  return false;
}

inline bool LineCompare::IsPreProcessing(const std::string& str) {
  std::vector<std::string> pre_processing = {"#include", "#ifndef", "#define",
                                             "#endif", "int main()"};
  for (size_t i = 0; i < 4; i++)
    if (str.find(pre_processing[i], 0) != std::string::npos) return true;
  return false;
}

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
