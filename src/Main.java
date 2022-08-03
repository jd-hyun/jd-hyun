import java.util.Scanner;

public class Main {
    public static void main (String[] args) {
        Scanner scanner = new Scanner(System.in);
        int n = scanner.nextInt();
        int k = scanner.nextInt();
        int[] value = new int[n];
        int min = 0;
        for (int i = 0; i < n; i++) 
            value[i] = scanner.nextInt();
        for (int i = n - 1; i >= 0; i--) {
            if (k / value[i] != 0) {
                min += k / value[i];
                k %= value[i];
            }
        }
        System.out.println(min);
        System.out.println(min);
        System.out.println(min);
        scanner.close();
    }
}