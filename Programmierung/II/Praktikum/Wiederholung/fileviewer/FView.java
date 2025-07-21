import java.awt.*;
import java.awt.event.*;
import java.io.*;

public class FView extends Panel {
    private Label flabel;
    private TextField inp;
    private Button show;
    private TextArea out;
    private Button q;
    private Panel toolbar;

    FView() {
        setLayout(new BorderLayout());

        flabel = new Label("File: ");
        inp = new TextField("", 50);
        show = new Button("show");
        out = new TextArea();
        q = new Button("quit");

        toolbar = new Panel(new BorderLayout());
        toolbar.add(flabel, BorderLayout.WEST);
        toolbar.add(inp, BorderLayout.CENTER);
        toolbar.add(show, BorderLayout.EAST);

        add(toolbar, BorderLayout.NORTH);
        add(out, BorderLayout.CENTER);
        add(q, BorderLayout.SOUTH);

        show.addActionListener(new ActionListener() {public void actionPerformed(ActionEvent e) {showper();}});
        q.addActionListener(new ActionListener() {public void actionPerformed(ActionEvent e) {quit(e);}});
        inp.addKeyListener(new KeyListener() {
            public void keyReleased(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_ENTER) showper();
            }
            public void keyPressed(KeyEvent e) {}
            public void keyTyped(KeyEvent e) {}
        });
    }

    public void quit(ActionEvent e) {
        System.exit(0);
    }

    public void showper() {
        String newOut = getContent(inp.getText());
        out.setText(newOut);
    }

    public String getContent(String path) {
        System.out.println("Grab content");
        File f = new File(path);
        //if (!f.exists()) return "File does not exist";
        if (f.isDirectory()) {
            String files = new String();
            for (String file: f.list()) {
                files += file + "\n";
            }
            return files;
        }
        else {
            try {
                FileReader reader = new FileReader(f);
                CharArrayWriter caw = new CharArrayWriter();

                byte c = 0;
                while (c != -1) {
                    c = (byte) reader.read();
                    caw.write(c);
                }

                return caw.toString();
            }
            catch (IOException e) {System.out.print("Error while reading the file '" + path + "'" + "Exception: " + e); return "Error while reading the file '" + path + "'";}
        }
    }

    public static void main(String[] args) {
        Frame F = new Frame("Fileviewer awt");
        FView P = new FView();

        F.addWindowListener(new WindowAdapter() {public void windowClosing(WindowEvent e) {System.exit(0);}});

        F.add(P);
        F.pack();
        F.setVisible(true);
    }
}
