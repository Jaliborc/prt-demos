function files = openFiles(path)
%Returns an array of file names that match the specified query.
    listing = dir(path);
    files = cell(size(listing, 1), 1);
    
    for i = 1:size(files)
        files{i} = fullfile(fileparts(path), listing(i).name);
    end
end