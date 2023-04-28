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

typedef std::vector<std::pair<std::vector<std::string>, string>> FilesData;

struct AnalyzeInfo {
  size_t id{};
  std::string git_link;
  size_t signature_info{};
  size_t line_info{};
  std::string file_path_ref;
  std::string file_path_review;
};
class Analyze {
 public:
  Analyze() {}
  ~Analyze() {}

  friend class LineCompare;
  friend class SignatureCompare;

  static void AnalyzeProject(const RepoPair& reference_path,
                             FilesData& peer_files,
                             std::vector<AnalyzeInfo>& result, size_t id) {
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

 private:
  static size_t GetLineCompareInfo(std::string reference_file,
                                   std::vector<std::string> peer_file_data) {
    size_t result{};
    LineCompare obj;
    result = obj.GetLineInfo(reference_file, peer_file_data);
    return result;
  }
  static size_t GetSignatureCompareInfo(
      std::string reference_file, std::vector<std::string> peer_file_data) {
    size_t result{};
    SignatureCompare obj;
    result = obj.GetSignatureInfo(reference_file, peer_file_data);
    return result;
  }
};
