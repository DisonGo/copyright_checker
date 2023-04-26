#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "file_manager.h"
#include "line_check.h"
#include "logger.h"
#include "repo_manager.h"
#include "signature_check.h"

std::string GetProjectName(int argc, char* argv[]);
std::string GetPath();

struct AnalyzeInfo {
  size_t id{};
  std::string git_path;
  size_t signature_info{};
  size_t line_info{};
  std::string file_path_ref;
  std::string file_path_review;
};

struct pred {
  bool operator()(const AnalyzeInfo& first, const AnalyzeInfo& second) {
    return first.signature_info > second.signature_info;
  }
};

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cout << "First argument should be project name.\n";
    std::cout << "Second argument should be peer name.\n";
    return 0;
  }
  RepoManager man(GetPath());
  std::string ProjectName = GetProjectName(argc, argv);
  RepoURLs urls = man.FetchRepoUrls(ProjectName);
  man.DownloadRepos(urls, ProjectName);
  std::cout << "\nDownloaded repositories:\n";

  for (auto& repo : man.repoPaths[ProjectName]) {
    std::cout << repo.first << "\t" << repo.second << "\n";
  }

  size_t id{};
  size_t signature{};
  size_t line{};
  std::vector<AnalyzeInfo> data;
  FilePathArrays peer_paths = FileManager::FindSourcesC(
      "/Users/evverenn/Desktop/Projects/C3_SimpleBashUtils-1");
  for (auto& path : man.repoPaths[ProjectName]) {
    FilePathArrays paths = FileManager::FindSourcesC(path.second);
    for (auto& filepath : paths.second) {
      for (auto& review_file : peer_paths.second) {
        signature = GetSignatureInfo(filepath, review_file);
        line = GetLineInfo(filepath, review_file);
        if (signature > 45 || line > 45) {
          AnalyzeInfo current;
          current.id = id++;
          current.git_path = path.first;
          current.signature_info = signature;
          current.line_info = line;
          current.file_path_ref = filepath;
          current.file_path_review = review_file;
          data.push_back(current);
        }
      }
    }
  }

  std::ofstream config_file("output.ini");
  InitLog(config_file, argv[2]);

  std::sort(data.begin(), data.end(), pred());
  for (auto& element : data) {
    WriteResult(config_file, element.id, element.git_path,
                element.signature_info, element.line_info,
                element.file_path_ref, element.file_path_review);
  }

  return 0;
}

std::string GetProjectName(int argc, char* argv[]) {
  if (argc < 2) exit(0);
  std::cout << "Project name = " << argv[1] << "\nConfirm?: ";
  std::string answer;
  std::cin >> answer;
  if (answer != "y") exit(0);
  return argv[1];
}

std::string GetPath() {
  std::string username = getenv("USER");
  return std::string("/Users/" + username + "/goinfre/repos");
}