function [poses, faces, coords] = geometry(path)
%Assembles the geometry from a set of .obj files into a matrix.
    files = openFiles(path);
    poses = [];
    
    for i = 1:size(files)
        file = fopen(files{i});
        pose = scanFile(file, 'v %f %f %f', 0);
        poses = [poses pose];
        fclose(file);
    end
    
    file = fopen(files{i});
    if nargout > 2
        faces = scanFile(file, 'f %d/%d%*s %d/%d%*s %d/%d%*s', 0);
        frewind(file);
        coords = scanFile(file, 'vt %f %f', 1);
    else
        faces = scanFile(file, 'f %d%*s %d%*s %d%*s', 0);
    end
    
    fclose(file);
end

