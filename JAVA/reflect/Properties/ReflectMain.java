import java.io.InputStream;
import java.lang.reflect.Method;
import java.util.Properties;

public class ReflectMain {
    public static void main(String[] args) throws Exception {

        ClassLoader classLoader = ReflectMain.class.getClassLoader();
        //加载pro.properties配置文件
        Properties properties = new Properties();
        InputStream inputStream = classLoader.getResourceAsStream("pro.properties");
        assert inputStream != null;
        properties.load(inputStream);

        //获取配置文件内容
        String className = properties.getProperty("className");
        String methodName = properties.getProperty("methodName");

        //加载类进内存
        Class<?> aClass = Class.forName(className);
        //创建对象
        Object newInstance = aClass.newInstance();
        //获取方法对象
        Method method = aClass.getMethod(methodName);
        method.invoke(newInstance);
    }
}
