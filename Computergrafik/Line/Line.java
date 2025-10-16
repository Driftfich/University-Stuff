import java.awt.Color;
import java.awt.Graphics2D;
import java.util.ArrayList;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class Line extends JPanel {

    private void setPixel(Graphics2D g, int x, int y, int color) {
        g.setColor(new Color(color));
        g.drawRect(x, y, 1, 1);
    }

    private void setPixel(Graphics2D g, double x, double y, int color) {
        int xint = (int) (x + 0.5);
        int yint = (int) (y + 0.5);

        setPixel(g, xint, yint, color);

        return;
    }

    public void draw1(Graphics2D g, int x1, int y1, int x2, int y2, int color) {
        int dx = x2 - x1;
        int dy = y2 - y1;

        double step = 1.0 / Math.sqrt(dx * dx * dy * dy);

        for (double t = 0.0; t < 1.0; t += step) {
            setPixel(g, x1 + t * dx, y1 + t * dy, color);
        }

        setPixel(g, x2, y2, color);

        return;
    }

    public void draw2(Graphics2D g, int x1, int y1, int x2, int y2, int color) {
        int dx = x2 - x1;
        int dy = y2 - y1;

        double y;
        double m = (double) dy / (double) dx;

        for (int x = x1; x <= x2; x++) {
            y = y1 + (x - x1) * m;
            setPixel(g, (double) x, y, color);
        }

        setPixel(g, x2, y2, color);

        return;
    }

    public void drawBresenham(
        Graphics2D g,
        int x1,
        int y1,
        int x2,
        int y2,
        int color
    ) {
        int dx = x2 - x1;
        int dy = y2 - y1;

        double m = (double) dy / (double) dx;
        double err = 0.0;

        int y = y1;

        for (int x = x1; x <= x2; x++) {
            setPixel(g, x, y, color);

            err += m;
            if (err >= 0.5) {
                y++;
                err -= 1.0;
            }
        }

        return;
    }

    public void drawBresenham2(
        Graphics2D g,
        int x1,
        int y1,
        int x2,
        int y2,
        int color
    ) {
        int dx = x2 - x1;
        int dy = y2 - y1;
        int m = 2 * dy;
        int err = 0;
        int y = y1;

        for (int x = x1; x <= x2; x++) {
            setPixel(g, x, y, color);

            err += m;
            if (err >= dx) {
                y++;
                err -= 2 * dx;
            }
        }

        return;
    }

    public void drawBresenham3(
        Graphics2D g,
        int x1,
        int y1,
        int x2,
        int y2,
        int color
    ) {
        int dx = x2 - x1;
        int dy = y2 - y1;
        int m = 2 * dy;
        int err = -dx;
        int schritt = 2 * dx;
        int y = y1;

        for (int x = x1; x <= x2; x++) {
            setPixel(g, x, y, color);

            err += m;
            if (err >= 0) {
                y++;
                err -= schritt;
            }
        }

        return;
    }

    public static void main(String[] args) {
        JFrame frame = new JFrame("Live Line View");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        Line line = new Line();
        PerfChart perfChart = new PerfChart();
        frame.add(line);

        frame.setSize(900, 600);
        frame.setVisible(true);
        frame.setLocationRelativeTo(null);
        frame.setResizable(false);

        // hold a list of times
        ArrayList<Double> v1t = new ArrayList<Double>(100);
        ArrayList<Double> v2t = new ArrayList<Double>(100);
        ArrayList<Double> v3t = new ArrayList<Double>(100);
        ArrayList<Double> v4t = new ArrayList<Double>(100);
        ArrayList<Double> v5t = new ArrayList<Double>(100);

        // create random color
        Color color;
        for (int i = 0; i < 1000; i++) {
            color = new Color((int) (Math.random() * 0xFFFFFF));

            // stop time
            double t1 = System.currentTimeMillis();
            line.draw1(
                (Graphics2D) frame.getGraphics(),
                0,
                100,
                600,
                500,
                color.getRGB()
            );
            double t2 = System.currentTimeMillis();
            v1t.add(t2 - t1);
            line.draw2(
                (Graphics2D) frame.getGraphics(),
                100,
                100,
                700,
                500,
                color.getRGB()
            );
            double t3 = System.currentTimeMillis();
            v2t.add(t3 - t2);
            line.drawBresenham(
                (Graphics2D) frame.getGraphics(),
                200,
                100,
                800,
                500,
                color.getRGB()
            );
            double t4 = System.currentTimeMillis();
            v3t.add(t4 - t3);
            line.drawBresenham2(
                (Graphics2D) frame.getGraphics(),
                300,
                100,
                900,
                500,
                color.getRGB()
            );
            double t5 = System.currentTimeMillis();
            v4t.add(t5 - t4);
            line.drawBresenham3(
                (Graphics2D) frame.getGraphics(),
                400,
                100,
                1000,
                500,
                color.getRGB()
            );
            double t6 = System.currentTimeMillis();
            v5t.add(t6 - t5);

            // Sleep for a short duration to slow down the animation
            // try {
            //     Thread.sleep(1);
            // } catch (InterruptedException e) {
            //     e.printStackTrace();
            // }
        }

        // // calculate average times
        // double v1avg = v1t
        //     .stream()
        //     .mapToDouble(Double::doubleValue)
        //     .average()
        //     .orElse(0);
        // double v2avg = v2t
        //     .stream()
        //     .mapToDouble(Double::doubleValue)
        //     .average()
        //     .orElse(0);
        // double v3avg = v3t
        //     .stream()
        //     .mapToDouble(Double::doubleValue)
        //     .average()
        //     .orElse(0);
        // double v4avg = v4t
        //     .stream()
        //     .mapToDouble(Double::doubleValue)
        //     .average()
        //     .orElse(0);
        // double v5avg = v5t
        //     .stream()
        //     .mapToDouble(Double::doubleValue)
        //     .average()
        //     .orElse(0);

        // System.out.println("Average time for draw1: " + v1avg + " ms");
        // System.out.println("Average time for draw2: " + v2avg + " ms");
        // System.out.println("Average time for draw3: " + v3avg + " ms");
        // System.out.println("Average time for draw4: " + v4avg + " ms");
        // System.out.println("Average time for draw5: " + v5avg + " ms");
        //

        try {
            Thread.sleep(20);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        // clear the frame
        frame.setBackground(Color.WHITE);

        frame.add(perfChart);
        ArrayList<String> labels = new ArrayList<String>();
        labels.add("Draw1");
        labels.add("Draw2");
        labels.add("Bresenham 1");
        labels.add("Bresenham 2");
        labels.add("Bresenham 3");
        ArrayList<ArrayList<Double>> data = new ArrayList<ArrayList<Double>>();
        data.add(v1t);
        data.add(v2t);
        data.add(v3t);
        data.add(v4t);
        data.add(v5t);
        perfChart.paintChart(
            (Graphics2D) frame.getGraphics(),
            labels,
            data,
            130,
            frame.getHeight()
        );
    }
}
