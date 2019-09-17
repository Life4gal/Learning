public class WithdrawThread implements Runnable{
    private Account account;

    public WithdrawThread(Account account) {
        this.account = account;
    }

    @Override
    public void run() {
        for (int i = 0; i < 5; i++) {
            account.withdraw(Math.random() * 500);
            try{
                Thread.sleep((long) (Math.random() * 1000));
            }
            catch (InterruptedException ignored){
            }
        }
    }
}
