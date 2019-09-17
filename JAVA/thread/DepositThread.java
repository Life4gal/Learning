public class DepositThread implements Runnable{
    private Account account;

    public DepositThread(Account account) {
        this.account = account;
    }

    @Override
    public void run() {
        for (int i = 0; i < 25; i++) {
            account.deposit(Math.random() * 100);
            try{
                Thread.sleep((long) (Math.random() * 1000));
            }
            catch (InterruptedException ignored){
            }
        }
    }
}
