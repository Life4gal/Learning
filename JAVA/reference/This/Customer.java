public class Customer {
    private void buyHouse(){
        System.out.println("买房子!");
    }

    private void Marry(Rich rich){
        rich.buy();
    }

    private void Happy(){
//        Marry(()->{
//            this.buyHouse();
//        });

        //使用方法引用
        Marry(this::buyHouse);
    }

    public static void main(String[] args) {
        new Customer().Happy();
    }
}
