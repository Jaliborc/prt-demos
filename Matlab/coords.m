function uv = coords(path)
%Returns the texture coordinates of an .obj file
    files = openFiles(path);
    file = files(1);

    faces = scanFile(file, 'f %f/%f %f/%f %f/%f');
    vertices = faces(1:2:end);
    coord = faces(2:2:end);
    
    index = zeros(max(vertices), 1);
    for i=1:size(index)
       index(i) = coord(find(vertices == i, 1));
    end
    
    frewind(file);
    vt = scanFile(file, 'vt %f %f');
    uv = zeros(size(index) * 2, 1);
    
    for i=1:size(index)
       uv(i*2 - 1) = vt(index(i) * 2 - 1);
       uv(i*2) = 1 - vt(index(i) * 2); % y is flipped
    end
    
    closeFiles(files);
end

