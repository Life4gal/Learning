public class SetUpAccount {
    public static void main(String[] args) {
        Account account = new Account("XXcompany", 50);
        WithdrawThread withdrawThread = new WithdrawThread(account);
        DepositThread depositThread = new DepositThread(account);
        new Thread(withdrawThread).start();
        new Thread(depositThread).start();
    }
}
