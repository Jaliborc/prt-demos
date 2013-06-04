function m = read(file)
%.Reads a matrix from a binary file
%   Detailed explanation goes here
    size = fread(file, 2, 'int32');
    m = fread(file, [size(1), size(2)], 'float32');
end

