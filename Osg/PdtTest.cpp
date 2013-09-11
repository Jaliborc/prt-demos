#include "Pdt.h"
#include <stdio.h>

int main() {
	Pdt bananas("../Captures/generated/100 paper poses.pdt");

	printf("%dx%d\n", bananas.jointAverage.n_rows, bananas.jointAverage.n_cols);
	printf("%dx%d\n", bananas.jointBasis.n_rows, bananas.jointBasis.n_cols);
	printf("%dx%d\n", bananas.sampleJoints.n_rows, bananas.sampleJoints.n_cols);

	printf("%dx%d\n", bananas.clusters.n_rows, bananas.clusters.n_cols);
	printf("%dx%d\n", bananas.sampleTransfer.n_rows, bananas.sampleTransfer.n_cols);

	/*fmat joints;
	joints << 0.59f << 7.62f << -4.43f << 5.28f << 1.04f << -49.49f << -14.01f << -5.42f << 20.74f << 0.00f <<  0.00f <<  2.64f <<  0.00f <<  6.09f <<  -10.21f <<  0.00f <<  0.62f <<  0.00f << 0.00f <<  0.00f <<  0.00f <<  0.00f <<  -4.37f <<  -0.27f <<  0.00f <<  1.67f <<  0.00f <<  0.00f <<  0.00f <<  0.00f <<  0.00f << -6.88 << 1.80f <<  0.00f <<  1.04f <<  0.00f <<  0.00f <<  0.51 <<  0.00f <<  0.00f <<  -1.59f<<7.33f <<  0.00f <<  2.13f <<  0.00f <<  0.00f <<  3.16f <<  0.00f << endr;
	joints = joints.t();

	fmat transfer = bananas.getTransfer(joints);
	printf("%dx%d: %f\n", transfer.n_rows, transfer.n_cols, sum(sum(transfer)));

	for (int i = 0; i < 10; i++) {
		transfer = bananas.getTransfer(i);
		printf("%dx%d: %f\n", transfer.n_rows, transfer.n_cols, sum(sum(transfer)));
	}*/

	return 0;
}