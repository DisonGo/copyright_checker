#pragma once
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <set>
#include <string>
#include <vector>

#include "file_manager.h"
#include "httplib.hpp"
using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;
typedef vector<string> RepoURLs;
typedef pair<string, string> RepoPair;
typedef set<RepoPair> UniqueRepoPairs;
class RepoManager {
 private:
  const string token{"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"};
  const string apiurl{"https://api.github.com"};
  const string searchRepoApiUrl{"/search/repositories"};
  httplib::Client cli{apiurl};
  string repoDir;
  string ParseGitNameFromStr(string str);
  bool IsKeywordUsed(const string& keyword);
  bool IsPairDownloaded(const RepoPair& pair, const UniqueRepoPairs& pair_set);
  void CheckRepoDir();
  int CloneRepo(const string& url, const string& path);

 public:
  RepoManager();
  RepoManager(string dir);
  ~RepoManager();
  map<string, UniqueRepoPairs> repoPaths;
  RepoURLs FetchRepoUrls(string keyword);
  void DownloadRepos(RepoURLs urls, const string& keyword);
  static string DefaultRepoPath();
};
