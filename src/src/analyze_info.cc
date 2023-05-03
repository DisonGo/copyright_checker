#include "analyze_info.h"

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