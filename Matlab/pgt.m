function pgt(folder, numComponents)
%Creates a "Precomputed Geometry and Transfer" file from a set of .obj and .tranfer files in a given folder.
    file = java.io.FileOutputStream(strcat(folder, '.pgt'));
    out = java.io.ObjectOutputStream(file);

    [Poses, Faces] = geometry(fullfile(folder, '*.obj'));
    [M, V, U] = isvd(Poses, numComponents);
    
    out.writeObject(single(M));
    out.writeObject(single(V));
    out.writeObject(single(U));
    out.writeObject(int32(Faces - 1));
    out.flush();
    
    [Poses] = transfer(fullfile(folder, '*.transfer'));
    [M, V, U] = isvd(Poses, numComponents);
    
    out.writeObject(single(M));
    out.writeObject(single(V));
    out.writeObject(single(U));
    out.close();
end