function samples = sampleMotion(data, count)
% Returns the requested number of most different poses from the given data

    samples = zeros(51, 1); % start with homing pose (for calibration purposes)
    dists = pdist2(data', samples', @angleDist);
    
    for k = 1:count
        [~, i] = max(dists);
        
        sample = data(:,i);
        samples = [samples sample];
        
        dists(i) = [];
        data(:,i) = [];
        dists = min(dists, pdist2(data', sample', @angleDist));
    end
end
