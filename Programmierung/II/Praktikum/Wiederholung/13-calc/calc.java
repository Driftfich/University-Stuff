import java.awt.*;
import java.awt.event.*;
import java.awt.font.*;

class calc extends Panel {
    private TextArea out;
    private Panel inp;

    private String[] labels = {"M+", "7", "8", "9", "/",
                            "M-", "4", "5", "6", "*",
                            "MR", "1", "2", "3", "-",
                            "CE", "0", ".", "=", "+"};

    calc() {
        setFont(new Font("Arial", Font.PLAIN, 22));
        setLayout(new BorderLayout());

        out = new TextArea();
        inp = new Panel(new GridLayout(4,5));

        add(out, BorderLayout.NORTH);
        add(inp, BorderLayout.CENTER);

        for (int i=0; i<labels.length; i++) {
            Button b = new Button(labels[i]);
            inp.add(b);
        }
    }

    public static void main(String[] args) {
        Frame F = new Frame();
        F.addWindowListener(new WindowAdapter() {public void windowClosing(WindowEvent e) {System.exit(0);}});
        calc P = new calc();
        F.add(P);
        F.pack();
        F.setVisible(true);
    }
}