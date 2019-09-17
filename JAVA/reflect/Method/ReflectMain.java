import java.lang.reflect.Method;

public class ReflectMain {
    public static void main(String[] args) throws Exception {

        Class<Person> personClass = Person.class;
        Person person = new Person();

        Method method1 = personClass.getMethod("method");
        method1.invoke(person);

        Method method2 = personClass.getMethod("method", String.class);
        method2.invoke(person, "这是参数");

        System.out.println("========================");

        //获取所有public修饰的方法(包括继承的方法）（所有类都直接或间接继承Object类）
        Method[] methods = personClass.getMethods();
        for (Method method : methods) {
            System.out.println(method);
        }

        System.out.println("===================");
        //获取类名,形式为类所在的包的"包名"+"."+"类名"
        String name = personClass.getName();
        System.out.println(name);
    }
}
