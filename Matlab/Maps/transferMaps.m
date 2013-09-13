function maps = transferMaps(objFile, transfers, width, minimum)
    [vertices, faces, coords] = geometry(objFile);
    numCoefs = int32(size(transfers) / size(vertices));
    numValues = numCoefs * 3;
   
    xy = coords(:, faces(2:2:end));
    x = xy(1,:)'; y = xy(2,:)';
    u = linspace(0, 1, width);
    v = 1 - u;
    
    image = zeros(width, width, 3);
    maps = {};
        
    for c = 1:numCoefs
        maps{c} = {};
        
        for p = 1:size(transfers, 2)
            for i = 1:3
                coef = transfers(i + c*3 - 3: numValues : end, p);
                values = coef(faces(1:2:end));
                f = scatteredInterpolant(x, y, values, 'linear', 'nearest');

                for a = 1:width
                    for b = 1:width
                        image(a,b,i) = f(u(a), v(b));
                    end
                end
            end
            
            minima = min(min(min(image)));
            if (-minima > minimum)
                error('Image contains value (%f) smaller than minimum bound provided', minima);
            end
            
            maps{c}{p} = image + minimum;
        end
    end
end

