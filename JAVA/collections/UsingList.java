import java.util.List;

public class UsingList {
    public static void main(String[] args) {
        List<String> info = ListUse.input();
        while (ListUse.needModify(info)){}
        System.out.println("=====program ended=====");
    }
}
