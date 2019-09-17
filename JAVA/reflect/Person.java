public class Person {
    private String name;
    private int age;

    public String string = "这是public修饰的成员变量";

    public Person() {
    }

    @Override
    public String toString() {
        return "Person{" +
                "name='" + name + '\'' +
                ", age=" + age +
                ", string='" + string + '\'' +
                '}';
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    public Person(String name, int age) {
        this.name = name;
        this.age = age;
    }

    public void method(){
        System.out.println("正在调用Person类的无参方法。。。");
    }

    public void method(String s){
        System.out.println("正在调用Person类的有参方法，参数为：" + s + "。。。");
    }

    public void test(){
        System.out.println("测试从配置文件中读取方法。。。");
    }
}
