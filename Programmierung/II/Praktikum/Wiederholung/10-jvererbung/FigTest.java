

public class FigTest {
    public static void main(String[] args) {
        Rect r = new Rect(10, 20);
        Figure[] figs = {r};
        for (Figure fig : figs) {
            System.out.println(fig);
        }
    }
}