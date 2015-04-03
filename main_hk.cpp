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


    if( argc < 5)
    {
      cerr << "Hoshen kopelman algorithm Patch Stats\n";
      cerr << "Usage: hk inputFile outputFile fileType{BI,SP} output{max|}" << endl;
      cerr << "File type BI: patches are elements >0" << endl;
      cerr << "File type SP: patches are different elements (Species)" << endl;
      exit(1);
    }

    string fname = argv[1];
    string oname = argv[2];
    string type = argv[3];
    string out="";
    if(argc==5)
        out=argv[4];

    ofstream dout;
    bool privez=false;
    dout.open( oname.c_str(), ios::in );
    if( !dout )
            privez=true;

    dout.close();
    dout.clear();
    dout.open( oname.c_str(), ios::app );
    if( !dout )
    {
            cerr << "Cannot open output file.\n";
            return 0;
    }
        
    hoshen_kopelman c;

    bool ff = file.ReadSeed(fname.c_str(), matrix,type.c_str());
    if(!ff)
        ff = file.ReadSeed(fname.c_str(), matrix,"BI"); // tries the default type BI
    if(!ff)
        exit(1);


    if(type=="BI"){

      /* Process the matrix */
        int numClus = c.clusters(matrix);

        // c.check_labelling(matrix);
        // Count the clusters and max cluster
        //
        int e=0;
        vector <int> sizeClus(numClus,0);
        int m = matrix.getRows();
        int n = matrix.getCols();

        for(int i=0; i<m; i++)
            for(int j=0; j<n; j++)
                if ((e=matrix(i,j))) { 
                    sizeClus[e-1]++;

                }

        int maxClus = *max_element(sizeClus.begin(),sizeClus.end());

        // Output the sizes of clusters
        if(out=="max"){

            if(privez) dout << "numClusters\tmaxCluster\n";
            dout << numClus << "\t" << maxClus << endl;
        }
        else 
        {
            // Output all clusters sizes
            if(privez) dout << "Species\tClusterSize\n";
            for(const auto &s : sizeClus){
                dout << "1\t" << s << endl;
            }
        }
    }
    else
    {
        vector<pair<int, unsigned int>> clusters = c.multiClusters(matrix,out);      
        
        // Title
        if(privez){
            if(out=="max")
                dout << "numClusters\tmaxClusterSize\n";
            else
                dout << "Species\tClusterSize\n";
        }
        for(auto ites=clusters.begin(); ites!=clusters.end(); ++ites){
            dout << ites->first << "\t" << ites->second << endl;
            
        }
        
        //cout << "multi HK reports " << clusters << " clusters found\n";
    }

    //fname = fname.insert(fname.find(".sed"),"_out");
    //file.WriteSeed(fname.c_str(), matrix);


    return 0;
}




