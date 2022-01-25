#ifndef H5_WRITE_CHARS_H
#define H5_WRITE_CHARS_H

#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>
#include <highfive/H5DataType.hpp>
#include <highfive/H5File.hpp>

#include "h5_write_1D_chars.h"
#include "serialize_dataprods.h"
#include "utilities.h"


//#include "dkmeta.h"
#include "dk2nu.h"

#include <string>
#include <cstddef>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include "TH1D.h"
#include "TChain.h"
#include "TH2D.h"
#include "TFile.h"
#include "h5_setup_mpi.h"
#include "mpi.h"
#include "H5FDmpio.h"

using namespace HighFive;
using product_t = std::vector<char>;

#define RANK 2 //This is not MPI RANK
#define NAME_LENGTH 17
#define DATASETNAME "run"
//#define GROUPNAME "lumi"
#define NX 8
#define NY 2*RANK

void ConvertHDF5_MPI(char* input_file_dir,int batch,int run_num,int lumi_num,std::string const& outname ){
  int argc; char **argv;
  int flag;
  //just the boiler plate from the example......
  //need to understand what each of these variables are needed for.....
  
  hid_t file_id;      //File ID
  hid_t fapl_id;     //File access property list
  hid_t group_id;   //Group ID
  hid_t dset_id;    // DataSet ID
  hid_t chunk_id;   //Chunk ID
  hid_t file_space_id;     // File DataSpace ID
  hsize_t file_dims[RANK];
  hsize_t max_dims[RANK];
  hsize_t chunk_dims[RANK];
  herr_t ret;     // Generic return value (whatever it means...)
  std::vector<product_t>products;
  char groupname[16];
  std::string ntuple_name = "CCQENu";
  auto f = TFile::Open(input_file_dir);
  auto e = (TTree*)f->Get(ntuple_name.c_str());
  auto l = e->GetListOfBranches();
  //get the total number of branches....
  auto n_branch = l->GetEntriesFast();
  auto nentries = e->GetEntriesFast();

  std::cout<<"Total Number of Entries are "<<nentries<<std::endl;
  
  MPI_Initialized(&flag);
  if(!flag)MPI_Init(&argc,&argv);
  //also put the global variables....

  int global_size,global_rank;
  
  MPI_Comm_size(MPI_COMM_WORLD,&global_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&global_rank);

  
  
  std::cout<<"MPI RANK "<<global_rank<<" MPI SIZE "<<global_size<<std::endl;

  if(IsMotherRank()){
    std::cout<<"This is the Mother "<<std::endl;

  }
  //lets try to put the barrier and see if I can get rid of the error::
  MPI_Barrier(MPI_COMM_WORLD);
  //now lets try to setup the file access property list with parallel I/O access

  fapl_id = H5Pcreate (H5P_FILE_ACCESS);
  //make sure that we have created the file access property list...
  assert(fapl_id>0);
  std::cout<<"File access property id is "<<fapl_id<<std::endl;
  //Below function only works if HDF5-Parallel is implemented.
    ret = H5Pset_fapl_mpio(fapl_id,MPI_COMM_WORLD,MPI_INFO_NULL);
    assert(ret>=0);

    chunk_dims[0] = n_branch;
    chunk_dims[1] = NY;
    
    //need to create a new file collectively and release property list identifier
    // I need to create an additional code for the creation and the management of the dataset...
    
    char const *H5File_NAME = "Trial_File.h5";
    file_id = H5Fcreate(H5File_NAME,H5F_ACC_TRUNC,H5P_DEFAULT,fapl_id);
    //Try the Chunking
    chunk_id = H5Pset_chunk(file_id,RANK, chunk_dims);
    std::cout<<"chunk id is "<<chunk_id<<std::endl;
    assert(chunk_id>0);
    ret = H5Pclose(fapl_id);
    assert(ret>=0);
    sprintf(groupname,"Group_%d",global_rank);
    group_id = H5Gcreate(file_id,groupname, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    assert(group_id>0);
    
    file_dims[0] = H5S_UNLIMITED;
    file_dims[1] = NY;

    max_dims[0] = H5S_UNLIMITED;
    max_dims[1] = H5S_UNLIMITED;
    
    file_space_id = H5Screate_simple(global_rank,file_dims,max_dims);//max_dims);
    assert(file_space_id>0);

    /* Create the dataset collectively */
    dset_id = H5Dcreate2(group_id, DATASETNAME, H5T_NATIVE_DOUBLE,
    			 file_space_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
     assert(dset_id > 0);
     //Close File Space
     ret = H5Sclose(file_space_id);
     assert(ret>=0);

     //Close the dataSet collectively...
     ret = H5Dclose(dset_id);
     assert(ret>=0);
      
     ret = H5Gclose(group_id);
     assert(ret>=0);

     ret = H5Fclose(file_id);
     assert(ret>=0);


    /*
    
    //Maybe open one group per rank i guess....
    sprintf(groupname,"lumi_%d",global_rank);
    group_id = H5Gopen(file_id, groupname, H5P_DEFAULT);
    std::cout<<"Opening the group ID "<<group_id<<std::endl;
    assert(group_id>0);
    
    // Open the dataset independently 
    dset_id = H5Dopen(group_id, DATASETNAME, H5P_DEFAULT);
    assert(dset_id > 0);
    std::cout<<"DataSet ID is "<<dset_id<<std::endl;
  
    */
     //Put the barrier in the end as well...
     MPI_Barrier(MPI_COMM_WORLD);
}


#endif
int main(int argc, char* argv[]){

  if(argc!=4){
    std::cerr << "Please specify input file, batch-size and outputfilename\n";
    return 1;
}

  auto run_num = 1;
  auto lumi_num = 1;
  auto input_filename = argv[1];
  auto batch_size = atoi(argv[2]);
  auto outname = argv[3];

  int rank,size;
  
  int ierr;
  ierr = MPI_Init(&argc,&argv);
  if(ierr!=0){
    std::cout << "\n";
    std::cout << "BONES - Fatal error!\n";
    std::cout << "  MPI_Init returned ierr = " << ierr << "\n";
    exit ( 1 );
  }

  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  ConvertHDF5_MPI(input_filename,batch_size,run_num,lumi_num,outname);
  MPI_Finalize();
  return 1;
  }
