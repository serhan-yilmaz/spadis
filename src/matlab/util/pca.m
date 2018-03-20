function [ PC, D ] = pca( X, n )
    if(n == 0)
       PC = zeros(size(X, 1), 0);
       D = zeros(1, 0);
    else
       Xn = (X - mean(X, 1)) ./ std(X, 1);
       [V, D] = eig(Xn * Xn');
       [D, ind] = sort(diag(D), 'descend');
       PC = V(:, ind(1:n));
    end
end

