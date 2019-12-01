#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <list>
#include <deque>
#include <queue>


using namespace std;

/*
 * first: 迭代器起始位置
 * last: 迭代器终止位置
 * value: 需要寻找的值
 * index: 返回找到的第一个元素的下标,如果找不到返回-1
 * 作用: 针对各种基本(也许更多)类型(int, double, float, string, long, ...),针对各种基本(也许更多)基础容器(array, vector, list, deque, queue),
 * 用以迭代容器找到并返回第一个与 val 匹配的值的地址,
 * 没找到默认返回最后一个位置的地址.
 */
template <typename iteratorType, typename elemType>
iteratorType
myFind(iteratorType first, iteratorType last, const elemType& value, int& index){
    for(int i = 0; first != last; first++, i++){
        if(value == *first){
            index = i;
            return first;
        }
    }
    index = -1;
    return last;
}

/*
 * first: 迭代器起始位置
 * last: 迭代器终止位置
 * at: 储存输出
 *      注意: at 的赋值必须是可迭代容器地址
 * val: 需要相比较的值
 * pred: 比较方法
 * 作用: 针对各种基本(也许更多)类型(int, double, float, string, ...),针对各种基本(也许更多)基础容器(array, vector, list, deque, ...),
 * 针对各种基本内置(也许更多)关系运算(less<type>, less_equal<type>, greater<type>, greater_equal<type>, equal_to<type>, not_equal_to<type>, ...),
 * 进行数据过滤,仅保留符合条件的值放在 at 迭代器中并返回.
 * 注意: 此处使用 bind2nd 将传入的 val 作为运算方法中的第二个值,如果换成 bind1st 则可以将 val 作为运算方法中的第二个值.
 */
template <typename inputIterator, typename outputIterator, typename elemType, typename Comparable>
outputIterator
myFilter(inputIterator first, inputIterator last, outputIterator at, const elemType& val, Comparable pred){
    while((first = find_if(first, last, bind2nd(pred, val))) != last){  // bind2nd: 将 val 的值绑定至 pred 所指的二元运算方法的第二个值
                                        // not1: 一元函数对象真伪值取反, not2: 二元函数对象真伪值取反, 例如上句中的
                                        // bind2nd(pred, val) 改为 not1(bind2nd(pred, val))
                                        // 当 pred 所指的二元运算方法为 less<type>时,实际效果会变成 greater_equal<type>
                                        // 当 pred 所指的二元运算方法为 equal_to<type>时,实际效果会变成 not_equal_to<type>
        cout << "found value " << *first<< endl;
        *at++ = *first++;
    }
    return at;
}



int main(){

    const int elem_size = 8;

    int arr[elem_size] = {12, 8, 43, 0, 6, 21, 3, 7};   // array

    double double_arr[elem_size] = {12.3453543, 45.643453, 1.6343543, 5.4384346, 33.4838436, 0.483483, 99.43884341, 46.1438438434};
    vector<double> vec(double_arr, double_arr+elem_size);   // vector

    float float_arr[elem_size] = {5.4, 6.2, 99.1, 48.3, 0.2, 1.6, 36.1, 26.9};
    list<float> ls(float_arr, float_arr+elem_size);   // list

    string string_arr[elem_size] = {"abc", "bcd", "cde", "def", "efg", "fgh", "ghi", "hij"};
    deque<string> dc(string_arr, string_arr+elem_size);  // deque

    cout << "test myFind() function" << endl;
    {
        /*
         * test int array
         */
        {
            cout << "test int array, type the value to find" << endl;
            int val;
            int index;
            cin >> val;
            cout << "find value: " << val << endl;
            if(myFind(arr, arr + elem_size, val, index) != arr + elem_size){
                cout << "found value " << val << " in position " << index << endl;
            }
            else{
                cout << "value not found" << endl;
            }
        }

        /*
         * test double vector
         */
        {
            cout << "test double vector, type the value to find" << endl;
            double val;
            int index;
            cin >> val;
            cout << "find value: " << val << endl;
            if(myFind(vec.begin(), vec.end(), val, index) != vec.end()){
                cout << "found value " << val << " in position " << index << endl;
            }
            else{
                cout << "value not found" << endl;
            }
        }

        /*
         * test float list
         */
        {
            cout << "test float list, type the value to find" << endl;
            float val;
            int index;
            cin >> val;
            cout << "find value: " << val << endl;
            if(myFind(ls.begin(), ls.end(), val, index) != ls.end()){
                cout << "found value " << val << " in position " << index << endl;
            }
            else{
                cout << "value not found" << endl;
            }
        }

        /*
         * test string deque
         */
        {
            cout << "test string deque, type the string to find" << endl;
            string val;
            int index;
            cin >> val;
            cout << "find string: " << val << endl;
            if(myFind(dc.begin(), dc.end(), val, index) != dc.end()){
                cout << "found string " << val << " in position " << index << endl;
            }
            else{
                cout << "string not found" << endl;
            }
        }
    }

    cout << "\n\n\n";

    cout << "test myFilter() function" << endl;
    {
        /*
         * test int array, less<int>
         */
        {
            cout << "test int array and less<int>, type the max value to filter" << endl;
            int val;
            int int_arr[elem_size];
            cin >> val;
            cout << "filter integer array for values less than " << val << endl;
            myFilter(arr, arr+elem_size, int_arr, val, less<int>());    // int 不可以省略!省略默认为less<void>,编译出错!
        }

        /*
         * test double vector, less_equal<double>
         */
        {
            cout << "test double vector, type the max value to filter" << endl;
            double val;
            vector<double> double_vector(elem_size);
            cin >> val;
            cout << "filter double vector for values less_equal than " << val << endl;
            myFilter(vec.begin(), vec.end(), double_vector.begin(), val, less_equal<double>());
        }

        /*
         * test float list, greater<float>
         */
        {
            cout << "test float list, type the min value to filter" << endl;
            float val;
            list<float> float_list(elem_size);
            cin >> val;
            cout << "filter float list for values greater than " << val << endl;
            myFilter(ls.begin(), ls.end(), float_list.begin(), val, greater<float>());
        }

        /*
         * test string deque, greater_equal
         */
        {
            cout << "test string deque, type the min string to filter(compare by ASCII)" << endl;
            string val;
            deque<string> string_deque(elem_size);
            cin >> val;
            cout << "filter string deque for values greater_equal than " << val << endl;
            myFilter(dc.begin(), dc.end(), string_deque.begin(), val, greater_equal<string>());
        }
    }

    return 0;
}
