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

#ifndef HAS_SPADIS_OPTIONS
#define HAS_SPADIS_OPTIONS

#include <limits>
#include <vector>

namespace spadis {
	class Options {
	public:
		unsigned int getK() const;
		double getDistanceParameter() const;
		double getBeta(unsigned int) const;
		unsigned int getBetaSize() const;

		void setK(unsigned int n);
		void setDistanceParameter(double D);
		void addBeta(double b);

	private:
		unsigned int k_ = 0;
		double D_ = 0;
		std::vector<double> betaList_;

	};
}

#endif




