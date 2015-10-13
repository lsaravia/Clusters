
# hk: Hoshen-Kopelman algorithm for cluster sizes of multiple/single types

This program implements a C++ class with Hoshen-Kopelman algorithm for cluster labeling of single or multiple types

The compiled program takes as input a matrix with different <int> values (species)
and I it returns the cluster sizes or the max cluster of all species.

## Usage 

    hk inputFile.sed outputFile BI|SP outputType{max|""}

* **inputFile.sed** is ascii file with the dimension in the first line, the kind of data SP|BI in the second line and a matrix in subsecuent lines. An example of the sed format is given in test_sp.sed test_bi.sed

* **outputFile** if the file exist it adds the new output

* **BI|SP** are the sed file types: 

    BI: in this cases the program counts the cluster formed by values >0 and separated by 0

    SP: the clusters are formed by the different values and also separated by 0 

* **outputType** 
    * if null all the clusters sizes for all species are given. The first row is the spanning species and total abundance of it, if there is no spanning species 0 0 is returned. Then species and cluster size.

    * If max 4 pairs are returned :

            species_number, max_cluster_size 
            species_number, total_number_clusters
            species_number, abundance_species(which has the max cluster)
            spanning_species abundance_of_spanning_species
 

It uses code from <https://github.com/lsaravia/SpatialAnalysis> 

    smattpl.h
    RWFile.h
    RWFile.cpp

# General License

    Copyright (c) 2015 Leonardo A. Saravia
    Hoshen-Kopelman class derived from C code by Tobin Fricke 
   
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
