/**
 * @file MyQuery header file
 */

#ifndef MYQUERY_H
#define MYQUERY_H

#include <string>
#include <set>
#include <utility>
#include <vector>
#include <map>
#include <iterator>

#include <memory>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

namespace MyQueryTool{

    class QueryResult;
    class QueryProcessor;
    class QueryBase;
    class QueryInterface;
    class WordQuery;
    class NotQuery;
    class BinaryQuery;
    class AndQuery;
    class OrQuery;

    class QueryResult{
    public:
        QueryResult(std::shared_ptr<std::map<std::string, std::shared_ptr<std::set<std::set<std::string>::size_type>>>>, std::string, std::shared_ptr<std::vector<std::string>>);
        /**
         * 输出查询结果，默认输出到 std::cout输出查询结果
         * @param os 输出流
         * @return
         */
        std::ostream& printResult(std::ostream& = std::cout) const;
        /**
         * 获取所给单词所出现行的 set 容器的迭代器首
         * @param sought 所查单词
         * @return
         */
        std::set<std::set<std::string>::size_type>::iterator begin(const std::string&) const;
        /**
         * 获取所给单词所出现行的 set 容器的迭代器尾
         * @param sought 所查单词
         * @return
         */
        std::set<std::set<std::string>::size_type>::iterator end(const std::string&) const;

    private:
        // 文件名字
        const std::string _file_name;
        // 文件内容行数
        std::shared_ptr<std::vector<std::string>> _file_content;
        std::shared_ptr<std::map<std::string, // 所查询单词
                std::shared_ptr<std::set<std::set<std::string>::size_type>>>> // 储存所查单词的行数的容器的指针
                _map_sought_lines;
    };

    // 查询处理类
    class QueryProcessor{
    public:
        /**
         * 接受一个文件名，具体处理交给一个私有处理函数处理
         * 构造函数只是初始分配一个储存文件内容的动态大小容器
         * @param file 处理的文本
         */
        explicit QueryProcessor(const std::string&);
        /**
         * 获取整个文件的大小(行数)
         * @return
         */
        size_t getFileSize() const;
        std::string getFileName() const;
        /**
         * 获取整个文件的内容
         * @return
         */
        std::shared_ptr<std::vector<std::string>> getFileContent() const;
        std::shared_ptr<std::map<std::string, std::shared_ptr<std::set<std::set<std::string>::size_type>>>> getLinesContent(const std::string&, bool) const;

    private:
        /**
         * 实际处理函数
         * @param file 实际进行处理文本的工作
         */
        void process(const std::string&);

        // "每一个"单词 -> 所有出现行号，行号内容用 shared_ptr 管理，为节省开销 QueryResult 将直接使用行号数据，所以应该要保证 QueryProcessor 的销毁不会导致 QueryResult 产生错误
        std::map<std::string, std::shared_ptr<std::set<std::set<std::string>::size_type>>> _word_map;
        // 查询的文件的内容，同理使用 shared_ptr 管理
        std::shared_ptr<std::vector<std::string>> _file_content;
        // 文件名
        std::string _file_name;
    };

    // 实际查询操作的抽象基类
    class QueryBase{
        friend class QueryInterface;

    protected:
        virtual ~QueryBase() = default;

    private:
        // 返回与当前 QueryInterface 匹配的 QueryResult
        virtual QueryResult queryEvaluation(const QueryProcessor&, bool) const = 0;
        // 返回一个能代表当前查询方式的一个字符串
        virtual std::string queryRepresent() const = 0;
    };

    // 实际对外接口
    class QueryInterface{
        // 友元操作符，访问私有构造函数
        // 非查询
        friend QueryInterface operator~(const QueryInterface&);
        // 或查询
        friend QueryInterface operator|(const QueryInterface&, const QueryInterface&);
        // 与查询
        friend QueryInterface operator&(const QueryInterface&, const QueryInterface&);

    public:
        /**
         * 默认构造一个 WordQuery
         * @param word 查询的单词
         */
        explicit QueryInterface(const std::string&);
        /**
         * 接受 QueryProcessor 处理之后的内容
         * 根据运行时构造的查询类调用其对应的查询方法
         * @param queryProcessor 查询的文本处理器
         * @return
         */
        QueryResult queryEvaluation(const QueryProcessor&, bool) const;
        /**
         * 根据运行时构造的查询类调用其对应的输出方法
         * @return
         */
        std::string queryRepresent() const;

    private:
        /**
         * 因为有隐式转换，所以不能声明为 explicit
         * @param query 运行时给定的查询类
         */
        QueryInterface(std::shared_ptr<QueryBase>);
        std::shared_ptr<QueryBase> _query;
    };

    // 单词查询实现类
    class WordQuery : public QueryBase {
        friend class QueryInterface;
        /**
         * 唯一一个实际在给定单词上进行实际操作的类
         * @param word 查询的单词
         */
        explicit WordQuery(std::string);
        /**
         * 获取单词查询结果
         * @param queryProcessor 查询的文本处理器
         * @return
         */
        QueryResult queryEvaluation(const QueryProcessor&, bool) const override;
        std::string queryRepresent() const override;

        std::string _word;
    };

    // 单词非查询实现类
    // 查询不存在目标单词的行
    class NotQuery : public QueryBase{
        friend QueryInterface operator~(const QueryInterface&);

        explicit NotQuery(QueryInterface);

        QueryResult queryEvaluation(const QueryProcessor&, bool) const override;
        std::string queryRepresent() const override;
        /**
         * 在正则表达式情况下根据实际返回一个能代表当前查询方式的一个字符串
         * @return
         */
        std::string queryRepresent(const std::string&) const;

        QueryInterface _queryInterface;
    };

    // 二元单词查询抽象基类
    class BinaryQuery : public QueryBase{
    protected:
        BinaryQuery(QueryInterface, QueryInterface, std::string);
        /**
         * 默认的返回一个能代表当前查询方式的一个字符串
         * @return
         */
        std::string queryRepresent() const override;
        /**
         * 在正则表达式情况下根据实际返回一个能代表当前查询方式的一个字符串
         * @return
         */
        std::string queryRepresent(const std::string&, const std::string&) const;
        /**
         * 将表达式包装一下，因为会有多元的查询单词
         * @param s 被包装的一个能代表当前查询方式的一个字符串
         * @return
         */
        std::string pack(const std::string&) const;

        QueryInterface _left, _right;
        std::string _operate;
    };

    // 单词与查询
    class AndQuery : public BinaryQuery{
        friend QueryInterface operator&(const QueryInterface&, const QueryInterface&);

        AndQuery(const QueryInterface&, const QueryInterface&);

        QueryResult queryEvaluation(const QueryProcessor&, bool) const override;
    };

    // 单词或匹配
    class OrQuery : public BinaryQuery{
        friend QueryInterface operator|(const QueryInterface&, const QueryInterface&);

        OrQuery(const QueryInterface&, const QueryInterface&);

        QueryResult queryEvaluation(const QueryProcessor&, bool) const override;
    };
}

#endif //MYQUERY_H
