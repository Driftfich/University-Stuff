import java.io.*;

public class Stream1 {
    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: java Stream1 <filename>");
            return;
        }
        // create a FileInputStream
        FileInputStream fis;
        File file;
        for (String filename : args) {
            try {
                file = new File(filename);
                fis = new FileInputStream(file);
                int len = (int) file.length();
                int bytesRead = 0;
                byte[] buffer = new byte[len];

                while (bytesRead < len) {
                    bytesRead += fis.read(buffer, bytesRead, len - bytesRead);
                }

                System.out.println("File: " + filename);
                System.out.println("Length: " + len + " bytes");
                System.out.println("Content: " + new String(buffer));
            } catch (FileNotFoundException e) {
                System.out.println("File not found: " + filename);
            } catch (IOException e) {
                System.out.println("Error reading file: " + filename);
            } finally {
                // try {
                //     if (fis != null) {
                //         fis.close();
                //     }
                // } catch (IOException e) {
                //     System.out.println("Error closing file: " + filename);
                // }
            }


        }
    }
}