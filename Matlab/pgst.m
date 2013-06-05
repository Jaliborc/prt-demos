function pgst(folder, numComponents)
%Creates a pgt file from a set of tranfer and obj files in a given folder.
    out = fopen(strcat(folder, '.pgst'), 'w');

    [Poses, Faces] = geometry(fullfile(folder, '*.obj'));
    [M, V, U] = isvd(Poses, numComponents);
    
    write(out, M); write(out, V); write(out, U);
    write(out, Faces, 'int32');
    
    [Poses, numHarmonics] = transfer(fullfile(folder, '*.transfer'));
    Bands = splitTransfer(Poses ./ pi, numHarmonics);
    fwrite(out, size(Bands, 2), 'int32');
    
    i = 0;
    for band = Bands
        [M, V, U] = isvd(band{1}, numComponents);
        write(out, M); write(out, V); write(out, U);
        title(i);
        i = i + 1;
    end
    
    fclose(out);
end