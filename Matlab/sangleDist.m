function off = sangleDist(a, b)
    off = bsxfun(@minus, a, b);
    off = abs(mod(off + 180, 360) - 180);
end

