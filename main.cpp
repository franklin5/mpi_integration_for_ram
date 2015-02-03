#include <mpi.h>
#include <iostream>
#include "integrate.h"
using namespace std;
using namespace MPI;
#define root 0

int main(int argc, char** argv){
  Init(argc,argv);
  int rank, size;
  rank = COMM_WORLD.Get_rank();
  size = COMM_WORLD.Get_size();

  if (rank == root) {
    cout << " I am rank " << rank << " which is root " << root << endl;
  }

  cIntegrate integrate_obj(rank,size,root);
  integrate_obj.integrate();


  Finalize();
  return 0;
}
