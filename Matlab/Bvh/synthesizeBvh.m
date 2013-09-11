function synthesizeBvh(path, destination, numPoses)
    [motion, header] = readBvh(path);
    samples = sampleMotion(motion, numPoses);
    %samples([7 17 26 35 44],:) = -samples([7 17 26 35 44],:);
    
    writeBvh(destination, samples, header);
end

