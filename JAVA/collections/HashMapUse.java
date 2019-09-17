import java.util.*;

class HashMapUse {

    static Map<String, String> run(){
        return new HashMap<>();
    }

    static Map<String, String> add(Map<String, String> info){
        System.out.println("Input key and value in the same line");
        Scanner scanner = new Scanner(System.in);
        String row;
        String[] r;
        while(!(row = scanner.nextLine()).trim().isEmpty()) {
            r = row.split(",");
            try {
                info.put(r[0], r[1]);
            } catch (Exception e) {
                System.out.println("Wrong Format!\n" + e.getMessage());
            }
        }/*
        Set<String> keys = info.keySet();
        for (String key: keys) {
            System.out.printf("Key: %s -> Value: %s", key, info.get(key));
        }*/
        Iterator<Map.Entry<String, String>> _info = info.entrySet().iterator();
        System.out.println("Input finish! now k-v:");
        while(_info.hasNext()){
            Map.Entry<String, String> line = _info.next();
            System.out.printf("Key: %s -> Value: %s\n", line.getKey(), line.getValue());
        }
        return info;
    }

    static void search(Map<String, String> info){
        System.out.println("Input the key to search");
        Scanner scanner = new Scanner(System.in);
        String row;
        while(!(row = scanner.nextLine().trim()).isEmpty()){
            System.out.println("Input the key to search");
            if(info.get(row) == null){
                System.out.println("The key not exist!");
            }
            else{
                System.out.printf("Key: %s -> Value: %s\n",row, info.get(row));
            }
        }
    }
}
