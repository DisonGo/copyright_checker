// #define _DEBUG

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "analyze.h"
#include "file_manager.h"
#include "flags.h"
#include "logger.h"
#include "repo_manager.h"

using std::string;

#ifdef _DEBUG
void CloseAllThreads(
    std::pair<std::vector<std::thread>, std::vector<std::string>>& threads);
#endif  //  _DEBUG

bool IsConfirmProject(Flags& flags);
string GetPath();
void FillPeerFilesData(FilePathArrays& paths, FilesData& filesdata);
void CloseAllThreads(std::vector<std::thread>& threads);
void GetResultConfig(string config_file, string peer_name,
                     std::vector<std::vector<AnalyzeInfo>>& analyze_results);
string ShowCreators() { return "@DisonGo && @simphoniia"; }
struct pred {
  bool operator()(const AnalyzeInfo& first, const AnalyzeInfo& second) {
    return first.signature_info > second.signature_info;
  }
};

int main() {
  vector<string> argv2 = {
      "main",
      "-project",
      "s21_stringplus",
      "-name",
      "evverenn",
      "-path",
      "/Users/evverenn/Desktop/Projects/C2_s21_stringplus-1"};
  Flags flags(argv2.size(), argv2);
  if (flags.GetState() == false) return 1;
  std::cout << ShowCreators() << "\n";
  RepoManager man(GetPath());
  // if (!IsConfirmProject(flags)) return 1;
  string project_name = flags.GetProjectName();
  RepoURLs urls = man.FetchRepoUrls(project_name);
  man.DownloadRepos(urls, project_name);
  std::cout << "Used repositories: " << man.repoPaths[project_name].size()
            << "\n";

  // for (auto& var : man.repoPaths[project_name]) {
  //   std::cout << var.first << "\t" << var.second << "\n";
  // }

  size_t id{};
  FilePathArrays peer_paths = FileManager::FindSourcesC(flags.GetProjectPath());

  FilesData peer_files;
  FillPeerFilesData(peer_paths, peer_files);
  if (peer_files.empty()) std::cout << "Peer files is empty\n";
#ifdef _DEBUG
  std::pair<std::vector<std::thread>, std::vector<std::string>>
      peer_files_thread;
#else
  std::vector<std::thread> peer_files_thread;
#endif  //  _DEBUG

  std::vector<std::vector<AnalyzeInfo>> results(
      man.repoPaths[project_name].size());
  size_t index{};
  for (auto& repo : man.repoPaths[project_name]) {
#ifdef _DEBUG
    peer_files_thread.first.push_back(
        std::thread(&Analyze::AnalyzeProject, repo, std::ref(peer_files),
                    std::ref(results[index++]), id++));
    peer_files_thread.second.push_back(repo.first);
#else
    peer_files_thread.push_back(std::thread(&Analyze::AnalyzeProject, repo,
                                            std::ref(peer_files),
                                            std::ref(results[index++]), id++));
#endif  //  _DEBUG
  }

  CloseAllThreads(peer_files_thread);
  GetResultConfig("output.txt", flags.GetPeerName(), results);

  return 0;
}

void GetResultConfig(string config_file, string peer_name,
                     std::vector<std::vector<AnalyzeInfo>>& analyze_results) {
  std::ofstream config(config_file);
  InitLog(config, peer_name);

  std::vector<AnalyzeInfo> results;
  for (auto& result_array : analyze_results) {
    results.insert(results.end(), result_array.begin(), result_array.end());
  }

  std::sort(results.begin(), results.end(), pred());
  for (auto& element : results) {
    WriteResult(config, element.id, element.git_link, element.signature_info,
                element.line_info, element.file_path_ref,
                element.file_path_review);
  }
}

void FillPeerFilesData(FilePathArrays& paths, FilesData& filesdata) {
  FileManager::ReadPathArrayData(paths.sources, filesdata);
}

void CloseAllThreads(std::vector<std::thread>& threads) {
  size_t threads_count = threads.size();
  while (threads_count) {
    for (size_t i = 0; i < threads.size(); i++) {
      if (threads[i].joinable()) {
        threads[i].join();
        threads_count--;
        std::cout << "Threads left: " << threads_count << "\n";
      }
    }
  }
}

#ifdef _DEBUG
void CloseAllThreads(
    std::pair<std::vector<std::thread>, std::vector<std::string>>& threads) {
  for (size_t i = 0; i < threads.first.size(); i++) {
    std::cout << "Thread id: " << i << " joining: " << threads.second[i]
              << "\n";
    threads.first[i].join();
    std::cout << "Next repository is " << threads.second[i + 1] << "\n";
  }
}
#endif  //  _DEBUG

bool IsConfirmProject(Flags& flags) {
  std::cout << "Project name = " << flags.GetProjectName() << "\n";
  std::cout << "Peer name = " << flags.GetPeerName() << "\n";
  std::cout << "Path to the peer project = " << flags.GetProjectPath() << "\n";
  std::cout << "Confirm?: ";
  string answer;
  std::cin >> answer;
  if (answer != "y" && answer != "yes") return false;
  return true;
}

string GetPath() {
  string username = getenv("USER");
  return string("/Users/" + username + "/goinfre/repos");
}