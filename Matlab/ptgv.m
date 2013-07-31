function ptgv(folder, numComponents)
%Creates a "Precomputed Textured Geometry and Visibility" file from a set of .obj and .visibility files in a given folder.
    file = java.io.FileOutputStream(strcat(folder, '.ptgv'));
    out = java.io.ObjectOutputStream(file);

    [Poses, Faces, Coords] = geometry(fullfile(folder, '*.obj'));
    [M, V, U] = isvd(Poses, numComponents);
    
    out.writeObject(single(M));
    out.writeObject(single(V));
    out.writeObject(single(U));
    
    [Poses] = transfer(fullfile(folder, '*.visibility'));
    [M, V, U] = isvd(Poses, numComponents);
    
    out.writeObject(single(M));
    out.writeObject(single(V));
    out.writeObject(single(U));

    out.writeObject(int32(Coords));
    out.writeObject(int32(Faces));
    out.close();
end