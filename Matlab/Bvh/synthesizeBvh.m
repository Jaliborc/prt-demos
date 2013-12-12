function synthesizeBvh(path, destination, numPoses)
    [motion, header] = readBvh(path);
    
    motion = restrainThumb(motion);
    samples = sampleMotion(motion, numPoses);
    
    writeBvh(destination, samples, header);
end

