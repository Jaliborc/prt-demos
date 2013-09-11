function off = angleDist(a, b)
% Custom distance function for euclidian angles
    off = sum(a(ones(1,size(b,1)), :) - b, 2);
    off = abs(mod(off + 180, 360) - 180);
end