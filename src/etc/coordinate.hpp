#ifndef COORDINATE
#define COORDINATE

#include <cmath>

class Coordinate {
private:
    int x;
    int y;

public:
    Coordinate() : Coordinate(0, 0) { }
    Coordinate(int x, int y) {
        this->x = x;
        this->y = y;
    }
    Coordinate(const Coordinate& others) {
        this->x = others.getX();
        this->y = others.getY();
    }

    int getX() const { return x; }
    int getY() const { return y; }

    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }

    void move(int dx, int dy) { this->x += dx; this->y += dy; }

    friend std::ostream &operator<<(std::ostream &output, const Coordinate& coordinate) {
        output << "(" << coordinate.getX() << ", " << coordinate.getY() << ")";
        return output;
    }

    Coordinate* transform(double scaleFactor, double rotation, Coordinate* center) {
        double cx = center->getX();
        double cy = center->getY();
        double x = ((double) this->x - cx) * 1.0 * scaleFactor + cx;
        double y = ((double) this->y - cy) * 1.0 * scaleFactor + cy;
        int newX = cos(rotation) * (x - cx) - sin(rotation) * (y - cy) + cx;
        int newY = sin(rotation) * (x - cx) + cos(rotation) * (y - cy) + cy;
        return new Coordinate(newX, newY);
    }
};

#endif