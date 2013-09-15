function maps = transferMaps(objFile, transfers, width)
    [vertices, faces, coords] = geometry(objFile);
    numCoefs = int32(size(transfers) / size(vertices));
    numValues = numCoefs * 3;
   
    xy = coords(:, faces(2:2:end));
    x = xy(1,:); y = xy(2,:); 
    uv = 1 - linspace(0, 1, width);
    
    image = zeros(width, width, 3);
    maps = {};
        
    for c = 1:numCoefs
        maps{c} = {};
        
        for p = 1:size(transfers, 2)
            for i = 1:3
                coef = transfers(i + c*3 - 3: numValues : end, p);
                values = coef(faces(1:2:end));
                f = scatteredInterpolant(x', y', values, 'linear', 'nearest');

                for a = 1:width
                    for b = 1:width
                        image(a,b,i) = f(uv(a), uv(b));
                    end
                end
                
                image(:,:,i) = rot90(image(:,:,i), -1);
            end
            
            image(isnan(image)) = 0;
            maps{c}{p} = image;
        end
    end
end

