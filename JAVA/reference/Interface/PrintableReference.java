public class PrintableReference {
    private static void printString(Printable printable){
        printable.print("Hello World");
    }

    public static void main(String[] args) {
        printString(s->{
            //创建MethodReference对象
            MethodReference methodReference = new MethodReference();
            //使用方法
            methodReference.UpperString(s);
        });

        /*
        使用方法引用优化lambda
        如果对象MethodReference是已经存在的       //前提！
        成员方法UpperString也是已经存在
        就可以用对象名引用成员方法
         */
        MethodReference methodReference = new MethodReference();
        printString(methodReference::UpperString);
    }
}
