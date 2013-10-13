function [poses, faces, coords] = geometry(path)
%Assembles the geometry from a set of .obj files into a matrix.
    files = openFiles(path);
    poses = [];
    
    for i = 1:size(files)
        pose = scanFile(files(i), 'v %f %f %f', 0);
        poses = [poses pose];
    end
    
    if nargout > 2
        frewind(files(1));
        faces = scanFile(files(1), 'f %d/%d%*s %d/%d%*s %d/%d%*s', 0);
        frewind(files(1));
        coords = scanFile(files(i), 'vt %f %f', 1);
    else
        frewind(files(1));
        faces = scanFile(files(1), 'f %d%*s %d%*s %d%*s', 0);
    end
    
    closeFiles(files);
end

