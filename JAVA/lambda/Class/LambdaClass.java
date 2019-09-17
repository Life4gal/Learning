import java.util.Arrays;

/*
    要求：
        使用数组来储存一些person对象并且按年龄升序排序输出
 */
public class LambdaClass {
    public static void main(String[] args) {

        Person[] people = {
                new Person("张三", 15),
                new Person("李四",12),
                new Person("王五", 25),
                new Person("赵六", 17)
        };
        //使用Arrays的sort方法进行排序
        //正常写法
        /*
        Arrays.sort(people, new Comparator<Person>() {
            @Override
            public int compare(Person person, Person t1) {
                return person.getAge() - t1.getAge();
            }
        });
        */
        /*
        //lambda写法
        Arrays.sort(people, (Person person, Person t1)->{
            return person.getAge() - t1.getAge();
        });
         */
        //lambda省略写法
        Arrays.sort(people, (p1, p2)->p1.getAge() - p2.getAge());

        for (Person person : people) {
            System.out.println(person.toString());
        }
    }
}
