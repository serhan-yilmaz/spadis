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

#include "Node.h"

spadis::Node::Node(unsigned int _id) : index_(_id)
{
	weighted_ = false;
}

spadis::Node::Node(unsigned int _id, bool _weighted) : index_(_id), weighted_(_weighted)
{
}
 
unsigned int spadis::Node::getNumberOfConnectedNodes() const
{
	return connectedNodes_.size();
}

unsigned int spadis::Node::getConnectedNode(unsigned int index) const
{
	return connectedNodes_.at(index);
}

unsigned int spadis::Node::getEdge(unsigned int index) const
{
	return connectedNodes_.at(index);
}

double spadis::Node::getEdgeWeight(unsigned int index) const
{
	return weighted_ ? edgeWeights_.at(index) : 0;
}

unsigned int spadis::Node::size() const
{
	return connectedNodes_.size();
}

bool spadis::Node::addConnection(unsigned int nodeIndex)
{	
	if (weighted_) {
		return false;
	}
	connectedNodes_.push_back(nodeIndex);
	return true;
}

bool spadis::Node::addConnection(unsigned int nodeIndex, double weight)
{
	if (!weighted_) {
		return false;
	}
	connectedNodes_.push_back(nodeIndex);
	edgeWeights_.push_back(weight);
	return true;
}

unsigned int spadis::Node::getIndex() const
{
	return index_;
}

void spadis::Node::reserve(unsigned int nNodes)
{
	connectedNodes_.reserve(nNodes);
	if (weighted_) {
		edgeWeights_.reserve(nNodes);
	}
}
