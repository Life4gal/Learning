import java.util.Arrays;

public class Method {
    public static double Min(double...nums){
        double min = nums[0];
        for (double i: nums) {
            if(min > i){
                min = i;
            }
        }
        return min;
    }

    public static double Max(double...nums){
        double max;
        max = nums[0];
        for (double i: nums) {
            if(max < i){
                max = i;
            }
        }
        return max;
    }

    public static double Sum(double...nums){
        double tot = 0;
        for(double i: nums){
            tot += i;
        }
        return tot;
    }

    @Override
    public String toString() {
        return "Method{}";
    }

    public static double[] Reverse(double...nums){
        double[] _nums = Arrays.copyOf(nums, nums.length);
        for(int i = 0; i < nums.length; i++){
            _nums[i] = nums[nums.length - i - 1];
        }

        return _nums;
    }
}
