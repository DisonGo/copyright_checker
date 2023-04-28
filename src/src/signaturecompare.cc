#include "signaturecompare.h"

int SignatureCompare::GetSignatureInfo(const string& reference_file,
                                       vector<string> checked_file) {
  if (reference_file.empty()) return 1;
  std::ifstream ref(reference_file);

  if (!ref.is_open()) {
    std::cout << "Reference file is not exist.\n";
    return 0;
  }

  vector<string> reference = SignatureNormalize(ref);
  SignatureNormalize(checked_file);
  RemoveVariables(reference);
  RemoveVariables(checked_file);

  if (reference.size() == 0 || checked_file.size() == 0) return 0;
  return static_cast<int>(GetMatchedPercentage(reference, checked_file));
}

double SignatureCompare::GetMatchedPercentage(vector<string>& ref,
                                              vector<string>& copy) {
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

vector<string> SignatureCompare::SignatureNormalize(std::ifstream& input_file) {
  vector<string> strings;

  string buffer;
  while (std::getline(input_file, buffer)) {
    RemoveExtra(buffer);
    strings.push_back(buffer);
  }
  return strings;
}

void SignatureCompare::SignatureNormalize(vector<string>& input_string) {
  for (auto& line : input_string) RemoveExtra(line);
}

void SignatureCompare::RemoveExtra(string& str) {
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

  str = result_string;
}

void SignatureCompare::RemoveQuotes(string& str) {
  size_t res = str.find('"', 0);
  if (res == string::npos) return;
  res++;
  while (res != str.size() && str[res] != '"') str.erase(res, 1);
}

void SignatureCompare::RemoveVariables(vector<string>& strings) {
  vector<string> var_types = {
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
  for (size_t i = 0; i < strings.size(); i++) {
    for (size_t j = 0; j < var_types.size(); j++) {
      find_result = strings[i].find(var_types[j], 0);
      if (find_result != string::npos) {
        find_result += var_types[j].size() + 1;
        ReadVariableName(strings[i], buffer, find_result);
        if (buffer.size()) variables.push_back(buffer);
      }
    }
  }
  RemoveVariableFromString(strings, variables);
}

void SignatureCompare::ReadVariableName(string& from, string& buffer,
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
  static const string filter(" =(,");
  return filter.find(current) != string::npos;
}

void SignatureCompare::RemoveVariableFromString(
    vector<string>& str, vector<string>& variable_names) {
  size_t find_iterator{};
  for (size_t i = 0; i < str.size(); i++) {
    for (size_t j = 0; j < variable_names.size(); j++) {
      find_iterator = str[i].find(variable_names[j], 0);
      if (find_iterator != string::npos) {
        str[i].erase(find_iterator, variable_names[j].size());
        j = 0;
      }
    }
  }
}
