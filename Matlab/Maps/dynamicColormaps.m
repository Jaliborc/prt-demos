function dynamicColormaps(destination, source, images, bvh, numPoses)
    poses = readBvh(bvh);
    base = poses(:,1); % assume we start with homing position
    baseImage = imread(fullfile(source, strcat(images{1}, '.png')));
    offs = sangleDist(poses', base')';
    axis = {};
    
    for i = 2:size(images, 2)
        if images{i} ~= 0
            entry = {};
            entry.image = imread(fullfile(source, strcat(images{i}, '.png'))) - baseImage;
            entry.offs = sum(abs(offs(i*3-2:i*3, :)));
            entry.offs = entry.offs / max(entry.offs);
            
            %figure; plot(entry.offs);
        else
            entry = 0;
        end
        
        axis{i} = entry;
    end
    
   for i = 1:numPoses
       image = baseImage;
        
       for k = 2:size(axis, 2)
           entry = axis{k};
           
           if ~isnumeric(entry)
              off = entry.offs(i);
              image = image + entry.image * off;
           end
       end
        
        imwrite(image, fullfile(destination, strcat(num2str(i, '%05.f'), '.png')));
    end
end

