public class LambdaThread {
    public static void main(String[] args) {
        //正常写法
//        Runnable runnable = new RunnableImpl(); //需要自己创建一个Impl文件并在其中实现run方法
//        Thread thread = new Thread(runnable);
//        thread.start();   //然后执行

        //匿名内部类写法
        new Thread(new Runnable() {     //匿名实现了实现类        简化了创建实现类文件步骤
            @Override
            public void run() {
                System.out.println(Thread.currentThread().getName() + " 创建了新线程");
            }
        }).start();

        //使用lambda表达式写法
        new Thread(() -> {  //  ()表示参数列表    ->表示将参数传给方法体
            System.out.println(Thread.currentThread().getName() + " 创建了新线程");
        }
        ).start();
        //lambda表达式省略写法
        new Thread(() ->System.out.println(Thread.currentThread().getName() + " 创建了新线程")).start();
    }
}
