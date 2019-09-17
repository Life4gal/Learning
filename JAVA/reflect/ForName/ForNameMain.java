package forName;

public class ForNameMain {
    public static void main(String[] args) throws Exception {

        //Class.forName("全类名")
        //多用于配置文件，将类名定义在配置文件中，读取文件，加载类
        Class<?> personClass1 = Class.forName("Person");
        System.out.println(personClass1);

        System.out.println("===============");

        //类名.class
        //多用于参数传递
        Class<Person> personClass2 = Person.class;
        System.out.println(personClass2);

        System.out.println("===============");

        //对象.getClass
        //多用于对象获取字节码的方式
        Person person = new Person();
        Class<? extends Person> personClass3 = person.getClass();
        System.out.println(personClass3);

        System.out.println("===============");

        assert personClass1 == personClass2;
        assert personClass1 == personClass3;
        assert personClass2 == personClass3;
        //是同一个对象，内存地址相同，断言无误

        System.out.println("结论：同一个 *.class 字节码文件在程序执行过程中只加载一次");
    }
}
