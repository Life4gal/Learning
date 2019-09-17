import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;

class ListUse {
    static List<String> input(){
        Scanner scanner = new Scanner(System.in);
        List<String> info = new ArrayList<>();
        System.out.println("Input Data:");
        String[] row = scanner.nextLine().split(",");
        Collections.addAll(info, row);
        System.out.println("Now data:\n" + info);
        return info;
    }

    private static void add(List<String> info) throws Exception{
        System.out.println("Input the location's index and content to insert");
        Scanner scanner = new Scanner(System.in);
        String[] row = scanner.nextLine().split(",");
        try{
            info.add(Integer.parseInt(row[0]), row[1]);
            System.out.println("Insert success! now data:\n" + info);
        }
        catch (Exception e){
            System.out.println("Insert Fail!\n" + e.getMessage());
        }
    }

    private static void remove(List<String> info) throws Exception{
        System.out.println("Input the location's index where you need to remove");
        Scanner scanner = new Scanner(System.in);
        int index = scanner.nextInt();
        try{
            info.remove(index);
            System.out.println("Remove success! now data:\n" + info);
        }
        catch (Exception e){
            System.out.println("Remove fail!\n" + e.getMessage());
        }
    }

    private static void modify(List<String> info) throws Exception{
        System.out.println("Input the location's index and content to modify");
        Scanner scanner = new Scanner(System.in);
        String[] row = scanner.nextLine().split(",");
        try{
            info.set(Integer.parseInt(row[0]), row[1]);
            System.out.println("Modify success! now data:\n" + info);
        }
        catch (Exception e){
            System.out.println("Modify fail!\n" + e.getMessage());
        }
    }

    static boolean needModify(List<String> info){
        System.out.println("Please input operation(add = add data, remove = remove data, modify = modify data), input quit to finish");
        Scanner scanner = new Scanner(System.in);
        String row = scanner.nextLine().trim();
        switch (row){
            case "quit":
                return false;
            case "add":
                try {
                    add(info);
                }
                catch (Exception ignored){
                }
                return true;
            case "remove":
                try {
                    remove(info);
                }
                catch (Exception ignored){
                }
                return true;
            case "modify":
                try {
                    modify(info);
                }
                catch (Exception ignored){
                }
                return true;
            default:
                System.out.println("Invalid operation! Input again!");
                return true;
        }
    }
}
