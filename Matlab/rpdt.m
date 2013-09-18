function rpdt(folder, numJointCoefs)
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
    
    [Maps, Minima] = transferMaps(Obj, [Transfer.M Transfer.U], 1024);
    NumMaps = size(Maps);
    
    for c = 1:NumMaps
        image = zeros(4096, 4096, 3);
        index = 1;
        
        for y = 1:1024:4096
            for x = 1:1024:4096
                image(x:x+1023, y:y+1023, :) = Maps{c}{index};
                index = index + 1;
            end
        end
           
       imwrite(image - Minima, strcat(folder, 0, '_', num2str(c-1), '.png'));
    end
    
    fwrite(out, Minima, 'float32');
    fwrite(out, [1 NumMaps], 'int32');
    fclose(out);
end