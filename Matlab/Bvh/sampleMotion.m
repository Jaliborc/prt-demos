function samples = sampleMotion(data, count)
% Returns the requested number of most different poses from the given data

    % start with homing pose for calibration purposes
    samples = [0 0 0 0.59 7.62 -4.43 5.28 1.04 -49.49 -14.01 -5.42 20.74 0.00 0.00 2.64	0 6.09 -10.21 0	0.62 0 0 0 0 0 -4.37 -0.27 0 1.67 0	0 0	0 0	-6.88 1.80 0 1.04 0	0 0.51 0 0 -1.59 7.33 0	2.13 0 0 3.16 0]';
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
