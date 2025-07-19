import java.io.*;
import java.net.*;

public class S2 {
    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: java S2 <url> ...");
            return;
        }

        URL u;
        InputStream is;

        for (String url : args) {
            try {
                u = new URL(url);
                is = u.openStream();

                int len = (int) is.available();
                byte[] buffer = new byte[len];
                int bytesRead = 0;

                while (bytesRead < len) {
                    bytesRead += is.read(buffer, bytesRead, len-bytesRead);
                }

                System.out.println("Url: " + url + "\n============\n" + new String(buffer));
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