import java.util.ArrayList;
import java.util.List;

public class ListStream {
    public static void main(String[] args) {
        List<String> list = new ArrayList<>();
        list.add("张三");
        list.add("李四");
        list.add("张三三");
        list.add("张四");
        list.add("王五");
        list.add("张三四");

        //要求：找出list集合中以张开头且长度为三个字符的所有元素并输出
        //正常方式：
        // for循环遍历找出张开头字符
        // for循环遍历找出长度为三的字符
        // for循环遍历打印元素
        //缺点：效率低，过于注重怎么做而不是要怎么样

        //流式方法
        list.stream()       //由于参数为函数式参数所以使用lambda表达式,进行两次过滤然后输出
                .filter(name->name.startsWith("张"))
                .filter(name->name.length()==3)
                .forEach(name-> System.out.println(name));
    }
}
