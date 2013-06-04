function pgt(folder, numComponents)
%Creates a pgt file from a set of tranfer and obj files in a given folder.
    out = fopen(strcat(folder, '.pgt'), 'w');

    [Poses, Faces] = geometry(fullfile(folder, '*.obj'));
    [M, V, U] = isvd(Poses, numComponents);
    
    write(out, M); write(out, V); write(out, U);
    write(out, Faces, 'int32');
    
    [Poses] = transfer(fullfile(folder, '*.transfer'));
    [M, V, U] = isvd(Poses, numComponents);
    
    write(out, M); write(out, V); write(out, U);
    fclose(out);
end