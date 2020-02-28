/**
 * @file Source of CommandLineQuery header file
 */

#include "CommandLineQuery.h"

namespace MyQueryTool{
    CommandLineQuery::CommandLineQuery(int argc, char** argv) {
        // 第 1 个是进程名(下标0)
        if(argc > 2){
            showHelpMsg();
        }else{
            *this = CommandLineQuery(argv[1]);
        }
    }

    CommandLineQuery::CommandLineQuery(const std::string& s) : _analyzer(new Analyzer(s)) {}

    void CommandLineQuery::getQueryResult() const {
        // 这里遍历每一个 ParamsTable，因为 ParamsTable 是按照改变查询文件分隔的
        size_t table_index = 0;
        while(table_index < _analyzer->getParamsTableSize()){
            std::shared_ptr<std::vector<QueryProcessor>> query_processor = std::make_shared<std::vector<QueryProcessor>>();

            std::vector<std::string> query_file = _analyzer->getQueryFile(table_index);
            // 获取每次查询操作查询的文件并交给处理器
            for(auto& file: query_file){
                #ifdef DEBUG_COMMANDLINE
                    std::cout << "query_file : " << file << std::endl;
                #endif
                query_processor->emplace_back(file);
            }

            /**
             * 对于 _operator_map 来说，最外围是一个 map 容器
             * 对于每一个 pair 都是一次 独立 的查询操作：
             *  first:
             *      std::string 查询的操作，标准形式应该为 -s-t(单词查询) -s~t(单词非查询) -s|T(单词或查询，并且同时输出结果到 std::cout)
             *      如果命令长度只有3则表示只输出到 std::cout
             *  second:
             *      std::map:
             *          first:
             *              std::vector<std::string> 存有此次查询操作所查询的单词
             *          second:
             *              std::vector<std::string> 存有此次查询操作输出的文件的名字
             *
             */
            // 获取到当前 ParamsTable 对应的 _operator_map
            auto table_map = _analyzer->getOperatorTable(table_index);

            auto table_iterator = table_map.begin();
            // 遍历 _operator_map 以获取每一次查询操作
            while(table_iterator != table_map.end()){
                // 从 _operator_map->first 获取到查询的操作
                std::string query_mode = table_iterator->first;
                #ifdef DEBUG_COMMANDLINE
                    std::cout << "query_mode : " << query_mode << std::endl;
                #endif
                // 从 _operator_map->second 获取到查询的单词和输出文件
                auto query_detail_iterator = table_iterator->second.begin();

                //有输出文件，命令长度为4，第二位是是否全词匹配(s, S)，第三位是查询模式(-, ~, |, &)，第四位是输出模式(-, T)
                //没有输出文件，命令长度为3
                if(query_mode.length() == 3 || query_mode.length() == 4){
                    /**
                     * 对于不同的操作命令如何实现对应的操作又是一个难点
                     * 而且还要考虑到二元查询操作只给予一个单词的情况
                     *
                     */
                    // 先获取到单词，因为不论查询操作是什么，单词是不会变的
                    // 被查询的单词
                    std::vector<QueryInterface> query_word(query_detail_iterator->first.begin(), query_detail_iterator->first.end());
                    #ifdef DEBUG_COMMANDLINE
                    for(auto& word: query_word){
                        std::cout << "query_word : " << word.queryRepresent() << std::endl;
                    }
                    #endif
                    // 获取输出的文件，不论对于 "T" 还是 ”t" 来说都要输出到文件
                    std::shared_ptr<std::vector<std::ofstream>> output_file = std::make_shared<std::vector<std::ofstream>>();
                    std::vector<std::string> o_file = query_detail_iterator->second;
                    for(auto& file: o_file){
                        #ifdef DEBUG_COMMANDLINE
                            std::cout << "output_file : " << file << std::endl;
                        #endif
                        /**
                         * 输出到目标文件究竟是用覆盖还是追加呢？
                         * 暂时选择追加输出
                         *
                         * @TODO 增加选择覆盖还是追加的选项
                         *
                         */
                        output_file->emplace_back(file, std::ios_base::app);
                    }
                    /**
                     * 到这里，对于当前 ParamsTable 对应的 _operator_map 对应的一次单词查询操作
                     * 已经完成了：
                     *  获取到查询的文件
                     *  获取到被查询的单词(是否需要检查有没有单词呢？如果单词查询不输入单词会发生什么？这是个未定义的行为)
                     *  获取到输出的文件
                     *
                     */

                    // 保证了如果有输出文件那么长度就为4，如果没有，那么长度就为3
                    bool no_output_file = output_file->empty() || query_mode.length() != 4;

                    if(query_mode[2] == '-' || query_mode[2] == '~'){
                        /**
                         * 一元查询对于每一个查询的单词单独输出
                         */
                         // 对于每一个查询文件
                         for(auto& processor: *query_processor){
                             // 对于每一个单词
                             for(auto& word: query_word){
                                 // 没有输出文件或者有输出文件但是同时出输出到 std::cout
                                 if(no_output_file || query_mode[3] == 'T'){
                                     (query_mode[2] == '-' ? word : ~word).queryEvaluation(processor, query_mode[1] == 'S').printResult();
                                 }
                                 // 只输出到文件或者且输出到文件
                                 if(query_mode[3] == 't' || query_mode[3] == 'T'){
                                     for(auto& file: *output_file){
                                         (query_mode[2] == '-' ? word : ~word).queryEvaluation(processor, query_mode[1] == 'S').printResult(file);
                                     }
                                 }
                             }
                         }
                    }else if(query_mode[2] == '|' || query_mode[2] == '&'){
                        /**
                         * 如何确保用户得的单词有两个呢？
                         *
                         * 对于二元操作，基本思路是先获取到第一个查询的单词，然后在第一个单词的基础上不断进行二元操作
                         * 在所有单词操作完之后，就可以统一输出到输出文件中去了
                         *
                         * 对于每一个文件分别查询，分别输出
                         *
                         */

//                        // 管他有几个词，只要不是没有就行，大不了不组合就是了
//                        if(query_word.size() == 1){
//                            query_word.push_back(query_word.front());
//                        }

                        // 先获取第一个单词，然后组合之后的单词
                        QueryInterface word = query_word.front();

                        for(auto i = 1; i < query_word.size(); ++i){
                            word = query_mode[2] == '|' ? word|query_word[i] : word&query_word[i];
                        }

                        //对于每一个查询文件
                        for(auto& processor: *query_processor){
                            // 没有输出文件或者有输出文件但是同时出输出到 std::cout
                            if(no_output_file || query_mode[3] == 'T'){
                                word.queryEvaluation(processor, query_mode[1] == 'S').printResult();
                            }
                            // 只输出到文件或者且输出到文件
                            if(query_mode[3] == 't' || query_mode[3] == 'T'){
                                for(auto& file: *output_file){
                                    word.queryEvaluation(processor, query_mode[1] == 'S').printResult(file);
                                }
                            }
                        }
                    }else{
                        std::cerr << "Unknown command: " << query_mode << std::endl;
                    }
                }else{
                    std::cerr << "Invalid query mode: [" << query_mode << "]" << std::endl;
                }
                ++table_iterator;
            }
            ++table_index;
        }
    }
}
