function closeFiles(files)
%Closes a set of files.
    for i = 1:size(files)
        fclose(files(i));
    end
end

