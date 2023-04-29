#include "linecompare.h"
int LineCompare::GetLineInfo(const string& reference_file,
                             const FileData& peer_file) {
  if (reference_file.empty()) return 0;
  FileData reference = FileManager::ReadFileContent(reference_file);
  FileData check = FileManager::TransformFileData(peer_file, NormalizeString);
  if (reference.size() == 0 || check.size() == 0) return 0;
  return GetMatchPercentage(reference, check);
}

int LineCompare::GetMatchPercentage(FileData& ref, FileData& check) {
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
  static const vector<string> pre_processing = {
      "#include", "#ifndef", "#define", "#endif", "int main()"};
  for (size_t i = 0; i < 4; i++)
    if (str.find(pre_processing[i], 0) != string::npos) return true;
  return false;
}