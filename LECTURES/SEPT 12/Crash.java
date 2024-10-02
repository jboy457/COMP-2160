public class Crash{
    public static void main(String[] args) {
        int[] wft = new int[5];
        wft[6] = 20;

        System.out.printf("%d", wft[6]);

    }
}