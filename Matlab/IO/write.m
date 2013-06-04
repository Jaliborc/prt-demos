function write(varargin)
%.Writes a matrix's size and contents into a file.
    [file, matrix] = varargin{1:2};
   
    if nargin == 3
        precision = varargin{3};
    else
        precision = 'float32';
    end

    fwrite(file, size(matrix), 'int32');
    fwrite(file, matrix, precision);
end

