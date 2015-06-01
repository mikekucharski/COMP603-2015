import java.util.Scanner;

class Echo {
public static void main(String[] args) {
char[] box = new char[30000];
int i = 0;
Scanner s = new Scanner(System.in);
box[i]++;
while(box[i] != 0) {
i++;
box[i] = s.next().charAt(0);
System.out.print(box[i]);
i--;
}
}
}
