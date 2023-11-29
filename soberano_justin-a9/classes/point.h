#ifndef POINT_H
#define POINT_H

class Point {
    private:
        int x;
        int y;
    
    public:
        Point() : x(0), y(0) {};
        Point(int x, int y) : x(x), y(y) {};

        int &get_position_x() { return x; }
        int &get_position_y() { return y; }

        void set_position_x(int x) { this -> x = x; }
        void set_position_y(int y) { this -> y = y; }
};

#endif 