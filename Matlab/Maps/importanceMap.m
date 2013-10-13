function importanceMap(destination, source, images)
    B = imread(fullfile(source, strcat(images{1}, '.png')));
    I = zeros(size(B));
    
    for i = 2:size(images, 2)
        o = imread(fullfile('/Users/Jaliborc/Documents/Research/PRTAnalysis/Models/hand/color', strcat(images{i}, '.png')));
        off = abs(o - B);

        I = I + single(off);
    end
    
    I(I == 0) = 0.5;
    imwrite(I, destination);
end

