#include <iostream>
#include <algorithm>
#include <string>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <fstream>
#include <cmath>
#include <ctime>
#include <memory>
using namespace std;

double get_time() {
    struct timeval tv;
    gettimeofday(&tv,0);
    return (double) tv.tv_sec+tv.tv_usec*1e-6;
}
clock_t ck() {return clock();}
string host()
{
  char hostnameC[HOST_NAME_MAX];
  gethostname(hostnameC, HOST_NAME_MAX);
  return  string(hostnameC);
}

void Init(std::unique_ptr<double[]>&  X,double L,int size)
{
  double h=L/size;
  for(int i=0;i<size;i++)
    if(i>size/8 && i<size/2+size/8)
      X[i]=1.-2*(i-size/8)*h/L;
    else
      X[i]=0.0;
}
void lapl_1(int size,std::unique_ptr<double[]>& In,
	    std::unique_ptr<double[]>& Out)
{
  double h2= 1./(size*size);
  for(int i=1;i<size-1;i++)
    Out[i]= h2*(In[i-1]- 2.0*In[i]+ In[i+1]);
  
}
double  dotest(int size)
{
  auto A=std::make_unique<double[]>(size);
  auto B=std::make_unique<double[]>(size);
  Init(A,1.,size); Init(B,1.,size);
  double T=0;
  double Tnew=std::pow(10.,20);
  int iter=10000;
  bool ok=false;
  do
    {
      double t1=get_time();
      for(int i=0;i<iter;i++)
	lapl_1(size,A,B);
      Tnew=(get_time()-t1);
  
      ok= std::abs(Tnew-2*T)/Tnew<0.1||iter>1000000;
      T=Tnew;
      if(!ok) iter*=2;
    }
  while(!ok);
  return T/iter;
}
  
int main()
{
 

  auto hostname = host();
  cout<<"hostname: "<<hostname<<endl;
  ofstream fb; fb.open("../RunningOn"+hostname+"_lapl_1");
  int sizemax=std::pow(10,5);
  int size=16;
  while(size<sizemax)
    {
      auto T=dotest(size);
      double flops=size*4/T;
      cout<<size<<" "<<T<<", Gflops/s: "<<flops*pow(10,-9)<<endl;
      fb<<size<<" "<<T<<endl;
      size*=2;
    }
  fb.close();
  cout<<"end"<<endl;
}
