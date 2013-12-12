function images = make_transferMaps(data, coef, dimention)
    coef = (coef - 1) * 3 + 1;
    coefs = coef:coef+2;
    
    uv = 1 - linspace(0, 1, dimention);
    image = zeros(dimention, dimention, 3);
    images = {};
    
    for column = 1:size(data.transfer, 2)
         for color = 1:3
            values = data.transfer(coefs(color):data.numValues:end, column);
            f = scatteredInterpolant(data.x, data.y, values(data.vertices), 'linear', 'nearest');
         
            for a = 1:dimention
                for b = 1:dimention
                    image(a,b,color) = f(uv(a), uv(b));
                end
            end
            
            image(:,:,color) = rot90(image(:,:,color), -1);
         end
         
         image(isnan(image)) = 0;
         images{column} = image;
         disp(strcat('Finished coeficient #', num2str(coef), ' column #', num2str(column)));
    end
end

