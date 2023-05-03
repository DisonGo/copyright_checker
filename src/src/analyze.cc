#include "analyze.h"

void Analyze::AnalyzeProject(const RepoPair& reference_path,
                             FilesData& peer_files, vector<AnalyzeInfo>& result,
                             size_t id) {
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
size_t Analyze::GetLineCompareInfo(string reference_file,
                                   vector<string> peer_file_data) {
  return LineCompare::GetLineInfo(reference_file, peer_file_data);
}
size_t Analyze::GetSignatureCompareInfo(string reference_file,
                                        vector<string> peer_file_data) {
  return SignatureCompare::GetSignatureInfo(reference_file, peer_file_data);
}
void Analyze::to_json(json& j, const AnalyzeInfo& i) {
  j = json{{"id", i.id},
           {"signature", i.signature_info},
           {"each_line", i.line_info},
           {"git_link", i.git_link},
           {"reference_file_path", i.file_path_ref},
           {"rewiew_file_path", i.file_path_review}};
}

void Analyze::from_json(const json& j, AnalyzeInfo& i) {
  j.at("id").get_to(i.id);
  j.at("signature").get_to(i.signature_info);
  j.at("each_line").get_to(i.line_info);
  j.at("git_link").get_to(i.git_link);
  j.at("reference_file_path").get_to(i.file_path_ref);
  j.at("rewiew_file_path").get_to(i.file_path_review);
}