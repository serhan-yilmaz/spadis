/*
* Copyright (C) 2018 Serhan Yýlmaz
*
* This file is part of SPADIS
*
* SPADIS is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* SPADIS is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mex.h"
#include "matrix.h"

#include <vector>
#include "Optimizer.h"

using Optimizer = spadis::Optimizer;
using Solution = spadis::Solution;
using Graph = spadis::Graph;
using Node = spadis::Node;
using Options = spadis::Options;

/*
	Input								Type						Size						
	------------------------------		-------						-------------------
	Node Prizes(C)						double						n x 1 vector
	Edge Weights(W)						logical or double			n x n sparse matrix
	Number of Features(N)				double						scalar
	Penalty Distance(D)					double						scalar
	Penalty(Beta)						double						scalar
*/

Graph createGraph(mwIndex * Wrow, mwIndex * Wcolumn, double* weights, int n){
	bool isWeighted = weights != nullptr;
	std::vector<Node> nodeList;
	nodeList.reserve(n);
	for (unsigned int i = 0; i < n; i++) {
		nodeList.push_back(Node(i, isWeighted));
	}
	for (unsigned int i = 0; i < n; i++) {
		mwIndex a1 = Wcolumn[i];
		mwIndex a2 = Wcolumn[i + 1];
		nodeList.at(i).reserve(a2 - a1);
		for (mwIndex j = a1; j < a2; j++) {
			mwIndex b = Wrow[j];
			unsigned int index = (unsigned int)b;
			if (index < nodeList.size()) {
				if (isWeighted) {
					nodeList.at(i).addConnection(index, weights[j]);
				} else {
					nodeList.at(i).addConnection(index);
				}
			}
		}
	}
	return Graph(nodeList, isWeighted);
}

Graph createGraph(mwIndex * Wrow, mwIndex * Wcolumn, int n) {
	return createGraph(Wrow, Wcolumn, nullptr, n);
}

void displayUsageMessage() {
	mexPrintf("Usage: spadis(\n \
		NodePrizes C<Vector, nx1> ,\n \
		EdgeWeights W<SparseMatrix, NxN>,\n \
		NumberOfFeatures N<scalar>,\n \
		NeighboringDistance D<scalar>, \n \
		NeighborWeight Beta<scalar>) \
		\n");
}

void mexFunction(int nlhs, mxArray *plhs[],
	int nrhs, const mxArray *prhs[])
{
	if (nrhs == 0 && nlhs == 0) {
		displayUsageMessage();
		return;
	}

	if (nrhs != 5) {
		displayUsageMessage();
		mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nrhs",
			"Five inputs are required.");
	}

	if (nlhs < 1) {
		mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nlhs",
			"At least one output is required.");
	}

	if (nlhs > 2) {
		mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nlhs",
			"At most two outputs are allowed.");
	}

	if (!mxIsDouble(prhs[0])) {
		mexErrMsgTxt("Node prize(C) vector must be of type double.");
	}

	if (!mxIsSparse(prhs[1])) {
		mexErrMsgTxt("Edge weight(W) matrix must be sparse.");
	}

	if (!mxIsLogical(prhs[1]) && !mxIsDouble(prhs[1])) {
		mexErrMsgTxt("Edge weight(W) matrix must be of type logical or double.");
	}

	if (!mxIsDouble(prhs[2])) {
		mexErrMsgTxt("Number of features(N) must be of type double.");
	}

	if (!mxIsDouble(prhs[3])) {
		mexErrMsgTxt("Distance limit parameter(D) must be of type double.");
	}

	if (!mxIsDouble(prhs[4])) {
		mexErrMsgTxt("Penalty parameter(Beta) must be of type double.");
	}

	if (!mxIsDouble(prhs[3])) {
		mexErrMsgTxt("Distance limit parameter(D) must be of type double.");
	}

	if (!mxIsScalar(prhs[3])) {
		mexErrMsgTxt("Distance limit parameter(D) must be a scalar.");
	}

	double* scores_ = mxGetPr(prhs[0]);
	mwSize nRowScores = mxGetM(prhs[0]);
	mwSize nRowNetwork = mxGetM(prhs[1]);
	mwSize nColumnScores = mxGetN(prhs[0]);
	mwSize nColumnNetwork = mxGetN(prhs[1]);

	double nSelection = mxGetScalar(prhs[2]);
	double D = mxGetScalar(prhs[3]);
	double* betaList = mxGetPr(prhs[4]);
	mwSize nRowBeta = mxGetN(prhs[4]);
	mwSize nColumnBeta = mxGetM(prhs[4]);
	mwSize nBeta = nRowBeta * nColumnBeta;

	if (nRowScores <= 0 || nColumnScores <= 0) {
		mexErrMsgTxt("Node prizes (C) cannot be empty.");
	}

	if (nRowNetwork <= 0 || nColumnNetwork <= 0) {
		mexErrMsgTxt("Edge Weights (W) cannot be empty.");
	}

	if (nRowScores != nRowNetwork) {
		mexErrMsgTxt("Number of rows in C and W must be equal!");
	}

	if (nRowNetwork != nColumnNetwork) {
		mexErrMsgTxt("W must be square matrix!");
	}

	if (nColumnScores != 1) {
		mexErrMsgTxt("C must be a row vector!");
	}

	if (D < 0) {
		mexErrMsgTxt("D must not be negative.");
	}

	if (isnan(D)) {
		mexErrMsgTxt("D must not be NaN.");
	}

	if (nRowBeta <= 0 || nColumnBeta <= 0) {
		mexErrMsgTxt("Beta cannot be empty.");
	}

	if (nRowBeta != 1 && nColumnBeta != 1) {
		mexErrMsgTxt("Beta must be a scalar or a vector.");
	}

	if (nSelection <= 0 || (int)nSelection != nSelection) {
		mexErrMsgTxt("Number of features(N) must be a positive integer.");
	}

	if (mxIsComplex(prhs[0]) || mxIsComplex(prhs[1]) || mxIsComplex(prhs[2]) || mxIsComplex(prhs[3]) || mxIsComplex(prhs[4])) {
		mexWarnMsgIdAndTxt("MATLAB:spadis:IgnoreImaginaryParts", "Imaginary parts of complex arguments are ignored.");
	}
	mwIndex* Wrows = mxGetIr(prhs[1]);
	mwIndex* Wcolumns = mxGetJc(prhs[1]);
	std::vector<double> c;
	c.reserve(nRowScores);
	for (mwSize i = 0; i < nRowScores; i++) {
		c.push_back(scores_[i]);
	}
	Graph graph;
	Options options;
	if (mxIsLogical(prhs[1])) {
		graph = createGraph(Wrows, Wcolumns, nColumnNetwork);
	} else {
		double* weights = mxGetPr(prhs[1]);
		graph = createGraph(Wrows, Wcolumns, weights, nColumnNetwork);
	}
	Optimizer optimizer(c, graph);
	options.setK(nSelection);
	bool hasInfiniteBeta = false;
	for (mwIndex i = 0; i < nBeta; i++) {
		if (isinf(betaList[i])) {
			betaList[i] = spadis::BETA_INFINITE;
		}
		if (isnan(betaList[i])) {
			mexErrMsgTxt("Beta values must not be NaN.");
		}
		if (betaList[i] < 0 && betaList[i] != spadis::BETA_INFINITE) {
			mexErrMsgTxt("Beta values must be greater than or equal to zero.");
		}
		if (betaList[i] == spadis::BETA_INFINITE) {
			hasInfiniteBeta = true;
		}
		options.addBeta(betaList[i]);
	}
	options.setDistanceParameter(D);
	plhs[0] = mxCreateLogicalMatrix(nRowNetwork, nBeta);
	mxLogical* indicators = mxGetLogicals(plhs[0]);
	optimizer.select(options);
	std::vector<Solution> solutions = optimizer.getSolutions();
	unsigned int index = 0;
	for (int i = 0; i < solutions.size(); i++) {
		const Solution solution = solutions.at(i);
		for (int j = 0; j < solution.indicators.size(); j++) {
			indicators[index] = solution.indicators.at(j);
			index++;
		}
	}
	if (nlhs >= 2) {
		if (hasInfiniteBeta) {
			plhs[1] = mxCreateDoubleMatrix(1, nBeta, mxCOMPLEX);
			double* fReal = mxGetPr(plhs[1]);
			double* fImag = mxGetPi(plhs[1]);
			for (int i = 0; i < solutions.size(); i++) {
				const auto F = solutions.at(i).optimizerValue;
				fReal[i] = F.real;
				fImag[i] = F.infinite;
			}
		}
		else {
			plhs[1] = mxCreateDoubleMatrix(1, nBeta, mxREAL);
			double* fReal = mxGetPr(plhs[1]);
			for (int i = 0; i < solutions.size(); i++) {
				fReal[i] = solutions.at(i).optimizerValue.real;
			}
		}
	}
}