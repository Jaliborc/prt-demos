function pdt(target, transferFiles, bhvFile, numClusters, numjointCoefs, numTransferCoefs)
    % Builds a pose dependant transfer file.
    transfers = smartSvd(transfer(transferFiles));
    joints = smartSvd(readBvh(bhvFile));
    
    jointAverage = joints.M;
    jointBasis = pinv(joints.U);
    
    out = fopen(target, 'w');
    writeMatrix(out, jointAverage, 'float32');
    writeMatrix(out, jointBasis(1:numjointCoefs,:)', 'float32');
    
    fwrite(out, numClusters, 'int32');
    samples = zeros(1 + numjointCoefs + numTransferCoefs, size(transfers.O, 2));
    clusters = kmeans(transfers.V(:,1:3), numClusters);

    for i = 1:numClusters
        indexes = clusters == i;
        cluster = smartSvd(transfers.O(:, indexes));
        poses = joints.V(indexes,:);
        
        writeMatrix(out, cluster.M, 'int32');
        writeMatrix(out, cluster.U(:,1:numjointCoefs), 'int32');
        
        samples(1,indexes) = i;
        samples(2:end,indexes) = [cluster.V(:,1:numTransferCoefs) poses(:,1:numjointCoefs)]';
    end
    
    writeMatrix(out, samples, 'int32');
end