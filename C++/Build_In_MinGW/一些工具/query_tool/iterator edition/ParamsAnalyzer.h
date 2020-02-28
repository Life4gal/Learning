#ifndef PARAMSANALYZER_H
#define PARAMSANALYZER_H

//#define DEBUG

/**
 * 这个文件的类不提供任何对外接口，全部依靠友元类 CommandLineQuery 进行交互
 */

#include <iostream>
#include <cstring>
#include <string>

#include <map>
#include <vector>
#include <set>

#include <memory>

class CommandLineQuery;
class Analyzer;

class ParamsTable{
    friend class Analyzer;

    /**
     * 以 -f 进行分隔，用 ParamsTable 类实际储存每次操作
     * 实际内容可以是 query_files... -t output_file1 -s word1 word2 -t output_file2 -s~ word3 word4 ......
     * 输出位置和查询单词可以变，但是查询的文件一直是这些
     * @param begin -f 命令所在位置的迭代器
     * @param end 下一个 -f 命令所在位置的迭代器
     */
    ParamsTable(const std::vector<std::string>::iterator& begin, const std::vector<std::string>::iterator& end){

        //指定输出命令
        const static std::set<std::string> output_command = {"-T", "-t"};
        //指定查询方式命令
        const static std::set<std::string> query_command = {"-s", "-s~", "-s|", "-s&"/*, "-S", "-S~", "-S|", "-S&"*/};

        //一个代替迭代器首移动的迭代器(让 begin 保持标识意义)
        std::vector<std::string>::iterator command_interval = begin; // NOLINT(hicpp-use-auto,modernize-use-auto)
        //一个用于遍历目标的迭代器
        std::vector<std::string>::iterator seeker = begin; // NOLINT(hicpp-use-auto,modernize-use-auto)

        //这个迭代器记录第一个查询的单词开始的位置，避免无法查询以 "-" 开头的单词
        std::vector<std::string>::iterator qc_pos = begin; // NOLINT(hicpp-use-auto,modernize-use-auto)
        //记录可能存在的 -t 命令
        std::vector<std::string>::iterator oc_pos = end; // NOLINT(hicpp-use-auto,modernize-use-auto)

        //进行的操作
        std::string operation;
        //输出的文件
        std::set<std::string> output_files;
        //查询的单词
        std::set<std::string> query_words;
        //因为迭代器是按照每次指定查询命令 -f 分隔的，所以查询的文件输入只有一次，暂时想不到好的不失一般性的办法
        bool isFileGot = false;

        //要求用户必须将 -t(如果有的话) 命令放置在 -s 命令之前
        while(command_interval != end){
            while(seeker != end){
                #ifdef DEBUG
                    std::cout << "Seeker command : " << *seeker << std::endl;
                #endif
                if((*seeker)[0] == '-'){
                    //到达一个疑似命令的位置，如果此时已经找到了查询单词操作的命令的位置，则如果确定是命令就可以将这一部分的参数分离(不论有没有输出命令)
                    if(qc_pos != begin && (query_command.find(*seeker) != query_command.end() || output_command.find(*seeker) != output_command.end())){
                        break;
                    }else if(output_command.find(*seeker) != output_command.end()){
                        oc_pos = seeker;
                    }else if(query_command.find(*seeker) != query_command.end()){
                        qc_pos = seeker;
                    }
                }
                ++seeker;
            }

            if(*qc_pos == "-s" /*|| *qc_pos == "-S"*/){
                //不失一般性，将 -s 长度变为3
                *qc_pos += "-";
            }

            if(oc_pos != end){
                //即已经找到了 -t 命令的位置

                if(!isFileGot){
                    //从 begin 之后的一个位置起至 oc_pos 的前一个位置为止都应该是查询的文件
                    std::vector<std::string>::iterator query_iterator(++command_interval); // NOLINT(hicpp-use-auto,modernize-use-auto)
                    while(query_iterator != oc_pos){
                        #ifdef DEBUG
                            std::cout << "query_file inserting! file : " << *query_iterator << std::endl;
                        #endif
                        _query_files.insert(*query_iterator++);
                    }
                    isFileGot = true;
                }

                //如果有 -t 命令那么命令长度就应该为4，换句话说，命令长度不为4则就应该没有 -t/-T 命令
                if(*oc_pos == "-t"){
                    //不失一般性，如果是 -t 则在命令之后加上 "-"
                    *qc_pos += "-";
                }else if(*oc_pos == "-T"){
                    //如果是 -T 加上标记
                    *qc_pos += "T";
                }

                //没有输出文件，那么就输出到 std::cout，换句话说，没有输出文件就应该没有 -t/-T 命令
                //从 oc_pos 之后的一个位置起至 qc_pos 的前一个位置为止都应该是输出的目标文件
                std::vector<std::string>::iterator output_iterator(++oc_pos); // NOLINT(hicpp-use-auto,modernize-use-auto)
                while(output_iterator != qc_pos){
                    output_files.insert(*output_iterator++);
                }
            }else if(!isFileGot){
                //没有 -t 命令且没有取得查询文件

                //从 begin 之后的一个位置起至 qc_pos 的前一个位置为止都应该是查询的文件
                std::vector<std::string>::iterator query_iterator(++command_interval); // NOLINT(hicpp-use-auto,modernize-use-auto)
                while(query_iterator != qc_pos){
                    #ifdef DEBUG
                        std::cout << "query_file " << *query_iterator << " inserting!" << std::endl;
                    #endif
                    _query_files.insert(*query_iterator++);
                }
                isFileGot = true;
            }
            operation = *qc_pos;
            //从 qc_pos 之后的一个位置起至 seeker 的前一个位置为止都应该是搜索的单词
            std::vector<std::string>::iterator word_iterator(++qc_pos); // NOLINT(hicpp-use-auto,modernize-use-auto)
            while(word_iterator != seeker){
                query_words.insert(*word_iterator++);
            }
            std::map<std::set<std::string>, std::set<std::string>> m;
            m.emplace(query_words, output_files);
            _operator_map.emplace(operation, m);

            //将 begin 移动到 seeker 的位置然后让 seeker 继续向后移动
            #ifdef DEBUG
                std::cout << "place begin to seeker pos!" << std::endl;
            #endif
            command_interval = seeker;
            //将 oc_command 移动回 end 位置，因为不需要保留输出命令，如果没有输出命令就输出到 std::cout
            oc_pos = end;
            //将 qc_command 移动回 begin 位置，因为不需要保留查询命令，如果之后有别的查询命令那就应该使用那一种查询方式
            qc_pos = begin;
            //将 operation 置空，因为一旦更换了查询方式那就应该获取到新的查询操作，如果因为什么奇怪的原因没有获取到也能够检查到
            operation.clear();
            //将 output_file 置空，因为不需要保留输出方式到下一次查询
            output_files.clear();
            //将 query_word 置空，因为不需要保留查询单词到下一次查询
            query_words.clear();
        }
    }

    ParamsTable() = default;

    //所有查询的文件
    std::set<std::string> _query_files;

    std::map<
        std::string /* 查询操作 */ ,
        std::map<
            std::set<std::string> /* 查询的单词 */ ,
            std::set<std::string> /* 输出的文件 */
                >
            >_operator_map;
};

/**
 * 一个分析给定命令的类
 * []中的命令可以省略，省略输出目标则只输出至标准输出流
 *
 * "-f"  查询的目标文件
 * "-t"  输出的文件
 * "-T"  既输出到文件，又输出到标准输出流
 * "-s"  指定查询单词
 * "-s~" 查询指定单词非
 * "-s|" 查询指定单词或
 * "-s&" 查询指定单词与
 * "-S"  全局指定模式查询指定单词
 * "-S~" 全局指定模式查询指定单词非
 * "-S|" 全局指定模式查询指定单词或
 * "-S&" 全局指定模式查询指定单词与
 * 注意：如果指定了全局模式，则从指定模式起直到指定其他全局模式为止"不论"使用何种命令都是按照全局指定的模式查找，之后可以不使用 -s 命令，但是所有
 * 给定参数都将放到同一个列表，如果要将几个单词单独分为一部分依旧需要使用 -s 进行分隔
 *
 * 一般的参数可以有：
 *  单单词查询：
 *      query -f query_file [-t output_file] -s word1 word2 word3...
 *      查询每一个单词出现的位置，分别输出
 *  单单词非查询：
 *      query -f query_file [-t output_file] -s~ word1 word2 word3...
 *      查询每一个单词没出现的位置，分别输出
 *  多单词或查询：
 *      query -f query_file [-t output_file] -s| word1 word2 word3...
 *      查询有给定的任意一个单词出现的位置
 *  多单词与查询：
 *      query -f query_file [-t output_file] -s& word1 word2 word3...
 *      查询同时有所有给定的单词出现的位置
 *
 *  复合查询：
 *      query -f query_file [-t output_file] -s word1 word2 -s~ word3 word4 -s| word5 word6
 *      查询目标文件中 word1 word2 出现的位置，分别输出
 *      查询目标文件中 word3 word4 未出现的位置，分别输出
 *      查询目标文件中 word5 或 word6 出现的位置
 *
 *  @TODO 由于全局模式有一些技术难点，所以暂时不加入实际功能
 *  全局模式：
 *      query -f query_file [-t output_file] -S& word1 word2 -s word3 word4 -s| word5 word6 -S| word7 word8
 *      指定全局查询模式为单词与查询
 *      查询 word1 word2 同时出现的位置输出(后面接了个 -s 将后面的单词与目前的单词分隔，不参与目前的查询)
 *      查询 word3 word4 同时出现的位置输出
 *
 *      -s|被全局模式屏蔽，依旧查询 word5 word6 同时出现的位置输出
 *
 *      -S|改变全局查询模式为单词或查询
 *      查询 word7 或 word8 出现的位置
 *
 *  多文件输出查询：
 *      query -f query_file -t output_file1 -s word1 word2 -s~ word3 word4 -T output_file2 -s| word5 word6
 *      查询 word1 word2 出现的位置，分别输出至 output_file1
 *      查询 word3 word4 未出现的位置，分别输出至 output_file1
 *
 *      查询 word5 或 word6 出现的位置，输出至 output_file2 以及 标准输出流
 *
 *  多文件查询：
 *      query -f query_file1 query_file2 -s word1 word2
 *
 */
class Analyzer{
    friend class CommandLineQuery;

    Analyzer(int argc, char** argv) : _params_table(std::make_shared<std::vector<ParamsTable>>()) {
        //最少应该有5个参数，query -f query_file -s word
        if (argc >= 5 && argv && !strcmp(argv[1], "-f")) {
            std::vector<std::string> command_vector(argv + 1, argv + argc);

            //遍历， +1 是为了跳过第一个 -f
            std::vector<std::string>::iterator command_iterator = command_vector.begin()+1; // NOLINT(hicpp-use-auto,modernize-use-auto)
            //记录头部位置
            std::vector<std::string>::iterator command_begin = command_vector.begin(); // NOLINT(hicpp-use-auto,modernize-use-auto)

            // +1 是为了不丢失最后一次查询操作
            while (command_iterator != command_vector.end() + 1) {
                if (command_iterator == command_vector.end() || *command_iterator == "-f") {
                    _params_table->push_back({command_begin, command_iterator});
                    //头部位置移动到此
                    command_begin = command_iterator;
                }

                #ifdef DEBUG
                    if(command_iterator != command_vector.end()){
                        std::cout << "Analyzer command : " << *command_iterator << std::endl;
                    }
                #endif
                ++command_iterator;
            }
            _analyzer_status = true;
        } else if(argc != 1){
            if (argc < 4) {
                std::cerr << "too few command provided" << std::endl;
            } else if (argc > 0 && argv && strcmp(argv[1], "-f") != 0) {
                std::cerr << "the first command must be -f" << std::endl;
            } else {
                std::cerr << "fetal error!" << std::endl;
            }
            *this = Analyzer();
            printHelpMsg();
        }
    }

    //因为需要快速随机访问，所以改用vector
    std::vector<std::string> getQueryFile(size_t index) const {
        return std::vector<std::string>((*_params_table)[index]._query_files.begin(), (*_params_table)[index]._query_files.end());
    }

    std::map<std::string, std::map<std::set<std::string>, std::set<std::string>>> getOperatorTable(size_t index) const {
        return (*_params_table)[index]._operator_map;
    };

    size_t getParamsTableSize() const {
        return _params_table->size();
    }

public:
    static void printHelpMsg() {
        std::cout << "query [formatted operations] [formatted operations] ...\n"
                     "\t formatted operations: \n"
                     "\t\t -f query_file1,query_file2... [-t output_file1,output_file2...] -query_mode query_word1, query_word2...\n"
                     "\t\t\t -f query_file(necessary operations) which file you want to query\n"
                     "\t\t\t -t output_file(optional operations) while file you want to output, default output to std::cout\n"
                     "\t\t\t -query_mode(necessary operations) words' query mode\n"
                     "\t\t\t\t -s : query where the words appears\n"
                     "\t\t\t\t -s~ : query where the words not appears\n"
                     "\t\t\t\t -s| : query where the word1 or word2 appears\n"
                     "\t\t\t\t -s& : query where the word1 and word2 appears at the same time\n"
                     "\t\t\t=================================================\n"
                     "\t\t\t= -S(query Global_Operation)(NOT SUPPORT NOW) : =\n"
                     "\t\t\t=\t -S : change Global_Query_Mode to -s\t=\n"
                     "\t\t\t=\t -S~ : change Global_Query_Mode to -s~\t=\n"
                     "\t\t\t=\t -S| : change Global_Query_Mode to -s|\t=\n"
                     "\t\t\t=\t -S& : change Global_Query_Mode to -s&\t=\n"
                     "\t\t\t=================================================\n"
                     "\t\t\t Global_Query_Mode : \n"
                     "\t\t\t\t starting from this Global_Operation to another Global_Operation, regardless of query\n"
                     "\t\t\t\t operation between, all use the Global_Operation specified.\n"
                     "\t\t\t\t if provided -t|-T operation, it must placed before query operation, \n"
                     "\t\t\t\t otherwise you may won't get the desired result."
                     << std::endl;
    }

private:
    Analyzer() = default;
    bool _analyzer_status = false;
    std::shared_ptr<std::vector<ParamsTable>> _params_table;
};

#endif //PARAMSANALYZER_H
