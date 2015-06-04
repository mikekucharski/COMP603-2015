import java.util.Scanner;

class Helloworld {
public static void main(String[] args) {
char[] box = new char[30000];
int i = 0;
Scanner s = new Scanner(System.in);
box[i] += 8;
while(box[i] != 0) {
i += 1;
box[i] += 4;
while(box[i] != 0) {
i += 1;
box[i] += 2;
i += 1;
box[i] += 3;
i += 1;
box[i] += 3;
i += 1;
box[i] += 1;
i -= 4;
box[i] -= 1;
}
i += 1;
box[i] += 1;
i += 1;
box[i] += 1;
i += 1;
box[i] -= 1;
i += 2;
box[i] += 1;
while(box[i] != 0) {
i -= 1;
}
i -= 1;
box[i] -= 1;
}
i += 2;
System.out.print(box[i]);
i += 1;
box[i] -= 3;
System.out.print(box[i]);
box[i] += 7;
System.out.print(box[i]);
System.out.print(box[i]);
box[i] += 3;
System.out.print(box[i]);
i += 2;
System.out.print(box[i]);
i -= 1;
box[i] -= 1;
System.out.print(box[i]);
i -= 1;
System.out.print(box[i]);
box[i] += 3;
System.out.print(box[i]);
box[i] -= 6;
System.out.print(box[i]);
box[i] -= 8;
System.out.print(box[i]);
i += 2;
box[i] += 1;
System.out.print(box[i]);
System.out.print(box[i]);
System.out.print(box[i]);
i += 1;
box[i] += 2;
System.out.print(box[i]);
}
}
