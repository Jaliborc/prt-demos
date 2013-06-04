function [M, V, U] = isvd(data, numComponents)
%Performs a centered incremental singular value decomposition on the data.
    M = mean(data, 2);
    data = bsxfun(@minus, data, M);
    
    [V, S, U] = seqkl(data, numComponents);
    U = S * U';
    
    figure; plot(diag(S));
end

