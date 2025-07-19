import java.io.*;

public class S1 {
    
    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: java S1 <filename>");
            return;
        }

        File f;
        FileInputStream fis;

        for (String filename : args) {
            try {
                f = new File(filename);
                fis = new FileInputStream(f);

                int len = (int) f.length();
                int bytesRead = 0;

                byte[] buffer = new byte[len];

                while (bytesRead < len) {
                    bytesRead += fis.read(buffer, bytesRead, len-bytesRead);
                }

                System.out.println("File: " + filename + "\n============\n" + new String(buffer));
            } 
            catch(FileNotFoundException e) {
                System.out.println("Error: File with name '" + filename + "' does not exist");
            }
            catch(IOException e) {
                System.out.println("Error when trying to read File: " + filename);
            }
        }
    }
}