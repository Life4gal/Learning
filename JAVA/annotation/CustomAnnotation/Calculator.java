import org.w3c.dom.ls.LSOutput;

public class Calculator {

    @Check
    public void add(){
        System.out.println("1 + 0 = " + (1 + 0));
    }

    @Check
    public void sub(){
        System.out.println("1 - 0 = " + (1 - 0));
    }

    @Check
    public void mul(){
        System.out.println("1 * 0 = " + (1 * 0));
    }

    @Check
    public void div(){
        System.out.println("1 / 0 = " + (1 / 0));
    }

    @Check
    public void str(){
        String str = null;
        System.out.println(str.toString());
    }

    public void show(){
        System.out.println("本方法不会被测试执行。。。");
    }
}
