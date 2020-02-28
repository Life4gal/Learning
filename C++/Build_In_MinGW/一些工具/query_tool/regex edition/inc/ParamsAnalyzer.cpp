/**
 * @file Source of ParamsAnalyzer header file
 */

#include "ParamsAnalyzer.h"

namespace MyQueryTool{
    ParamsTable::ParamsTable(const std::string& params){
        process(std::forward<const std::string&>(params));
    }

    void ParamsTable::process(const std::string& params){
        // -f ifs1 ifs2... ([-t ofs1 ofs2...] -s qw1 qw2...)+
        /**
         * @TODO 暂时没想到什么好的解决[命名(查询文件命名，输出文件命名，查询单词命名)]冲突的办法
         * 现在用的是通过 -[tTsS][ ~|&] 进行分隔
         */
        std::string if_reg = "-f (.*?)(?=-[tTsS][ ~|&])";
        std::smatch if_result;

        if(std::regex_search(params, if_result, std::regex(if_reg))){
            // 不需要那个 -f 字符串
            std::istringstream if_iss(if_result.str(1));
            std::string ifs;
            while(if_iss >> ifs){
                _query_file.push_back(ifs);
            }

            // ([-t ofs1 ofs2...] -s qw1 qw2...)+
            // 在后面填充一段 -t-s ，不然拿不到最后一段参数
            std::string op_string = params.substr(if_result.str().length()) + "-t -s ";
            /**
             * @TODO 暂时没想到什么好的解决[命名(查询文件命名，输出文件命名，查询单词命名)]冲突的办法
             * 现在用的是通过 -[tTsS][ ~|&] 进行分隔
             */
            std::string op_reg = "(-[tT] .*?)?(?=-[sS][ ~|&])(-[sS].*?)(?=-[tTsS][ ~|&])";
            std::smatch op_result;
            std::regex op_r(op_reg);

            for(std::sregex_iterator it(op_string.begin(), op_string.end(), op_r), end; it != end; ++it){
                std::vector<std::string> query_word;
                std::vector<std::string> output_file;
                std::string operation;

                #ifdef DEBUG_ANALYZER
                    std::cout << "-t[" << it->str(1) << "] -s[" << it->str(2) << "]" << std::endl;
                #endif
                // it->str(1) --> -t if->str(2) --> -s
                std::istringstream of_iss(it->str(1));
                std::istringstream qw_iss(it->str(2));
                // 先处理 -s 命令
                if(qw_iss >> operation){
                    if(operation == "-s" || operation == "-S"){
                        // 不失一般性将命令长度填充至3
                        operation.append("-");
                    }
                    std::string qw;
                    while(qw_iss >> qw){
                        query_word.emplace_back(qw);
                    }
                }else{
                    /**
                     * @TODO 谁知道会不会有人明明要查询单词却没有给单词呢？
                     * 也许改一下正则表达式就可以让他无法匹配没给单词的命令
                     * 或者说，在这里直接丢出一个异常
                     */
                    std::cerr << "Can not find any word in param " << it->str() << std::endl;
                }
                // 有了 operation 就可以根据 tT 更改 operation 了
                std::string oc;
                if(of_iss >> oc){
                    // 如果有输出命令的话
                    // 将 tT 填充到 operation 后面，这样有没有输出可以通过命令长度(3或4)知晓
                    // 丢掉 "-" 部分
                    operation.append(&oc[1]);
                    std::string of;
                    while(of_iss >> of){
                        output_file.emplace_back(of);
                    }
                }

                /**
                 * @TODO 为什么如此构造会报错
                 */
                //_operator_map.emplace(operation, std::map<std::set<std::string>, std::set<std::string>>(query_word, output_file));
                std::map<std::vector<std::string>, std::vector<std::string>> m;
                m.emplace(query_word, output_file);
                _operator_map.emplace(operation, m);

//                // 清空容器
//                query_word.clear();
//                output_file.clear();
//                operation.clear();
            }
        }else{
            std::cerr << "Can not find any file in param " << params << std::endl;
        }
    }

    Analyzer::Analyzer(const std::string& p) : _params_table(std::make_shared<std::vector<ParamsTable>>()) {
        std::shared_ptr<std::string> params = std::make_shared<std::string>(p);

        process(*params);
    }

    void Analyzer::process(std::string& params) {
        // -f xxxxx -f xxxxx.....
        /**
         * @TODO 暂时没想到什么好的解决[命名(查询文件命名，输出文件命名，查询单词命名)]冲突的办法
         * 现在用的是通过 -f[:space:] 进行分隔
         */
        std::string pt_reg = "-f (.*?)(?=-f )";
        // 填充字符串，不然拿不到最后一段参数
        const std::string& pt_string = params.append(" -f ");
        std::regex pt_r(pt_reg);
        for(std::sregex_iterator it(pt_string.begin(), pt_string.end(), pt_r), end; it != end; ++it){
            #ifdef DEBUG_ANALYZER
                std::cout << "-f command found: [" << it->str() << "]" <<  std::endl;
            #endif
            /**
             * @TODO 为什么 emplace_back(it->str()) 报错不能访问到构造函数呢？要改为 public 才能访问到
             */
            //_params_table->emplace_back(it->str());
            _params_table->push_back(ParamsTable(it->str()));
        }
    }

    std::vector<std::string> Analyzer::getQueryFile(size_t index) const {
        return (*_params_table)[index]._query_file;
    }

    std::multimap<std::string, std::map<std::vector<std::string>, std::vector<std::string>>> Analyzer::getOperatorTable(size_t index) const {
        return (*_params_table)[index]._operator_map;
    }

    size_t Analyzer::getParamsTableSize() const {
        return _params_table->size();
    }
}




