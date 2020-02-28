#include "MyQuery.h"
#include "ParamsAnalyzer.h"

int main(int argc, char** argv) {

    try {
        CommandLineQuery clq(argc, argv);
        if(clq.getAnalyzerStatus()){
            std::cout << "complete" << std::endl;
        }else{
            std::cerr << "Fetal Error!" << std::endl;
        }
    }catch (...){
        std::cerr << "Unhandled Exception" << std::endl;
    }



    return 0;
}
