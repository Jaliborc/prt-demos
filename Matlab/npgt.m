function npgt(folder, numComponents)
%Creates a "Precomputed Geometry and Transfer" file in native format from a set of .obj and .tranfer files in a given folder.
    file = fopen(strcat(folder, '.npgt'), 'w');

    [Poses, Faces] = geometry(fullfile(folder, '*.obj'));
    [M, V, U] = isvd(Poses, numComponents);
    
    fwrite(file, [size(V) size(U, 2)], 'int32');
    fwrite(file, M, 'float32');
    fwrite(file, V, 'float32');
    fwrite(file, U, 'float32');
    
    [Poses] = transfer(fullfile(folder, '*.transfer'));
    [M, V, U] = isvd(Poses, numComponents);
    
    fwrite(file, [size(V) size(U, 2)], 'int32');
    fwrite(file, M, 'float32');
    fwrite(file, V, 'float32');
    fwrite(file, U, 'float32');
    
    fwrite(file, size(Faces, 1), 'int32');
    fwrite(file, Faces - 1, 'int32');
    fclose(file);
end