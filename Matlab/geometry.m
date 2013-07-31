function [poses, faces, coords] = geometry(path)
%Assembles the geometry from a set of .obj files into a matrix.
    files = openFiles(path);
    poses = [];
    
    for i = 1:size(files)
        pose = scanFile(files(i), 'v %f %f %f');
        poses = [poses pose];
    end
    
    if nargout > 2
        frewind(files(1));
        faces = scanFile(files(1), 'f %d/%d %d/%d %d/%d') - 1;
        frewind(files(1));
        coords = scanFile(files(i), 'vt %f %f');
    else
        frewind(files(1));
        faces = scanFile(files(1), 'f %d%*s %d%*s %d%*s') - 1;
    end
    
    closeFiles(files);
end

