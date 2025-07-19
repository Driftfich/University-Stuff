class Rect implements Figure
{
    private double heigth;
    private double width;

    public Rect(double heigth, double width) {this.heigth = heigth; this.width = width;}

    @Override
    public double area() {return heigth * width;}

    @Override
    public double circumference() {return 2 * (this.heigth + this.width);}

    @Override
    public double getHeight() {return this.heigth;}

    @Override
    public double getWidth() {return this.width;}

    @Override
    public String toString() {return "Rect: " + this.heigth + "x" + this.width + "\n\tArea: " + this.area() + "\n\tCircumference: " + this.circumference();}
}