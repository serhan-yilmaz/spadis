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

#include "Graph.h"

spadis::Graph::Graph()
{

}

spadis::Graph::Graph(std::vector<spadis::Node> nodeList, bool isWeighted) : nodeList_(std::move(nodeList)), isWeighted_(isWeighted)
{

}

unsigned int spadis::Graph::getNumberOfNodes() const
{
	return nodeList_.size();
}

spadis::Node spadis::Graph::getNode(unsigned int index) const
{
	return nodeList_.at(index);
}

bool spadis::Graph::isWeighted() const
{
	return isWeighted_;
}
