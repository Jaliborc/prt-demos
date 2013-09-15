function [M, U, V, S] = isvd(data, numComponents)
%Performs a centered incremental singular value decomposition on the data.
    M = mean(data, 2);
    data = bsxfun(@minus, data, M);
    
    [U, S, V] = seqkl(data, numComponents);
    V = S * V';
    
    figure; plot(diag(S));
end

