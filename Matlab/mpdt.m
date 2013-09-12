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
    jointBasis = joints.U(:,1:numjointCoefs)';
    sampleJoints = joints.V(:,1:numjointCoefs)';
    
    out = fopen(strcat(target, '.mpdt'), 'w');
    writeMatrix(out, jointAverage, 'float32');
    writeMatrix(out, jointBasis, 'float32');
    writeMatrix(out, sampleJoints, 'float32');
    
    sampleTransfer = zeros(numTransferCoefs, size(transfers.O, 2));
    clusters = kmeans(transfers.V(:,1:3), numClusters);
    figure; hist(clusters, unique(clusters));

    for i = 1:numClusters
        indexes = clusters == i;
        cluster = smartSvd(transfers.O(:, indexes));
        maps = transferMaps(objFile, [cluster.M cluster.U(:,1:numTransferCoefs)], mapsSize, 4);
        numMaps = size(maps);
        
        for c = 1:numMaps
           limit = mapsSize * 2;
           half = mapsSize + 1;
           
           image = zeros(limit, limit, 3);
           image(1:mapsSize, 1:mapsSize, :) = maps{c}{1};
           image(half:limit, 1:mapsSize, :) = maps{c}{2};
           image(1:mapsSize, half:limit, :) = maps{c}{3};
           image(half:limit, half:limit, :) = maps{c}{4};
           
           hdrwrite(image, strcat(target, num2str(i-1), '_', num2str(c-1), '.hdr'));
        end
        
        sampleTransfer(:,indexes) = cluster.V(:,1:numjointCoefs)';
    end
    
    writeMatrix(out, sampleTransfer, 'float32');
    writeMatrix(out, clusters - 1, 'int32');
    
    fwrite(out, [numClusters numMaps], 'int32');
end