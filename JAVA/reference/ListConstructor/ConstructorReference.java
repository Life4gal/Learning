public class ConstructorReference {
    private static int[] createArray(int length, ArrayBuilder arrayBuilder){
        return arrayBuilder.buildArray(length);
    }

    public static void main(String[] args) {
//        int[] array = createArray(5, (l) -> {
//            return new int[l];
//        });

        //使用引用
        int[] array = createArray(10, int[]::new);
        System.out.println(array.length);
    }
}
