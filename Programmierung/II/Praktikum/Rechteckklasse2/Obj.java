public abstract class Obj {
    private Integer x;
    private Integer y;

    Obj(Integer x, Integer y) {
        setX(x);
        setY(y);
    }

    public void setX(Integer x) {
        this.x = x;
    }

    public void setY(Integer y) {
        this.y = y;
    }

    public Integer getX() {
        return x;
    }

    public Integer getY() {
        return y;
    }

    public Integer getArea() {
        return 0;
    }

    public Integer getCircumreference() {
        return 0;
    }

    public String toString() {
        return "Obj(" + x + ", " + y + ")";
    }
}
