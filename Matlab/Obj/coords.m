function [i, x, y] = coords(objFile)
% Computes every vertex/texture coordinate pair in an .obj file.
    [vertices, faces, coords] = geometry(objFile);
   
    i = faces(1:2:end);
    xy = coords(:, faces(2:2:end));
    x = xy(1,:); y = xy(2,:); 
end

