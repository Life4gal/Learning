import java.util.Arrays;
import java.util.Scanner;

public class Using {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        String[] str = scanner.nextLine().split("[,]");
        double[] nums = new double[str.length];
        int i = 0;
        for (String s : str) {
            if (!s.trim().isEmpty()) {
                nums[i] = Double.parseDouble(s);
                i++;
            }
        }
        nums = Arrays.copyOf(nums, i);
        System.out.println("\nMax = " + Method.Max(nums));
        System.out.println("\nMin = " + Method.Min(nums));
        System.out.println("\nSum = " + Method.Sum(nums));
        double[] new_nums = Method.Reverse(nums);
        for (double num: new_nums){
            System.out.print(num + " ");
        }
    }
}
