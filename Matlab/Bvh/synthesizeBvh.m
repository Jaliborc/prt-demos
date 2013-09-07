function synthesizeBvh(path, destination, numPoses)
    [motion, header] = readBvh(path);
    samples = sampleMotion(motion, numPoses);
    
    writeBvh(destination, samples, header);
end

