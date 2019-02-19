#include <iostream>
#include <algorithm>
#include <cmath>
#include <map>
#include <string>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <fstream>
using namespace std;

double get_time() {
    struct timeval tv;
    gettimeofday(&tv,0);
    return (double) tv.tv_sec+tv.tv_usec*1e-6;
}

struct F {
  inline double operator()(double x) const {return exp(-x)*x*x;}
};

struct G {
  inline double operator()(double x) const
  {
    double h=0.0;
    if(x<0.5)
	 h=-exp(-x);
    else
      h= exp(x);
    return h*x*x;
  }
};

struct implicit{
  inline double operator()(double t) const 
  {
    // implicit = root of  4*sin(x)-exp(x)+t
    // Newton iterations, starting from zero:
    double x=0.0;
    double F= 4*sin(x)-exp(x)+t;
    while(abs(F)> 1.e-15)
      {
	x-= F/(4*cos(x) - exp(x));
	F=  4*sin(x)-exp(x)+t;
      }
    return x;
  }
};

template<class Fonc> double trapz(double a, double b, int n)
{
  const Fonc F;
  const auto h=(b-a)/n;
  auto sum=0.5*(F(a)+F(b));
  for(int i=1;i<=n;i++)
    sum+=F(i*h);
  
  return sum*h;
}
string host()
{
  char hostnameC[HOST_NAME_MAX];
  gethostname(hostnameC, HOST_NAME_MAX);
  return  string(hostnameC);
}
int main()
{
  auto hostname = host();
  cout<<"hostname: "<<hostname<<endl;
  ofstream f; f.open("../RunningOn"+hostname);
  
  const int loops=10000;
  double sum;
  
  double t1=get_time();
  for(int i=0;i<loops;i++)
    sum= trapz<F>(0.,1.,1000);
  double t2=(get_time()-t1)/loops;
  cout<<"f, computing time: "<<t2<<endl;
  cout<<sum<<endl;
  f<<"f: "<<t2<<endl;

  t1=get_time();
  for(int i=0;i<loops;i++)
    sum= trapz<G>(0.,1.,1000);
  t2=(get_time()-t1)/loops;
  cout<<"g, computing time: "<<t2<<endl;
  cout<<sum<<endl;
  f<<"g: "<<t2<<endl;
  
  t1=get_time();
  for(int i=0;i<loops;i++)
    sum= trapz<implicit>(0.,1.,1000);
  t2=(get_time()-t1)/loops;
  cout<<"implicit, computing time: "<<t2<<endl;
  cout<<sum<<endl;
  f<<"implicit: "<<t2<<endl;

  f.close();

  cout<<"end."<<endl;
}