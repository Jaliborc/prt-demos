function writeBvh(destination, data, header)
    file = fopen(destination, 'wt');
    fprintf(file, header);
    fprintf(file, 'MOTION\nFrames: %d\nFrame Time: 0.023541\n', size(data, 2));
    fclose(file);
    
    dlmwrite(destination, data', 'delimiter', ' ', '-append');
end

