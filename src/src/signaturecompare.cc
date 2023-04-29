#include "signaturecompare.h"
int SignatureCompare::GetSignatureInfo(const string& reference_file,
                                       FileData checked_file) {
  if (reference_file.empty()) return 1;
  FileData reference = FileManager::ReadFileContent(reference_file);
  SignatureNormalize(reference);
  SignatureNormalize(checked_file);
  RemoveVariables(reference);
  RemoveVariables(checked_file);

  if (reference.size() == 0 || checked_file.size() == 0) return 0;
  return static_cast<int>(GetMatchedPercentage(reference, checked_file));
}

double SignatureCompare::GetMatchedPercentage(FileData& ref, FileData& copy) {
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

FileData SignatureCompare::SignatureNormalize(std::ifstream& path) {
  return FileManager::ReadFileContent(path, RemoveExtra);
}

void SignatureCompare::SignatureNormalize(FileData& data) {
  data = std::move(FileManager::TransformFileData(data, RemoveExtra));
}

string SignatureCompare::RemoveExtra(const string& str) {
  string result_string;

  for (size_t i = 0; i < str.size(); i++) {
    if (str[i] != ' ')
      result_string.push_back(str[i]);
    else if (result_string.empty())
      continue;
    else if (i < str.size() - 1 && str[i + 1] != ' ')
      result_string.push_back(str[i]);
  }

  RemoveQuotes(result_string);
  return result_string;
}

void SignatureCompare::RemoveQuotes(string& str) {
  size_t res = str.find('"', 0);
  if (res == string::npos) return;
  res++;
  while (res != str.size() && str[res] != '"') str.erase(res, 1);
}

void SignatureCompare::RemoveVariables(FileData& data) {
  static const vector<string> var_types = {
      "int*",         "float*",        "char*",
      "double*",      "int",           "char",
      "float",        "double",        "void",
      "enum",         "struct",        "size_t",
      "short",        "long double",   "unsigned",
      "unsigned int", "unsigned char", "unsigned long long",
      "signed",       "signed int",    "signed char"};

  vector<string> variables;

  string buffer;
  size_t find_result{};
  for (const auto& line : data) {
    for (const auto& var_type : var_types) {
      find_result = line.find(var_type, 0);
      if (find_result != string::npos) {
        find_result += var_type.size() + 1;
        ReadVariableName(line, buffer, find_result);
        if (buffer.size()) variables.push_back(buffer);
      }
    }
  }
  RemoveVariableFromFileData(data, variables);
}

void SignatureCompare::ReadVariableName(const string& from, string& buffer,
                                        size_t& pos) {
  // if (from[pos] == ')') return;
  // buffer.clear();
  // string::const_iterator it = from.begin() + pos;
  // for (; it != from.end(); it++) {
  //   if (IsEndOfName(*it)) break;
  //   buffer.push_back(*it);
  // }
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
  static const string filter(" =(,");
  return filter.find(current) != string::npos;
}

void SignatureCompare::RemoveVariableFromFileData(
    FileData& data, const vector<string>& variable_names) {
  size_t find_iterator{};

  for (auto& line : data)
    for (auto& var_name : variable_names)
      while ((find_iterator = line.find(var_name, 0)) != string::npos)
        line.erase(find_iterator, var_name.size());
}
