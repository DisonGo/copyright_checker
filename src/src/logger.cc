#include "logger.h"

void InitLog(std::ofstream& log_file, std::string peer_name) {
    if (!log_file.is_open()) {
        std::cout << "File is not exist. . .";
        return;
    }

    log_file << "New peer is [ " << peer_name << " ]\n";
    log_file << "Id\tSignature\tEachLine\tReference\n";
}

void WriteResult(std::ofstream& log_file, size_t id, std::string git_link, int signature_percent, int line_percent) {
    if (!log_file.is_open()) {
        std::cout << "File is not exist. . .";
        return;
    }
    log_file << id << "\t" << signature_percent << "\t\t\t" << line_percent << "\t\t\t" << git_link << "\n";
}