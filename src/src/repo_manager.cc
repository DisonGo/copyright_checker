
#include "repo_manager.h"

#include "git2.h"
#include "iostream"
#include "json.hpp"

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
RepoManager::RepoManager() {
  git_libgit2_init();
  cli.set_bearer_token_auth(token);
  cli.enable_server_certificate_verification(false);
}
RepoManager::~RepoManager() { git_libgit2_shutdown(); }
RepoURLs RepoManager::FetchRepoUrls(string keyword) {
  RepoURLs urls;
  try {
    if (cli.is_valid()) {
      httplib::Params params = {{"q", keyword}};
      httplib::Headers headers = {{"Accept", "application/vnd.github+json"}};
      auto res = cli.Get("/search/repositories", params, headers);
      json responseJSON = json::parse(res->body);
      json items = responseJSON["items"];
      for (auto& repo : items) {
        urls.push_back(repo["clone_url"]);
      }
    }
  } catch (const std::exception& e) {
    std::cout << "Response Error:" << e.what() << "\n";
  }
  return urls;
}

void RepoManager::DownloadRepos(string dir, RepoURLs urls) {
  for (auto& url : urls) {
    string gitName = ParseGitNameFromStr(url);
    string path = dir + "/" + gitName;
    git_clone_options opt;
    git_clone_init_options(&opt, GIT_CLONE_OPTIONS_VERSION);
    opt.bare = 0;
    git_repository* repo = NULL;
    std::cout << "Downloading " << url << "\n";
    int error = git_clone(&repo, url.c_str(), path.c_str(), &opt);
    if (error)
      std::cerr << git_error_last()->message << "\n";
    else
      repoPaths.insert({url, path});
  }
}
