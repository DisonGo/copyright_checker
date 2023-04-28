#pragma once

#include <fstream>
#include <iostream>
#include <vector>

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