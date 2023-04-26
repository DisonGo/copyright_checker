
#include <fstream>
#include <iostream>
#include <vector>

#include "logger.h"
#include "signature_check.h"
#include "line_check.h"
#include "repo_manager.h"
#include "file_manager.h"

std::string GetProjectName(int argc, char* argv[]);
std::string GetPath();

int main(int argc, char* argv[]) {
  if (argc < 3) {
      std::cout << "First argument should be project name.\n";
      std::cout << "Second argument should be peer name.\n";
      return 0;
  }
  RepoManager man;
  std::string ProjectName = GetProjectName(argc, argv);
  RepoURLs urls = man.FetchRepoUrls(ProjectName);
  man.DownloadRepos(GetPath(), urls);
  std::cout << "\nDownloaded repositories:\n";
  
  std::ofstream config_file("output.ini");
  InitLog(config_file, argv[2]);

  for (auto& repo : man.repoPaths) {
    std::cout << repo.first << "\t" << repo.second << "\n";
  }

  size_t id{};
  size_t signature{};
  size_t line{};
  FilePathArrays peer_paths = FileManager::FindSourcesC("./PeerReview");
  for (auto& path : man.repoPaths) {
    FilePathArrays paths = FileManager::FindSourcesC(path.second);
    for (auto& filepath : paths.second) {
        for (auto& review_file : peer_paths.second) {
            signature = GetSignatureInfo(filepath, review_file);
            line = GetLineInfo(filepath, review_file);
            if (signature > 50 || line > 50) WriteResult(config_file, id++, path.first, signature, line);
        }
    }
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