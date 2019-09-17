public class Account {
    private String name;
    private double balance;

    public Account(String name, double balance) {
        this.name = name;
        this.balance = balance;
        System.out.printf("%s set up an account, account funding $%.2f \n", name, balance);
    }

    synchronized void withdraw(double money){
        if(money > 0){
            while(money > balance){
                try{
                    System.out.printf("Withdrawal amount is $%.2f, account balance is $%.2f, lack of funds...\n", money, balance);
                    super.wait();
                }
                catch (InterruptedException e){
                    System.out.println("Funds deposited!");
                }
            }
            balance -= money;
            System.out.println("Account Balance is $" + balance);
        }
        else{
            System.out.println("Deposit amount can not less than 0!");
        }
    }
    synchronized void deposit(double money){
        if(money > 0){
            balance += money;
            System.out.printf("Deposit amount is $%.2f, account balance is $%.2f\n", money, balance);
            super.notifyAll();
        }
        else{
            System.out.println("Deposit amount can not less than 0!");
        }
    }
}
