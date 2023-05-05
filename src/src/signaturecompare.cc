#include "signaturecompare.h"
using fm = FileManager;
int SignatureCompare::GetSignatureInfo(const string& reference_file,
                                       FileData checked_file) {
  if (reference_file.empty()) return 1;
  FileData reference = fm::ReadFileContent(reference_file, RemoveCommentaries);
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
string SignatureCompare::RemoveCommentaries(const string& line) {
  static bool IsOpenCommentary{};
  if (line.size() < 2 && !IsOpenCommentary) return line;
  string new_line = line;

  size_t one_line_comment = new_line.find("//", 0);
  bool IsOneLineFound = (one_line_comment != string::npos);
  if (!IsOpenCommentary && IsOneLineFound) {
    new_line.erase(one_line_comment, new_line.size() - one_line_comment);
    return new_line;
  }

  size_t commentary_start = new_line.find("/*", 0);
  size_t commentary_end = new_line.find("*/", 0);

  bool IsOpenFound = (commentary_start != string::npos);
  bool IsCloseFound = (commentary_end != string::npos);

  if (!IsOpenCommentary) IsOpenCommentary = IsOpenFound;

  if (IsOpenCommentary) {
    if (IsCloseFound) {
      size_t offset = IsOpenFound ? commentary_start : 0;
      new_line.erase(offset, commentary_end + 2);
      IsOpenCommentary = (IsOpenFound && commentary_start > commentary_end);
    } else {
      if (!IsOpenFound) return "";
      new_line.erase(commentary_start, new_line.size() - commentary_start);
    }
  }
  return new_line;
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
  data = std::move(fm::TransformFileData(data, RemoveCommentaries));
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
  vector<string> var_types = {"int*",
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
  UniqVarNames typedef_names = GetTypedefNames(data);
  for (auto& var : typedef_names) {
    variables.insert(var);
    var_types.push_back(var);
  }

  size_t find_result{};
  string buffer;
  for (const auto& line : data) {
    for (const auto& var_type : var_types) {
      find_result = 0;
      while ((find_result = line.find(var_type, find_result)) != string::npos) {
        bool IsStartVarType = IsStartOfVar(line[find_result - 1]);
        bool IsEndVarType = std::isspace(line[find_result + var_type.size()]);
        if (find_result == 0 || (IsStartVarType && IsEndVarType)) {
          find_result += var_type.size() + 1;
          ReadVariableName(line, buffer, find_result);
          std::cout << "ReadVarName() = " << buffer << "\n";
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
    const FileData& data) {
  bool IsTypedef{};
  UniqVarNames typedef_names;
  std::stack<char> brackets;

  for (auto& line : data) {
    if (line.find("typedef", 0) != string::npos) {
      IsTypedef = true;
      brackets.push('{');
    } else if (!IsTypedef) {
      continue;
    } else if (line.find("{", 0) != string::npos) {
      brackets.push('{');
    } else if (line.find("}", 0) != string::npos) {
      if (brackets.size() > 1) brackets.pop();
      else {
        brackets.pop();
        IsTypedef = false;
        string buffer;
        buffer.reserve(line.size());
        for (size_t i = 0; i < line.size(); i++)
          if (std::isalpha(line[i]) || line[i] == ',') 
            buffer.push_back(line[i]);
        PushTypedefNames(buffer, typedef_names);
      }
    }
  }

  std::cout << "====\n";
  for (auto& var : typedef_names) {
    std::cout << var << "\n";
  }


  return typedef_names;
}

void SignatureCompare::PushTypedefNames(string& str, UniqVarNames& names) {
  size_t start{};
  size_t end{};
  while (end != string::npos) {
    end = str.find(',', start);
    if (end != string::npos) names.insert(str.substr(start, end - start));
    else names.insert(str.substr(start, str.size() - start));
    start = end + 1;
  }
}

void SignatureCompare::ReadVariableName(const string& from, string& buffer,
                                        size_t& pos) {
  if (from[pos] == ')') return;
  size_t from_size = from.size();
  buffer.clear();
  for (; pos < from_size; pos++) {
    char current = from[pos];
    if (IsEndOfVar(current)) break;
    buffer.push_back(current);
  }
}

inline bool SignatureCompare::IsEndOfName(const char current) {
  static const string filter(" =(){},;\"'");
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
            (find_iterator == 0 || SignatureCompare::IsStartOfVar(line[find_iterator - 1]));
        IsEndOfVar = SignatureCompare::IsEndOfVar(line[find_iterator + var_name.size()]);
        if (IsStartOfVar && IsEndOfVar)
          line.erase(find_iterator, var_name.size());
      }
      find_iterator = 0;
    }
  }
}

inline bool SignatureCompare::IsStartOfVar(const char symbol) {
  return !(std::isalpha(symbol));
}

inline bool SignatureCompare::IsEndOfVar(const char symbol) {
  static const string filter("0123456789_");
  return !((filter.find(symbol, 0) != string::npos) || (std::isalpha(symbol)));
}