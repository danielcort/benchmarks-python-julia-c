#pragma once
#include "PreSparse.hpp"
#include <memory>
#include <tuple>
#include <iostream>
#include <exception>
//#include <iostream>
//#define DEBUG
class Csr
{
  std::unique_ptr<double[]> V;
  std::unique_ptr<int[]> ia,ja;
  int nlig,nc;
public:
  Csr(const PreSparse& P)
  {
    int ncol;
    std::tie(nlig,ncol,nc)=P.sizes();
    cout<<"--> "<<nlig<<" "<<ncol<<" "<<nc<<endl;
    if(nlig!=ncol)
      throw std::runtime_error("Csr: not a square matrix: nlig, ncol= "+
			       to_string(nlig)+" "+to_string(ncol));
    ia=std::make_unique<int[]>(nlig+1);
    ja=std::make_unique<int[]>(nc);
    V=std::make_unique<double[]>(nc);
    //
    for(int i=0;i<nlig+1;i++) ia[i]=0.0;
    for(int i=0;i<nc;i++) ja[i]=0;
    for(int i=0;i<nc;i++) V[i]=0.0;
    //
    int pia=0,curia=0;
    auto Iend=P.cend();
    for(auto I=P.cbegin();I!=Iend;I++)
      {
	
	int cc=I->first.first;
	//cout<<curia<<" "<<cc<<" "<<I->first.second<<" "<<pia<<endl;
	if(cc!=curia)
	  {
	    //cout<<"cc "<<cc<<" "<<pia<<endl;
	    curia=cc;
	    ia[curia]=pia; //start of the next line
	    // for(int z=0;z<=curia;z++) cout<<ia[z]<<" ";
	    // cout<<" <---"<<endl;
	    
	  }
	ja[pia]=I->first.second;
	//cout<<cc<<" "<<curia<<" "<<I->second<<" "<<pia<<endl; //exit(0);
	V[pia++]=I->second;
      }
    ja[curia+1]=pia+1;
  
  }
  ~Csr()
  {
  }
  void prod(std::unique_ptr<double[]>& In, std::unique_ptr<double[]>& Out)
  {
    for(int i=0;i<nlig;i++)
      {
	double t=0.0;
	for(int j=ia[i];j<ia[i+1]-1;j++)
        {
#ifdef DEBUG
          if(j<0||ja[j]<0)
            throw  std::runtime_error("Csr: bad indice.");
#endif
          t+=V[j]*In[ja[j]];
        }
      Out[i]=t;
    }

  }
  void print()
  {
    for(int i=0;i<10;i++) cout<<ia[i]<<" "; cout<<endl;
    std::cout<<"order= "<<nlig<<" nzeros= "<<nc<<std::endl;
    for(int i=0;i<nlig;i++)
    {
      int d= i==0? 0:ia[i-1];
      std::cout<<"line: "<<i<<" start at: "<<d<<
	" end at:"<<ia[i+1]-1<<" len: "<<ia[i+1]-d<<std::endl;
      for(int j=d;j<ia[i+1];j++)
	std::cout<<'('<<ja[j]<<' '<<V[j]<<')'<<' ';
      std::cout<<std::endl;
    }
  }
};
