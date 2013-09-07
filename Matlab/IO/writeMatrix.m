function writeMatrix(file, matrix, mode)
    size(matrix)
    fwrite(file, size(matrix), 'int32');
    fwrite(file, matrix, mode);
end