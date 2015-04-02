
# hk: Hoshen-Kopelman algorithm for cluster sizes of multiple/single types

This program implements a C++ class with Hoshen-Kopelman algorithm for cluster labeling of multiple types

The input file is a matrix with different <int> values (species)
and I it returns the cluster sizes and max cluster of of all species.

## Usage 

    hk file.sed BI|SP

An file.sed is ascii file with the dimension in the first line, the kind of data SP|BI in the second line and a matrix in subsecuent lines. An example of the sed format is given in test_sp.sed.

* sed file types: 

    BI: in this cases the program counts the cluster formed by values >0 and separated by 0

    SP: the clusters are formed by the different values and also separated by 0 

* It uses code from <https://github.com/lsaravia/SpatialAnalysis> 

    smattpl.h
    RWFile.h

# General License

    Copyright (c) 2015 Leonardo A. Saravia

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
