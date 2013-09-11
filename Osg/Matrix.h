#include <osg/Vec2i>
#include <fstream>

using namespace osg;
using namespace std;

template <class Type>
struct Matrix {
    Matrix() {}
    Matrix(ifstream& stream) {
         stream.read((char*) &size, sizeof(Vec2i));
         values = new Type[area()];
         stream.read((char*) values, sizeof(Type) * area());
    }

    Type operator[] (int i) {
        return values[i];
    }

    Type operator() (int x, int y) {
        return y * width() + x;
    }

    Type* operator() (Type* other) {
        Type result = new Type[height()];
        for (int y = 0; y < height(); y++)
            result[y] = 0;

        for (int i = 0; i < area(); i++)
            result[i % width()] += this[i] * other[i];

        return result;
    }

    int area() {return width() * height();}
    int height() {return size.y();}
    int width() {return size.x();}

    Type *values;
    Vec2i size;
};

typedef Matrix<float> Matrixf;
typedef Matrix<int> Matrixi;