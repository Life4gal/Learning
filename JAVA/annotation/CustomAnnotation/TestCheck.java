import java.io.BufferedWriter;
import java.io.FileWriter;
import java.lang.reflect.Method;

public class TestCheck {
    public static void main(String[] args) throws Exception {

        int bugTimes = 0;
        BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter("bug.txt"));

        //创建对象
        Calculator calculator = new Calculator();
        //获取字节码文件对象
        Class aClass = calculator.getClass();
        //获取方法
        Method[] methods = aClass.getMethods();
        //判断方法上是否有@Check注解
        for (Method method : methods) {
            //执行有@Check注解的方法
            if(method.isAnnotationPresent(Check.class)){
                try{
                    method.invoke(calculator);
                    //捕获异常
                }catch (Exception e) {
                    bugTimes++;
                    bufferedWriter.write("第 " + bugTimes + " 次异常");
                    bufferedWriter.newLine();
                    bufferedWriter.write(method.getName() + " 方法出现异常");
                    bufferedWriter.newLine();
                    bufferedWriter.write("异常的名称 " + e.getCause().getClass().getSimpleName());
                    bufferedWriter.newLine();
                    bufferedWriter.write("异常的原因为 " + e.getCause().getMessage());
                    bufferedWriter.newLine();
                    bufferedWriter.write("==============================");
                    bufferedWriter.newLine();
                }
            }
        }
        bufferedWriter.write("本次测试总共有 " + bugTimes + " 次异常。");
        bufferedWriter.close();
    }
}
