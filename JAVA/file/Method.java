import javax.swing.*;
import java.io.*;
import java.util.Arrays;

class Method {
    public static class Student implements Serializable {
        private static final long serialVersionUID = 1L;
        private String name;
        private String id;
        private String sex;

        Student(String name, String id, String sex) {
            this.name = name;
            this.id = id;
            this.sex = sex;
        }

        @Override
        public String toString() {
            return "Student{" +
                    "name='" + name + '\'' +
                    ", id='" + id + '\'' +
                    ", sex='" + sex + '\'' +
                    '}';
        }
    }
    static String Show(){
        String target;
        JFileChooser jFileChooser = new JFileChooser();
        System.out.println("Choose the file path to save data!");
        if(jFileChooser.showSaveDialog(null) == JFileChooser.APPROVE_OPTION){
            target = jFileChooser.getSelectedFile().toString();
            System.out.println("Format: name, id, sex");
            return target;
        }
        return "";
    }
    static void WriteData(String file) throws Exception{
        if(file.equals("")){
            throw new Exception("You must choose a file path to save!");
        }
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(System.in));
        BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(file));
        String[] row;
        while(true){
            try{
                row = bufferedReader.readLine().split("[,]");
                bufferedWriter.write(new Student(row[0], row[1], row[2]).toString());
                bufferedWriter.newLine();
            }
            catch (Exception e){
                bufferedWriter.close();
                System.out.println("Write complete!");
                break;
            }
        }
    }
    static void ReadData(String file) throws Exception{
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
        String row;
        while((row = bufferedReader.readLine()) != null){
            System.out.println(row);
        }
    }
}
