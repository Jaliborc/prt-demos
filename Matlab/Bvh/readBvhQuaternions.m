function quats = readBvhQuaternions(file)
% Opens a set of bvh files and returns the angles for each pose in quaternion form.

    euler = readBvh(file);
    quats = [];
    
    for i = 4:3:size(euler)
       joint = SpinCalc('EA123toQ', euler(i:i+2,:)', 0, 0);
       
       for k = 1:size(joint)
          if joint(k,4) < 0
              joint(k,:) = -joint(k,:);
          end
       end
       
       joint = joint';
       quats = [quats; joint(4,:); joint(1,:); joint(3,:); joint(2,:)];
    end
    
    quats([7 11 15],:) = -quats([7 11 15],:);
end

