function pgv(folder, numComponents)
%Creates a "Precomputed Geometry and Visibility" file from a set of .obj and .visibility files in a given folder.
    file = java.io.FileOutputStream(strcat(folder, '.pgv'));
    out = java.io.ObjectOutputStream(file);

    [Poses, Faces] = geometry(fullfile(folder, '*.obj'));
    [M, V, U] = isvd(Poses, numComponents);
    
    out.writeObject(single(M));
    out.writeObject(single(V));
    out.writeObject(single(U));
    
    [Poses] = transfer(fullfile(folder, '*.visibility'));
    [M, V, U] = isvd(Poses, numComponents);
    
    out.writeObject(single(M));
    out.writeObject(single(V));
    out.writeObject(single(U));

    out.writeObject(int32(Faces - 1));
    out.close();
end