function [poses, numHarmonics] = transfer(path)
%Assembles a set of transfer files into a matrix.
    files = openFiles(path);
    poses = [];
    
    for i = 1:size(files)
        file = fopen(files{i});
        numHarmonics = fread(file, 1, 'int32');
        pose = fread(file, inf, 'float32');
        poses = [poses pose];
        fclose(file);
    end
end