#include<iostream>
#include <scitbx/error.h>
#include <annlib_adaptbx/ann_adaptor.h>

namespace af = scitbx::af;

using namespace annlib_adaptbx;

AnnAdaptor::AnnAdaptor(
  af::shared<ANNcoord> data,int dimension):
  dimension(dimension),
  k(1),
  eps(0.0){constructor_core(data,dimension);}

AnnAdaptor::AnnAdaptor(
  af::shared<ANNcoord> data,int dimension,int nk):
  dimension(dimension),
  k(nk),
  eps(0.0){constructor_core(data,dimension);}

void
AnnAdaptor::constructor_core(af::shared<ANNcoord> data,int dimension){
  SCITBX_ASSERT (data.size() > 0);
  SCITBX_ASSERT (data.size()%dimension == 0);
  af::shared<ANNcoord>::iterator yyy = data.begin();  //double*
  af::shared<ANNcoord*> xxx(data.size()/dimension);
  for (int i=0; i<data.size()/dimension; ++i,yyy+=dimension){
    xxx[i]=yyy;
  }
  persist_data = data;
  persist_ptr = xxx;
  ANNpointArray dataPts = xxx.begin(); //double**
  annptr A(new ANNkd_tree(                   // build search structure
                       dataPts,              // the data points
                       data.size()/dimension,// number of points
                       dimension));
  kdTree = A;
  //not known if the annlib requires the data to be persistent
  // but initial tests led to the implementation of persist_data and persist_ptr
}

AnnAdaptor::~AnnAdaptor(){
  //The annClose() call is removed because it is not clear that
  //KD_TRIVIAL really leads to a memory leak.  Also, if it does, could it
  //just be declared static and leave it at that?
  //annClose();//because of this, it is not possible to have two instances
             //so not thread safe!!
}

void AnnAdaptor::query(af::shared<ANNcoord> data){
  af::shared<ANNcoord>::iterator queryPt = data.begin();
  SCITBX_ASSERT (data.size()%dimension == 0);
  nn = af::flex_int(data.size()*k/dimension);
  distances = af::flex_double(data.size()*k/dimension);
  af::shared<ANNidx>::iterator nnIdx = nn.begin();
  af::shared<ANNdist>::iterator dists = distances.begin();
  for (;queryPt!=data.end();queryPt+=dimension,nnIdx+=k,dists+=k){
    kdTree->annkSearch(// search
      queryPt,
      k,
      nnIdx,// nearest neighbors (returned)
      dists,// distance (returned)
      eps);
  }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
AnnAdaptorSelfInclude::AnnAdaptorSelfInclude(
  af::shared<annself_include::ANNcoord> data,int dimension):
  dimension(dimension),
  k(1),
  eps(0.0){constructor_core(data,dimension);}

AnnAdaptorSelfInclude::AnnAdaptorSelfInclude(
  af::shared<annself_include::ANNcoord> data,int dimension,int nk):
  dimension(dimension),
  k(nk),
  eps(0.0){constructor_core(data,dimension);}

void
AnnAdaptorSelfInclude::constructor_core(af::shared<annself_include::ANNcoord> data,int dimension){
  SCITBX_ASSERT (data.size() > 0);
  SCITBX_ASSERT (data.size()%dimension == 0);
  af::shared<annself_include::ANNcoord>::iterator yyy = data.begin();  //double*
  af::shared<annself_include::ANNcoord*> xxx(data.size()/dimension);
  for (int i=0; i<data.size()/dimension; ++i,yyy+=dimension){
    xxx[i]=yyy;
  }
  persist_data = data;
  persist_ptr = xxx;
  annself_include::ANNpointArray dataPts = xxx.begin(); //double**
  annptr A(new annself_include::ANNkd_tree(                   // build search structure
                       dataPts,              // the data points
                       data.size()/dimension,// number of points
                       dimension));
  kdTree = A;
  //not known if the annlib requires the data to be persistent
  // but initial tests led to the implementation of persist_data and persist_ptr
}

AnnAdaptorSelfInclude::~AnnAdaptorSelfInclude(){
  //The annClose() call is removed because it is not clear that
  //KD_TRIVIAL really leads to a memory leak.  Also, if it does, could it
  //just be declared static and leave it at that?
  //annClose();//because of this, it is not possible to have two instances
             //so not thread safe!!
}

void AnnAdaptorSelfInclude::query(af::shared<annself_include::ANNcoord> data){
  af::shared<annself_include::ANNcoord>::iterator queryPt = data.begin();
  SCITBX_ASSERT (data.size()%dimension == 0);
  nn = af::flex_int(data.size()*k/dimension);
  distances = af::flex_double(data.size()*k/dimension);
  af::shared<annself_include::ANNidx>::iterator nnIdx = nn.begin();
  af::shared<annself_include::ANNdist>::iterator dists = distances.begin();
  for (;queryPt!=data.end();queryPt+=dimension,nnIdx+=k,dists+=k){
    kdTree->annkSearch(// search
      queryPt,
      k,
      nnIdx,// nearest neighbors (returned)
      dists,// distance (returned)
      eps);
  }
}
