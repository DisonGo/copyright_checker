#pragma once
#include "string"
using std::string;

struct AnalyzeInfo {
  size_t id{};
  string git_link;
  size_t signature_info{};
  size_t line_info{};
  string file_path_ref;
  string file_path_review;
};
