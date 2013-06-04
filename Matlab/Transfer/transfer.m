function [poses, numHarmonics] = transfer(path)
%Assembles a set of transfer files into a matrix.
    files = openFiles(path);
    poses = [];
    
    for i = 1:size(files)
        numHarmonics = fread(files(i), 1, 'int32');
        pose = fread(files(i), inf, 'float32');
        poses = [poses pose];
    end
    
    closeFiles(files);
end