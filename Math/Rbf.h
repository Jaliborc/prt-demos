#include <armadillo>
using namespace arma;

// C = Xc'
void RunRBF(fmat* input, fmat& C, fmat& W,  int k) {
	fmat phi = fmat(1, C.n_cols);

	for (int i=0; i < C.n_cols; i++) {
		if (k) {
			float r = norm(*input - C.col(i), 2);
			
			if (r) {
				phi(i) = pow(r, k);
			
				if (!(k % 2))
					phi(i) *= log(r);
			} else
				phi(i) = 0;
		} else
			phi(i) = 1;
	}
	
	(*input) = phi * W;
}