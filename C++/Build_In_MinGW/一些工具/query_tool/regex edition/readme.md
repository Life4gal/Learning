### :star:一个因为闲得无聊做的单词查询工具的正则表达式版本:star:

基于一开始做的迭代器版本进行了一些改进(?)

作用：查询单词在文件中出现的位置

优点：支持查询多个文件 输出多个文件 查询多个单词 多种单词查询模式

缺点：除了优点都是缺点



一元查询操作对于每个单词单独进行查询，单独进行输出

二元查询操作不限查询单词，将所有单词组合起来进行查询



参数说明：

​	-f queryfile1 queryfile2....... 所有查询的文件，不限个数，可以加路径(只要std::iftream()能够找到就行。。。。。 :dog: :dog::dog: )

​	-t/T 

​		-t 只输出到文件

​		-T 既输出到文件又输出到 std::cout

​		缺省 只输出到 std::cout

​	-s/S

​	-s 简单单词查询

​	-s~ 单词非查询

​	-s| 单词或查询

​	-s& 单词与查询

​	-S 非全词匹配查询(即只要包含所查询单词的单词就算匹配，如 -S ite 可以匹配到 iterator)

 	-S~ 非全词匹配非查询

​	-S| 非全词匹配或查询

 	-S& 非全词匹配与查询

#### 最好不要在~(非查询)或者|(或查询)或者&(与查询)查询下使用S(非全词匹配)，不然后果自负。。。。。:see_no_evil::see_no_evil::see_no_evil:

"-f query_file1.txt -s a b -t output_file1.txt c d -s| e f g h -T output_file2.txt -S it name -f query_file2.txt query_file3.txt query_file3.txt -T output_file3.txt output_file4.txt -s| this is a joke"



#### 除了进程名之外，所有命令请写在一个引号里，因为这是正则表达式版本。。。



一个简单的例子：

 .\query_regex.exe "-f stream_iterator.h -S type name std -T -S& This is an internal header file -s| public private protect"
[char_type.] occurs 1 time(s) in file [stream_iterator.h]:
        (line 149)         *  @tparam  _CharT  The ostream char_type.
[char_type;] occurs 2 time(s) in file [stream_iterator.h]:
        (line 53)             typedef _CharT                         char_type;
        (line 160)            typedef _CharT                         char_type;
[istream_iterator(istream_type&] occurs 1 time(s) in file [stream_iterator.h]:
        (line 68)             istream_iterator(istream_type& __s)
[istream_type*] occurs 1 time(s) in file [stream_iterator.h]:
        (line 58)             istream_type*     _M_stream;
[istream_type;] occurs 1 time(s) in file [stream_iterator.h]:
        (line 55)             typedef basic_istream<_CharT, _Traits> istream_type;
[ostream_iterator(ostream_type&] occurs 2 time(s) in file [stream_iterator.h]:
        (line 171)            ostream_iterator(ostream_type& __s)
        (line 184)            ostream_iterator(ostream_type& __s, const _CharT* __c)
[ostream_type*] occurs 1 time(s) in file [stream_iterator.h]:
        (line 166)            ostream_type*     _M_stream;
[ostream_type;] occurs 1 time(s) in file [stream_iterator.h]:
        (line 162)            typedef basic_ostream<_CharT, _Traits> ostream_type;
[template<typename] occurs 3 time(s) in file [stream_iterator.h]:
        (line 47)         template<typename _Tp, typename _CharT = char,
        (line 128)        template<typename _Tp, typename _CharT, typename _Traits, typename _Dist>
        (line 152)        template<typename _Tp, typename _CharT = char,
[traits_type;] occurs 2 time(s) in file [stream_iterator.h]:
        (line 54)             typedef _Traits                        traits_type;
        (line 161)            typedef _Traits                        traits_type;
[type] occurs 3 time(s) in file [stream_iterator.h]:
        (line 144)         *  This class provides an iterator to write to an ostream.  The type Tp is
        (line 145)         *  the only type written by this iterator and there must be an
        (line 148)         *  @tparam  _Tp  The type to write to the ostream.
[typedef] occurs 7 time(s) in file [stream_iterator.h]:
        (line 53)             typedef _CharT                         char_type;
        (line 54)             typedef _Traits                        traits_type;
        (line 55)             typedef basic_istream<_CharT, _Traits> istream_type;
        (line 159)            /// Public typedef
        (line 160)            typedef _CharT                         char_type;
        (line 161)            typedef _Traits                        traits_type;
        (line 162)            typedef basic_ostream<_CharT, _Traits> ostream_type;
[typename] occurs 5 time(s) in file [stream_iterator.h]:
        (line 47)         template<typename _Tp, typename _CharT = char,
        (line 48)                  typename _Traits = char_traits<_CharT>, typename _Dist = ptrdiff_t>
        (line 128)        template<typename _Tp, typename _CharT, typename _Traits, typename _Dist>
        (line 152)        template<typename _Tp, typename _CharT = char,
        (line 153)                 typename _Traits = char_traits<_CharT> >
[@headername{iterator}] occurs 1 time(s) in file [stream_iterator.h]:
        (line 27)        *  Do not attempt to use it directly. @headername{iterator}
[namespace] occurs 2 time(s) in file [stream_iterator.h]:
        (line 37)       namespace std _GLIBCXX_VISIBILITY(default)
        (line 220)      } // namespace
[template<typename] occurs 3 time(s) in file [stream_iterator.h]:
        (line 47)         template<typename _Tp, typename _CharT = char,
        (line 128)        template<typename _Tp, typename _CharT, typename _Traits, typename _Dist>
        (line 152)        template<typename _Tp, typename _CharT = char,
[typename] occurs 5 time(s) in file [stream_iterator.h]:
        (line 47)         template<typename _Tp, typename _CharT = char,
        (line 48)                  typename _Traits = char_traits<_CharT>, typename _Dist = ptrdiff_t>
        (line 128)        template<typename _Tp, typename _CharT, typename _Traits, typename _Dist>
        (line 152)        template<typename _Tp, typename _CharT = char,
        (line 153)                 typename _Traits = char_traits<_CharT> >
[_M_stream(std::__addressof(__s))] occurs 1 time(s) in file [stream_iterator.h]:
        (line 69)             : _M_stream(std::__addressof(__s))
[_M_stream(std::__addressof(__s)),] occurs 1 time(s) in file [stream_iterator.h]:
        (line 172)            : _M_stream(std::__addressof(__s)), _M_string(0) {}
[std] occurs 1 time(s) in file [stream_iterator.h]:
        (line 37)       namespace std _GLIBCXX_VISIBILITY(default)
[std::__addressof((operator*()));] occurs 1 time(s) in file [stream_iterator.h]:
        (line 87)             operator->() const { return std::__addressof((operator*())); }