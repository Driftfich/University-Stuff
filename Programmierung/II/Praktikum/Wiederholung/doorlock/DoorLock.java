import java.awt.*;
import java.awt.event.*;

public class DoorLock extends Panel {
    private TextArea info;
    private Panel inp;

    private ActionListener bttn_lis;

    private String key;
    private String uinp;

    DoorLock() {
        setLayout(new BorderLayout());
        info = new TextArea("Closed");
        inp = new Panel(new GridLayout(4,3));

        add(info, BorderLayout.NORTH);
        add(inp, BorderLayout.CENTER);

        bttn_lis = new ActionListener() {public void actionPerformed(ActionEvent e) {numPressed(e);}};

        for (int i=1; i<10; i++) {
            Button b = new Button(String.valueOf(i));
            b.addActionListener(bttn_lis);
            inp.add(b);
        }
        inp.add(new Label(" "));
        Button b = new Button("0");
        b.addActionListener(bttn_lis);
        inp.add(b);

        inp.add(new Label(" "));

        uinp = new String();
        key = new String();
    }

    public void numPressed(ActionEvent e) {
        if (uinp.length() == 5) {
            uinp = "";
        }
        uinp += e.getActionCommand();
        if (uinp.length() == 5) {
            if (uinp.equals(key)) {
                info.setText("Open");
            }
            else info.setText("Wrong password");
            return;
        }
        String asteriks = new String();
        for (int i=0; i< uinp.length(); i++) asteriks += "*";
        info.setText(asteriks);
        //System.out.println("Num Button pressed");
    }

    public void setKey(String k) {key = k;}

    public static void main(String[] args) {
        if (args.length != 1) {
            System.out.println("Usage: java DoorLock <5 number pin>");
            return;
        }

        Frame F = new Frame();
        DoorLock lock = new DoorLock();
        lock.setKey(args[0]);
        F.addWindowListener(new WindowAdapter() {public void windowClosing(WindowEvent e) {System.exit(0);}});
        F.add(lock);
        F.pack();
        F.setVisible(true);
    }


}
