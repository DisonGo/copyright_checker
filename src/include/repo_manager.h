#pragma once
#include <string>
#include <vector>

#include "httplib.hpp"
using std::string;
using std::vector;
typedef vector<string> RepoURLs;
class RepoManager {
 private:
  const string token{"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"};
  const string apiurl{"https://api.github.com"};
  const string searchRepoApiUrl{"/search/repositories"};
  httplib::Client cli;

 public:
  RepoManager();
  RepoURLs FetchRepoUrls(string keyword);
};
