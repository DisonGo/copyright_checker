#include "analyze.h"
using fm = FileManager;
#define CAT(repo, peer_files, results, id)                     \
  thread(&Analyze::AnalyzeProject, repo, std::ref(peer_files), \
         std::ref(results), id)

bool Analyze::CheckAnalyzeInput(const string& keyword,
                                const string& project_path) {
  bool k_empty = keyword.empty();
  bool p_p_empty = project_path.empty();
  if (k_empty) std::cerr << "Empty keyword\n";
  if (p_p_empty) std::cerr << "Empty project_path\n";
  return (k_empty || p_p_empty);
}
void Analyze::CloseAllThreads(pair<vector<thread>, vector<string>>& threads) {
  for (size_t i = 0; i < threads.first.size(); i++) {
    std::cout << "Thread id: " << i << " joining: " << threads.second[i]
              << "\n";
    threads.first[i].join();
    std::cout << "Next repository is " << threads.second[i + 1] << "\n";
  }
}
void Analyze::CloseAllThreads(vector<thread>& threads) {
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
void Analyze::StartAnalyze(string keyword, string project_path,
                           string repos_dir) {
  if (CheckAnalyzeInput(keyword, project_path)) return;
  RepoManager rm(repos_dir);
  rm.DownloadRepos(keyword);
  std::cout << "Used repositories: " << rm.repoPaths[keyword].size() << "\n";

  FilePathArrays peer_paths = fm::FindSourcesC(project_path);

  Analyze::PeerFilesData peer_files{};
  FillPeerFilesData(peer_paths, peer_files);
  if (peer_files.empty()) std::cout << "Peer files is empty\n";
#ifdef _DEBUG
  pair<vector<thread>, vector<string>> peer_files_thread;
#else
  vector<thread> peer_files_thread;
#endif  //  _DEBUG

  Logger::ThreadsAnalyzeResults results(rm.repoPaths[keyword].size());
  size_t id{};
  size_t index{};
  for (auto& repo : rm.repoPaths[keyword]) {
#ifdef _DEBUG
    peer_files_thread.first.push_back(
        CAT(repo, peer_files, results[index++], id++));
    peer_files_thread.second.push_back(repo.first);
#else
    peer_files_thread.push_back(CAT(repo, peer_files, results[index++], id++));
#endif  //  _DEBUG
  }

  CloseAllThreads(peer_files_thread);
  Logger::GetResultConfig(results);
}

void Analyze::AnalyzeProject(const RepoPair& reference_path,
                             PeerFilesData& peer_files,
                             vector<AnalyzeInfo>& results, size_t id) {
  size_t signature{};
  size_t line{};
  FilePathArrays paths = fm::FindSourcesC(reference_path.second);
  for (auto& peer_file : peer_files) {
    for (auto& reference_files : paths.sources) {
      signature = GetSignatureCompareInfo(reference_files, peer_file.first);
      line = GetLineCompareInfo(reference_files, peer_file.first);
      AnalyzeInfo current;
      current.id = id;
      current.git_link = reference_path.first;
      current.signature_info = signature;
      current.line_info = line;
      current.file_path_ref = reference_files;
      current.file_path_review = peer_file.second;
      results.push_back(current);
    }
  }
}
void Analyze::FillPeerFilesData(FilePathArrays& paths,
                                PeerFilesData& peerData) {
  FilesData filesdata;
  fm::ReadPathArrayData(paths.sources, filesdata);
  Analyze::FromFilesData(peerData, filesdata);
}

size_t Analyze::GetLineCompareInfo(const string& reference_file,
                                   FileData peer_file_data) {
  return LineCompare::GetLineInfo(reference_file, peer_file_data);
}

size_t Analyze::GetSignatureCompareInfo(const string& reference_file,
                                        FileData peer_file_data) {
  return SignatureCompare::GetSignatureInfo(reference_file, peer_file_data);
}
size_t Analyze::GetLineCompareInfo(const string& reference_file,
                                   const PeerFileData& peer_file_data) {
  return LineCompare::GetLineInfo(reference_file, peer_file_data);
}

size_t Analyze::GetSignatureCompareInfo(const string& reference_file,
                                        const PeerFileData& peer_file_data) {
  return SignatureCompare::GetSignatureInfo(reference_file, peer_file_data);
}
