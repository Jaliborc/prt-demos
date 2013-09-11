function [motion, header] = readBvh(path)
    % Opens a set of .bhv files and returns the recorded poses and the header
    files = openFiles(path);
    motion = [];
    
    for i = 1:size(files)
        frame = scanFile(files(i), '%f', 1);
        motion = [motion frame];
    end
    
    motion(motion == -180) = 180;
    header = readBvhHeader(files(1));
    closeFiles(files);
end