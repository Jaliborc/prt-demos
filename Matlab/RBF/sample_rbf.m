function values = sample_rbf(Xc, width, W, k_i, basisfunction)
    values = zeros(width, width, size(W'));
    u = linspace(min(Xc(:,1)), max(Xc(:,1)), width);
    v = linspace(min(Xc(:,2)), max(Xc(:,2)), width);
    
    for x = 1:width
        for y = 1:width
           values(x,y,:) = sim_rbf(Xc, [u(x), v(y)], W, k_i, basisfunction);
        end
    end
end
