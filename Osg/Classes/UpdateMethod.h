#ifndef UPDATE_METHOD_H
#define UPDATE_METHOD_H value
#include <armadillo>

struct UpdateMethod {
	virtual void updatePose(arma::fmat joints) = 0;
};

#endif