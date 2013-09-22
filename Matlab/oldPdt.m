function pdt(folder, numClusterRange, numJointCoefs, numTransferCoefs, mapsSize)
    Joints = readBvhQuaternions(strcat(folder, '.bvh'));
    Joints = svdStruct(Joints, numJointCoefs);
    
    out = fopen(strcat(folder, '.pdt'), 'w');
    writeMatrix(out, Joints.M, 'float32');
    writeMatrix(out, Joints.U', 'float32');
    writeMatrix(out, Joints.V, 'float32');
    
    [Transfer] = transfer(fullfile(folder, '*.transfer'));
    [Transfer] = svdStruct(Transfer, 15);
    [Clusters, NumClusters] = recursiveClusters(Transfer.V', numClusterRange, 50);
    
    Objs = dir(fullfile(folder, '*.obj'));
    Obj = fullfile(folder, Objs(1).name);
    
    SampleTransfer = [];
    Minima = 0; Maxima = 0;
    Maps = {};

    for i = 1:NumClusters
        Poses = Clusters == i;
        Cluster = svdStruct(Transfer.O(:, Poses), numTransferCoefs);
        
        Maps{i} = transferMaps(Obj, [Cluster.M Cluster.U], mapsSize);
        SampleTransfer(:,Poses) = Cluster.V;
        NumMaps = size(Maps{i});
        
        for a = 1:NumMaps
            for b = 1:size(Maps{i}{a}')
               Minima = min(Minima, min(min(min(Maps{i}{a}{b}))));
               Maxima = max(Maxima, max(max(max(Maps{i}{a}{b}))));
            end
        end
    end
    
    for i = 1:NumClusters
        for c = 1:NumMaps
           limit = mapsSize * 2;
           half = mapsSize + 1;
           
           image = zeros(limit, limit, 3);
           image(1:mapsSize, 1:mapsSize, :) = Maps{i}{c}{1};
           image(half:limit, 1:mapsSize, :) = Maps{i}{c}{2};
           image(1:mapsSize, half:limit, :) = Maps{i}{c}{3};
           image(half:limit, half:limit, :) = Maps{i}{c}{4};
           
           imwrite(image - Minima, strcat(folder, num2str(i-1), '_', num2str(c-1), '.png'));
        end
    end
    
    writeMatrix(out, SampleTransfer, 'float32');
    writeMatrix(out, Clusters - 1, 'int32');
    
    fwrite(out, Minima, 'float32');
    fwrite(out, [NumClusters NumMaps], 'int32');
    fclose(out);
end