
#include <iostream>
#include <vector>
#include <fstream>

#include "logger.h"
//#include "signature_check.h"
#include "line_check.h"

int main() {

    srand(time(NULL));

    std::string git_info = "https://hello_world/username";
    std::ofstream obj;
    obj.open("output.txt");
    InitLog(obj, std::string("manhunte"));
    
    int line_info = GetLineInfo("/Users/burnsdov/Eye_of_Verter/src/src/reference.txt", "/Users/burnsdov/Eye_of_Verter/src/src/dolboeb.txt");
    std::cout << line_info << "%";



    return 0;
}