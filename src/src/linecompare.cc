#include "linecompare.h"
int LineCompare::GetLineInfo(const string& reference_file,
                             const vector<string>& peer_file) {
  if (reference_file.empty()) return 0;

  std::ifstream ref(reference_file);
  if (!ref.is_open()) {
    std::cout << "Reference file does not exist.\n";
    return 0;
  }

  vector<string> reference;
  vector<string> check;

  string buffer;
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

int LineCompare::GetMatchPercentage(vector<string>& ref,
                                    vector<string>& check) {
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

string LineCompare::NormalizeString(const string& str) {
  if (IsPreProcessing(str)) return "";
  string new_string;
  for (size_t i = 0; i < str.size(); i++) {
    if (!IsForbidden(str[i])) new_string.push_back(str[i]);
  }
  return new_string;
}

inline bool LineCompare::IsForbidden(const char symbol) {
  string forbiddeg_symbols = "(){} ;\t";
  if (forbiddeg_symbols.find(symbol, 0) != string::npos) return true;
  return false;
}

inline bool LineCompare::IsPreProcessing(const string& str) {
  vector<string> pre_processing = {"#include", "#ifndef", "#define", "#endif",
                                   "int main()"};
  for (size_t i = 0; i < 4; i++)
    if (str.find(pre_processing[i], 0) != string::npos) return true;
  return false;
}