/**
 * @file Source of MyQuery header file
 */

/**
 * @TODO WTF 我到底在写啥？有点过分的追求所谓的引用(不拷贝)而越写越看不懂了。。。。
 */

#include "MyQuery.h"

namespace MyQueryTool{
    QueryResult::QueryResult(std::shared_ptr<std::map<std::string, std::shared_ptr<std::set<std::set<std::string>::size_type>>>> map_sought_lines,
                        std::string file_name,
                        std::shared_ptr<std::vector<std::string>> file_content)
                                    :
                                    _file_name(std::move(file_name)),
                                    _file_content(std::move(file_content)),
                                    _map_sought_lines(std::move(map_sought_lines)) {}

    std::ostream& QueryResult::printResult(std::ostream& os) const {
        for(auto& map: *_map_sought_lines){
            if(map.second->empty()){
                os << "[" << map.first << "] not occurs in file [" << _file_name << "]!" << std::endl;
            }else{
                /**
                * @note 这里使用 "\n" + std::ends 不刷新缓冲区会出现一些奇怪的问题
                */
                os << "[" << map.first << "] occurs " << map.second->size() << " time(s) in file [" << _file_name << "]:" << std::endl;
                for(auto& line: *map.second){
                    os << "\t(line " << line << ")\t" << *(_file_content->begin()+line-1) << std::endl;
                }
            }
        }
        return os;
    }

    std::set<std::set<std::string>::size_type>::iterator QueryResult::begin(const std::string& sought) const {
        return _map_sought_lines->find(sought)->second->begin();
    }

    std::set<std::set<std::string>::size_type>::iterator QueryResult::end(const std::string& sought) const {
        return _map_sought_lines->find(sought)->second->end();
    }

    QueryProcessor::QueryProcessor(const std::string& file) : _file_content(std::make_shared<std::vector<std::string>>()) {
        process(std::forward<const std::string&>(file));
    }

    size_t QueryProcessor::getFileSize() const {
        return _file_content->size();
    }

    std::string QueryProcessor::getFileName() const {
        return _file_name;
    }

    std::shared_ptr<std::vector<std::string>> QueryProcessor::getFileContent() const {
        return _file_content;
    }

    std::shared_ptr<std::map<std::string, std::shared_ptr<std::set<std::set<std::string>::size_type>>>> QueryProcessor::getLinesContent(const std::string& sought, bool needRegex = false) const {
        //一个默认的空容器，表示没有找到
        static std::shared_ptr<std::set<std::set<std::string>::size_type>> nodata = std::make_shared<std::set<std::set<std::string>::size_type>>();

        /**
         * 这里试图使用一个正则表达式来完成非全词匹配
         * 但是怎么获取到查询命令来了解到是否需要使用正则？
         * 因为本质上所有查询都是基于 WordQuery 的，也只有 WordQuery 会使用到这个函数
         * 增加一个默认为 false 的参数，允许指明是否需要正则匹配
         * 非全词匹配的另一个问题： 很多个匹配结果
         * 暂时想到的办法只有扩展参数包或者额外的容器了。。。但是查询单词和时出现的行两个东西都是可变的。。。
         * @note 为了实现正则匹配，不得已将查询的单词和出现的行数使用 map 进行储存
         * 所有情况 必须 一次性传给结果，不能分成几个结果，不然性能会大打折扣
         * @TODO 找到一个更好的办法
         */

        std::shared_ptr<std::map<std::string, std::shared_ptr<std::set<std::set<std::string>::size_type>>>> m = std::make_shared<std::map<std::string, std::shared_ptr<std::set<std::set<std::string>::size_type>>>>();

        if(needRegex){
            std::regex r("[^ ]*" + sought + "[^ ]*");
            std::smatch result;
            for(auto& map: _word_map){
                if(std::regex_match(map.first, result, r)){
                    m->emplace(map.first, map.second);
                }
            }
            if(m->empty()){
                m->emplace(sought, nodata);
            }
        }else{
            auto loc = _word_map.find(sought);

            if(loc != _word_map.end()){
                m->emplace(sought, loc->second);
            }else{
                m->emplace(sought, nodata);
            }
        }
        return std::forward<std::shared_ptr<std::map<std::string, std::shared_ptr<std::set<std::set<std::string>::size_type>>>>>(m);
    }

    void QueryProcessor::process(const std::string& file){
        _file_name = file;
        std::ifstream ifs(file);
        if(ifs){
            std::string per_line;
            while(getline(ifs, per_line)){
                // 每次读取一行并且存入文件内容中
                _file_content->push_back(per_line);
                // 文件储量即为对应行号
                size_t file_curr_line_no = _file_content->size();
                // 操作一行中的内容
                std::istringstream per_word(per_line);
                std::string word;
                // 对于一行中的每一个单词(以 white-space 分隔的)
                while(per_word >> word){
                    // 试图获取 _word_map 中 word 对应的行号数据
                    std::shared_ptr<std::set<std::set<std::string>::size_type>>& word_include_line_no = _word_map[word];
                    if(!word_include_line_no){
                        // 如果没获取到就动态分配一个
                        word_include_line_no.reset(new std::set<std::set<std::string>::size_type>);
                    }
                    // 插入行号，如果已经存在则什么都不会做，所以不需要检验
                    word_include_line_no->insert(file_curr_line_no);
                }
            }
        }else{
            std::cerr << "Fetal Error! File " << _file_name << " Not Exist or No Access to Open it!" << std::endl;
        }
    }

    WordQuery::WordQuery(std::string word) : _word(std::move(word)) {}

    QueryResult WordQuery::queryEvaluation(const QueryProcessor& queryProcessor, bool needRegex) const {
        return QueryResult(queryProcessor.getLinesContent(_word, needRegex),
                queryProcessor.getFileName(),
                std::forward<std::shared_ptr<std::vector<std::string>>>(queryProcessor.getFileContent()));
    }

    std::string WordQuery::queryRepresent() const {
        return _word;
    }

    NotQuery::NotQuery(QueryInterface queryInterface) : _queryInterface(std::move(queryInterface)) {}

    QueryResult NotQuery::queryEvaluation(const QueryProcessor& queryProcessor, bool needRegex) const {
        std::shared_ptr<std::map<std::string, std::shared_ptr<std::set<std::set<std::string>::size_type>>>> m = std::make_shared<std::map<std::string, std::shared_ptr<std::set<std::set<std::string>::size_type>>>>();

        std::shared_ptr<std::set<std::set<std::string>::size_type>> ret_line = std::make_shared<std::set<std::set<std::string>::size_type>>();

        if(!needRegex){
            std::string query_word = _queryInterface.queryRepresent();
            QueryResult result = _queryInterface.queryEvaluation(std::forward<const QueryProcessor&>(queryProcessor), false);

            auto first = result.begin(query_word);

            for(auto i = 1; i <= queryProcessor.getFileSize(); ++i){
                //如果行号没有出现在结果里，那就应该加入非查询结果，如果已经遍历到了结果的最后，那么之后的行号全部加入非查询结果
                if(i != *first || first == result.end(query_word)){
                    ret_line->insert(i);
                }else{
                    ++first;
                }
            }

            m->emplace(queryRepresent(), ret_line);
        }else{
            auto result = queryProcessor.getLinesContent(_queryInterface.queryRepresent(), needRegex);

            for(auto& map: *result){

                auto first = map.second->begin();

                for(auto i = 1; i <= queryProcessor.getFileSize(); ++i){
                    if(i != *first || first == map.second->end()){
                        ret_line->insert(i);
                    }else{
                        ++first;
                    }
                }
                std::shared_ptr<std::set<std::set<std::string>::size_type>> empty = std::make_shared<std::set<std::set<std::string>::size_type>>();

                std::swap(ret_line, empty);

                m->emplace(queryRepresent(map.first), empty);
            }
        }
        return QueryResult(std::forward<std::shared_ptr<std::map<std::string, std::shared_ptr<std::set<std::set<std::string>::size_type>>>>>(m),
                           queryProcessor.getFileName(),
                           std::forward<std::shared_ptr<std::vector<std::string>>>(queryProcessor.getFileContent()));
    }

    std::string NotQuery::queryRepresent() const {
        return "~(" + _queryInterface.queryRepresent() + ")";
    }

    std::string NotQuery::queryRepresent(const std::string& s) const {
        return "~(" + s + ")";
    }

    BinaryQuery::BinaryQuery(QueryInterface left, QueryInterface right, std::string operate)
    : _left(std::move(left)), _right(std::move(right)), _operate(std::move(operate)) {}

    std::string BinaryQuery::queryRepresent() const {
        return _left.queryRepresent() + " "
                + _operate + " "
                + _right.queryRepresent();
    }

    std::string BinaryQuery::queryRepresent(const std::string& left, const std::string& right) const {
        return left + " "
            + _operate + " "
            + right;
    }

    std::string BinaryQuery::pack(const std::string& s) const {
        return "(" + s + ")";
    }

    AndQuery::AndQuery(const QueryInterface& left, const QueryInterface& right)
    : BinaryQuery(left, right, "&") {}

    QueryResult AndQuery::queryEvaluation(const QueryProcessor& queryProcessor, bool needRegex) const {
        std::shared_ptr<std::map<std::string, std::shared_ptr<std::set<std::set<std::string>::size_type>>>> m = std::make_shared<std::map<std::string, std::shared_ptr<std::set<std::set<std::string>::size_type>>>>();

        std::shared_ptr<std::set<std::set<std::string>::size_type>> ret_line = std::make_shared<std::set<std::set<std::string>::size_type>>();

        if(!needRegex){
            std::string left_word = _left.queryRepresent();
            std::string right_word = _right.queryRepresent();
            QueryResult left = _left.queryEvaluation(std::forward<const QueryProcessor&>(queryProcessor), false);
            QueryResult right = _right.queryEvaluation(std::forward<const QueryProcessor&>(queryProcessor), false);


            // 调用标准库算法取交集
            std::set_intersection(left.begin(left_word), left.end(left_word),
                    right.begin(right_word), right.end(right_word),
                    std::inserter(*ret_line, ret_line->begin()));

            m->emplace(pack(queryRepresent()), ret_line);
        }else{
            /**
            * 非全词匹配对于对单词组合来说可不是个简单的问题
            */
            auto left = queryProcessor.getLinesContent(_left.queryRepresent(), needRegex);
            auto right = queryProcessor.getLinesContent(_right.queryRepresent(), needRegex);

            /**
             * @TODO WTF 这个效率要有多低啊。。。
             */
            for(auto& l_map: *left){
                for(auto& r_map: *right){

                    std::set_intersection(l_map.second->begin(), l_map.second->end(),
                            r_map.second->begin(), r_map.second->end(),
                            std::inserter(*ret_line, ret_line->begin()));

                    std::shared_ptr<std::set<std::set<std::string>::size_type>> empty = std::make_shared<std::set<std::set<std::string>::size_type>>();

                    std::swap(ret_line, empty);

                    m->emplace(pack(queryRepresent(l_map.first, r_map.first)), empty);
                }
            }
        }
        return QueryResult(std::forward<std::shared_ptr<std::map<std::string, std::shared_ptr<std::set<std::set<std::string>::size_type>>>>>(m),
                           queryProcessor.getFileName(),
                           std::forward<std::shared_ptr<std::vector<std::string>>>(queryProcessor.getFileContent()));
    }

    OrQuery::OrQuery(const QueryInterface& left, const QueryInterface& right)
            : BinaryQuery(left, right, "|") {}

    QueryResult OrQuery::queryEvaluation(const MyQueryTool::QueryProcessor& queryProcessor, bool needRegex) const {
        std::shared_ptr<std::map<std::string, std::shared_ptr<std::set<std::set<std::string>::size_type>>>> m = std::make_shared<std::map<std::string, std::shared_ptr<std::set<std::set<std::string>::size_type>>>>();

        if(!needRegex){
            std::string left_word = _left.queryRepresent();
            std::string right_word = _right.queryRepresent();
            QueryResult left = _left.queryEvaluation(std::forward<const QueryProcessor&>(queryProcessor), false);
            QueryResult right = _right.queryEvaluation(std::forward<const QueryProcessor&>(queryProcessor), false);

            std::shared_ptr<std::set<std::set<std::string>::size_type>> ret_line = std::make_shared<std::set<std::set<std::string>::size_type>>(left.begin(left_word), left.end(left_word));

            // 插入一个集合自动去重
            ret_line->insert(right.begin(right_word), right.end(right_word));

            m->emplace(pack(queryRepresent()), ret_line);
        }else{
            /**
            * 非全词匹配对于对单词组合来说可不是个简单的问题
            */
            auto left = queryProcessor.getLinesContent(_left.queryRepresent(), needRegex);
            auto right = queryProcessor.getLinesContent(_right.queryRepresent(), needRegex);

            /**
             * @TODO WTF 这个效率要有多低啊。。。
             */
            for(auto& l_map: *left){
                for(auto& r_map: *right){
                    std::shared_ptr<std::set<std::set<std::string>::size_type>> ret_line = std::make_shared<std::set<std::set<std::string>::size_type>>(l_map.second->begin(), l_map.second->end());

                    ret_line->insert(l_map.second->begin(), l_map.second->end());

                    std::shared_ptr<std::set<std::set<std::string>::size_type>> empty = std::make_shared<std::set<std::set<std::string>::size_type>>();

                    std::swap(ret_line, empty);

                    m->emplace(pack(queryRepresent(l_map.first, r_map.first)), empty);
                }
            }
        }
        return QueryResult(std::forward<std::shared_ptr<std::map<std::string, std::shared_ptr<std::set<std::set<std::string>::size_type>>>>>(m),
                           queryProcessor.getFileName(),
                           std::forward<std::shared_ptr<std::vector<std::string>>>(queryProcessor.getFileContent()));
    }

    QueryInterface::QueryInterface(const std::string& word) : _query(new WordQuery(word)) {}

    QueryResult QueryInterface::queryEvaluation(const QueryProcessor& queryProcessor, bool needRegex = false) const {
        return _query->queryEvaluation(std::forward<const QueryProcessor&>(queryProcessor), needRegex);
    }

    std::string QueryInterface::queryRepresent() const {
        return _query->queryRepresent();
    }

    QueryInterface::QueryInterface(std::shared_ptr<QueryBase> query) : _query(std::move(query)) {}

    QueryInterface operator~(const QueryInterface& queryInterface) {
        return std::shared_ptr<QueryBase>(new NotQuery(std::forward<const QueryInterface&>(queryInterface)));
    }

    QueryInterface operator|(const QueryInterface& left, const QueryInterface& right) {
        return std::shared_ptr<QueryBase>(new OrQuery(std::forward<const QueryInterface&>(left), std::forward<const QueryInterface&>(right)));
    }

    QueryInterface operator&(const QueryInterface& left, const QueryInterface& right) {
        return std::shared_ptr<QueryBase>(new AndQuery(std::forward<const QueryInterface&>(left), std::forward<const QueryInterface&>(right)));
    }
}
