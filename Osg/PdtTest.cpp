#include "Pdt.h"
#include <stdio.h>

int main() {
	Pdt bananas("../Captures/generated/100 paper poses.pdt");

	printf("%dx%d\n", bananas.jointAverage.n_rows, bananas.jointAverage.n_cols);
	printf("%dx%d\n", bananas.jointBasis.n_rows, bananas.jointBasis.n_cols);
	printf("%dx%d\n", bananas.sampleJoints.n_rows, bananas.sampleJoints.n_cols);

	printf("%dx%d\n", bananas.clusters.n_rows, bananas.clusters.n_cols);
	printf("%dx%d\n", bananas.sampleTransfer.n_rows, bananas.sampleTransfer.n_cols);

	return 0;
}