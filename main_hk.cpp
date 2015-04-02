/* C++ implementation of the
   Hoshen-Kopelman algorithm for
   cluster labeling.
  
   Copyright (c) 2015 by Leonardo A. Saravia
   Distributed under the terms of the GNU Public License.

   Derived from C code by Tobin Fricke <tobin@splorg.org>
   http://www.ocf.berkeley.edu/~fricke/projects/hoshenkopelman/hoshenkopelman.html

*/

#include "hk.h" 
#include "RWFile.h"
#include <string> 


/* The sample program reads a matrix from a sed file, runs the HK algorithm on
   it, and outputs a new file with labelled clusters.

a sample input file is the following:

8 8
BI
1 1 1 1 1 1 1 1
0 0 0 0 0 0 0 1
1 0 0 0 0 1 0 1
1 0 0 1 0 1 0 1
1 0 0 1 0 1 0 1
1 0 0 1 1 1 0 1
1 1 1 1 0 0 0 1
0 0 0 1 1 1 0 1 

this sample input gives the following output:

 --input-- 
  1   1   1   1   1   1   1   1 
  0   0   0   0   0   0   0   1 
  1   0   0   0   0   1   0   1 
  1   0   0   1   0   1   0   1 
  1   0   0   1   0   1   0   1 
  1   0   0   1   1   1   0   1 
  1   1   1   1   0   0   0   1 
  0   0   0   1   1   1   0   1 
 --output-- 
  1   1   1   1   1   1   1   1 
  0   0   0   0   0   0   0   1 
  2   0   0   0   0   2   0   1 
  2   0   0   2   0   2   0   1 
  2   0   0   2   0   2   0   1 
  2   0   0   2   2   2   0   1 
  2   2   2   2   0   0   0   1 
  0   0   0   2   2   2   0   1 
HK reports 2 clusters found

*/

int main(int argc, char * argv[])
{
    RWFile file;
    simplmat <int> matrix;


    if( argc < 3)
    {
      cerr << "Hoshen kopelman algorithm Patch Stats\n";
      cerr << "Usage: hk inputFile fileType{BI,SP}" << endl;
      cerr << "File type BI: patches are elements >0" << endl;
      cerr << "File type SP: patches are different elements (Species)" << endl;
      exit(1);
    }

    string fname = argv[1];
    string type = argv[2];
    hoshen_kopelman c;
    int clusters;

    bool ff = file.ReadSeed(fname.c_str(), matrix,type.c_str());
    if(!ff)
      exit(1);


    if(type=="BI"){

      /* Process the matrix */
        clusters = c.clusters(matrix);

        c.check_labelling(matrix);

        cout << "HK reports " << clusters << " clusters found\n";
    }
    else
    {
        clusters = c.multiClusters(matrix);      

        cout << "multi HK reports " << clusters << " clusters found\n";

    }

    //fname = fname.insert(fname.find(".sed"),"_out");
    //file.WriteSeed(fname.c_str(), matrix);


    return 0;
}




