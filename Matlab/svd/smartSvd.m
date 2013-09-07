function results = smartSvd(input)
% Performs a centered SVD decomposition and premultiplies the singular vectors. 
    results = {};
    results.O = input;
    results.M = mean(input, 2);
    
        [results.U, results.S, results.V] = svd(bsxfun(@minus, results.O, results.M), 'econ');
    results.U = results.U * results.S;
end

