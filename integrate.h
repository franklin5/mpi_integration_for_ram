#ifndef integrate_h
#define integrate_h

#include <mpi.h>
#include <iostream>


class cIntegrate{
 private:
  int _NK;
  double _kc, _total_sum;
  double *_gauss_k, *_gauss_w_k;
 protected:
  int _rank, _size, _root;
  int recvcount, sendcount, stride;
  int *sendbuf, *recvbuf;
  int *sendcounts, *displs;
  int *recvcounts, *displs_r, offset;
 public:
  cIntegrate (const int rank, const int size, const int root):
    _NK(5000), _kc(10), _rank(rank), _size(size), _root(root){}
    ~cIntegrate(){
      delete []_gauss_k;
      delete []_gauss_w_k;
      if (_root==_rank) {
	delete []sendbuf;
	delete []sendcounts;
	delete []displs;
	delete []recvcounts;
	delete []displs_r;
      }
      delete []recvbuf;
    }
    int compute_count(int,int,int);
    void distribute(int);
    void integrate();
    double integrand(double);
};





#endif

