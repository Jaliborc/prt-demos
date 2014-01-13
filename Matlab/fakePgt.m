function fakePgt(folder, numPoses)
%Creates a "Precomputed Geometry and Transfer" file from a set of .obj and .tranfer files in a given folder that matches the original data.
    file = java.io.FileOutputStream(strcat(folder, ' fake.pgt'));
    out = java.io.ObjectOutputStream(file);

    [Poses, Faces] = geometry(fullfile(folder, '*.obj'));
    M = zeros(size(Poses), 1);
    V = Poses(:,1:numPoses);
    U = eye(numPoses);
    
    out.writeObject(single(M));
    out.writeObject(single(V));
    out.writeObject(single(U));
    out.writeObject(int32(Faces - 1));
    out.flush();
    
    [Poses] = transfer(fullfile(folder, '*.transfer'));
    M = zeros(size(Poses), 1);
    V = Poses(:,1:numPoses);
    U = eye(numPoses);
    
    out.writeObject(single(M));
    out.writeObject(single(V));
    out.writeObject(single(U));
    out.close();
end

