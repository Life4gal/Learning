/**
 * @file CommandLineQuery header file
 */

//#define DEBUG_COMMANDLINE

#ifndef COMMANDLINEQUERY_H
#define COMMANDLINEQUERY_H

#include "MyQuery.h"
#include "ParamsAnalyzer.h"

namespace MyQueryTool{
    class CommandLineQuery{
    public:
        CommandLineQuery(int argc, char** argv);

        explicit CommandLineQuery(const std::string&);

        void getQueryResult() const;

    private:
        std::shared_ptr<Analyzer> _analyzer;
    };

    static void showHelpMsg() {
        std::cout << "Incorrect Command Format!\n"
                     "\t-f query_files [-t output_files] -s query_words\n"
                     "\t\t-f Specify the file to be queried, you can specify any number of query file\n"
                     "\t\t\tIf the queried file is not in the same directory with the program, enter the path and file name together\n"
                     "\t\t-t Specify output mode, -t = output to file only, -T = output to file and std::cout together\n"
                     "\t\t\tOutput to std::cout only by default(no -t|T command)\n"
                     "\t\t-s Simple whole word matching\n"
                     "\t\t\t -s~ = Specify the word does not appear\n"
                     "\t\t\t -s| = Where any specified word appears\n"
                     "\t\t\t -s& = Where all specified word appears\n"
                     "\t\t-S Simple incomplete word matching\n"
                     "\t\t\t -S~ = Specify the word does not appear(incomplete match)\n"
                     "\t\t\t -S| = Where any specified word appears(incomplete match)\n"
                     "\t\t\t -S& = Where all specified word appears(incomplete match)\n"
                     "\tDo not try to query query_file_name|output_file_name with [space|any command(-f-t-s)] in the name, otherwise it will be in vain\n"
                     << std::endl;
    }
}


#endif //COMMANDLINEQUERY_H
