addpath(genpath([cd(), '/binaries/']));
addpath(genpath([cd(), '/src/matlab/']));
%% Prepare Data
phenotypeIndex = 1;     % Index of the phenotype - 4W
networkIndex   = 1;     % Index of the network - GS
nPCs           = 3;     % Number of Principal Components used
                        % to correct population stratification

load('data/data.mat');
load('data/networks.mat');

% Valid indices for the selected phenotype
sampleIndices = ~isnan(Y(:, phenotypeIndex));
% X : genotype  - n x m  matrix - n : Samples, m : Features (SNPs)
X = double(X(sampleIndices, :)); 
% Y : phenotype - n x 1 column vector
Y = Y(sampleIndices, phenotypeIndex);
% W : network   - m x m sparse matrix
W = networks{1};
% C : scores    - m x 1 column vector
C = computeSKAT(X, Y, 'k', nPCs);

%% Run SPADIS for a single parameter pair
k    = 100;             % Number of Features to be selected
Beta = Inf;             % Beta parameter of SPADIS determines
                        %  the magnitude of penalty for  
                        %  selecting close vertices
D    = 100;             % Distance limit parameter of SPADIS
                        %  determines the maximum distance for
                        %  two vertices to be considered as close
          
% The first output of SPADIS (S) is an m x 1 logical column vector.
% S contains the indicators for a subset of features
% i.e. feature i is selected iff S[i] = true
[S] = spadis(C, W, k, D, Beta);

% The second output of SPADIS (V) is a double scalar
% containing the value of the optimization function.
% If Beta is Inf, v is complex. The imaginary part indicates the 
% divergent part of the optimization function (F).
% i.e. F = real(V) + Beta * imag(V)
[~, V] = spadis(C, W, k, D, Beta);

%% Run SPADIS for a range of beta values
k    = 100;             % Number of Features to be selected
D    = 3;               % Distance limit parameter of SPADIS
                        %  determines the maximum distance for
                        %  two vertices to be considered as close
b    = 5;               % Number of beta values to be tested

% Create a vector of b beta values in logarithmic scale
BetaVector = logspace(-b + 1, 0, b) * max(C);
% SPADIS can be run for a vector of beta values efficiently.
% S (n x b matrix) contains the indicators for each beta.
% V (b x 1 vector) contains the optimization function value for each beta.
[S, V] = spadis(C, W, k, D, BetaVector);

