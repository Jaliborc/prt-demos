function [V, U, keys] = clusteredTransfer(path, numComponents, clusterOff)
%Performs an incremental clustering and singular value decomposition on a set of transfer files.
    files = openFiles(path);
    for i = 1:size(files)
        numHarmonics = fread(files(i), 1, 'int32');
    end
    
    height = numHarmonics * 3;
    clusters = [];
    keys = [];
    count = 1;
    
    while count > 0
        transfer = [];
        
        for i = 1:size(files)
            [newTransfer, count] = fread(files(i), [height, 6000], 'float32');
            transfer = [transfer; newTransfer];
        end
        
        [newClusters, newKeys] = MeanShiftCluster(transfer, clusterOff);
        clusters = [clusters newClusters];
        keys = [keys newKeys];
    end
    
    fclose('all');
    [clusters, clusterKeys] = MeanShiftCluster(clusters, clusterOff * .9);
    keys = clusterKeys(keys);
    poses = [];

    while size(clusters, 1) > 0
        pose = clusters(1:height,:);
        clusters(1:height,:) = [];
        poses = [poses pose(:)]; 
    end
    
    [V, S, U] = seqkl(poses, numComponents);
    U = S * U';
end