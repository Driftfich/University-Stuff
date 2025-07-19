import java.io.*;
import java.net.*;


public class S3 {
    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: java S2 <url> ...");
            return;
        }

        URL u;
        InputStream is;
        ByteArrayOutputStream bas = new ByteArrayOutputStream();

        for (String url : args) {
            try {
                u = new URL(url);
                is = u.openStream();

                int len = (int) is.available();
                // byte[] buffer = new byte[len];
                // int bytesRead = 0;
                byte b = 0;
                do {
                    bas.write(b);
                    b = (byte) is.read();
                } while (b != -1);
                

                System.out.println("Url: " + url + "\n============\n" + bas.toString());
                bas.reset();
            } 
            catch(MalformedURLException e) {
                System.out.println("Error: url with name '" + url + "' does not exist");
            }
            catch(ConnectException e) {
                System.out.println("Error: Connection to '" + url + "'failed");
            }
            catch(IOException e) {
                System.out.println("Error when trying to read url: " + url);
            }
        }
    }
}