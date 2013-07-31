function pgst(folder, numComponents)
%Creates a "Precomputed Geometry and band-Split Transfer" file from a set of .obj and .tranfer files in a given folder.
    file = java.io.FileOutputStream(strcat(folder, '.pgst'));
    out = java.io.ObjectOutputStream(file);

    [Poses, Faces] = geometry(fullfile(folder, '*.obj'));
    [M, V, U] = isvd(Poses, numComponents);
    
    out.writeObject(single(M));
    out.writeObject(single(V));
    out.writeObject(single(U));
    out.writeObject(int32(Faces));
    
    [Poses, numHarmonics] = transfer(fullfile(folder, '*.transfer'));
    Bands = splitTransfer(Poses ./ pi, numHarmonics);
    out.writeObject(int32(size(Bands, 2)));
    
    i = 0;
    for band = Bands
        [M, V, U] = isvd(band{1}, numComponents);
        
        out.writeObject(single(M));
        out.writeObject(single(V));
        out.writeObject(single(U));

        title(i);
        i = i + 1;
    end
    
    out.close();
end