import java.lang.reflect.Field;

public class ReflectMain {
    public static void main(String[] args) throws Exception {
        //1.获取Person的Class对象
        Class personClass = Person.class;
        //2.获取 public 成员变量（只获取public修饰的变量）
        Field[] fields = personClass.getFields();   //getFields()获取所有   getField(String name)获取指定的
        for (Field field : fields) {
            System.out.println(field);
        }
        //通过以下方法可以获取到成员的值
        Field string = personClass.getField("string");
        Person person = new Person();
        Object o1 = string.get(person);
        System.out.println(o1);
        //重新设置成员的值
        string.set(person, "这是反射方法获取后修改的成员变量。。。");
        Object o2 = string.get(person);
        System.out.println(o2);
        System.out.println("这是修改后的Person对象: " + person);

        System.out.println("=========================");

        //获取所有成员变量（不考虑修饰符）
        Field[] declaredFields = personClass.getDeclaredFields();
        for (Field declaredField : declaredFields) {
            System.out.println(declaredField);
        }
        //但是私有成员变量依旧无法直接获取
        Field name = personClass.getDeclaredField("name");
        //如果直接获取将会报错
        //除非暴力忽略访问权限限制
        name.setAccessible(true);
        Object o3 = name.get(person);
        System.out.println("尝试获取私有成员变量name: " + o3);
    }
}
