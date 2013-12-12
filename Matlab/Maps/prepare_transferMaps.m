function data = prepare_transferMaps(obj, transfer)
%Precomputes variables for map generation
    [p, x, y] = coords(obj);
    
    data = {};
    data.x = x';
    data.y = y';
    data.vertices = p;
    data.transfer = transfer;
    data.numValues = int32(size(transfer, 1) / max(p));
    data.numCoefs = int32(data.numValues / 3);
end

