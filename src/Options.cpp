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

#include "Options.h"

unsigned int spadis::Options::getK() const
{
	return k_;
}

double spadis::Options::getDistanceParameter() const
{
	return D_;
}

double spadis::Options::getBeta(unsigned int index) const
{
	return betaList_.at(index);
}

unsigned int spadis::Options::getBetaSize() const
{
	return betaList_.size();
}

void spadis::Options::setK(unsigned int k)
{
	k_ = k;
}

void spadis::Options::setDistanceParameter(double D)
{
	D_ = D;
}

void spadis::Options::addBeta(double beta)
{
	betaList_.push_back(beta);
}
