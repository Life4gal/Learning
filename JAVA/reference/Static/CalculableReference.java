public class CalculableReference {
    private static int Method(int number, Calculable calculable){
        return calculable.calcAbs(number);
    }

    public static void main(String[] args) {
        int result1 = Method(-10, (n) -> {
            return Math.abs(n);
        });
        System.out.println(result1);

        /*
        使用方法引用优化lambda
        */
        int result2 = Method(-20, Math::abs);
        System.out.println(result2);
    }

}
