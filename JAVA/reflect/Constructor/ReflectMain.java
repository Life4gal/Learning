import java.lang.reflect.Constructor;

public class ReflectMain {
    public static void main(String[] args) throws Exception {
        //和Field使用类似
        Class<Person> personClass = Person.class;

        Constructor<Person> constructor = personClass.getConstructor(String.class, int.class);

        Person person1 = constructor.newInstance("张三", 15);
        System.out.println(person1);

        Object o = personClass.newInstance();
        System.out.println(o);
    }
}
