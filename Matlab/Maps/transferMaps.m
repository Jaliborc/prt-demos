function transferMaps(obj, transf, width)
    [vertices, faces, coords] = geometry(obj);
    transf = transfer(transf);
    numValues = size(transf) / size(vertices) * 3;

    image = zeros(width, width, 3);
    pixels = linspace(0, 1, width);
    xy = coords(:, faces(2:2:end));
    x = xy(1,:)'; y = xy(2,:)';
    
    for p = 1:size(transf, 2)
        for c = (1:3:numValues) - 1
            for i = 1:3
                coef = transf(i+c:numValues:end,p);
                v = coef(faces(1:2:end));
                f = scatteredInterpolant(x, y, v);

                for a = 1:width
                    for b = 1:width
                        image(a,b,i) = f(pixels(a), pixels(b));
                    end
                end
            end

            figure; imshow(image);
        end
    end
end

