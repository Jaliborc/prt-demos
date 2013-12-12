function motion = restrainThumb(motion)
    upperBound = [0,0,0, 0,0,0, 39,22,0, -13,0,29];
    lowerBound = [0,0,0, 0,0,0, 0,0,0, 0,-8,0];
    
    for i = 1:size(upperBound, 2)
        if upperBound(i)
            motion(i,:) = restrain(motion(i,:), upperBound(i));
        end
        
        if lowerBound(i)
            motion(i,:) = -restrain(-motion(i,:), -lowerBound(i));
        end
    end
end

function motion = restrain(motion, limit)
    off = motion - limit;
    fails = off > 0;
    %devi = off(fails) .^ 2;
    %range = (limit - min(motion)) / 4 / max(devi);

    %motion(fails) = motion(fails) - off(fails) + devi * range;
    motion(fails) = motion(fails) - off(fails) * 0.9;
end