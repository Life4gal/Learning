interface Shape{
    double getGirth();
    double getArea();
}

interface Square{
    double getBorder();
    void setBorder(double border);
}

class Graph implements Shape, Square{
    private double border;

    Graph() {
    }

    Graph(double border) {
        this.border = border;
    }
    @Override
    public double getBorder() {
        return border;
    }
    @Override
    public void setBorder(double border) {
        this.border = border;
    }
    @Override
    public double getArea(){
        return Math.pow(border, 2);
    }
    @Override
    public double getGirth(){
        return border * 4;
    }
}

class GraphSquare extends Graph{
    GraphSquare() {
    }

    GraphSquare(double border) {
        super(border);
    }
}

public class Interface {
    public static void main(String[] args) {
        GraphSquare square = new GraphSquare(5.0);
        System.out.println("\nBorder = " + square.getBorder() + "\nArea = " + square.getArea() + "\nGirth = " + square.getGirth());
    }
}
