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

    Objs = dir(fullfile(folder, '*.obj'));
    Obj = fullfile(folder, Objs(1).name);
    Maps = transferMaps(Obj, [Transfer.M Transfer.U], mapSize);
    NumHarmonics = size(Maps');
    
    fwrite(out, [1 NumHarmonics], 'int32');
    
    for c = 1:NumHarmonics
        totalSize = mapSize*4;
        image = zeros(totalSize, totalSize, 3);
        index = 1;
        
        for y = 1:mapSize:totalSize
            for x = 1:mapSize:totalSize
                vector = Maps{c}{index};
                minima = min(vector(:))
                scale = max(vector(:)) - minima
       
                fwrite(out, [minima scale], 'float32');
                image(x:x+mapSize-1, y:y+mapSize-1, :) = (vector - minima) / scale;
                index = index + 1;
            end
        end
           
       imwrite(image, strcat(folder, num2str(0), '_', num2str(c-1), '.png'), 'BitDepth', 16);
    end
   
    fclose(out);
end