#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "repo_manager.h"

#include "iostream"
#include "json.hpp"

using json = nlohmann::json;

RepoManager::RepoManager() : cli(apiurl) {
  cli.set_bearer_token_auth(token);
  cli.enable_server_certificate_verification(false);
}
RepoURLs RepoManager::FetchRepoUrls(string keyword) {
  RepoURLs urls;
  httplib::Params params = {{"q", keyword}};
  httplib::Headers headers = {{"Accept", "application/vnd.github+json"}};
  try {
    auto res = cli.Get(searchRepoApiUrl, params, headers);
    json responseJSON = json::parse(res->body);
    json items = responseJSON["items"];
    for (auto& repo : items) {
      urls.push_back(repo["url"]);
    }
  } catch (const std::exception& e) {
    std::cout << "Response Error:" << e.what() << "\n";
  }
  return urls;
}
