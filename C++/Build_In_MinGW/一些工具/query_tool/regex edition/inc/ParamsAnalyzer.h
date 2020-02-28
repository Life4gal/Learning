/**
 * @file ParamsAnalyzer header file
 */

//#define DEBUG_ANALYZER

#ifndef PARAMSANALYZER_H
#define PARAMSANALYZER_H

#include <memory>
#include <regex>

#include <vector>
#include <map>

#include <iostream>


/**
 * 使用正则表达式获取参数并进行分析
 *
 * 一个完整的简单表达式应该是
 * -f query_file1 query_file2... [-t output_file1, output_file2...] -s query_word1 query_word2...
 * 其中 -f 至少且只需要指定一次
 * -t 输出目标可有可无，如果没有默认输出到 std::cout
 * -s 查询的单词，必须要有
 *
 * 在指定了查询的文件之后
 *  可以多次更换输出目标
 *  可以多次更换查询方式
 * 一个复合查询表达式可以是
 * -f query_file1 query_file2... -t output_file1 -s query_word1 query_word2 -T output_file2 -s| query_word3 query_word4 -s& query_word5 query_word6
 * 对于每一个 -s 查询命令前面都会有一个指定输出位置的命令 -t ，如果缺省则只输出到 std::cout
 *
 * -f 指定查询文件
 * -t
 *  -t 只输出到文件
 *  -T 既输出到文件有输出到又输出到 std::cout
 *  缺省 只输出到std::cout
 * -s
 *  -s 简单单词查询
 *  -s~ 单词非查询
 *  -s| 单词或查询
 *  -s& 单词与查询
 *  -S 非全词匹配查询(即只要包含所查询单词的单词就算匹配，如 -S ite 可以匹配到 iterator)
 *  -S~ 非全词匹配非查询
 *  -S| 非全词匹配或查询
 *  -S& 非全词匹配与查询
 */
namespace MyQueryTool{
    class ParamsTable;
    class Analyzer;
    class CommandLineQuery;

    class ParamsTable{
        friend class Analyzer;

        explicit ParamsTable(const std::string&);

        void process(const std::string&);

        // 所有被查询的文件
        std::vector<std::string> _query_file;
        std::multimap< // 对于同一个查询文件可以有同样的查询方式
            std::string, // 查询的方式
            std::map<
                std::vector<std::string>, // 查询的单词
                std::vector<std::string> // 输出的目标文件
                    >
                > _operator_map;
    };

    class Analyzer{
        friend class CommandLineQuery;

        explicit Analyzer(const std::string&);

        void process(std::string&);

        //因为需要快速随机访问，所以改用vector
        std::vector<std::string> getQueryFile(size_t index) const;

        std::multimap<std::string, std::map<std::vector<std::string>, std::vector<std::string>>> getOperatorTable(size_t index) const;

        size_t getParamsTableSize() const;

        std::shared_ptr<std::vector<ParamsTable>> _params_table;
    };
}











#endif //PARAMSANALYZER_H
