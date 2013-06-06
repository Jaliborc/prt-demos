function pgv(folder, numComponents)
%Creates a pgv file from a set of visibility and obj files in a given folder.
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

    out.writeObject(int32(Faces));
    out.close();
end