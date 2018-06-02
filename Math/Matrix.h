#ifndef MATH_MATRIX_H
#define MATH_MATRIX_H 1
#include <armadillo>
#include <sstream>
#include <fstream>

using namespace std;
using namespace arma;

template<class type>
void readMatrix(Mat<type>* target, ifstream& source) {
	int size[2];
	source.read((char*) &size, sizeof(int) * 2);

	int total = size[0] * size[1];
	type* content = new type[total];
	source.read((char*) content, sizeof(type) * total);

	*target = Mat<type>(content, size[0], size[1]);
	delete content;
}

#endif