function svd = svdStruct(input, numCoefs)
% Performs and isvd and stores all values in a structure.
    [M, U, V, S] = isvd(input, numCoefs);

    svd = {};
    svd.O = input;
    svd.M = M;
    svd.U = U;
    svd.V = V;
    svd.S = S;
end

