import java.awt.*;
import java.awt.event.*;
import java.awt.font.*;

class calc2 extends Panel {
    private TextArea out;
    private Panel inp;

    private String[] labels = {"M+", "7", "8", "9", "/",
                            "M-", "4", "5", "6", "*",
                            "MR", "1", "2", "3", "-",
                            "CE", "0", ".", "=", "+"};

    private ActionListener n1 = new ActionListener() {public void actionPerformed(ActionEvent e) {num(e);}};
    private ActionListener o1 = new ActionListener() {public void actionPerformed(ActionEvent e) {op(e);}};
    private ActionListener d1 = new ActionListener() {public void actionPerformed(ActionEvent e) {dot(e);}};
    private ActionListener m1 = new ActionListener() {public void actionPerformed(ActionEvent e) {m(e);}};
    private ActionListener ce1 = new ActionListener() {public void actionPerformed(ActionEvent e) {ce(e);}};

    private ActionListener[] listener = {m1, n1, n1, n1, o1,
                                         m1, n1, n1, n1, o1,
                                         m1, n1, n1, n1, o1,
                                         ce1, n1, d1, o1, o1,};

    calc2() {
        setFont(new Font("Arial", Font.PLAIN, 22));
        setLayout(new BorderLayout());

        out = new TextArea();
        inp = new Panel(new GridLayout(4,5));

        add(out, BorderLayout.NORTH);
        add(inp, BorderLayout.CENTER);

        for (int i=0; i<labels.length; i++) {
            Button b = new Button(labels[i]);
            b.addActionListener(listener[i]);
            inp.add(b);
        }
    }

    public static void main(String[] args) {
        Frame F = new Frame();
        F.addWindowListener(new WindowAdapter() {public void windowClosing(WindowEvent e) {System.exit(0);}});
        calc2 P = new calc2();
        F.add(P);
        F.pack();
        F.setVisible(true);
    }

    public void num(ActionEvent e) {
        System.out.println("Num clicked");
    }

    public void op(ActionEvent e) {

    }

    public void dot(ActionEvent e) {
        
    }

    public void m(ActionEvent e) {

    }

    public void ce(ActionEvent e) {

    }
}