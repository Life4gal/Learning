class Circle{
    private double radius;

    Circle() {
    }

    Circle(double radius) {
        this.radius = radius;
    }

    double getRadius() {
        return radius;
    }

    void setRadius(double radius) {
        this.radius = radius;
    }

    double getArea(){
        return Math.PI * radius * radius;
    }

    double getGirth(){
        return Math.PI * radius * 2;
    }
}

public class Class {
    public static void main(String[] args) {
        Circle circle1 = new Circle();
        circle1.setRadius(5.1);
        System.out.println("\nRadius = " + circle1.getRadius() + "\nArea = " + circle1.getArea() + "\nGirth = " + circle1.getGirth());
        Circle circle2 = new Circle(6.8);
        System.out.println("\nRadius = " + circle2.getRadius() + "\nArea = " + circle2.getArea() + "\nGirth = " + circle2.getGirth());
    }
}
