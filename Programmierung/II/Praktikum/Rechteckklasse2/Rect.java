
public class Rect extends Obj {
    private Integer sizeX;
    private Integer sizeY;

    Rect(Integer sizeX, Integer sizeY, Integer x, Integer y) {
        super(x,y); 
        setSizeX(sizeX);
        setSizeY(sizeY);
    }

    public void setSizeX(Integer sizeX) {
        this.sizeX = sizeX;
    }

    public void setSizeY(Integer sizeY) {
        this.sizeY = sizeY;
    }

    @Override
    public Integer getArea() {
        return sizeX * sizeY;
    }

    @Override
    public Integer getCircumreference() {
        return 2 * (sizeX + sizeY);
    }
}
