function [clusters, k] = recursiveClusters(data, range, tries)
    bests = [];
    results = {};
    
    if range ~= 1
        for k = range
            bests(k) = 0;

            for i = 1:tries
                try
                    attempt = kmeans(data, k, 'dist','city');
                    performance = mean(silhouette(data, attempt, 'city'));
                catch
                    continue;
                end

                if performance > bests(k)
                    bests(k) = performance;
                    results{k} = attempt;
                end
            end
        end

        k = find(bests == max(bests));
        clusters = results{k};
        
        hist(clusters, unique(clusters));
        scatterClusters(data', clusters);
    else
        k = 1;
        clusters = ones(1, size(data));
    end
end

