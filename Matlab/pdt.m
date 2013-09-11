function pdt(target, transferFiles, bhvFile, numClusters, numjointCoefs, numTransferCoefs)
    % Builds a pose dependant transfer file.
    transfers = smartSvd(transfer(transferFiles));
    angles = readBvh(bhvFile);
    angles = angles(4:end, 1:size(transfers.O, 2));
    joints = [];
    
    for i = 1:3:size(angles)
       joint = SpinCalc('EA123toQ', angles(i:i+2,:)', 0, 0);
       
       for k = 1:size(joint)
          if joint(k,4) < 0
              joint(k,:) = joint(k,:) * -1;
          end
       end
       
       joints = [joints; joint(:,4)'; joint(:,1:3)'];
    end
    
    joints = smartSvd(joints);
    jointAverage = joints.M;
    jointBasis = joints.U';
    sampleJoints = joints.V(:,1:numTransferCoefs)';
    
    out = fopen(target, 'w');
    writeMatrix(out, jointAverage, 'float32');
    writeMatrix(out, jointBasis(1:numjointCoefs,:), 'float32');
    writeMatrix(out, sampleJoints, 'float32');
    
    fwrite(out, numClusters, 'int32');
    sampleTransfer = zeros(numTransferCoefs, size(transfers.O, 2));
    clusters = kmeans(transfers.V(:,1:3), numClusters);
    figure; hist(clusters, unique(clusters));

    for i = 1:numClusters
        indexes = clusters == i;
        cluster = smartSvd(transfers.O(:, indexes));
        
        writeMatrix(out, cluster.M, 'float32');
        writeMatrix(out, cluster.U(:,1:numjointCoefs), 'float32');

        sampleTransfer(:,indexes) = cluster.V(:,1:numjointCoefs)';
    end
    
    writeMatrix(out, clusters - 1, 'int32');
    writeMatrix(out, sampleTransfer, 'float32');
end