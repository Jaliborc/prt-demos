function mpdt(target, objFile, bhvFile, transferFiles, numClusters, numjointCoefs, numTransferCoefs, mapsSize)
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
    
    out = fopen(strcat(target, '.mpdt'), 'w');
    writeMatrix(out, jointAverage, 'float32');
    writeMatrix(out, jointBasis(1:numjointCoefs,:), 'float32');
    writeMatrix(out, sampleJoints, 'float32');
    
    sampleTransfer = zeros(numTransferCoefs, size(transfers.O, 2));
    clusters = kmeans(transfers.V(:,1:3), numClusters);
    figure; hist(clusters, unique(clusters));
    
    fwrite(out, numClusters, 'int32');
    fwrite(out, numTransferCoefs, 'int32');

    for i = 1:numClusters
        indexes = clusters == i;
        cluster = smartSvd(transfers.O(:, indexes));
        maps = transferMaps(objFile, [cluster.M cluster.U(:,1:numjointCoefs)], mapsSize, 4);
        
        for c = 1:numTransferCoefs
           limit = mapsSize * 2;
           half = mapsSize + 1;
           
           image = zeros(limit, limit, 3);
           image(1:mapsSize, 1:mapsSize, :) = maps{c}{1};
           image(half:limit, 1:mapsSize, :) = maps{c}{2};
           image(1:mapsSize, half:limit, :) = maps{c}{3};
           image(half:limit, half:limit, :) = maps{c}{4};
           
           hdrwrite(image, strcat(target, num2str(i), '_', num2str(c), '.hdr'));
        end
        
        sampleTransfer(:,indexes) = cluster.V(:,1:numjointCoefs)';
    end
    
    writeMatrix(out, clusters - 1, 'int32');
    writeMatrix(out, sampleTransfer, 'float32');
end