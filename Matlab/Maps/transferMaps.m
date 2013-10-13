function [maps, numCoefs] = transferMaps(objFile, transfers, width)
    [v, x, y] = coords(objFile);
    
    numValues = int32(size(transfers, 1) / max(v));
    numCoefs = int32(numValues / 3);
    
    uv = 1 - linspace(0, 1, width);
    image = zeros(width, width, 3);
    maps = {};
        
    for c = 1:numCoefs
        maps{c} = {};
        
        for p = 1:size(transfers, 2)
            for i = 1:3
                coef = transfers(i + c*3 - 3: numValues : end, p);
                f = scatteredInterpolant(x', y', coef(v), 'linear', 'nearest');

                for a = 1:width
                    for b = 1:width
                        image(a,b,i) = f(uv(a), uv(b));
                    end
                end
                
                image(:,:,i) = rot90(image(:,:,i), -1);
            end
            
            image(isnan(image)) = 0;
            maps{c}{p} = image;
            disp(strcat('Finished coeficient #', num2str(c), ' vector #', num2str(p)));
        end
    end
end

