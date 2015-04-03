/* C++ implementation of the
   Hoshen-Kopelman algorithm for
   cluster labeling.
  
   Copyright (c) 2015 by Leonardo A. Saravia
   Distributed under the terms of the GNU Public License.

   Derived from C code by Tobin Fricke <tobin@splorg.org>
   http://www.ocf.berkeley.edu/~fricke/projects/hoshenkopelman/hoshenkopelman.html

   gcc-3.0 -Wall -std=c99 hk.c -o hk

*/
#ifndef HK_H
#define HK_H


#include "smattpl.h"
#include <algorithm> 
#include <assert.h>
using namespace std;

/* Implementation of Union-Find Algorithm */


/* The 'labels' array has the meaning that labels[x] is an alias for the label x; by
   following this chain until x == labels[x], you can find the canonical name of an
   equivalence class.  The labels start at one; labels[0] is a special value indicating
   the highest label already used. */

class hoshen_kopelman {

  int *labels;
  int  n_labels;     /* length of the labels array */

  public:

  hoshen_kopelman() { n_labels = 0;};

  ~hoshen_kopelman() { if( labels!=NULL) delete []labels; };

  void initialize(int max_labels);

  int uf_find(int x);

  int uf_union(int x, int y);

  int clusters(simplmat<int>& matrix);
  
  vector<pair<int, unsigned int>> multiClusters(simplmat<int>& matrix,const string& output="");
  
  int make_set();

  void check_labelling(simplmat<int>& matrix);
  

};

#endif

