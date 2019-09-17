public class ConstructorReference {
    private static void printName(String name, PersonBuilder personBuilder){
        Person person = personBuilder.buildPerson(name);
        System.out.println(person.getName());
    }

    public static void main(String[] args) {
        //printName("张三", name->new Person(name));

        //优化lambda
        //成员对象创建方法new已存在
        //构造方法new Person(name)存在
        printName("张三", Person::new);
    }
}
