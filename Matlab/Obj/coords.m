function [i, x, y] = coords(path)
% Computes every vertex/texture coordinate pair in an .obj file.
    files = openFiles(path);
    [~, faces, coords] = geometry(files{1});
   
    i = faces(1:2:end);
    xy = coords(:, faces(2:2:end));
    x = xy(1,:); y = xy(2,:); 
end

