#ifndef H5_SETUP_MPI_H
#define H5_SETUP_MPI_H

#include "mpi.h"
#include "utilities.h"


int GetMPILocalRank(){
  int flag,rank;
  MPI_Initialized(&flag);
  if(flag)MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  else{rank=-999;};
  return rank;
}

int IsMotherRank(){
  bool isMoth = false;
  int rank = GetMPILocalRank();
  if(rank==0)isMoth=true;
  return isMoth;
}

#endif
