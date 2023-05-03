#ifndef SRC_INCLUDE_ANALYZE_H
#define SRC_INCLUDE_ANALYZE_H

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "analyze_info.h"
#include "file_manager.h"
#include "linecompare.h"
#include "logger.h"
#include "repo_manager.h"
#include "signaturecompare.h"
using std::pair;
using std::string;
using std::vector;
typedef vector<pair<FileData, string>> FilesData;
namespace Analyze {
void AnalyzeProject(const RepoPair& reference_path, FilesData& peer_files,
                    vector<AnalyzeInfo>& result, size_t id);
size_t GetLineCompareInfo(string reference_file, vector<string> peer_file_data);
size_t GetSignatureCompareInfo(string reference_file,
                               vector<string> peer_file_data);
}  // namespace Analyze
#endif  //  SRC_INCLUDE_NALYZE__H