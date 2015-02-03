/*
 * tBdG.cpp
 *
 *  Created on: Feb 03, 2015
 *      Author: ld7
 */
#include <iostream>
#include "integrate.h"
#include "lgwt.h"
#include <math.h>
using namespace std;
using namespace MPI;
void cIntegrate::integrate(){
  int nk;
  double local_sum;
  _gauss_k = new double [_NK];
  _gauss_w_k = new double [_NK];
  gauss_lgwt(_NK,-_kc,_kc,_gauss_k,_gauss_w_k); // This function calls the routine in lgwt.h and predefine a mesh grid from -kc to kc with NK point. _gauss_k records grid value and _gauss_w_k records weight value on the corresponding grid. 
  distribute(_NK);
  for (int ig = 0; ig < _size; ++ig) {
    if (ig==_rank) {
      for (int i = 0; i < recvcount; ++i) {
	nk = recvbuf[i];
	local_sum += _gauss_w_k[nk] * integrand(_gauss_k[nk]);
      }
    }
  }
  for(int ig = 0; ig<_size; ++ig) {
    if (ig ==_rank){
      cout << "hey, I am rank " << ig << " and I have local sum value of " << local_sum << endl;
      cout << "hi there, I have a global sum value of " << _total_sum << " before broadcasting, which should be wrong or undefined." << endl;
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }
  MPI_Reduce(&local_sum, &_total_sum, 1, MPI_DOUBLE, MPI_SUM, _root, MPI_COMM_WORLD);
  if (_rank == _root) cout << "Now we are talking! We have global sum value at root: " << _total_sum << endl;
  MPI_Bcast(&_total_sum, 1, MPI_DOUBLE, _root, MPI_COMM_WORLD);
  for(int ig = 0; ig<_size; ++ig) {
    if (ig ==_rank){
      cout << "hey, I am rank " << ig << " and I have global sum value of " << _total_sum << endl;
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }
}

double  cIntegrate::integrand(double kx){
  return exp(-kx*kx)/sqrt(M_PI);
}

int cIntegrate::compute_count(int rank, int size, int NJOBS){
  int result;
  if (rank != size-1) {
    result = int(NJOBS/size);
  } else {
    result = int(NJOBS/size) + NJOBS % size;
  }
  return result;
}



void cIntegrate::distribute(int NJOBS){
  if (_rank == _root){ // send process is only root significant                                                                                               
    sendbuf = new int[NJOBS];
    for(int i = 0; i< NJOBS; ++i){
      sendbuf[i] = i;
    }
    sendcounts = new int[_size];
    displs = new int[_size];
    for(int i=0; i<_size; i++){
      sendcounts[i] = compute_count(i,_size,NJOBS);
      displs[i] = i*int(NJOBS/_size);
    }
  }
  recvcount = compute_count(_rank,_size,NJOBS); // This is a rank dependent variable.                                                                         
  recvbuf = new int[recvcount]; // So is this array: rank dependent size                                                                                      
  MPI_Scatterv(sendbuf,sendcounts,displs,MPI_INT,recvbuf,recvcount,MPI_INT,_root,COMM_WORLD);
  if (_root==_rank){
    recvcounts = new int[_size];
    displs_r = new int[_size];
    offset = 0;
    for(int ig = 0;ig<_size;++ig){
      recvcounts[ig] = compute_count(ig,_size,NJOBS);
      displs_r[ig] = offset;
      offset += recvcounts[ig];
    }
  }

}

