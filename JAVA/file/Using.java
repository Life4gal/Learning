public class Using {
    public static void main(String[] args) throws Exception{
        String file = Method.Show();
        Method.WriteData(file);
        System.out.println("=======READING=======");
        Method.ReadData(file);
    }
}
