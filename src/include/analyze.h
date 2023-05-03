#pragma once

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "analyze_info.h"
#include "file_manager.h"
#include "json.hpp"
#include "linecompare.h"
#include "logger.h"
#include "repo_manager.h"
#include "signaturecompare.h"
using json = nlohmann::json;
using std::pair;
using std::string;
using std::vector;
typedef vector<pair<vector<string>, string>> FilesData;

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
  static void to_json(json& j, const AnalyzeInfo& i);
  static void from_json(const json& j, AnalyzeInfo& i);
};
