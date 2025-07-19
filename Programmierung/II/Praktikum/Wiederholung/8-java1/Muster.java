import java.awt.*;
import java.awt.event.*;

class Muster extends Panel
{
    Button ok;

    public Muster()
    {
        ok = new Button("ok");
        this.add(ok);
    }

    public static void main(String args[]) {
        Frame F = new Frame();
        Muster P = new Muster();
        F.add(P);
        F.pack();
        F.setVisible(true);
    }
}