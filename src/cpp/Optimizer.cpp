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

#include "Optimizer.h"
#include <algorithm>

spadis::Optimizer::Optimizer(std::vector<double> scores, Graph graph) : scores_(scores), graph_(graph)
{
}

void spadis::Optimizer::select(const Options options)
{
	std::vector<std::vector<Neighbor>> neighborSets;
	neighborSets.reserve(scores_.size());
	std::vector<double> penaltySums;
	penaltySums.resize(scores_.size());
	flagsDijkstra_.resize(scores_.size());
	handlesDijkstra_.resize(scores_.size());
	std::vector<OptimizationHandle> optimizationHandles;
	optimizationHandles.resize(scores_.size());
	for (unsigned int i = 0; i < scores_.size(); i++) {
		neighborSets.push_back(std::vector<Neighbor>());
		flagsDijkstra_.at(i) = Flag::WHITE;
	}
	unsigned int N = scores_.size();
	for (unsigned int betaIndex = 0; betaIndex < options.getBetaSize(); betaIndex++) {
		optimizationHandles.clear();
		OptimizationQueue optimizationQueue;
		Solution solution;
		const double beta = options.getBeta(betaIndex);
		
		double betaPrime = beta / (2 * options.getK());
		if (beta == BETA_INFINITE) {
			betaPrime = 1.0 / (2 * options.getK());
		}
		solution.indicators.reserve(N);
		for (unsigned int i = 0; i < N; i++) {
			solution.indicators.push_back(false);
			penaltySums.at(i) = 0;
			OptimizerValue value(scores_.at(i));
			optimizationHandles.push_back(optimizationQueue.push(OptimizerHeapData(value, i)));
		}
		int nSelection = 0;
		OptimizerValue Ftotal;
		while (nSelection < options.getK()) {
			auto a = optimizationQueue.top();
			OptimizerValue Fmax = a.key;
			unsigned int maxFIndex = a.index;
			optimizationQueue.pop();
			auto handlex = optimizationHandles.at(maxFIndex);
			(*handlex).key.infinite -= 1;
			optimizationQueue.update(handlex);
			if (beta != BETA_INFINITE) {
				Ftotal.real += beta + Fmax.real;
			} else {
				Ftotal.real += Fmax.real;
				Ftotal.infinite += 1 + Fmax.infinite;
			}
			solution.indicators.at(maxFIndex) = true;
			if (neighborSets.at(maxFIndex).empty()) {
				neighborSets.at(maxFIndex) = findNeighbors(maxFIndex, options);
			}
			for (unsigned int j = 0; j < neighborSets.at(maxFIndex).size(); j++) {
				auto pair = neighborSets.at(maxFIndex).at(j);
				double Kvalue = 2 * (1 - pair.second / options.getDistanceParameter());
				penaltySums.at(pair.first) += Kvalue;
				if(!solution.indicators.at(pair.first)) {
					auto handle = optimizationHandles.at(pair.first);
					if (beta != BETA_INFINITE) {
						(*handle).key.real -= betaPrime * Kvalue;
					} else {
						(*handle).key.infinite -= betaPrime * Kvalue;
					}
					optimizationQueue.update(handle);
				}	
			}
			nSelection++;
		}
		solution.optimizerValue = Ftotal;
		solutions_.push_back(std::move(solution));
	}
}

std::vector<spadis::Solution> spadis::Optimizer::getSolutions() const
{
	return solutions_;
}

std::vector<spadis::Optimizer::Neighbor> spadis::Optimizer::findNeighbors(unsigned int sourceIndex, const Options options)
{
	std::vector<Neighbor> out;
	if (!graph_.isWeighted()) {		// Breath-First Search
		std::queue<unsigned int> q;
		std::queue<unsigned int> q_next;
		q.push(sourceIndex);
		int d = 0;
		flagsDijkstra_.at(sourceIndex) = Flag::BLACK;
		flagModificationList_.push_back(sourceIndex);
		while (!q.empty()) {
			if (d >= options.getDistanceParameter()) {
				break;
			}
			while (!q.empty()) {
				auto index = q.front();
				q.pop();
				out.push_back(Neighbor(index, d));
				const Node node = graph_.getNode(index);
				for (unsigned int i = 0; i < node.size(); i++) {
					unsigned int ix = node.getEdge(i);
					if (flagsDijkstra_.at(ix) == Flag::WHITE) {
						flagsDijkstra_.at(ix) = Flag::BLACK;
						flagModificationList_.push_back(ix);
						q_next.push(ix);
					}
				}
			}
			std::swap(q, q_next);
			d = d + 1;
		}
	} else {						// Dijkstra's Algorithm
		PQDijkstra q;
		handlesDijkstra_.at(sourceIndex) = q.push(DijkstraHeapData(0, sourceIndex));
		flagsDijkstra_.at(sourceIndex) = Flag::BLACK;
		flagModificationList_.push_back(sourceIndex);
		while (!q.empty()) {
			auto a = q.top();
			q.pop();
			if (a.key >= options.getDistanceParameter()) {
				break;
			}
			out.push_back(Neighbor(a.index, a.key));
			flagsDijkstra_.at(a.index) = Flag::BLACK;
			const Node node = graph_.getNode(a.index);
			for (unsigned int i = 0; i < node.size(); i++) {
				unsigned int ix = node.getEdge(i);
				double w = node.getEdgeWeight(i);
				if (flagsDijkstra_.at(ix) == Flag::WHITE) {
					flagsDijkstra_.at(ix) = Flag::GRAY;
					flagModificationList_.push_back(ix);
					handlesDijkstra_.at(ix) = q.push(DijkstraHeapData(a.key + w, ix));
				}
				else if (flagsDijkstra_.at(ix) == Flag::GRAY) {
					auto h = handlesDijkstra_.at(ix);
					if ((*h).key > a.key + w) {
						(*h).key = a.key + w;
						q.increase(h);
					}
				}
			}
		}
	}
	for (int i = 0; i < flagModificationList_.size(); i++) {
		unsigned int index = flagModificationList_.at(i);
		flagsDijkstra_.at(index) = Flag::WHITE;
	}
	flagModificationList_.clear();
	return std::move(out);
}