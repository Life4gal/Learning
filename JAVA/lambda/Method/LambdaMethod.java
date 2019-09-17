public class LambdaMethod {
    public static void main(String[] args) {
        //匿名内部类写法
        invokeAdd(10, 20, new Calculator() {
            @Override
            public int add(int a, int b) {
                return a + b;
            }
        });
        //lambda写法
        invokeAdd(10, 20, (int a, int b)->{
            return a + b;
        });
        //lambda省略写法
        invokeAdd(10, 20, (a, b)->a + b);

    }
    private static void invokeAdd(int a, int b, Calculator calculator){
        System.out.println(calculator.add(a, b));
    }
}
