function files = openFiles(path)
%Returns a vector of the file handles that match the specified query.
    listing = dir(path);
    files = zeros(size(listing), 1);
    
    for i = 1:size(files)
        files(i) = fopen(fullfile(fileparts(path), listing(i).name));
    end
end

