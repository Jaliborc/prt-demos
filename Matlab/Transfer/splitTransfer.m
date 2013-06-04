function bands = splitTransfer(transfer, numHarmonics)
%Splits a tranfer matrix into bands as an array of matrixes.
    numHarmonics = numHarmonics * 3;
    harmonics = cell(numHarmonics, 1);
    for i = 1:numHarmonics
        harmonics{i} = transfer(i:numHarmonics:end,:);
    end
    
    numPoses = size(transfer, 2);
    start = 1;  width = 3; 
    bands = {};
    
    while start < numHarmonics
        stop = start + width - 1;
        band = horzcat(harmonics{start:stop});
        band = reshape(band', numPoses, [])';
        bands{end+1} = band;
        
        start = stop + 1;
        width = width + 6;
    end
end

