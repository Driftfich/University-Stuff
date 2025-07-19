import java.io.*;
// import java


public class S4 {
    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: java S2 <filename> ...");
            return;
        }

        FileReader fr;
        CharArrayWriter caw = new CharArrayWriter();

        for (String filename : args) {
            try {
                fr = new FileReader(filename);

                byte b = 0;
                do {
                    caw.write(b);
                    b = (byte) fr.read();
                } while (b != -1);
                

                System.out.println("File: " + filename + "\n============\n" + caw.toString());
                caw.reset();
            } 
            catch(FileNotFoundException e) {
                System.out.println("Error: file with name '" + filename + "' does not exist");
            }
            catch(IOException e) {
                System.out.println("Error when trying to read file: " + filename);
            }
        }
    }
}