#include "signaturecompare.h"
using fm = FileManager;
int SignatureCompare::GetSignatureInfo(const string& reference_file,
                                       FileData checked_file) {
  if (reference_file.empty()) return 1;
  FileData reference = fm::ReadFileContent(reference_file);
  SignatureNormalize(reference);
  SignatureNormalize(checked_file);
  RemoveVariables(reference);
  RemoveVariables(checked_file);

  if (reference.size() == 0 || checked_file.size() == 0) return 0;
  return static_cast<int>(GetMatchedPercentage(reference, checked_file));
}
int SignatureCompare::GetSignatureInfo(const string& reference_file,
                                       const PeerFileData& peer_file) {
  if (reference_file.empty()) return 1;
  FileData reference = fm::ReadFileContent(reference_file);
  SignatureNormalize(reference);
  RemoveVariables(reference);

  if (reference.size() == 0 || peer_file.signatureData.size() == 0) return 0;
  return static_cast<int>(
      GetMatchedPercentage(reference, peer_file.signatureData));
}

double SignatureCompare::GetMatchedPercentage(const FileData& ref,
                                              const FileData& copy) {
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
void SignatureCompare::PrepareFilesData(FilesData& data) {
  for (auto& fileData : data) SignatureCompare::PrepareFileData(fileData.first);
}
void SignatureCompare::PrepareFileData(FileData& data) {
  SignatureNormalize(data);
  RemoveVariables(data);
}

FileData SignatureCompare::SignatureNormalize(std::ifstream& path) {
  return fm::ReadFileContent(path, RemoveExtra);
}

void SignatureCompare::SignatureNormalize(FileData& data) {
  data = std::move(fm::TransformFileData(data, RemoveExtra));
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
  static vector<string> var_types = {"int*",
                                     "float*",
                                     "char*",
                                     "double*",
                                     "int",
                                     "char",
                                     "float",
                                     "double",
                                     "void",
                                     "enum",
                                     "struct",
                                     "size_t",
                                     "short",
                                     "long double",
                                     "unsigned int",
                                     "unsigned char",
                                     "unsigned long long",
                                     "unsigned",
                                     "signed int",
                                     "signed char",
                                     "signed"};

  UniqVarNames variables;
  // UniqVarNames typedef_names = GetTypedefNames(data);
  // for (auto& var : typedef_names) {
  //   variables.insert(var);
  //   var_types.push_back(var);
  // }

  size_t find_result{};
  string buffer;
  for (const auto& line : data) {
    for (const auto& var_type : var_types) {
      find_result = 0;
      while ((find_result = line.find(var_type, find_result)) != string::npos) {
        if (!find_result || !(std::isalpha(line[find_result - 1]))) {
          find_result += var_type.size() + 1;
          ReadVariableName(line, buffer, find_result);
          if (buffer.size()) variables.insert(buffer);
        } else {
          find_result += 1;
        }
      }
    }
  }

  // std::cout << "Var names: \n";
  // for (auto& var : variables)
  //   std::cout << var << "\n";
  // std::cout << "End of names\n";

  RemoveVariableFromFileData(data, variables);
}

UniqVarNames SignatureCompare::GetTypedefNames(
    const std::vector<string>& data) {
  bool IsTypedef{};
  size_t index{};
  UniqVarNames typedef_names;
  std::stack<char> brackets;
  for (const auto& line : data) {
    index = 0;
    if (line.find("typedef", index) != std::string::npos) {
      brackets.push('{');
      IsTypedef = true;
      continue;
    } else if (IsTypedef && line.find('{', index) != std::string::npos) {
      brackets.push('{');
      continue;
    } else if (IsTypedef && line.find('}', index) != std::string::npos) {
      if (brackets.size() == 1) {
        brackets.pop();
        IsTypedef = false;
        string buffer;
        for (size_t i = 0; i < line.size() && line[i] != ','; i++) {
          if (std::isalpha(line[i])) buffer.push_back(line[i]);
        }
        typedef_names.insert(buffer);
        continue;
      } else {
        brackets.pop();
        continue;
      }
    }
  }
  return typedef_names;
}

inline string SignatureCompare::GetTypedefNameFromLine(const string& line) {
  string buffer;
  size_t i{};
  for (i = 0; i < line.size() && !(std::isalpha(line[i])); i++) {
  }
  for (; i < line.size() && std::isalpha(line[i]); i++) {
    buffer.push_back(line[i]);
  }
  return buffer;
}

void SignatureCompare::ReadVariableName(const string& from, string& buffer,
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
  static const string filter(" =(){},;\"");
  return filter.find(current) != string::npos;
}

void SignatureCompare::RemoveVariableFromFileData(
    FileData& data, const UniqVarNames& variable_names) {
  size_t find_iterator{};

  // <position index, size of var>
  std::vector<std::pair<size_t, size_t>> var_positions;

  bool IsStartOfVar{};
  bool IsEndOfVar{};
  for (auto& line : data) {
    for (auto& var_name : variable_names) {
      while ((find_iterator = line.find(var_name, find_iterator + 1)) !=
             string::npos) {
        IsStartOfVar =
            (find_iterator == 0 || !(std::isalpha(line[find_iterator - 1])));
        IsEndOfVar = !(std::isalpha(line[find_iterator + var_name.size()]));
        if (IsStartOfVar && IsEndOfVar)
          line.erase(find_iterator, var_name.size());
      }
      find_iterator = 0;
    }
  }
}
