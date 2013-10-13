function colors = vertexColors(objFile, map)
% Computes colors at vertex positions from a color map.
    u = 1 - linspace(0, 1, size(map, 1));
    v = 1 - linspace(0, 1, size(map, 2));
    
    [u,v] = meshgrid(u, v);
    [p, x, y] = coords(objFile);
    
    n = max(p);
    colors = zeros(n, 3);
    
    for c = 1:3
        channel = map(:,:,c);
        f = scatteredInterpolant(u(:), v(:), double(channel(:)), 'linear', 'nearest');
    
        color = zeros(n, 1);
        for i = 1:size(p)
            color(p(i)) = f(x(i), y(i));
        end
        
        colors(:,c) = color(:);
    end
end