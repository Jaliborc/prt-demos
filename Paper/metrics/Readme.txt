"Transfer/Poses PCA Diagonal" shows the singular values resulting of the incremental singular decomposition of the transfer/poses. It is visible that poses can be represented with less components than the transfer (and hence the reason for using 15 components for the transfer). Remember that the average was subtracted before the incremental SVD was performed.

"Transfer 15 to 30 samples comparison" proves that it's not necessary to compute the transfer for every recorded pose to achieve good results. The transfer principal components interpolate just fine skipping half of the samples.

"Samples in Pose/Transfer Space" show the samples scattered according to the first 3 principal components of the pose or transfer space.

"First/Second/Third Transfer Component Depending on Pose Components" shows how each of these transfer components changes depending on the pose PCA analysis using the RBF function.

"Transfer Kmeans Clusters Displayed on Pose PCA Space" show that the transfer functions are closely related to the hand pose. In this graph, the  samples are scattered according to the first 3 principal components of the pose space, but colored according to a kmeans clustering of the transfer functions.