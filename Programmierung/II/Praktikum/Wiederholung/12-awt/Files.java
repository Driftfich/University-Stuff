import java.io.*;
import java.awt.*;
import java.awt.event.*;

public class Files extends Panel {
    private Button show;
    private Button quit;
    private Label fLab;
    private TextArea out;
    private TextField inp;
    private Panel toolbar;

    Files() {
        show = new Button("Show");
        quit = new Button("Quit");
        fLab = new Label("Path: ");
        out = new TextArea();
        inp = new TextField("/");
        inp.setColumns(50);
        toolbar = new Panel();

        toolbar.add(fLab);
        toolbar.add(inp);
        toolbar.add(show);

        setLayout(new BorderLayout());
        this.add(toolbar, BorderLayout.NORTH);
        this.add(out, BorderLayout.CENTER);
        this.add(quit, BorderLayout.SOUTH);

        quit.addActionListener(new ActionListener() {public void actionPerformed(ActionEvent e) { quit();}});

        show.addActionListener(new ActionListener() {public void actionPerformed(ActionEvent e) { inpChanged(inp.getText());}});
    }

    public static void main(String[] args) {
        Frame f = new Frame("Filemanager");
        Files files = new Files();
        f.add(files);

        // f.addWindowListener(new WindowAdapter() {public void WindowClosing(WindowEvent e) {quit();}});
        f.addWindowListener(new WindowAdapter()
        {
            public void windowClosing(WindowEvent e)
            { quit();}
        });

        f.pack();
        f.setVisible(true);
    }

    public void inpChanged(String path) {
        out.setText(getContent(path));
        return;
    }

    public static String getContent(String path) {
        File f = new File(path);
        if (!f.exists()) {
            return "File or dir does not exist";
        }

        if (!f.isDirectory()) {
            try {
                FileReader fis = new FileReader(f);
                CharArrayWriter caw = new CharArrayWriter();
                
                byte b = 0;
                do {
                    b = (byte) fis.read();
                    caw.write(b);
                } while (b != -1);
                
                return caw.toString();
            }
            catch(FileNotFoundException e) {
                System.out.println("Error: file with name '" + path + "' does not exist");
                return "Error: file with name '" + path + "' does not exist";
            }
            catch(IOException e) {
                System.out.println("Error when trying to read file: " + path);
                return "Error when trying to read file: " + path;
            }
        }
        else {
            String s = new String("Available files:\n");
            for (String file: f.list()) {
                s += file + "\n";
            }
            return s;
        }
    }

    public static void quit() {
        System.exit(0);
    }


}