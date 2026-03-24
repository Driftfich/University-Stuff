import java.io.*;
import java.net.*;
import javax.net.ssl.*;
import java.util.Base64;

public class WGet_simple {
    public static Socket clientSocket = null;
    public static String targethost, targetfile;
    public static int targetport;
    public static String targetpath;
    public static String request, req_type, host, content_type, params, content_length, auth, username, password, credentials_enc, res, nl;
    public static void main(String[] args) {
        if (args.length < 3) {
            System.out.println("Aufruf: java WGet_simple host port pfad [params] [username password]");
            System.exit(-1);
        }
        targethost = args[0]; // url-host
        targetport = Integer.parseInt(args[1]); // url-port
        targetpath = args[2]; // url-path

        if (args.length >= 4) {
            params = args[3]; // request parameters
        } else {
            params = "";
        }

        if (args.length >= 6) {
            username = args[4];
            password = args[5];
            credentials_enc = Base64.getEncoder().encodeToString((username + ":" + password).getBytes());
            auth = "Authorization: Basic " + credentials_enc;
        }
        try {
            // TCP-Socket (unverschlüsselt)
            clientSocket = new Socket( targethost, targetport );
            // alternativ TLS-Socket (verschlüsselt)
            // SSLSocketFactory sslFact = (SSLSocketFactory) SSLSocketFactory.getDefault();
            // clientSocket = (SSLSocket) sslFact.createSocket( targethost, targetport );
            // Bietet formatierte Textausgabe u.a. println()
            PrintWriter clientOut = new PrintWriter(new BufferedOutputStream( clientSocket.getOutputStream()) );
            BufferedReader clientIn = new BufferedReader(new InputStreamReader( clientSocket.getInputStream() ));
            //TODO
            nl = "\r\n"; // new line definition for internet protocols
            req_type = "POST " + targetpath + " HTTP/1.1"; // request line 1
            host = "Host: " + targethost; // request line 2
            // params = "a=1&b=2"; //
            content_type = "Content-Type: application/x-www-form-urlencoded";
            content_length = "Content-Length: " + params.length();
            request = req_type + nl
                    + host + nl
                    + content_type + nl
                    + content_length + nl;

            if (credentials_enc != null) {
                    request += auth + nl;
            }
            request += nl + params + nl + nl; // total request string to send over TCP-connection

            System.out.println("HTTP-Request-Header:\n-------------------");
            System.out.println(request + System.lineSeparator() );
            // Sende Request + Leerzeile über TCP-Stream
            clientOut.print( request );
            clientOut.flush();
            System.out.println("HTTP-Response-Header:\n-------------------");
            while ( !(res = clientIn.readLine()).equals("") ) {
                System.out.println( res );
            }
            System.out.println("\nHTTP-Daten:\n----------------");
            while ((res = clientIn.readLine()) != null) {
                System.out.println(res);
            }
            System.out.println("\n------\nVerbindung vom Server beendet");
            clientSocket.close();
        }
        catch (IOException ex){ System.out.println(ex);}
    }
}
