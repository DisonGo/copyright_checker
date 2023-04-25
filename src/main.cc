
#include <fstream>
#include <iostream>
#include <vector>

#include "logger.h"
//#include "signature_check.h"
#include "line_check.h"
#include "repo_manager.h"
int main() {
  RepoManager man;
  RepoURLs urls = man.FetchRepoUrls("s21_math");
  man.DownloadRepos("/Users/evverenn/goinfre/repos", urls);
  std::cout << "\nDownloaded repositories:\n";
  for (auto& repo : man.repoPaths) {
    std::cout << repo.first << "\t" << repo.second << "\n";
  }
  return 0;
}