function header = readBvhHeader(file)
    frewind(file);
    lines = textscan(file, '%s', 'delimiter','\n');
    header = '';
    
    for i = 1:size(lines{1})
        line = lines{1}(i);
        if strcmp(line, 'MOTION') == 1
            break;
        end
        
        header = strcat(header, line, '\n');
    end
    
    header = header{1};
end

