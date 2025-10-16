// Peformance Chart class. Takes in a ArrayList of Strings and a ArrayList of ArrayList<Double>
// Shows a chart of the performance data with box plots (min, max, median, p10, p90)

import java.awt.Color;
import java.awt.Graphics2D;
import java.util.ArrayList;
import java.awt.Font;
import javax.swing.JPanel;

public class PerfChart extends JPanel {

    public void paintSingleChart(
        Graphics2D g2d,
        String title,
        ArrayList<Double> data,
        int index,
        int size,
        double windowHeight
    ) {
        // set x based on index
        int x = index * size;

        // calculate the min, max, median, p10, p90
        double min = data.stream().min(Double::compare).orElse(0.0);
        double max = data.stream().max(Double::compare).orElse(0.0);
        double median = data.stream().sorted().skip(data.size() / 2).findFirst().orElse(0.0);
        double p10 = data.stream().sorted().skip((int) (data.size() * 0.1)).findFirst().orElse(0.0);
        double p90 = data.stream().sorted().skip((int) (data.size() * 0.9)).findFirst().orElse(0.0);

        // paint the min, max, median, p10, p90
        double minOffset = windowHeight - min;
        g2d.drawRect(x, (int) (minOffset - max), size, (int) max);
        g2d.drawLine(x, (int) (minOffset - p90), x+size, (int) (minOffset - p90));
        g2d.drawLine(x, (int) (minOffset - median), x+size, (int) (minOffset - median));
        g2d.drawLine(x, (int) (minOffset - p10), x+size, (int) (minOffset - p10));

        // Paint the labels
        // set font size
        g2d.setFont(new Font("Arial", Font.PLAIN, size / 5));
        g2d.drawString(title, x, (int) (windowHeight - max - size));

        return;
    }

    public void paintChart(
        Graphics2D g2d,
        ArrayList<String> titles,
        ArrayList<ArrayList<Double>> data,
        int size,
        double windowHeight
    ) {
        if (titles.size() != data.size()) {
            throw new IllegalArgumentException(
                "Titles and data must have the same size"
            );
        }

        // Paint the chart here
        for (int i = 0; i < titles.size(); i++) {
            paintSingleChart(g2d, titles.get(i), data.get(i), i, size, windowHeight);
        }
    }
}
