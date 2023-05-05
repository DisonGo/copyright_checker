#include "linecompare.h"
using fm = FileManager;
int LineCompare::GetLineInfo(const string& reference_file,
                             const FileData& peer_file) {
  if (reference_file.empty()) return 0;
  FileData reference = fm::ReadFileContent(reference_file, NormalizeString);
  FileData check = fm::TransformFileData(peer_file, NormalizeString);
  if (reference.size() == 0 || check.size() == 0) return 0;

  return GetMatchPercentage(reference, check);
}

int LineCompare::GetLineInfo(const string& reference_file,
                             const PeerFileData& peer_file) {
  if (reference_file.empty()) return 0;
  FileData reference = fm::ReadFileContent(reference_file, NormalizeString);
  if (reference.size() == 0 || peer_file.lineCompData.size() == 0) return 0;
  return GetMatchPercentage(reference, peer_file.lineCompData);
}

int LineCompare::GetMatchPercentage(const FileData& ref,
                                    const FileData& check) {
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
void LineCompare::PrepareFilesData(FilesData& data) {
  for (auto& fileData : data) LineCompare::PrepareFileData(fileData.first);
}
void LineCompare::PrepareFileData(FileData& data) {
  data = std::move(fm::TransformFileData(data, NormalizeString));
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
  const static string forbiddeg_symbols = "(){};\t";
  return (forbiddeg_symbols.find(symbol, 0) != string::npos);
}

inline bool LineCompare::IsPreProcessing(const string& str) {
  static const vector<string> pre_processing = {"#include", "#ifndef",
                                                "#define", "#endif"};
  for (size_t i = 0; i < pre_processing.size(); i++)
    if (str.find(pre_processing[i], 0) != string::npos) return true;
  return false;
}