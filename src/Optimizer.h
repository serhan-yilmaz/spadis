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

#ifndef HAS_SPADIS_OPTIMIZER
#define HAS_SPADIS_OPTIMIZER

#include <vector>
#include <numeric>
#include <algorithm>
#include <queue>
#include <utility>
#include <functional>
#include <boost/heap/binomial_heap.hpp>
#include <queue>

#include "Graph.h"
#include "Options.h"

namespace spadis {
	constexpr int BETA_INFINITE = -1;
	struct OptimizerValue {
		double real = 0;
		double infinite = 0;
		OptimizerValue() {}
		OptimizerValue(double _real) : real(_real) {}
		OptimizerValue(double _real, double _infinite) : real(_real), infinite(_infinite){}
		OptimizerValue& operator+=(const OptimizerValue& rhs) {
			real     += rhs.real;
			infinite += rhs.infinite;
			return *this;
		}
		OptimizerValue& operator-=(const OptimizerValue& rhs) {
			real     -= rhs.real;
			infinite -= rhs.infinite;
			return *this;
		}
		bool operator>(OptimizerValue const & rhs) const {
			return infinite == rhs.infinite ? real > rhs.real : infinite > rhs.infinite;
		}
		bool operator<(OptimizerValue const & rhs) const {
			return infinite == rhs.infinite ? real < rhs.real : infinite < rhs.infinite;
		}
	};
	struct Solution {
		std::vector<bool> indicators;
		OptimizerValue optimizerValue;
	};
	class Optimizer {
	public:
		Optimizer(std::vector<double> scores, Graph graph);
		void select(const Options fso);
		std::vector<Solution> getSolutions() const;

	private:
		enum class Flag {
			WHITE, GRAY, BLACK
		};
		struct DijkstraHeapData {
			double key;
			unsigned int index;
			DijkstraHeapData(double _key, unsigned int _index) : key(_key), index(_index){ }
			bool operator<(DijkstraHeapData const & rhs) const { return key > rhs.key; }
		};
		struct OptimizerHeapData {
			OptimizerValue key;
			unsigned int index;
			OptimizerHeapData(OptimizerValue _key, unsigned int _index) : key(_key), index(_index) { }
			bool operator<(OptimizerHeapData const & rhs) const { return key < rhs.key; }
		};

		typedef boost::heap::binomial_heap<DijkstraHeapData> PQDijkstra;
		typedef PQDijkstra::handle_type HandleDijkstra;
		typedef std::pair<unsigned int, double> Neighbor;
		typedef boost::heap::binomial_heap<OptimizerHeapData> OptimizationQueue;
		typedef OptimizationQueue::handle_type OptimizationHandle;

		std::vector<Neighbor> findNeighbors(unsigned int sourceIndex, const Options options);

		std::vector<Solution> solutions_;
		std::vector<double> scores_;
		std::vector<Flag> flagsDijkstra_;
		std::vector<HandleDijkstra> handlesDijkstra_;
		std::vector<unsigned int> flagModificationList_;
		Graph graph_;
	};
}

#endif









