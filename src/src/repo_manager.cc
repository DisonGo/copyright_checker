
#include "repo_manager.h"

#include "iostream"
#include "json.hpp"
#include "thread"
// CreateCloningThread
#define CCT(url, path, error_ref) \
  std::thread(&RepoManager::CloneRepoThread, url, path, std::ref(error_ref))
using std::thread;
using json = nlohmann::json;
string RepoManager::ParseGitNameFromStr(string str) {
  size_t last_slash_pos = str.find_last_of('/');

  string path = str.substr(0, last_slash_pos);
  string NameWithExt = str.substr(last_slash_pos + 1);

  string Name = NameWithExt.substr(0, NameWithExt.find_last_of('.'));

  last_slash_pos = path.find_last_of('/');
  string userName = path.substr(last_slash_pos + 1);

  string gitName = userName + '/' + Name;
  return gitName;
}

bool RepoManager::IsKeywordUsed(const string& keyword) {
  return repoPaths.find(keyword) != repoPaths.end();
}
bool RepoManager::IsPairDownloaded(const RepoPair& pair,
                                   const UniqueRepoPairs& pair_set) {
  return pair_set.find(pair) != pair_set.end();
}
void RepoManager::CheckRepoDir() {
  static const string gitPrefix = "https://github.com/";
  vector<string> keywords = FileManager::FindSubDirs(repoDir);
  for (auto& keyword : keywords) {
    string keyword_path = repoDir + "/" + keyword;
    vector<string> users = FileManager::FindSubDirs(keyword_path);
    UniqueRepoPairs repo_pairs;
    for (auto& user : users) {
      string user_path = keyword_path + "/" + user;
      vector<string> repos = FileManager::FindSubDirs(user_path);
      for (auto& repo : repos) {
        string repoPrefix = user + '/' + repo;
        string gitUrl = gitPrefix + repoPrefix + ".git";
        repo_pairs.insert({gitUrl, user_path + "/" + repo});
      }
    }
    repoPaths.insert({keyword, repo_pairs});
  }
}
void RepoManager::CloneRepoThread(const string& url, const string& path,
                                  pair<RepoPair, int>& repo_pair) {
  git_clone_pair c_p;
  repo_pair.second = git_clone(&c_p.repo, url.c_str(), path.c_str(), &c_p.opt);
  if (repo_pair.second) std::cerr << git_error_last()->message << "\n";
}
int RepoManager::CloneRepo(const string& url, const string& path) {
  git_clone_pair c_p;
  int error = git_clone(&c_p.repo, url.c_str(), path.c_str(), &c_p.opt);
  if (error) std::cerr << git_error_last()->message << "\n";
  return error;
}
RepoManager::RepoManager() {
  repoDir = DefaultRepoPath();
  git_libgit2_init();
  CheckRepoDir();
  cli.set_bearer_token_auth(token);
  cli.enable_server_certificate_verification(false);
}
RepoManager::RepoManager(string dir) {
  if (*(dir.end() - 1) == '/') dir.erase(dir.end() - 1, dir.end());
  repoDir = dir.empty() ? DefaultRepoPath() : dir;
  git_libgit2_init();
  CheckRepoDir();
  cli.set_bearer_token_auth(token);
  cli.enable_server_certificate_verification(false);
}
RepoManager::~RepoManager() { git_libgit2_shutdown(); }
RepoURLs RepoManager::FetchRepoUrls(string keyword) {
  RepoURLs urls;
  if (!cli.is_valid()) return urls;
  try {
    httplib::Params params = {{"q", keyword}};
    httplib::Headers headers = {{"Accept", "application/vnd.github+json"}};

    auto res = cli.Get("/search/repositories", params, headers);

    json responseJSON = json::parse(res->body);
    json items = responseJSON["items"];

    for (auto& repo : items) urls.push_back(repo["clone_url"]);

  } catch (const std::exception& e) {
    std::cout << "Response Error:" << e.what() << "\n";
  }
  return urls;
}

void RepoManager::DownloadRepos(const string& keyword) {
  RepoURLs urls = FetchRepoUrls(keyword);
  const bool keyword_used = IsKeywordUsed(keyword);
  if (!keyword_used) repoPaths.insert({keyword, {}});
  auto& current_key_set = repoPaths[keyword];

  vector<thread> threads;
  vector<pair<RepoPair, int>> errors(urls.size());

  for (auto& error : errors) error.second = -2;

  for (auto& url : urls) {
    string gitName = ParseGitNameFromStr(url);
    string path = repoDir + "/" + keyword + "/" + gitName;

    RepoPair new_pair{url, path};

    if (keyword_used && IsPairDownloaded(new_pair, current_key_set)) continue;

    errors.push_back({new_pair, 0});
    auto& last_error = errors[errors.size() - 1];

    threads.push_back(CCT(url, path, last_error));
  }

  for (auto& th : threads) th.join();

  for (auto& error : errors)
    if (!error.second) current_key_set.insert(error.first);
}
void RepoManager::DownloadRepos(const RepoURLs& urls, const string& keyword) {
  const bool keyword_used = IsKeywordUsed(keyword);
  if (!keyword_used) repoPaths.insert({keyword, {}});
  auto& current_key_set = repoPaths[keyword];

  vector<thread> threads;
  vector<pair<RepoPair, int>> errors(urls.size());

  for (auto& error : errors) error.second = -2;

  for (auto& url : urls) {
    string gitName = ParseGitNameFromStr(url);
    string path = repoDir + "/" + keyword + "/" + gitName;

    RepoPair new_pair{url, path};

    if (keyword_used && IsPairDownloaded(new_pair, current_key_set)) continue;

    errors.push_back({new_pair, 0});
    auto& last_error = errors[errors.size() - 1];

    threads.push_back(CCT(url, path, last_error));
  }

  for (auto& th : threads) th.join();

  for (auto& error : errors)
    if (!error.second) current_key_set.insert(error.first);
}

string RepoManager::DefaultRepoPath() {
  string username = getenv("USER");
  return string("/Users/" + username + "/goinfre/repos");
}
