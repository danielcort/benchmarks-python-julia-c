#include "rando.hpp"
#include <iostream>
#include "Array.hpp"
#include "RandomFeedMatrix.hpp"
#include <map>
#include <sys/time.h>
#include <string>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <fstream>
#include "protos_lapack.hpp"
using namespace std;
using namespace odes;
using namespace Arrays;
double get_time() {
  struct timeval tv;
  gettimeofday(&tv,0);
  return (double) tv.tv_sec+tv.tv_usec*1e-6;
}
int main()
{
  char hostnameC[HOST_NAME_MAX];
  gethostname(hostnameC, HOST_NAME_MAX);
  auto hostname = string(hostnameC);
  cout<<"hostname: "<<hostname<<endl;
  map<int,double> times;

  int sizeG=2048;
  int size=sizeG;
  int liminf=2;
  int loop=1;
  while(size>liminf)
    {
      rando R;
      Array<double,2>  M(size,size+1);
      cout<<size<<endl;
      int lda=size;
      int *piv=new int[size];
      int info;
      double t1=get_time();
      for(int iter=0;iter<loop;iter++)
	{
	  RandomFeedMatrix(M,R);
	  //factorMatrix(M);
	  dgetrf_(&size,&size,M.data(),&lda,piv,&info);
	}
      double t2=get_time();
      double t=(t2-t1)/loop;
 
      times[size]=t;
      size /=2;
      if(size<=128)
	loop*=4;
      else
	loop*=8;
    }

  size=sizeG; loop=1;
  while(size>liminf)
    {
      
      rando R;
      Array<double,2>  M(size,size+1);
      
      double t1=get_time();
      for(int iter=0;iter<loop;iter++)
	RandomFeedMatrix(M,R);
      double t2=get_time();
      double t=(t2-t1)/loop;
  
      times[size]-=t;
      size /=2;
      if(size<=128)
	loop*=4;
      else
	loop*=8;
    }

  ofstream f; f.open("../RunningOn"+hostname);
  for(auto e : times)
      f<<e.first<<" "<<e.second<<endl;
  f.close();
  cout<<"end"<<endl;
}
