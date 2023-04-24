#include "line_check.h"

int GetLineInfo(std::string reference_file, std::string checked_file) {
    if (checked_file.empty() || reference_file.empty()) return 0;

    std::ifstream ref(reference_file);
    std::ifstream checked(checked_file);
    if (!ref.is_open() || !checked.is_open()) {
        std::cout << "Reference or checked file is not exist.";
        return 0;
    }

    std::vector<std::string> reference;
    std::vector<std::string> check;

    std::string buffer;
    while (std::getline(ref, buffer)) {
        if (buffer.size() > 1) reference.push_back(buffer);
    }
    buffer.clear();
    while (std::getline(checked, buffer)) {
        if (buffer.size() > 1) check.push_back(buffer);
    }


    return GetMatchPercentage(reference, check);
}

int GetMatchPercentage(std::vector<std::string>& ref, std::vector<std::string>& check) {

    size_t all_count{};
    size_t matched_count{};
    std::regex rx;
    bool found{};

    for (size_t i = 0; i < check.size(); i++) {
        found = false;
        rx.assign(check[i]);
        for (size_t j = 0; j < ref.size() && !found; j++) {
            found = regex_match(ref[i].begin(), ref[i].end(), rx);
            if (found) matched_count++;
        }
        found = false;
        all_count++;
    }

    size_t reference_size = ref.size();
    size_t check_size = check.size();

    reference_size > check_size ?
        all_count += reference_size - check_size : all_count += check_size - reference_size;
    


    return (100 / all_count * matched_count);
}