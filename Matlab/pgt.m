function pgt(folder, numComponents)
%Creates a pgt file from a set of tranfer and obj files in a given folder.
    file = java.io.FileOutputStream(strcat(folder, '.pgt'));
    out = java.io.ObjectOutputStream(file);

    [Poses, Faces] = geometry(fullfile(folder, '*.obj'));
    [M, V, U] = isvd(Poses, numComponents);
    
    out.writeObject(single(M));
    out.writeObject(single(V));
    out.writeObject(single(U));
    out.writeObject(int32(Faces));
    
    [Poses] = transfer(fullfile(folder, '*.transfer'));
    [M, V, U] = isvd(Poses ./ pi, numComponents);
    
    out.writeObject(single(M));
    out.writeObject(single(V));
    out.writeObject(single(U));
    out.close();
end