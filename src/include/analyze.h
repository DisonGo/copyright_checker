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
namespace Analyze {
using std::pair;
using std::string;
using std::thread;
using std::vector;
void CloseAllThreads(pair<vector<thread>, vector<string>>& threads);
void CloseAllThreads(vector<thread>& threads);

void StartAnalyze(string keyword, string project_path, string repos_dir = "");
bool CheckAnalyzeInput(const string& keyword, const string& project_path);
void AnalyzeProject(const RepoPair& reference_path, PeerFilesData& peer_files,
                    vector<AnalyzeInfo>& result, size_t id);

void FillPeerFilesData(FilePathArrays& paths, PeerFilesData& peerData);

size_t GetLineCompareInfo(const string& reference_file,
                          FileData peer_file_data);

size_t GetSignatureCompareInfo(const string& reference_file,
                               FileData peer_file_data);
size_t GetLineCompareInfo(const string& reference_file,
                          const PeerFileData& peer_file_data);

size_t GetSignatureCompareInfo(const string& reference_file,
                               const PeerFileData& peer_file_data);
}  // namespace Analyze
#endif  //  SRC_INCLUDE_NALYZE__H