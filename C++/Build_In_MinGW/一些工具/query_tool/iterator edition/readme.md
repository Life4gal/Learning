### :star:一个因为闲得无聊做的单词查询工具的迭代器版本:star:

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

"-f query_file1.txt -s a b -t output_file1.txt c d -s| e f g h -T output_file2.txt -S it name -f query_file2.txt query_file3.txt query_file3.txt -T output_file3.txt output_file4.txt -s| this is a joke"



#### 除了进程名之外，每个命令都要单独写在一个引号里，因为这是迭代器版本。。。



一个简单的例子：

.\query_iterator.exe "-f" "stream_iterator.h" "-s" "namespace" "is" "-s|" "This" "is" "a" "public" "-T" "output_file1.txt" "-s&" "is" "an" "by"                                                                                                                                     [(an & by & is)] occurs 1 time(s) in file [stream_iterator.h] :
        (line 26)        *  This is an internal header file, included by other library headers.
[is] occurs 7 time(s) in file [stream_iterator.h] :
        (line 5)        // This file is part of the GNU ISO C++ Library.  This library is free
        (line 11)       // This library is distributed in the hope that it will be useful,
        (line 26)        *  This is an internal header file, included by other library headers.
        (line 144)         *  This class provides an iterator to write to an ostream.  The type Tp is
        (line 177)             *  The delimiter string @a c is written to the stream after every Tp
        (line 178)             *  written to the stream.  The delimiter is not copied, and thus must
        (line 179)             *  not be destroyed while this iterator is in use.
[namespace] occurs 2 time(s) in file [stream_iterator.h] :
        (line 37)       namespace std _GLIBCXX_VISIBILITY(default)
        (line 220)      } // namespace
[(This | a | is | public)] occurs 11 time(s) in file [stream_iterator.h] :
        (line 5)        // This file is part of the GNU ISO C++ Library.  This library is free
        (line 11)       // This library is distributed in the hope that it will be useful,
        (line 20)       // You should have received a copy of the GNU General Public License and
        (line 21)       // a copy of the GCC Runtime Library Exception along with this program;
        (line 26)        *  This is an internal header file, included by other library headers.
        (line 50)           : public iterator<input_iterator_tag, _Tp, _Dist, const _Tp*, const _Tp&>
        (line 144)         *  This class provides an iterator to write to an ostream.  The type Tp is
        (line 155)          : public iterator<output_iterator_tag, void, void, void, void>
        (line 177)             *  The delimiter string @a c is written to the stream after every Tp
        (line 178)             *  written to the stream.  The delimiter is not copied, and thus must
        (line 179)             *  not be destroyed while this iterator is in use.
complete