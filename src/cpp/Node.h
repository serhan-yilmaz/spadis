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

#ifndef HAS_SPADIS_NODE
#define HAS_SPADIS_NODE

#include <vector>
namespace spadis {
	class Node {
	public:
		Node(unsigned int index_);
		Node(unsigned int index_, bool weighted_);

		unsigned int getNumberOfConnectedNodes() const;
		unsigned int getConnectedNode(unsigned int index) const;
		unsigned int getEdge(unsigned int index) const;
		double getEdgeWeight(unsigned int index) const;
		unsigned int size() const;
		bool addConnection(unsigned int node);
		bool addConnection(unsigned int node, double weight);
		unsigned int getIndex() const;
		void reserve(unsigned int n);

	private:
		std::vector<unsigned int> connectedNodes_;
		std::vector<double> edgeWeights_;
		unsigned int index_;
		bool weighted_;
	};
}

#endif 




