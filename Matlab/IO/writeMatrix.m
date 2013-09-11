function writeMatrix(file, matrix, mode)
    fwrite(file, size(matrix), 'int32');
    fwrite(file, matrix, mode);
end