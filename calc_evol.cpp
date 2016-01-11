#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include "organism.h"
using namespace std;

#define POPSIZE 500 //TODO: 500 for biped, 250 for maze
#define TRIALS 300
#define SAMPLES_PER 20
#define MAXDIM 30
#define DELTA 0.01 //was 0.01

#define BIN 0.05

double dist(double *x,double *y,int dim) {
 double sum=0.0; 
 for (int k=0;k<dim;k++) {
  double d= x[k]-y[k];
  sum+=d*d;
 }
 return sqrt(sum);
}

int distinct(double *pts,int len,int dim) {
double points[POPSIZE*MAXDIM];
int size=0;

for(int x=0;x<len;x++) {
 bool pass=true;
 for(int y=0;y<size;y++) 
  if(dist(&points[y*dim],&pts[x*dim],dim)<DELTA) {
   pass=false;
   break;
  }
  
  if(pass) {
   for(int i=0;i<dim;i++) {
   points[size*dim+i]=pts[x*dim+i];
   }
   size++;
  }
 }
 return size;
}
// Compute heterogeneity of offspring's performance, which represents the evolvability of their parent.
double distnict_per(std::vector<NEAT::Organism*>&pts, int len, int index)
{
double sum = 0;

for (int x =0;x<len;x++)
{
	//cout<<"pts[index]- pts[x] : "<<pts[index] <<" -"<< pts[x]<<" = "<< std::abs(pts[index]- pts[x]) <<"\n";//Henok debugging
	sum += std::abs(pts[index]->fitness- pts[x]->fitness);
}
return sum/(len-1);
}
double bin_metric( std::vector<NEAT::Organism*> offsorgs,int len)
{
	std::vector<vector<double> > bins;
    std::vector<double> subs;

    subs.push_back(offsorgs[0]->fitness);
    bins.push_back(subs);
    bool pass;
  for(int x=1;x<len;x++) {
     for (int k =0;k<bins.size();k++)
       {
			 pass=true;
			 for(int y=0;y<bins[k].size();y++)
				 {
					//cout<<bins[k][y]<<" "<<offsorgs[x]<<" "<<fabs(bins[k][y]-offsorgs[x])<<endl;
					if(fabs(bins[k][y]-offsorgs[x]->fitness) > BIN)
					 {
					   pass=false;
					   break;
					 }
				 }
			 if(pass)
			 {
			    	bins[k].push_back(offsorgs[x]->fitness);
			    	break;
			 }
       }
     if(!pass)
          {
             std::vector<double> subs;
             subs.push_back(offsorgs[x]->fitness);
             bins.push_back(subs);
          }

   }
return bins.size();
}
