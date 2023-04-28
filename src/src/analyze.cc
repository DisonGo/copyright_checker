#include "analyze.h"

void Analyze::AnalyzeProject(const RepoPair& reference_path,
                             FilesData& peer_files, vector<AnalyzeInfo>& result,
                             size_t id) {
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
size_t Analyze::GetLineCompareInfo(string reference_file,
                                   vector<string> peer_file_data) {
  size_t result{};
  LineCompare obj;
  result = obj.GetLineInfo(reference_file, peer_file_data);
  return result;
}
size_t Analyze::GetSignatureCompareInfo(string reference_file,
                                        vector<string> peer_file_data) {
  size_t result{};
  SignatureCompare obj;
  result = obj.GetSignatureInfo(reference_file, peer_file_data);
  return result;
}