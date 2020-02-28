#ifndef MYQUERY_H
#define MYQUERY_H

#include <string>
#include <set>
#include <utility>
#include <vector>
#include <map>
#include <iterator>

#include <memory>
#include <utility>
#include <utility>
#include <algorithm>

#include <iostream>
#include <fstream>
#include <sstream>

#include "ParamsAnalyzer.h"

/**
 *  @TODO
 *  支持正则表达式
 */

//抽象基类
class QueryBase;
//实际接口
class Query;
//直接继承抽象基类，但是不提供任何接口，通过Query实际调用
class WordQuery;
class NotQuery;
//二元抽象基类，直接继承抽象基类
class BinaryQuery;
//直接继承二元抽象基类，间接继承抽象基类
class AndQuery;
class OrQuery;

/**
 * 查询结果类
 * 三参数构造：
 *  _sought : 被查找的单词
 *  _lines : 满足被查找单词要求的所在行
 *  _file : 所查找的文件
 *
 * 无参数构造：
 *  默认合成构造
 *
 * 接口：
 *  print() 打印查询结构
 *  begin(),end() 获得行号的迭代器
 *  getFile() 获得文件信息
 */
class QueryResult{
public:
    QueryResult(std::string sought,
                std::shared_ptr<std::set<std::vector<std::string>::size_type>> lines,
                std::shared_ptr<std::vector<std::string>> file,
                std::string file_name)
            : _sought(std::move(sought)), _lines(std::move(lines)), _file(std::move(file)), _file_name(std::move(file_name)) {}

    std::ostream& print(std::ostream& os = std::cout) const {
        if(_lines->empty()){
            os << "[" << _sought << "] not occurs in file [" << _file_name << "] !" << std::endl;
        }else{
            os << "[" << _sought << "] occurs " << _lines->size() << " time(s) in file [" << _file_name << "] :" << std::endl;
            for(size_t num: *_lines){
                os << "\t(line " << num << ")\t" << *(_file->begin()+num-1) << std::endl;
            }
        }

        return os;
    }
//  ofstream可以转化成ostream
//    std::ofstream& print(std::ofstream& file) const {
//        file << "[" << _sought << "] occurs " << _lines->size() << " time(s) in file [" << _file_name << "] :" << std::endl;
//        for(size_t num: *_lines){
//            file << "\t(line " << num << ")\t" << *(_file->begin()+num-1) << std::endl;
//        }
//        return file;
//    }

    std::set<std::vector<std::string>::size_type>::iterator begin() const {
        return _lines->begin();
    }

    std::set<std::vector<std::string>::size_type>::iterator end() const {
        return _lines->end();
    }

    std::shared_ptr<std::vector<std::string>> getFile() const {
        return _file;
    }

    std::string getFileName() const {
        return _file_name;
    }

private:
    std::string _sought; //查询的单词
    std::shared_ptr<std::set<std::vector<std::string>::size_type>> _lines;    //行号
    std::shared_ptr<std::vector<std::string>> _file; //查询的文件
    std::string _file_name; //查询的文件名
};

/**
 * 文本查询类
 * 单参数构造：
 *  _file : 一个管理储存查找的目标文件每一行内容的 vector 容器的指针 shared_ptr
 *  _word_map : 一个 map 容器
 *      key : 每一个单词(注意是文件中所有以 ASCII-空格 分隔的单词)
 *      value : 最内层的 vector 储存着对应行的整行内容,外层的 set 储存着每一个 key 对应的所有行，使用 shared_ptr 进行管理
 *
 * 无参数构造：
 *  默认合成构造
 *
 * 接口：
 *  query(word) 对目标文件进行 word 的查询操作，word 是一个 std::string
 */
class TextQuery{
public:
    explicit TextQuery(const std::string& file)
            : _file(std::make_shared<std::vector<std::string>>()), _file_name(file){
        std::ifstream ifs(file);
        if(ifs){
            std::string per_line;
            while(getline(ifs, per_line) /* 每次从文件中读取一行 */ ){
                //将其存入 _file 中
                _file->push_back(per_line);
                //对应行号即为 _file 的实际储量
                size_t file_curr_line_no = _file->size();
                //将一整行内容转化为 istringstream
                std::istringstream per_word(per_line);
                std::string word;
                while(per_word >> word /* 对于 istringstream 中的每一个以 ASCII-空格 分隔的单词进行判断 */ ){
                    //试图获取 _word_map 中 word 对应的值
                    std::shared_ptr<std::set<std::vector<std::string>::size_type>>& word_include_line_no = _word_map[word];
                    if(!word_include_line_no /* 如果不存在则动态分配一个储存行号的 set 容器 */ ){
                        word_include_line_no.reset(new std::set<std::vector<std::string>::size_type>);
                    }
                    //插入对应行号
                    word_include_line_no->insert(file_curr_line_no);
                }
            }
        }else{
            std::cerr << "Fetal Error! File " << _file_name << " not found or no access to open!" << std::endl;
            *this = TextQuery();
        }
    }



    QueryResult query(const std::string& sought) const{
        //一个默认的空容器，表示没有找到
        static std::shared_ptr<std::set<std::vector<std::string>::size_type>> nodata(new std::set<std::vector<std::string>::size_type>);
        std::map<std::string, std::shared_ptr<std::set<std::vector<std::string>::size_type>>>::const_iterator  /* NOLINT(hicpp-use-auto,modernize-use-auto)*/ \
        loc = _word_map.find(sought);
        if(loc != _word_map.end()){
            return QueryResult(sought, loc->second, _file, _file_name);
        }
        else{
            return QueryResult(sought, nodata, _file, _file_name);
        }
    }

private:
    TextQuery() = default;
    std::string _file_name;
    std::shared_ptr<std::vector<std::string>> _file;
    std::map<std::string, std::shared_ptr<std::set<std::vector<std::string>::size_type>>> _word_map;
};

/**
 * 查询方法的抽象基类，不提供任何接口
 * 通过友元 Query 类提供的接口实际调用功能
 *
 * 定义有两个纯虚函数：
 *  eval() 传入一个 TextQuery 以获得对应的文件并要求派生类定义实际操作，返回一个 QueryResult
 *  rep() 返回进行的查询操作说明，要求派生类根据不同的查询方法给出对应的操作说明，返回一个 std::string
 */
class QueryBase{
    friend class Query;

protected:
    virtual ~QueryBase() = default;

private:
    //返回与当前 Query 匹配的 QueryResult
    virtual QueryResult eval(const TextQuery&) const = 0;
    //表示查询的一个 string
    virtual std::string rep() const = 0;
};

/**
 * 隐藏各类查询操作之间的继承关系
 * 提供实际接口
 *
 * 公开的单参数构造：
 *  接受一个字符串(查询的单词)并默认构造一个 WordQuery 类，实际实现在 WordQuery 实现之后，使用 shared_ptr 管理指向构造的 WordQuery 的指针
 *
 * 私有单参数构造函数：
 *  接受一个管理指向 QueryBase 的指针的 shared_ptr，利用隐式转换可以实现指向实际实现操作的派生类
 *  这是提供给友元运算符的工具构造函数，用于改变查询方式
 *
 * 接口：
 *  eval() 实际调用运行时动态绑定的对象的对应查询方式
 *  rep() 实际调用运行时动态绑定的对象的对应查询说明
 */
class Query{
    //友元一些操作符使其可以访问私有构造函数
    friend Query operator~(const Query&);
    friend Query operator|(const Query&, const Query&);
    friend Query operator&(const Query&, const Query&);

public:
    //默认构建一个新的 WordQuery
    explicit Query(const std::string&);
    //接口函数
    QueryResult eval(const TextQuery& text) const {
        //根据 _query 运行时动态绑定的对象调用其对应的实现查询方法
        return _query->eval(text);
    }
    std::string rep() const {
        return _query->rep();
    }

private:
    //因为有隐式转换，所以不能声明为 explicit
    Query(std::shared_ptr<QueryBase> query) : _query(std::move(query)) {}; // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    std::shared_ptr<QueryBase> _query;
};

/**
 * 程序响应接口
 */
class CommandLineQuery{
public:
    /**
     * 获取并分析程序启动参数
     * @param argc main 函数的argc
     * @param argv main 函数的argv
     */
    CommandLineQuery(int argc, char** argv) : _analyzer(new Analyzer(argc, argv)) {
        /**
         * 对于每一个 ParamsTable 只有被查询的文件是不变的
         * 对于每一个 查询操作 都有其对应的 被查询单词 以及 输出文件
         * 而以上的这些都是可以变的，或者说一个 ParamsTable 中存有多个这样的对应关系
         *
         * 这里有两个选择：
         *  一：
         *      不保存所有查询对应关系，每次查询之后丢弃有关此次查询的记录
         *  二：
         *      使用一个 map 保留每次操作的相关记录，或者说
         *      将 ParamsTable 中的 _operator_map->second 复制下来，因为它的设计初衷就是保留这些对应关系
         *
         *  @TODO
         *  暂时使用第一张方式
         */
        //这里遍历每一个 ParamsTable，因为 ParamsTable 是按照改变查询文件分隔的
        size_t table_index = 0;
        while(table_index < _analyzer->getParamsTableSize()){
            //每一个 ParamsTable 中被查询的文件
            /**
             * @TODO
             * 为什么用
             *  std::shared_ptr<std::vector<TextQuery>> text_query = std::make_shared<std::vector<TextQuery>>(_analyzer->getQueryFile(table_index).begin(), _analyzer->getQueryFile(table_index).end());
             * 或者
             *  std::shared_ptr<std::vector<TextQuery>> text_query = std::make_shared<std::vector<TextQuery>>();
             *  std::vector<std::string>::iterator query_file_iterator = _analyzer->getQueryFile(table_index).begin(); // NOLINT(hicpp-use-auto,modernize-use-auto)
             *  while(query_file_iterator != _analyzer->getQueryFile(table_index).end()){
             *      text_query->emplace_back(*query_file_iterator++);
             *  }
             * 都会产生异常呢？
             */
            std::shared_ptr<std::vector<TextQuery>> text_query = std::make_shared<std::vector<TextQuery>>();
            std::vector<std::string> query_file = _analyzer->getQueryFile(table_index);
            for(size_t s = 0; s < _analyzer->getQueryFile(table_index).size(); ++s){
                text_query->emplace_back(query_file[s]);
            }
            /**
             * 对于 _operator_map 来说，最外围是一个 map 容器
             * 对于每一个 pair 都是一次 独立 的查询操作：
             *  first:
             *      std::string 查询的操作，标准形式应该为 -s--(单词查询) -s~-(单词非查询) -s|T(单词或查询，并且同时输出结果到 std::cout)
             *      如果命令长度只有3则表示只输出到 std::cout
             *  second:
             *      std::map:
             *          first:
             *              std::set<std::string> 存有此次查询操作所查询的单词
             *          second:
             *              std::set<std::string> 存有此次查询操作输出的文件的名字
             *
             */
            //获取到当前 ParamsTable 对应的 _operator_map
            std::map<std::string, std::map<std::set<std::string>, std::set<std::string>>>\
                    table_map = _analyzer->getOperatorTable(table_index);

            std::map<std::string, std::map<std::set<std::string>, std::set<std::string>>>::iterator/* NOLINT(hicpp-use-auto,modernize-use-auto)*/\
                table_iterator = table_map.begin();

            //遍历 _operator_map 以获取每一次查询操作
            while(table_iterator != table_map.end()){
                //从 _operator_map->first 获取到查询的操作
                std::string query_mode = table_iterator->first;
                //从 _operator_map->second 获取到查询的单词和输出文件
                std::map<std::set<std::string>, std::set<std::string>>::iterator/* NOLINT(hicpp-use-auto,modernize-use-auto)*/\
                query_detail_iterator = table_iterator->second.begin();

                //有输出文件，命令长度为4，第三位是查询模式(-, ~, |, &)，第四位是输出模式(-, T)
                //没有输出文件，命令长度为3
                if(!query_mode.empty()){
                    /**
                     * 对于不同的操作命令如何实现对应的操作又是一个难点
                     * 而且还要考虑到二元查询操作只给予一个单词的情况
                     *
                     */
                    //先获取到单词，因为不论查询操作是什么，单词是不会变的
                    //被查询的单词
                    /**
                     * @TODO
                     * 为什么这里用
                     *  std::vector<Query> query_word(query_detail_iterator->first.begin(), query_detail_iterator->first.end());
                     * 代替
                     *  std::vector<Query> query_word;
                     *  std::set<std::string>::iterator query_word_iterator = query_detail_iterator->first.begin(); // NOLINT(hicpp-use-auto,modernize-use-auto)
                     *  while(query_word_iterator != query_detail_iterator->first.end()){
                     *      query_word.emplace_back(*query_word_iterator++);
                     *  }
                     * 不会像上面那样产生异常呢？
                     */
                    std::vector<Query> query_word(query_detail_iterator->first.begin(), query_detail_iterator->first.end());
                    /**
                     * 如果没有获得到查询的单词，是跳过呢，还是报错呢？
                     * @TODO
                     * 暂时选择跳过
                     */
                    if(query_word.empty()){
                        continue;
                    }
                    /**
                     * 接下来对于每种操作进行实际查询工作
                     *
                     * @TODO
                     * 暂时使用第一张方式
                     *
                     * 不保留查询记录，查询之后输出结果，然后丢弃
                     *
                     */
                    //获取输出的文件，不论对于 "T" 还是 ”t" 来说都要输出到文件
                    std::shared_ptr<std::vector<std::ofstream>> output_file = std::make_shared<std::vector<std::ofstream>>();
                    std::set<std::string>::iterator output_file_iterator = query_detail_iterator->second.begin(); // NOLINT(hicpp-use-auto,modernize-use-auto)
                    while(output_file_iterator != query_detail_iterator->second.end()){
                        /**
                         * 输出到目标文件究竟是用覆盖还是追加呢？
                         * 暂时选择追加输出
                         *
                         * @TODO
                         * 增加选择覆盖还是追加的选项
                         *
                         */
                        output_file->emplace_back(*output_file_iterator++, std::ios_base::app);
                    }

                    /**
                     * 到这里，对于当前 ParamsTable 对应的 _operator_map 对应的一次单词查询操作
                     * 已经完成了：
                     *  获取到查询的文件
                     *  获取到被查询的单词(是否需要检查有没有单词呢？如果单词查询不输入单词会发生什么？这是个未定义的行为)
                     *  获取到输出的文件
                     *
                     *  由于需要快速随机访问，所以使用 vector 储存从 set 中获取到的文件信息并用 shared_ptr 管理
                     *
                     */

                    bool no_output_file = false;
                    //保证了如果有输出文件那么长度就为4，如果没有，那么长度就为3
                    if(output_file->empty() || query_mode.length() != 4){
                        no_output_file = true;
                    }
                    if(query_mode[2] == '-' || query_mode[2] == '~') {
                        /**
                         * 常规单词查询，对于每一个单词单独查询，分别输出
                         */
                        //对于每一个单词
                        for (size_t word_index = 0; word_index < query_word.size(); ++word_index) { // NOLINT(modernize-loop-convert)
                            //对于每一个查询文件
                            for(size_t file_index = 0; file_index < text_query->size(); ++file_index){
                                if(no_output_file){ //没有输出文件
                                    (query_mode[2] == '-' ? query_word[word_index] : (~query_word[word_index])).eval((*text_query)[file_index]).print();
                                }else if(query_mode[3] == '-'){    //有输出文件且仅输出至文件
                                    for(size_t output_index = 0; output_index < output_file->size(); ++output_index){
                                        (query_mode[2] == '-' ? query_word[word_index] : (~query_word[word_index])).eval((*text_query)[file_index]).print((*output_file)[output_index]);
                                    }
                                }else{
                                    //不然就是既输出到 std::cout 又输出到文件
                                    (query_mode[2] == '-' ? query_word[word_index] : (~query_word[word_index])).eval((*text_query)[file_index]).print();
                                    for(size_t output_index = 0; output_index < output_file->size(); ++output_index){
                                        (query_mode[2] == '-' ? query_word[word_index] : (~query_word[word_index])).eval((*text_query)[file_index]).print((*output_file)[output_index]);
                                    }
                                }
                            }
                        }
                    }else if(query_mode[2] == '|' || query_mode[2] == '&'){
                        /**
                         * 如何确保用户得的单词有两个呢？
                         *
                         * 由于接受命令是储存单词的容器是 set，所以理论上用户查询两个相同的单词或是可以的，但是现在拿到的单词可能就只有一个
                         * 所以如果拿到的单词量不够两个，就复制一个第一个单词，这样就可以不失一般性了
                         *
                         * 对于二元操作，基本思路是先获取到第一个查询的单词，然后在第一个单词的基础上不断进行二元操作
                         * 在所有单词操作完之后，就可以统一输出到输出文件中去了
                         *
                         * 对于每一个文件分别查询，分别输出
                         *
                         */
                        if(query_word.size() == 1){
                            query_word.push_back(query_word[0]);
                        }

                        //先获取第一个单词，然后组合之后的单词
                        Query word(query_word[0]);
                        if(query_mode[2] == '|'){
                            for(size_t word_index = 1; word_index < query_word.size(); ++word_index){
                                word = word|query_word[word_index];
                            }
                        }else{
                            for(size_t word_index = 1; word_index < query_word.size(); ++word_index){
                                word = word&query_word[word_index];
                            }
                        }

                        //对于每一个查询文件
                        for(size_t file_index = 0; file_index < text_query->size(); ++file_index){
                            if(no_output_file){ //没有输出文件
                                word.eval((*text_query)[file_index]).print();
                            }else if(query_mode[3] == '-'){    //有输出文件且仅输出至文件
                                for(size_t output_index = 0; output_index < output_file->size(); ++output_index){
                                    word.eval((*text_query)[file_index]).print((*output_file)[output_index]);
                                }
                            }else{
                                //不然就是既输出到 std::cout 又输出到文件
                                word.eval((*text_query)[file_index]).print();
                                for(size_t output_index = 0; output_index < output_file->size(); ++output_index){
                                    word.eval((*text_query)[file_index]).print((*output_file)[output_index]);
                                }
                            }
                        }
                    }else{
                        /**
                         * 如果命令不匹配，是跳过呢，还是报错呢？
                         * @TODO
                         * 暂时选择跳过
                         */
                        std::cerr << "unknown command" << std::endl;
                        continue;
                    }
                }else{
                    /**
                     * 如果命令太短，是跳过呢，还是报错呢？
                     * @TODO
                     * 暂时选择跳过
                     */
                    std::cerr << "command to short" << std::endl;
                    continue;
                }
                ++table_iterator;
            }
            //之前保存的查询文件清空
            text_query->clear();
            ++table_index;
        }
    }

    bool getAnalyzerStatus() const {
        return _analyzer->_analyzer_status;
    }
private:
    std::shared_ptr<Analyzer> _analyzer;
};

/**
 * 单词查询类
 * 唯一一个在给定单词上进行实际操作的类
 * 调用 TextQuery 的 query() 方法查询给定的单词
 */
class WordQuery: public QueryBase{
    friend class Query;

    explicit WordQuery(std::string word) : _word(std::move(word)) {}

    QueryResult eval(const TextQuery& text) const override {
        return text.query(_word);
    }
    std::string rep() const override {
        return _word;
    }

    std::string _word;
};

//此构造函数必须实现在 WordQuery 实现之后
inline Query::Query(const std::string& s) : _query(new WordQuery(s)) {}

/**
 * 单词非查询类
 * 查询所有不存在所查询单词的行
 */
class NotQuery : public QueryBase{
    friend Query operator~(const Query&);

    explicit NotQuery(Query query) : _query(std::move(query)) {};

    QueryResult eval(const TextQuery& text) const override {
        QueryResult result = _query.eval(text);

        std::shared_ptr<std::set<std::vector<std::string>::size_type>> ret_line = std::make_shared<std::set<std::vector<std::string>::size_type>>();

        std::set<std::vector<std::string>::size_type>::iterator first = result.begin(); // NOLINT(hicpp-use-auto,modernize-use-auto)

        for(size_t i = 1; i <= result.getFile()->size(); ++i){
            //如果行号没有出现在结果里，那就应该加入非查询结果，如果已经遍历到了结果的最后，那么之后的行号全部加入非查询结果
            if(i != *first || first == result.end()){
                ret_line->insert(i);
            }else{
                ++first;
            }
        }

        return QueryResult(rep(), ret_line, result.getFile(), result.getFileName());
    }

    std::string rep() const override {
        return "~(" + _query.rep() + ")";
    }

    Query _query;
};

class BinaryQuery : public QueryBase{
//抽象类，不提供实际操作，保存操作两个运算对象的查询所需数据
protected:
    BinaryQuery(Query lquery, Query rquery, std::string operate) : _lquery(std::move(lquery)), _rquery(std::move(rquery)), _operate(std::move(operate)) {};

    std::string rep() const override {
        return  _lquery.rep() + " "
                + _operate + " "
                + _rquery.rep() ;
    }
    //对于二元运算符，为防止多参数情况下括号错乱，加入一个包装函数
    std::string pack() const {
        return "(" + rep() + ")";
    }

    Query _lquery, _rquery;
    std::string _operate;
};

class AndQuery : public BinaryQuery{
    friend Query operator&(const Query&, const Query&);

    AndQuery(const Query& left, const Query& right) : BinaryQuery(left, right, "&") {};

    //或的实现由两个查询结果的交集取得
    QueryResult eval(const TextQuery& text) const override {
        QueryResult left = _lquery.eval(text), right = _rquery.eval(text);

        std::shared_ptr<std::set<std::vector<std::string>::size_type>> ret_line = std::make_shared<std::set<std::vector<std::string>::size_type>>();

        //调用标准库算法取交集
        std::set_intersection(left.begin(), left.end(), right.begin(), right.end(), inserter(*ret_line, ret_line->begin()));

        return QueryResult(pack(), ret_line, left.getFile(), left.getFileName());
    }
};

class OrQuery : public BinaryQuery{
    friend Query operator|(const Query&, const Query&);

    OrQuery(const Query& left, const Query& right) : BinaryQuery(left, right, "|") {};

    //或的实现由两个查询结果的并集取得
    QueryResult eval(const TextQuery& text) const override {

        QueryResult left = _lquery.eval(text), right = _rquery.eval(text);

        std::shared_ptr<std::set<std::vector<std::string>::size_type>> ret_line = std::make_shared<std::set<std::vector<std::string>::size_type>>(left.begin(), left.end());

        ret_line->insert(right.begin(), right.end());

        return QueryResult(pack(), ret_line, left.getFile(), left.getFileName());
    }
};

inline Query operator|(const Query& left, const Query& right){
    return std::shared_ptr<QueryBase>(new OrQuery(left, right));
}

inline Query operator&(const Query& left, const Query& right){
    return std::shared_ptr<QueryBase>(new AndQuery(left, right));
}

inline Query operator~(const Query& query){
    return std::shared_ptr<QueryBase>(new NotQuery(query));
}

std::ostream& operator<<(std::ostream& os, const Query& query) {
    return os << query.rep();
}

#endif //MYQUERY_H