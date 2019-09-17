import Junit.CalculatorMethod;
import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

public class CalculatorMethodTest {
    //测试add方法

    @Before
    public void init(){
        System.out.println("使用@Before注解，在所有方法执行前执行。。。");
    }
    @After
    public void destroy(){
        System.out.println("使用@After注解，在所有方法执行后执行。。。");
    }

    @Test   //使用@Test注解即可独立运行
    public void testAdd(){
        //如果测试的方法有问题则会以绿色显示结果，如果有问题则会以红色输出
        System.out.println("这是测试加法运行");
        CalculatorMethod calculatorMethod = new CalculatorMethod();
        System.out.println(calculatorMethod.add(1, 2));
        System.out.println(calculatorMethod.sub(2, 3));
        //注意，打印并不一定能够找出程序问题，例如加法返回的值是减法，而减法返回加法
        //此时虽然输出的结果于期望不对，但是程序执行是正确的，所以应该使用断言方法
        //断言正确则正常
        Assert.assertEquals(3, calculatorMethod.add(1, 2));
        //断言失败报错
        //Assert.assertEquals(4, calculatorMethod.sub(1, 2));
    }
}
