function pgv(folder, numComponents)
%Creates a pgv file from a set of visibility and obj files in a given folder.
    out = fopen(strcat(folder, '.pgv'), 'w');

    [Poses, Faces] = geometry(fullfile(folder, '*.obj'));
    [M, V, U] = isvd(Poses, numComponents);
    
    write(out, M); write(out, V); write(out, U);
    write(out, Faces, 'int32');
    
    [Poses] = transfer(fullfile(folder, '*.visibility'));
    [M, V, U] = isvd(Poses, numComponents);
    
    write(out, M); write(out, V); write(out, U);
    
    [UV] = coords(fullfile(folder, '*.obj'));
    write(out, UV);
    
    fclose(out);
end