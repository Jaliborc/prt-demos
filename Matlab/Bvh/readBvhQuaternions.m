function quats = readBvhQuaternions(file)
    euler = readBvh(file);
    quats = [];
    
    for i = 4:3:size(euler)
       joint = SpinCalc('EA123toQ', euler(i:i+2,:)', 0, 0);
       
       for k = 1:size(joint)
          if joint(k,4) < 0
              joint(k,:) = joint(k,:) * -1;
          end
       end
       
       quats = [quats; joint(:,4)'; joint(:,1:3)'];
    end
end

