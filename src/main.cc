#define _DEBUG

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "file_manager.h"
#include "logger.h"
#include "repo_manager.h"
#include "flags.h"
#include "analyze.h"

using std::string;
typedef std::vector<std::pair<std::vector<std::string>, string>> FilesData;

constexpr time_t TIMEOUT = 20;

#ifdef _DEBUG
void CloseAllThreads(std::pair<std::vector<std::thread>, RepoPair>& threads);
#endif  //  _DEBUG

bool IsConfirmProjectName(std::string project_name);
string GetPath();
void FillPeerFilesData(FilePathArrays& paths, FilesData& filesdata);
void CloseAllThreads(std::vector<std::thread>& threads);
void GetResultConfig(string config_file, string peer_name, std::vector<std::vector<AnalyzeInfo>>& analyze_results);
struct pred {
  bool operator()(const AnalyzeInfo& first, const AnalyzeInfo& second) {
    return first.signature_info > second.signature_info;
  }
};

int main(int argc, char* argv[]) {

  Flags flags(argc, argv);
  if (flags.GetState() == false) return 1;
  RepoManager man(GetPath());
  if (!IsConfirmProjectName(flags.GetProjectName())) return 1;
  RepoURLs urls = man.FetchRepoUrls(flags.GetProjectName());
  man.DownloadRepos(urls, flags.GetProjectName());
  std::cout << "Used repositories: " << man.repoPaths[flags.GetProjectName()].size() << "\n";

  for (auto& var : man.repoPaths[flags.GetProjectName()]) {
    std::cout << var.first << "\t" << var.second << "\n";
  }

  size_t id{};
  FilePathArrays peer_paths = FileManager::FindSourcesC(flags.GetProjectPath());


  FilesData peer_files;
  FillPeerFilesData(peer_paths, peer_files);
  
  #ifdef _DEBUG
    std::pair<std::vector<std::thread>, RepoPair> peer_files_thread;
  #else
    std::vector<std::thread> peer_files_thread;
  #endif  //  _DEBUG

  std::vector<std::vector<AnalyzeInfo>> results(man.repoPaths[flags.GetProjectName()].size());
  size_t index{};
  for (auto& repo : man.repoPaths[flags.GetProjectName()]) {
      peer_files_thread.first.push_back(std::thread(&Analyze::AnalyzeProject, repo, std::ref(peer_files), std::ref(results[index++]), id++));
      peer_files_thread.second = repo;
  }

  CloseAllThreads(peer_files_thread);
  GetResultConfig("output.txt", flags.GetPeerName(), results);

  return 0;
}

void GetResultConfig(string config_file, string peer_name, std::vector<std::vector<AnalyzeInfo>>& analyze_results) {
  std::ofstream config(config_file);
  InitLog(config, peer_name);

  std::vector<AnalyzeInfo> results;
  for (auto& result_array : analyze_results) {
    results.insert(results.end(), result_array.begin(), result_array.end());
  }

  std::sort(results.begin(), results.end(), pred());
  for (auto& element : results) {
    WriteResult(config, element.id, element.git_link,
                element.signature_info, element.line_info,
                element.file_path_ref, element.file_path_review);
  }
}

void FillPeerFilesData(FilePathArrays& paths, FilesData& filesdata) {
  std::ifstream each_peer_file;
  for (auto& file : paths.second) {
    each_peer_file.open(file);
    if (!each_peer_file.is_open()) continue;
  
    string buffer;
    std::vector<string> file_data;
    while (std::getline(each_peer_file, buffer))
      file_data.push_back(buffer);

    filesdata.push_back({file_data, file});
    each_peer_file.close();
  }
}

void CloseAllThreads(std::vector<std::thread>& threads) {
  std::time_t join_start = std::time(nullptr);
  std::time_t join_end;

  size_t thread_size = threads.size();
  while (thread_size > 0) {
    for (size_t i = 0; i < threads.size(); i++) {
      if (threads[i].joinable()) {
        threads[i].join();
        std::cout << "thread id: " << (i + 1) << " finished!\n";
        thread_size--;
        join_start = std::time(nullptr);
      } else {
        join_end = std::time(nullptr);
        if (join_end - join_start > TIMEOUT) {
          threads[i].detach();
          thread_size--;
        }
        std::cout << "thread id: " << (i + 1) << " DETACHED!\n";
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

#ifdef _DEBUG
void CloseAllThreads(std::pair<std::vector<std::thread>, RepoPair>& threads) {
  for (size_t i = 0; i < threads.first.size(); i++) {
    if (i == 2 || i == 11) {
      std::cout << "Detached thread is " << i << "\n";
      std::cout << "Project is " << threads.second.first << "\n";
      threads.first[i].detach();
    }
    if (threads.first[i].joinable()) {
      std::cout << "Joined thread is " << i << "\n";
      std::cout << "Project is " << threads.second.first << "\n";
      threads.first[i].join();
    }
  }
}
#endif  //  _DEBUG


bool IsConfirmProjectName(std::string project_name) {
  std::cout << "Project name = " << project_name << "\nConfirm?: ";
  string answer;
  std::cin >> answer;
  if (answer != "y") return false;
  return true;
}

string GetPath() {
  string username = getenv("USER");
  return string("/Users/" + username + "/goinfre/repos");
}