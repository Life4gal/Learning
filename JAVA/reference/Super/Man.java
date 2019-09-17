//子类
public class Man extends Human{
    //重写父类sayHello方法
    @Override
    public void sayHello(){
        System.out.println("Hello! 这是子类方法");
    }

    private void Method(Greet greet){
        greet.greet();
    }

    private void show(){
//        Method(()->{
//            //创建父类对象
//            Human human = new Human();
//            human.sayHello();
//        });

        //因为有继承关系，所以可以用super关键字
        //Method(()->super.sayHello());

        //父类成员super已经存在，且父类成员方法已经存在
        Method(super::sayHello);
    }

    public static void main(String[] args) {
        new Man().show();
    }
}
