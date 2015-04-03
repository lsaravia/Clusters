/* C++ implementation of the
   Hoshen-Kopelman algorithm for
   cluster labeling.
  
   Copyright (c) 2015 by Leonardo A. Saravia
   Distributed under the terms of the GNU Public License.

   Derived from C code by Tobin Fricke 
   http://www.ocf.berkeley.edu/~fricke/projects/hoshenkopelman/hoshenkopelman.html

*/

#include "smattpl.h"
#include <algorithm> 
#include <assert.h>
#include "hk.h" 
#include <unordered_map>

/* Implementation of Union-uf_Find Algorithm */


/* The 'labels' array has the meaning that labels[x] is an alias for the label x; by
   following this chain until x == labels[x], you can find the canonical name of an
   equivalence class.  The labels start at one; labels[0] is a special value indicating
   the highest label already used. */
   

/*  find returns the canonical label for the equivalence class containing x */

int hoshen_kopelman::uf_find(int x) {
  int y = x;
  while (labels[y] != y)
    y = labels[y];
  
  while (labels[x] != x) {
    int z = labels[x];
    labels[x] = y;
    x = z;
  }
  return y;
}

/*  uf_union joins two equivalence classes and returns the canonical label of the resulting class. */

int hoshen_kopelman::uf_union(int x, int y) {
  return labels[uf_find(x)] = uf_find(y);
}

/*  uf_make_set creates a new equivalence class and returns its label */

int hoshen_kopelman::make_set() {
  labels[0] ++;
  assert(labels[0] < n_labels);
  labels[labels[0]] = labels[0];
  return labels[0];
}

/*  uf_intitialize sets up the data structures needed by the union-find implementation. */

void hoshen_kopelman::initialize(int max_labels) {
  n_labels = max_labels;
  labels = new int[n_labels];
  labels[0] = 0;
}

/* End Union-Find implementation */

/* Label the clusters in "matrix".  Return the total number of clusters found. */

int hoshen_kopelman::clusters(simplmat<int>& matrix) {

    int m = matrix.getRows();
    int n = matrix.getCols();
    initialize(m * n / 2);

    /* scan the matrix */

    for (int i=0; i<m; i++)
      for (int j=0; j<n; j++)
        if (matrix(i,j)) {                        // if occupied ...

          int up = (i==0 ? 0 : matrix(i-1,j));    //  look up  
          int left = (j==0 ? 0 : matrix(i,j-1));  //  look left

          switch (!!up + !!left) {

          case 0:
            matrix(i,j) = make_set();      // a new cluster
            break;

          case 1:                              // part of an existing cluster
            matrix(i,j) = max(up,left);       // whichever is nonzero is labelled
            break;

          case 2:                              // this site binds two clusters
            matrix(i,j) = uf_union(up, left);
            break;
        }

    }

    /* apply the relabeling to the matrix */

    /* This is a little bit sneaky.. we create a mapping from the canonical labels
       determined by union/find into a new set of canonical labels, which are 
       guaranteed to be sequential. */

    int *new_labels = new int[n_labels](); // allocate array, initialized to zero

    for (int i=0; i<m; i++)
      for (int j=0; j<n; j++)
        if (matrix(i,j)) {

          int x = uf_find(matrix(i,j));

          if (new_labels[x] == 0) {
              new_labels[0]++;
              new_labels[x] = new_labels[0];
          }

          matrix(i,j) = new_labels[x];
        }

    int numClus = new_labels[0];

    delete []new_labels;

    return numClus;
}

/* Label the clusters in "matrix" belonging to different types.  
 * Return the total number of clusters found. 
 */
vector<pair<int, unsigned int>> hoshen_kopelman::multiClusters(simplmat<int>& matrix,const string& output) {

    int m = matrix.getRows();
    int n = matrix.getCols();
    initialize(m * n / 2);
    simplmat<int> mat1sp(matrix);
    
    int sumSp=0;
    typedef unordered_map<int,unsigned int> CounterMap;
    CounterMap counts;
    vector<pair<int, unsigned int>> speciesClus;
    
    // First find the different species in matrix
    // this counts the number of each species
    for(int i=0; i<m; i++)
        for(int j=0; j<n; j++)
          if (int e=matrix(i,j)) {                        
                sumSp++;
                CounterMap::iterator i(counts.find(e));
                if (i != counts.end()){
                    i->second++;
                } else {
                    counts[e] = 1;
                    // numSp++; total number of species = counts.size()
               }
            }
    int totClus=0;
    //cout << "Total species: " << counts.size()<< endl;
    
    // Then find the clusters for each species 
    // build the matriz mat1sp
    //
    for(auto iter=counts.begin(); iter!=counts.end(); ++iter)
    {
        //cout << iter->first << ":" << iter->second << ":";
        int e =iter->first;
        for(int i=0; i<m; i++)
            for(int j=0; j<n; j++)
                if (e==matrix(i,j)) { 
                    mat1sp(i,j)=matrix(i,j);
                } else
                    mat1sp(i,j)=0;
        // find the clusters;
        int numClus =clusters(mat1sp);
        //cout << numClus << ":";

        // Count the cluster sizes and fill an unordered_map
        // Calculate the max cluster 
        //
        int maxClus=0;
        if(numClus>1)
        {
            vector <int> sizeClus(numClus,0);
            for(int i=0; i<m; i++)
                for(int j=0; j<n; j++)
                    if ((e=mat1sp(i,j))) { 
                        sizeClus[e-1]++;
                      
                    }
            // Calculate max cluster by species
            maxClus = *max_element(sizeClus.begin(),sizeClus.end());
            iter->second = maxClus;
            e = iter->first;
            for(const auto &s : sizeClus)
                speciesClus.push_back(make_pair(e,s));
        } 
        else {
            maxClus = iter->second;
            speciesClus.push_back(make_pair(e,maxClus));
        }
        //cout << maxClus << "\t|\t";
        totClus += numClus;
    }

    if(output=="max")
    {
        auto maxClus = max_element(speciesClus.begin(), speciesClus.end(),
      [](const pair<int, unsigned>& p1, const pair<int, unsigned>& p2) {
        return p1.second < p2.second; });
        speciesClus.assign(1, make_pair(totClus,maxClus->second));
    }
    // Title 
//    cout << "Species\tmaxCluster\n";
//    
//    for(auto iter=counts.begin(); iter!=counts.end(); ++iter)
//    {
//        cout << iter->first << "\t" << iter->second << endl;
//    }            
//    cout << endl;
//    cout << endl;
//    
//    // Title 
//    cout << "Species\tClusterSize\n";
//    for(auto ites=speciesClus.begin(); ites!=speciesClus.end(); ++ites){
//        cout << ites->first << "\t" << ites->second << endl;
//    }
//    cout << endl;
    return(speciesClus);
}

/* This procedure checks to see that any occupied neighbors of an occupied site
   have the same label. */

void hoshen_kopelman::check_labelling(simplmat<int>& matrix) {
  int N,S,E,W;
  int m = matrix.getRows();
  int n = matrix.getCols();

  for (int i=0; i<m; i++)
    for (int j=0; j<n; j++)
      if (matrix(i,j)) {
      	N = ( i==0 ? 0 : matrix(i-1,j) );
      	S = ( i==m-1 ? 0 : matrix(i+1,j) );
      	E = ( j==n-1 ? 0 : matrix(i,j+1) );
      	W = ( j==0 ? 0 : matrix(i,j-1) );
      	
      	assert( N==0 || matrix(i,j)==N );
      	assert( S==0 || matrix(i,j)==S );
      	assert( E==0 || matrix(i,j)==E );
      	assert( W==0 || matrix(i,j)==W );
      }
}
