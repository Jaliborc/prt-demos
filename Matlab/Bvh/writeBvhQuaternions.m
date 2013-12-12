function writeBvhQuaternions(path)
%Converts a bvh file motion to quaternion binary format.
    out = fopen(strcat(path, '.quat'), 'w');
    writeMatrix(out, readBvhQuaternions(strcat(path, '.bvh')), 'float32');
    fclose(out);
end

