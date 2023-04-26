#pragma once
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <set>
#include <string>
#include <vector>

#include "file_manager.h"
#include "httplib.hpp"
using std::pair;
using std::set;
using std::string;
using std::vector;
typedef vector<string> RepoURLs;
class RepoManager {
 private:
  const string token{"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"};
  const string apiurl{"https://api.github.com"};
  const string searchRepoApiUrl{"/search/repositories"};
  httplib::Client cli{apiurl};
  string repoDir;
  string ParseGitNameFromStr(string str);
  void CheckRepoDir();

 public:
  RepoManager(string dir);
  ~RepoManager();
  set<pair<string, string>> repoPaths;
  RepoURLs FetchRepoUrls(string keyword);
  void DownloadRepos(RepoURLs urls);
};
