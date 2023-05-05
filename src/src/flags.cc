#include "flags.h"

bool Flags::IsConfirmProject() {
  std::cout << "Project name = " << GetProjectName() << "\n";
  std::cout << "Peer name = " << GetPeerName() << "\n";
  std::cout << "Path to the peer project = " << GetProjectPath() << "\n";
  std::cout << "Confirm?: ";
  string answer;
  std::cin >> answer;
  if (answer != "y" && answer != "yes") return false;
  return true;
}

int Flags::DetectFlags(int argc, char* argv[]) {
  if (argc == 1) Help();
  for (int i = 1; i < argc; i++) {
    string current = argv[i];
    if (current == "-help")
      Help();
    else if (current == "-project") {
      if (i == argc - 1) Help();
      project_name = argv[i++ + 1];
    } else if (current == "-name") {
      if (i == argc - 1) Help();
      peer_name = argv[i++ + 1];
    } else if (current == "-path") {
      if (i == argc - 1) Help();
      project_path = argv[i++ + 1];
    } else {
      Help();
    }
  }
  return 0;
}

int Flags::DetectFlags(int argc, vector<string> argv) {
  if (argc == 1) Help();
  for (int i = 1; i < argc; i++) {
    string& current = argv[i];
    if (current == "-help")
      Help();
    else if (current == "-project") {
      if (i == argc - 1) Help();
      project_name = argv[i++ + 1];
    } else if (current == "-name") {
      if (i == argc - 1) Help();
      peer_name = argv[i++ + 1];
    } else if (current == "-path") {
      if (i == argc - 1) Help();
      project_path = argv[i++ + 1];
    } else {
      Help();
    }
  }
  return 0;
}

int Flags::Help() {
  std::cout << "-project *project name*\n-name *peer name*\n-path *path to "
               "the peer project*\n";
  this->IsCorrect = false;
  exit(0);
}
