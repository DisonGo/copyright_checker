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

using std::string;
typedef std::vector<std::pair<std::vector<std::string>, string>> FilesData;

string GetProjectName(int argc, char* argv[]);
string GetPath();

struct AnalyzeInfo {
  size_t id{};
  string git_link;
  size_t signature_info{};
  size_t line_info{};
  string file_path_ref;
  string file_path_review;
};

void AnalyzeProject(const RepoPair& reference_path, FilesData& peer_files,
    AnalyzeInfo& result, size_t id) {
    size_t signature{};
    size_t line{};
    FilePathArrays paths = FileManager::FindSourcesC(reference_path.second);
    for (auto& peer_file : peer_files) {
      for (auto& reference_files : paths.second) {
        signature = GetSignatureInfo(reference_files, peer_file.first);
        line = GetLineInfo(reference_files, peer_file.first);
        result.id = id;
        result.git_link = reference_path.first;
        result.signature_info = signature;
        result.line_info = line;
        result.file_path_ref = reference_files;
        result.file_path_review = peer_file.second;
      }
    }
}

struct pred {
  bool operator()(const AnalyzeInfo& first, const AnalyzeInfo& second) {
    return first.signature_info > second.signature_info;
  }
};

int main(int argc, char* argv[]) {

  if (argc < 4) {
    std::cout << "First argument should be project name.\n";
    std::cout << "Second argument should be peer name.\n";
    std::cout << "Third argument should be a path to the peer project.\n";
    return 0;
  }
  RepoManager man(GetPath());
  string ProjectName = GetProjectName(argc, argv);
  RepoURLs urls = man.FetchRepoUrls(ProjectName);
  man.DownloadRepos(urls, ProjectName);
  std::cout << "Used repositories: " << man.repoPaths[ProjectName].size() << "\n";

  size_t id{};
  std::vector<AnalyzeInfo> analyze_result(100);
  FilePathArrays peer_paths = FileManager::FindSourcesC(
      argv[3]);

  std::vector<std::thread> peer_files_thread;

  FilesData peer_files;
  std::ifstream each_peer_file;
  for (auto& file : peer_paths.second) {
    each_peer_file.open(file);
    if (!each_peer_file.is_open()) continue;
    
    string buffer;
    std::vector<string> file_data;
    while (std::getline(each_peer_file, buffer)) {
      file_data.push_back(buffer);
    }

    peer_files.push_back({file_data, file});
    each_peer_file.close();
  }


  for (auto& repo : man.repoPaths[ProjectName]) {
      analyze_result.push_back({});
      peer_files_thread.push_back(std::thread(&AnalyzeProject, repo, std::ref(peer_files), std::ref(*(analyze_result.end() - 1)),
      id++));
  }

  size_t counter = 1;
  for (auto& thread : peer_files_thread) {
      thread.join();
      std::cout << "Project analyzed: " << counter++ << std::endl;
  }


  std::ofstream config_file("output.ini");
  InitLog(config_file, argv[2]);
  std::sort(analyze_result.begin(), analyze_result.end(), pred());
  for (auto& element : analyze_result) {
    WriteResult(config_file, element.id, element.git_link,
                element.signature_info, element.line_info,
                element.file_path_ref, element.file_path_review);
  }


  return 0;
}


string GetProjectName(int argc, char* argv[]) {
  if (argc < 2) exit(0);
  std::cout << "Project name = " << argv[1] << "\nConfirm?: ";
  string answer;
  std::cin >> answer;
  if (answer != "y") exit(0);
  return argv[1];
}

string GetPath() {
  string username = getenv("USER");
  return string("/Users/" + username + "/goinfre/repos");
}