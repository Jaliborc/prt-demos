function scatterClusters(data, clusters)
    [~, ~, V] = isvd(data, 3);
    
    colors = {'b' 'g' 'r' 'y' 'k' 'c' 'm' [1 0.5 0] [0.5 1 0] [1 0 0.5] [0.5 0 1] [0 1 0.5] [0 0.5 1] [0.5 0.5 1] [0.5 1 0.5] [1 0.5 0.5]};
    legends = {};
    figure; 
    
    for i = 1:min(size(unique(clusters)), size(colors'))
        legends{i} = strcat('cluster ', num2str(i));
        scatter3(V(1,clusters==i), V(2,clusters==i), V(3,clusters==i), 20, colors{i});
        hold on;
    end
    
    legend(legends)
end