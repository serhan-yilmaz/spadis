function [ C, P ] = computeSKAT( X, Y, varargin )
    p = inputParser;
    p.CaseSensitive = false;
    validMatrix = @(x) validateattributes(x, {'numeric'}, {'2d', 'nonempty', 'nonsparse', 'real', 'nonnan'});
    validColumn = @(x) validateattributes(x, {'numeric'}, {'column', 'nonempty', 'nonsparse', 'real', 'nonnan'});
    validScalar = @(x) validateattributes(x, {'numeric'}, {'scalar', 'nonempty', 'real', 'nonnan', 'nonnegative'});
    addRequired(p, 'X', validMatrix);
    addRequired(p, 'Y', validColumn);
    addOptional(p, 'P', [], validMatrix);
	addParameter(p, 'k', 0, validScalar);
    parse(p, X, Y, varargin{:});
    if(size(X, 1) ~= size(Y, 1)); error('Number of rows in X and Y must be equal.'); end
    P = p.Results.P;
    if(isempty(P))
        P = pca(X, p.Results.k);
    else
        if(size(X, 1) ~= size(Y, 1)); error('Number of rows in X and P must be equal.'); end
    end
    cov = [ones(size(X, 1), 1), P];    %% Covariates  - n x (k + 1) matrix
    y = cov * (cov \ Y);               %% Estimated Y - n x 1 column vector
    r = Y - y;                         %% Residuals   - n x 1 column vector
    C = ((X' * r) / size(X, 1)) .^ 2;  %% SKAT Scores - m x 1 column vector
end

