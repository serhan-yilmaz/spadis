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

#ifndef HAS_SPADIS_GRAPH
#define HAS_SPADIS_GRAPH

#include <vector>
#include "Node.h"
namespace spadis {
	class Graph {
	public:
		Graph();
		Graph(std::vector<Node> nodeList, bool isWeighted);
		unsigned int getNumberOfNodes() const;
		Node getNode(unsigned int index) const;
		bool isWeighted() const;

	private:
		std::vector<Node> nodeList_;
		bool isWeighted_ = false;
	};
}

#endif
