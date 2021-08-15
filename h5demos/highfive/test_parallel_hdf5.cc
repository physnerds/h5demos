#ifndef H5_WRITE_CHARS_H
#define H5_WRITE_CHARS_H

/*
#define DEBUG 
*/
#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>
#include <highfive/H5DataType.hpp>
#include <highfive/H5File.hpp>


#include "h5_write_1D_chars.h"
#include "h5_write_1D_chars_MPI.h"
#include "serialize_dataprods.h"
#include "utilities.h"
#include "time.h"

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
#include "hdf5.h"
#include "h5_setup_mpi.h"
#include "mpi.h"
#include "H5FDmpio.h"
//#include "H5Cpp.h"
using namespace HighFive;
using product_t = std::vector<char>;

#define RANK 2 //This is not MPI RANK
#define NAME_LENGTH 17
#define DATASETNAME "CCQENu"
//#define GROUPNAME "lumi"
#define NX 8
#define NY 4
#define CH_NX 2
#define CH_NY 2

int test_parallel_hdf5(){
  int flag;
  MPI_Initialized(&flag);

  int rank,size;
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  std::cout<<" MPI RANK "<<rank<<" MPI SIZE "<<size<<std::endl;

  auto plist_id = H5Pcreate(H5P_FILE_ACCESS);
  assert(plist_id>0);

  auto ret = H5Pset_fapl_mpio(plist_id,MPI_COMM_WORLD,MPI_INFO_NULL);
  assert(ret>=0);

  auto H5FILE_NAME = "test_file.hdf5";

  auto file_id = H5Fcreate(H5FILE_NAME, H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);

  auto run_name = "/run";
  auto scalar_id = H5Screate(H5S_SCALAR);

  auto run = H5Gcreate(file_id,run_name,H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT);
  
  auto scalar_attr =  H5Acreate(run,run_name,H5T_NATIVE_INT,
				scalar_id,H5P_DEFAULT,H5P_DEFAULT);

  auto lumi_name = "lumi";
  auto lumi = H5Gcreate(run,lumi_name,H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT);
  //create the attribute for lumi....
  auto lumi_att_id = H5Screate(H5S_SCALAR);
  auto lumi_attr = H5Acreate(lumi,lumi_name,H5T_NATIVE_INT,lumi_att_id,H5P_DEFAULT,H5P_DEFAULT);


  //START OF THE TEST LINE
  //create a data-set...
  hsize_t dimsf[1],
    max_dims[1],
    mem_dims[1],
    num_blocks[1],
    hslab_start_pos[1];

  dimsf[0]=1;
  max_dims[0] = H5S_UNLIMITED;
  num_blocks[0]=1;
  hslab_start_pos[0]=0;
  mem_dims[0]=1;

  auto dspace_id = H5Screate_simple(1,dimsf,max_dims);
  auto dplist_id = H5Pcreate(H5P_DATASET_CREATE);
  
  H5Pset_chunk(dplist_id,1,dimsf);
  auto dname = "test_data";
  auto dset_id = H5Dcreate(lumi,dname,H5T_NATIVE_CHAR,dspace_id,
			   H5P_DEFAULT,dplist_id,H5P_DEFAULT);

  auto xf_id = H5Pcreate(H5P_DATASET_XFER);
  H5Pset_dxpl_mpio(xf_id,H5FD_MPIO_COLLECTIVE);
  ret = H5Sselect_hyperslab(dspace_id,H5S_SELECT_SET,hslab_start_pos,
				 NULL,num_blocks,dimsf);

  assert(ret>=0);

  auto mspace_id = H5Screate_simple(1,mem_dims,NULL);
  std::vector<int>buff_ = {1};
  std::vector<char>buff = {static_cast<char>(buff_[0])};
  char* _buff = buff.data();

  
  auto status = H5Dwrite(dset_id,H5T_NATIVE_CHAR,mspace_id,
			 dspace_id,xf_id,_buff);
  

  auto flush_err = H5Fflush(dset_id,H5F_SCOPE_LOCAL);
  assert(flush_err>=0);
  
  MPI_Barrier(MPI_COMM_WORLD);
  H5Sclose(mspace_id);
  H5Sclose(dspace_id);
  H5Dclose(dset_id);
  H5Pclose(dplist_id);

  
  //now the extension in the loop.

  std::vector<int>num_list;
  for(int i =0;i<10;i++)num_list.push_back(i);
  srand(time(NULL)+rank);
  for(int i = 0;i<num_list.size();i++){
    int random_num;
    random_num = num_list[i];
    
    std::cout<<"Random number "<<random_num<<std::endl;
    std::vector<char>char_buff = {static_cast<char>(random_num)};
    auto curr_size = static_cast<unsigned long long>(char_buff.size());
    auto _curr_size = static_cast<int>(curr_size);
    
    int tot_size;
    MPI_Scan(&_curr_size,&tot_size,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);

    auto _dataset_id = H5Dopen(lumi,dname,H5P_DEFAULT);
    auto _xf_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(_xf_id,H5FD_MPIO_COLLECTIVE);
    auto _dspace_id = H5Dget_space(_dataset_id);

    auto ndims = H5Sget_simple_extent_ndims(_dspace_id);
    
    hsize_t curr_dims[ndims];
    H5Sget_simple_extent_dims(_dspace_id,curr_dims,NULL);

    int tot_dims;
    int _curr_dims = static_cast<int>(curr_dims[0]);
   #ifdef DEBUG 
    std::cout<<"curr dims "<<_curr_dims<<" total size "<<tot_size
	     <<" total dimensions "<<ndims<<" rank "<<rank<<std::endl;
    // MPI_Scan(&_curr_dims,&tot_dims,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
   #endif
    hsize_t new_dims[1] = {curr_dims[0]+curr_size};

        
    //extend the data-set
    auto status_id = H5Dset_extent(_dataset_id,new_dims);
    _dspace_id = H5Dget_space(_dataset_id); 
    assert(status_id>=0);
    //test if the new dimension is updated or not....
    hsize_t new_curr_dims[ndims];
    H5Sget_simple_extent_dims(_dspace_id,new_curr_dims,NULL);
    #ifdef DEBUG
    std::cout<<" Size of the dataspace after extension "<<new_curr_dims[0]<<std::endl;
    #endif
    H5S_seloper_t op = H5S_SELECT_SET;
    hsize_t offset[1] = {curr_dims[0]};
    hsize_t count[1] = {1};
    hsize_t buff_size[1] = {curr_size};
    
    #ifdef DEBUG
    std::cout<<"offset "<<offset[0]<<" new_dims "<<new_dims[0]
	     <<" buffer size "<<buff_size[0]
	     <<" current size "<<curr_dims[0]<<std::endl;
    #endif

    auto space_status = H5Sselect_hyperslab(_dspace_id,op,
					    offset,NULL,count,buff_size);

    
    auto mspace_id = H5Screate_simple(1,buff_size,NULL);
    char* __buff = char_buff.data();
    
    auto _status = H5Dwrite(_dataset_id,H5T_NATIVE_CHAR,mspace_id,
			    _dspace_id,_xf_id,__buff);

    
    assert(_status>=0);
    
    H5Pclose(_xf_id);
    H5Sclose(_dspace_id);
    H5Sclose(mspace_id);
    H5Dclose(_dataset_id);
    
  }
  
  
  //END OF THE TEST LINE

  //close everything here...
  
  H5Sclose(scalar_id);
  H5Sclose(lumi_att_id);
  H5Pclose(plist_id);
  H5Gclose(lumi);
  H5Gclose(run);

  H5Aclose(scalar_attr);
  H5Aclose(lumi_attr);


  size_t obj_id = H5Fget_obj_count(file_id,H5F_OBJ_ALL);
  std::cout<<"Total number of open HDF5 Objects "<<obj_id<<std::endl;
  H5Fclose(file_id);
  MPI_Finalize();

  return 1;
  
}

#endif

int main(int argc, char* argv[]){

  int err = MPI_Init(&argc,&argv);
  if(err!=0){
    std::cout<<" Something went wrong with your mpi arguments "<<std::endl;
    return 1;
  }
  test_parallel_hdf5();
  return 1;

}

