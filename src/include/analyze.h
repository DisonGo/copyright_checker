#pragma once

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "file_manager.h"
#include "linecompare.h"
#include "logger.h"
#include "repo_manager.h"
#include "signaturecompare.h"

using std::pair;
using std::string;
using std::vector;
typedef vector<pair<vector<string>, string>> FilesData;
struct AnalyzeInfo {
  size_t id{};
  string git_link;
  size_t signature_info{};
  size_t line_info{};
  string file_path_ref;
  string file_path_review;
};
class Analyze {
 public:
  Analyze() {}
  ~Analyze() {}

  friend class LineCompare;
  friend class SignatureCompare;

  static void AnalyzeProject(const RepoPair& reference_path,
                             FilesData& peer_files, vector<AnalyzeInfo>& result,
                             size_t id);

 private:
  static size_t GetLineCompareInfo(string reference_file,
                                   vector<string> peer_file_data);
  static size_t GetSignatureCompareInfo(string reference_file,
                                        vector<string> peer_file_data);
};
