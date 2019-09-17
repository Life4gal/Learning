public class LambdaInterface {
    public static void main(String[] args) {
        //使用匿名内部类写法
        InvokePerson(new Person() {
            @Override
            public void work() {
                System.out.println("这是匿名内部类实现方法");
            }
        });
        //使用lambda简化
        InvokePerson(()->{
            System.out.println("这是lambda表达式简化实现方法");
        });
        //lambda省略写法
        InvokePerson(()->System.out.println("这是lambda省略写法实现方法"));
    }
    private static void InvokePerson(Person person){
        person.work();
    }
}
