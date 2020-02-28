#include "inc/CommandLineQuery.h"

int main(int argc, char** argv) {
    try{
        MyQueryTool::CommandLineQuery(argc, argv).getQueryResult();
    }
    catch (...){
        std::cerr << "Unhandled Exception!" << std::endl;
    }
    return 0;
}
