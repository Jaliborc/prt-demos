function pdt(folder, numJointCoefs, mapSize)
    Joints = readBvhQuaternions(strcat(folder, '.bvh'));
    Joints = svdStruct(Joints, numJointCoefs);
    
    out = fopen(strcat(folder, '.pdt'), 'w');
    writeMatrix(out, Joints.M, 'float32');
    writeMatrix(out, Joints.U', 'float32');
    writeMatrix(out, Joints.V, 'float32');
    
    Transfer = transfer(fullfile(folder, '*.transfer'));
    Transfer = svdStruct(Transfer, 15);
    W = train_rbf(Joints.V', Transfer.V', Joints.V', 2, 'polyharmonicspline');
    
    writeMatrix(out, W, 'float32');
    writeMatrix(out, ones(1, size(Transfer)), 'int32');

    MapData = prepare_transferMaps(fullfile(folder, '*.obj'), [Transfer.M Transfer.U]);
    fwrite(out, [1 MapData.numCoefs], 'int32');
    
    for c = 1:MapData.numCoefs
        actualSize = mapSize/4;
        maps = make_transferMaps(MapData, c, actualSize);
        
        image = zeros(mapSize, mapSize, 3);
        index = 1;
        
        for y = 1:actualSize:mapSize
            for x = 1:actualSize:mapSize
                vector = maps{index};
                minima = min(vector(:));
                scale = max(vector(:)) - minima;
       
                fwrite(out, [minima scale], 'float32');
                image(x:x+actualSize-1, y:y+actualSize-1, :) = (vector - minima) / scale;
                index = index + 1;
            end
        end
           
       imwrite(image, strcat(folder, num2str(0), '_', num2str(c-1), '.jpg'));
    end
   
    fclose(out);
end