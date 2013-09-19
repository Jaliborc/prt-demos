#include <math.h>
#include <osg/Array>

#define MAX_SH_ORDER 3
#define MAX_SH_COEF MAX_SH_ORDER * MAX_SH_ORDER
#define SH_SCALE osg::PI * 16.0f/17.0f
#include "ShLight.h"
#include "ShEval.h"

using namespace osg;
using namespace std;

float toGrayscale(Vec3& color) {
	return color.x() * 0.3f + color.y() * 0.59f + color.z() * 0.11f;
}

Vec3 dominantSHDirection(Vec3Array* coefs) {
	Vec3 direction = Vec3(-toGrayscale(coefs->at(3)), toGrayscale(coefs->at(1)), -toGrayscale(coefs->at(2)));
	direction.normalize();

	return direction;
}

Vec3 dominantSHColor(Vec3Array* coefs, Vec3& w) {
	float y[MAX_SH_COEF];
	shEval(w.x(), -w.y(), -w.z(), y);

	Vec3 color;
	float scale = 0;

	for (int i = 1; i <= 3; i++) {
		float t = y[i] * SH_SCALE;

		color += coefs->at(i) * t;
		scale += t * t;
	}

	color /= scale;

	for (int i = 0; i < MAX_SH_COEF; i++)
		coefs->at(i) -= color * y[i] * SH_SCALE;

	return color;
} 

Vec3Array* RotateSH(Vec3 angle, Vec3Array* coefs) {
	ShWigner D = shNewWigner(3);
	shEvalWignerYZ(D, angle.y(), angle.z(), 3);
    shPreMultiplyByRZ(D, angle.x(), 3);

    Vec3Array *out = new Vec3Array(coefs->size());

	for (int l = 0; l <= 3; l++) {
        for (int m = -l; m <= l; m++) {
			int index = l * (l+1) + m;
			
            for (int n = -l; n <= l; n++)
                out->at(index) += coefs->at(l * (l+1) + n) * D[l][m][n];
        }
	}

	return out;
}