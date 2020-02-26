#include <iostream>
#include <numeric>
#include <algorithm>
#include <vector>
#include <list>
#include <string>
#include <set>

int main() {
    static const int container_size = 9;

    static const int int_array[container_size] = {1, 3, 6, 10, 15, 21, 28, 36, 36};
    //以下两个容器不能申明为 const ,否则下面的获取地址迭代器没有可重载赋值
    static std::vector<int> int_vector(int_array, int_array + container_size);
    static std::list<int> int_list(int_array, int_array + container_size);

    {
        std::cout << "=======================" << std::endl;

        //容器全体元素累计运算,第三个参数为运算初值
        //默认累加
        int accumulate_plus = std::accumulate(int_array, int_array + container_size, 0);
        //改成累乘
        int accumulate_multi = std::accumulate(int_vector.begin(), int_vector.end(), 1,
                                               std::multiplies<int>()); // NOLINT(modernize-use-transparent-functors)

        std::cout << "accumulate: \n"
                  << "plus -> " << accumulate_plus << "\n"
                  << "multi -> " << accumulate_multi << std::endl;

    }
    {
        std::cout << "=======================" << std::endl;

        //容器相邻元素逐个运算,第三个参数指定为另一个容器的接收起始位置(注意容量)
        //默认减去前面一个元素(第一个元素不变)
        std::vector<int> adjacent_minus(container_size);
        std::adjacent_difference(int_list.begin(), int_list.end(), adjacent_minus.begin());
        //改成乘上前面一个元素(第一个元素不变)
        std::vector<int> adjacent_multi(container_size);
        std::adjacent_difference(int_array, int_array + 8, adjacent_multi.begin(),
                                 std::multiplies<int>()); // NOLINT(modernize-use-transparent-functors)

        std::cout << "adjacent: \n"
                  << "minus -> " << std::endl;
        for (auto e: adjacent_minus) {
            std::cout << e << ' ';
        }
        std::cout << "\n" << "multi -> " << std::endl;
        for (auto e: adjacent_multi) {
            std::cout << e << ' ';
        }


        //容器相邻元素逐个运算,返回指向找到的第一组符合条件元素的第一个的迭代器
        //默认相等
        std::vector<int>::iterator equal_iterator;
        equal_iterator = std::adjacent_find(int_vector.begin(), int_vector.end());
        //改成减5等于前面的元素
        class minus_5_equal {
        public:
            bool operator()(int lhs, int rhs) {
                return lhs == rhs - 5;
            }
        };
        std::vector<int>::iterator minus5_equal_iterator;
        minus5_equal_iterator = std::adjacent_find(int_vector.begin(), int_vector.end(), minus_5_equal());

        std::cout << "\n"
                  << "equal -> \n"
                  << *equal_iterator << " == " << *++equal_iterator << "\n"
                  << *minus5_equal_iterator << " == " << *++minus5_equal_iterator << " - 5" << std::endl;

    }
    {
        std::cout << "========================" << std::endl;

        //二元搜索,默认假设容器元素已经按照 less_than 的顺序进行过排序,如果采用其他排序方式则需传入此排序方式,返回true|false
        std::cout << "sorted search: \n";
        bool found_it;
        int search_value = 21;
        //搜索元素 21 是否在容器中
        found_it = std::binary_search(int_list.begin(), int_list.end(), search_value);
        std::cout << "binary search: \n";
        if (found_it) {
            std::cout << "found value " << search_value << std::endl;
        } else {
            std::cout << "value " << search_value << "not found" << std::endl;
        }
        //判断一个容器中的每一个元素是否都出现在另一个容器中,要求两个容器元素都必须已经经过排序,找到返回true,否则返回false
        int includes_parent_array[12] = {13, 1, 21, 2, 0, 34, 5, 1, 8, 3, 21, 34};
        int includes_pattern_array[6] = {21, 2, 8, 3, 5, 1};
        //使用标准库的排序
        std::sort(includes_parent_array, includes_parent_array+12);
        std::sort(includes_pattern_array, includes_pattern_array+6);
        if(std::includes(includes_parent_array, includes_parent_array+12, includes_pattern_array, includes_pattern_array+6))
        {
            std::cout << "the parent array includes pattern array" << std::endl;
        }
        else
        {
            std::cout << "the parent array not includes pattern array" << std::endl;
        }

    }
    {
        std::cout << "========================" << std::endl;

        std::cout << "copy: \n";
        //复制,将第一个容器一一复制到第二个
        std::vector<int> copy_target(container_size);
        std::copy(int_array, int_array + container_size, copy_target.begin());
        //逆向复制,注意是从给定容器的指定位置向前一一复制,而不是将原容器元素逆向复制到目标容器(可以通过改变传入位置验证,例如传入位置改为倒数第二个位置,则目标容器的最后一个元素为默认值(如果有未初始化取值保护,则整型默认为0),而原容器的第一个元素将无法复制(显示))
        std::vector<int> copy_target_backward(container_size);
        std::copy_backward(int_array, int_array + container_size, copy_target_backward.end());
        std::cout << "forward_copy -> \n";
        for (auto e: copy_target) {
            std::cout << e << ' ';
        }
        std::cout << "\n" << "backward_copy -> \n";
        for (auto e: copy_target_backward) {
            std::cout << e << ' ';
        }

    }
    {
        std::cout << "\n" << "========================" << std::endl;

        std::cout << "count: \n";
        //计数,默认返回指定容器中与给定值相等的元素个数
        int count_value = 36;
        //统计 36 出现次数
        std::cout << "the value " << count_value << " occurs "
                  << std::count(int_vector.begin(), int_vector.end(), count_value)
                  << " time(s) in this vector" << std::endl;

        //特定情况统计次数,例如大于
        int count_value_compare_greater_than_15 = 15;
        std::cout << "found "
                  << std::count_if(int_vector.begin(), int_vector.end(),
                          std::bind2nd(std::greater<int>(),count_value_compare_greater_than_15)) // NOLINT(modernize-use-transparent-functors)
                  << " value(s) greater than "
                  << count_value_compare_greater_than_15
                  << " in this vector" << std::endl;

    }
    {
        std::cout << "========================" << std::endl;

        std::cout << "find: \n";
        //查找,不要求容器已经排序,如果找到返回指向该元素的迭代器,否则返回容器末尾位置
        std::list<int>::iterator find_iterator;
        int find_value = 15;
        find_iterator = std::find(int_list.begin(), int_list.end(), find_value);
        if (find_iterator != int_list.end()) {
            std::cout << "found value " << find_value << " in this list" << std::endl;
        } else {
            std::cout << "value " << find_value << " not found in this list" << std::endl;
        }

        //子序列(容器)查找,如果找到返回指向子序列(容器)出现的最后位置的迭代器,如果没找到返回被查找容器的末尾位置,支持第五个参数指定比较方法,默认是相等
        //四个参数,前两个为目标容器指定的搜索范围,后两个为子序列(容器)的需要被搜索的范围
        int find_end_parent_array[17] = {7, 3, 3, 7, 6, 5, 8, 7, 2, 1, 3, 7, 6, 3, 8, 4, 3};
        int find_end_pattern_array[5] = {5, 3, 7, 6, 4};
        int *find_end_ptr = nullptr;
        //试图从被查找容器中找到子容器中从第一位到第四位元素之间的子序列
        find_end_ptr = std::find_end(find_end_parent_array, find_end_parent_array + 17, find_end_pattern_array + 1,
                                     find_end_pattern_array + 4);
        if (find_end_ptr != find_end_parent_array + 17) {
            //如果找到
            std::cout << "found pattern array in this array, print element(s) from the last position" << std::endl;
            while (find_end_ptr != find_end_parent_array + 17) {
                std::cout << *find_end_ptr++ << ' ';
            }
            std::cout << std::endl;
        } else {
            std::cout << "pattern array not found in this array" << std::endl;
        }

        //查找某个子序列中的元素是否在目标容器中出现过,参数与 find_end() 相同,但是只要子序列中元素出现过即算找到,返回指向被找到的第一个位置的迭代器,否则返回目标容器末尾位置
        int find_first_of_parent_array[10] = {5, 1, 3, 7, 9, 5, 3, 4, 9, 10};
        int find_first_of_pattern_array[3] = {11, 6, 3};
        int *find_first_of_ptr = nullptr;
        //试图在被查找容器中判断是否含有子容器中的元素
        find_first_of_ptr = std::find_first_of(find_first_of_parent_array, find_first_of_parent_array + 10,
                                               find_first_of_pattern_array, find_first_of_pattern_array + 3);
        if (find_first_of_ptr != find_first_of_parent_array + 10) {
            std::cout << "found pattern array's element(s) in this array, print element(s) from the first position"
                      << std::endl;
            while (find_first_of_ptr != find_first_of_parent_array + 10) {
                std::cout << *find_first_of_ptr++ << ' ';
            }
            std::cout << std::endl;
        } else {
            std::cout << "pattern array's element(s) not found in this array" << std::endl;
        }

        //查找第一个符合条件的元素,如果找到返回指向第一个符合条件的元素的位置的迭代器,没找到返回容器末尾,需指定第三个参数的比较方法
        std::vector<int>::iterator find_if_iterator;
        //从容器中找到第一个比 15 大的元素
        int find_if_greater_value = 15;
        find_if_iterator = std::find_if(int_vector.begin(), int_vector.end(),
                std::bind2nd(std::greater<int>(), find_if_greater_value)); // NOLINT(modernize-use-transparent-functors)
        if(find_if_iterator != int_vector.end())
        {
            std::cout << "found value greater than " << find_if_greater_value << " , print element(s) from the first position"
                        << std::endl;
            while(find_if_iterator != int_vector.end())
            {
                std::cout << *find_if_iterator++ << ' ';
            }
            std::cout << std::endl;
        }
        else
        {
            std::cout << "no element(s) found in this vector greater than " << find_if_greater_value << std::endl;
        }
    }
    {
        std::cout << "========================" << std::endl;

        std::cout << "operation: \n";
        //对于指定容器的指定范围内的元素进行指定运算操作,如果有返回值将被忽略,而且这个操作不能更改元素
        //lambda表达式
        auto for_each_print_elements = [](int ele) -> void
        {
            std::cout << ele << ' ';
        };
        std::cout << "for_each traversal list's element(s)" << std::endl;
        std::for_each(int_list.begin(), int_list.end(), for_each_print_elements);
        std::cout << std::endl;

        //将指定运算的结果填入指定容器
        auto generate_element_plus_2 = []() -> int
        {
            static int static_ele = -1;
            return static_ele += 2;
        };
        int generate_array[container_size];
        std::generate(generate_array, generate_array+container_size, generate_element_plus_2);
        std::cout << "generate element(s) plus 2 per operation from -1" << std::endl;
        for(auto e: generate_array)
        {
            std::cout << e << ' ';
        }
        std::cout << std::endl;

        //对一个运算运行n次(次数取决于给定容器容积),将这n次运算的每次运算结果存入给定容器
        const int seed = 100;
        auto generate_n_plus_2 = [=]() -> int
        {
            static int _seed = seed;
            return _seed += 2;
        };
        int generate_n_array[container_size];
        std::generate_n(generate_n_array, container_size, generate_n_plus_2);
        std::cout << "generate element(s) base on seed " << seed << " and plus 2 per operation" << std::endl;
        for(auto e: generate_n_array)
        {
            std::cout << e << ' ';
        }
        std::cout << std::endl;

        //内积,默认将两个容器在对应的元素相乘后累加,再加上给定的初值,返回最终的累积值(注意：第一第二个参数作为范围限定,第三个参数只需要给定另一个容器起始位置)
        int multi_plus_result = std::inner_product(int_vector.begin(), int_vector.end(), int_list.begin(), 0);
        std::cout << "inner_product default: " << multi_plus_result << std::endl;
        //第五个参数指定为各个对应元素运算后的结果的行为,第六个参数指定为哥哥对应元素之间的运算行为
        //注意,改变各个元素运算后的行为对第一对元素同样有效,因为第一个元素实际为给定的初始值,例如下面的结果为 0-(1+1)-(3+3)-(6+6)-(10+10)-(15+15)-(21+21)-(28+28)-(36+36)-(36+36)
        int plus_minus_result = std::inner_product(int_vector.begin(), int_vector.end(), int_list.begin(), 0,
                std::minus<int>(), std::plus<int>()); // NOLINT(modernize-use-transparent-functors)
        std::cout << "inner_product plus and minus: " << plus_minus_result << std::endl;

        //将两个迭代器指向的元素互换
        int iter_swap_array[5] = {1, 2, 3, 4, 5};
        std::cout << "before swap: ";
        for(auto e: iter_swap_array)
        {
            std::cout << e << ' ';
        }
        std::cout << "\n" << "after swap: ";
        std::iter_swap(iter_swap_array+1, iter_swap_array+3);
        for(auto e: iter_swap_array)
        {
            std::cout << e << ' ';
        }
        std::cout << std::endl;

        //以字典顺序逐个比较两容器元素,直到可以判断两元素大小,如果第一容器元素小于第二个则返回true,否则返回false,可以改变第五参数更改默认的 less_than 运算
        std::string compare_array_first[3] = {"aaa", "bbb", "ccb"};
        std::string compare_array_second[3] = {"aaa", "bbb", "ccc"};
        std::cout << "in the order of lexicographical_compare" << std::endl;
        std::cout << "the first array [";
        for(const auto& e: compare_array_first)
        {
            std::cout << e << ' ';
        }
        if(std::lexicographical_compare(compare_array_first, compare_array_first+3, compare_array_second, compare_array_second+3))
        {
            std::cout << "] less-than the second array [";
        }
        else
        {
            std::cout << "] greater-than the second array [";
        }
        for(const auto& e: compare_array_second)
        {
            std::cout << e << ' ';
        }
        std::cout << "] " << std::endl;

        //改变比较方式,改为长度比较
        class size_compare{
        public:
            bool operator() (const std::string& a, const std::string& b)
            {
                return a.length() <= b.length();
            }
        };
        std::string size_compare_first[3] = {"aa", "cc", "eee"};
        std::string size_compare_second[3] = {"c", "e", "aa"};
        std::cout << "in the order of string_size_compare" << std::endl;
        std::cout << "the first array [";
        for(const auto& e: size_compare_first)
        {
            std::cout << e << ' ';
        }
        if(std::lexicographical_compare(size_compare_first, size_compare_first+3, size_compare_second, size_compare_second+3, size_compare()))
        {
            std::cout << "] less-than the second array [";
        }
        else
        {
            std::cout << "] greater-than the second array [";
        }
        for(const auto& e: size_compare_second)
        {
            std::cout << e << ' ';
        }
        std::cout << "] " << std::endl;

        //将两个已经排序容器中的元素按照默认 less-than 的运算方式进行合并排序放置到一个新的容器,第五个参数指定放置的容器,第六个参数指定排序方式
        int merge_array_first[14] = {29, 23, 20, 22, 17, 15, 26, 51, 27, 12, 35, 40, 74, 16};
        int merge_array_second[10] = {39, 54, 21, 44, 62, 10, 27, 41, 68, 71};
        int merge_array_target[24];
        //改为从大到小排序
        std::sort(merge_array_first, merge_array_first+14, std::greater<int>()); // NOLINT(modernize-use-transparent-functors)
        std::sort(merge_array_second, merge_array_second+10, std::greater<int>()); // NOLINT(modernize-use-transparent-functors)
        std::merge(merge_array_first, merge_array_first+14,
                merge_array_second, merge_array_second+10,
                merge_array_target,std::greater<int>()); // NOLINT(modernize-use-transparent-functors)

        std::cout << "after merger and print order by greater-than: " << std::endl;
        for(auto e: merge_array_target)
        {
            std::cout << e << ' ';
        }
        std::cout << std::endl;
    }
    {
        std::cout << "========================" << std::endl;

        std::cout << "partial: \n";
        //局部排序,前三个参数指定排序开始的位置,排序结束的位置,容器的末尾位置,第四个参数默认为按照 less-than 排序
        //注意是只能保证指定范围内是排序的,指定范围填满后其他的元素顺序则不会保证
        int partial_array[12] = {29, 23, 20, 22, 17, 15, 26, 51, 19, 12, 35, 40};
        int partial_first = 0;
        int partial_middle = 5;
        std::cout << "before partial_sort: \n";
        for(int i = 0; i < 12; i++)
        {
            if(i == partial_first)
            {
                std::cout << "[";
            }
            std::cout << partial_array[i];
            if (i == partial_middle)
            {
                std::cout << "] ";
            }
            else
            {
                std::cout << ' ';
            }
        }
        std::cout << "\n" << "after partial_sort from position " << partial_first << " to " << partial_middle << ": \n";
        std::partial_sort(partial_array+partial_first, partial_array+partial_middle, partial_array+12);
        for(int i = 0; i < 12; i++)
        {
            if(i == partial_first)
            {
                std::cout << "[";
            }
            std::cout << partial_array[i];
            if (i == partial_middle)
            {
                std::cout << "] ";
            }
            else
            {
                std::cout << ' ';
            }
        }
        std::cout << std::endl;

        //局部总和,返回一个新序列,序列的每一个元素都是给定序列对应位置的前面所有元素值总和,第四个参数默认为加法
        int partial_sum_array[7] = {1, 3, 4, 5, 7, 8, 9};
        int partial_sum_target[7];
        std::partial_sum(partial_sum_array, partial_sum_array+7, partial_sum_target);
        std::cout << "after partial_sum: \n";
        for(auto e: partial_sum_target)
        {
            std::cout << e << ' ';
        }
        std::cout << std::endl;

        //切割,将容器中元素按照一个一元运算结果(true|false)进行切割
        //partition() 不保证切割后原序列的相对顺序, stable_partition() 保证原来的相对顺序
        auto is_even_element = [](int ele) -> bool
        {
            return !(ele%2);
        };
        int partition_array[12] = {29, 23, 20, 22, 17, 15, 26, 51, 19, 12, 35, 40};
        std::partition(partition_array, partition_array+12, is_even_element);
        std::cout << "after partition by even or not: \n";
        for(auto e: partition_array)
        {
            std::cout << e << ' ';
        }
        std::cout << std::endl;
    }
    {
        std::cout << "========================" << std::endl;

        std::cout << "remove|replace: \n";

        //移除元素,指定一个要移除元素的条件,所有不满足条件的元素会向前放置到最近的可放置的位置,由于不会删除元素,所以容器大小不会改变,返回指向最后一个不符合条件的元素的后一位的地址的迭代器,一般用于传给erase()用于擦除不需要的(符合条件的)元素并改变容器大小
        //数组不易改变大小,所以一般使用remove_copy()将结果复制到一个指定位置
        int remove_array[10] = {0, 1, 0, 2, 0, 3, 0, 4, 0, 5};
        std::vector<int> remove_vector(remove_array, remove_array+10);
        std::cout << "before remove, the vector's size: " << remove_vector.size() << std::endl;
        //试图移除值为0的元素
        remove_vector.erase(std::remove(remove_vector.begin(), remove_vector.end(), 0), remove_vector.end());
        std::cout << "after remove, the vector's size: " << remove_vector.size() << std::endl;
        //remove_if()按照给定的条件进行移除,行为与remove()相同,相应的也有remove_copy_if()

        //替换元素,将所有数值等于 old_value(第三个参数) 的元素替换为 new_value(第四个参数)
        //replace_copy()将结果复制到另一个容器
        int replace_array[10] = {0, 1, 0, 2, 0, 3, 0, 4, 0, 5};
        std::cout << "before replace: \n";
        for(auto e: replace_array)
        {
            std::cout << e << ' ';
        }
        std::cout << "\n" << "after replace: \n";
        std::replace(replace_array, replace_array+10, 0, 999);
        for(auto e: replace_array)
        {
            std::cout << e << ' ';
        }
        std::cout << std::endl;
        //replace_if()按照给定的条件进行替换,与replace()行为相同,相应的也有replace_copy_if()
    }
    {
        std::cout << "========================" << std::endl;

        std::cout << "reverse|rotate: \n";

        //颠倒容器中元素顺序
        //reverse_copy()将结果复制到另一个容器中
        int reverse_array[10] = {1, 2, 3, 7, 8, 9, 4, 5, 6, 10};
        std::cout << "before reverse: \n";
        for(auto e: reverse_array)
        {
            std::cout << e << ' ';
        }
        std::cout << "\n" << "after reverse: \n";
        std::reverse(reverse_array, reverse_array+10);
        for(auto e: reverse_array)
        {
            std::cout << e << ' ';
        }
        std::cout << std::endl;

        //旋转,将容器分为两部分,前半部分与后半部分交换,整块移动,而不是逐个移动
        //rotate_copy()将结果复制到另一个容器中
        int rotate_array[10] = {1, 4, 7, 8, 5, 2, 10, 3, 6, 9};
        std::cout << "before rotate: \n";
        for(auto e: rotate_array)
        {
            std::cout << e << ' ';
        }
        std::cout << "\n" << "after rotate: \n";
        std::rotate(rotate_array, rotate_array+5, rotate_array+10);
        for(auto e: rotate_array)
        {
            std::cout << e << ' ';
        }
        std::cout << std::endl;
    }
    {
        std::cout << "========================" << std::endl;

        std::cout << "set: \n";
        //默认接收五个参数,前两个限定第一序列范围,三四个限定第二序列范围,第五个用于接收符合条件的元素,第六个元素为排序方式,默认为 less-than 排序
        int set_array_first[7] = {0, 1, 2, 3, 4, 5, 6};
        int set_array_second[7] = {4, 5, 6, 7, 8, 9, 10};
        std::set<int> set_set_target;

        //差集,即只出现在第一个序列而未出现在第二个序列的所有元素
        std::cout << "set_difference() between two array: \n";
        std::set_difference(set_array_first, set_array_first+7, set_array_second, set_array_second+7, std::inserter(set_set_target, set_set_target.begin()));
        for(auto e: set_set_target)
        {
            std::cout << e << ' ';
        }
        //擦掉所有元素
        set_set_target.clear();
        //交集,既出现在第一序列又出现在第二序列的所有元素
        std::cout << "\n" << "set_intersection() between two array: \n";
        std::set_intersection(set_array_first, set_array_first+7, set_array_second, set_array_second+7, std::inserter(set_set_target, set_set_target.begin()));
        for(auto e: set_set_target)
        {
            std::cout << e << ' ';
        }
        set_set_target.clear();
        //对称差集,即两个序列独有的元素
        std::cout << "\n" << "set_symmetric_difference() between two array: \n";
        std::set_symmetric_difference(set_array_first, set_array_first+7, set_array_second, set_array_second+7, std::inserter(set_set_target, set_set_target.begin()));
        for(auto e: set_set_target)
        {
            std::cout << e << ' ';
        }
        set_set_target.clear();
        //并集,不重复的或者两个序列的所有元素
        std::cout << "\n" << "set_union() between two array: \n";
        std::set_union(set_array_first, set_array_first+7, set_array_second, set_array_second+7, std::inserter(set_set_target, set_set_target.begin()));
        for(auto e: set_set_target)
        {
            std::cout << e << ' ';
        }
    }
    {
        std::cout << "========================" << std::endl;

        std::cout << "transform: \n";
        int transform_array_first[5] = {4, 5, 6, 8, 2};
        int transform_array_second[7] = {7, 8, 1, 3, 6, 3, 4};
        std::vector<int> transform_vector_target;
        //以一个(两个)序列为基础进行(交互)运行指定运算并产生另一个序列
        //两个版本,第一个版本传入一个一元运算符
        std::cout << "before double element(s): \n";
        for(auto e: transform_array_first)
        {
            std::cout << e << ' ';
        }
        auto transform_double_elements = [](int ele) -> int
        {
            return 2*ele;
        };
        std::transform(transform_array_first, transform_array_first+5, std::inserter(transform_vector_target, transform_vector_target.begin()), transform_double_elements);
        std::cout << "\n" << "after double element(s): \n";
        for(auto e: transform_vector_target)
        {
            std::cout << e << ' ';
        }
        transform_vector_target.clear();
        //第二个版本传入一个二元运算符,可以交互两个序列对应的元素,直到有一个序列到达容器尾部
        auto transform_first_minus_second_elements = [](int first, int second) -> int
        {
            return first - second;
        };
        std::cout << "\n" << "after the first array's element(s) minus the second array's element(s): \n";
        std::transform(transform_array_first, transform_array_first+5, transform_array_second, std::inserter(transform_vector_target, transform_vector_target.begin()), transform_first_minus_second_elements);
        for(auto e: transform_vector_target)
        {
            std::cout << e << ' ';
        }
    }
    return 0;
}
