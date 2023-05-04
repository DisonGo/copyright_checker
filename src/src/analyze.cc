#include "analyze.h"

void Analyze::AnalyzeProject(const RepoPair& reference_path,
                             PeerFilesData& peer_files,
                             vector<AnalyzeInfo>& result, size_t id) {
  size_t signature{};
  size_t line{};
  FilePathArrays paths = FileManager::FindSourcesC(reference_path.second);
  for (auto& peer_file : peer_files) {
    for (auto& reference_files : paths.sources) {
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

size_t Analyze::GetLineCompareInfo(const string& reference_file,
                                   FileData peer_file_data) {
  return LineCompare::GetLineInfo(reference_file, peer_file_data);
}

size_t Analyze::GetSignatureCompareInfo(const string& reference_file,
                                        FileData peer_file_data) {
  return SignatureCompare::GetSignatureInfo(reference_file, peer_file_data);
}
size_t Analyze::GetLineCompareInfo(const string& reference_file,
                                   const PeerFileData& peer_file_data) {
  return LineCompare::GetLineInfo(reference_file, peer_file_data);
}

size_t Analyze::GetSignatureCompareInfo(const string& reference_file,
                                        const PeerFileData& peer_file_data) {
  return SignatureCompare::GetSignatureInfo(reference_file, peer_file_data);
}
