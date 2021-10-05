#ifndef H5_WRITE_CHARS_H
#define H5_WRITE_CHARS_H


#define DEBUG 

#include "time.h"
#include <ctime>
//#include "dkmeta.h"
#include "dk2nu.h"

#include <string>
#include <cstddef>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <cassert>
#include "hdf5.h"
#include "h5_setup_mpi.h"
#include "mpi.h"
#include "H5FDmpio.h"
//#include "H5Cpp.h"


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

  dimsf[0]= {0};
  // hsize_t chunk_dims[1] = {static_cast<hsize_t>(size)};
  int _iter = 3;
  hsize_t chunk_dims[1] = {static_cast<hsize_t>(size)};
  max_dims[0] = H5S_UNLIMITED;
  num_blocks[0]=1;
  hslab_start_pos[0]=0;
  mem_dims[0]=1;

  auto dspace_id = H5Screate_simple(1,dimsf,max_dims);
  auto dplist_id = H5Pcreate(H5P_DATASET_CREATE);
  
  H5Pset_chunk(dplist_id,1,chunk_dims);
  auto dname = "test_data";
  auto dset_id = H5Dcreate(lumi,dname,H5T_NATIVE_CHAR,dspace_id,
			   H5P_DEFAULT,dplist_id,H5P_DEFAULT);
  /*
  auto xf_id = H5Pcreate(H5P_DATASET_XFER);
  H5Pset_dxpl_mpio(xf_id,H5FD_MPIO_COLLECTIVE);
  ret = H5Sselect_hyperslab(dspace_id,H5S_SELECT_SET,hslab_start_pos,
				 NULL,num_blocks,dimsf);

  assert(ret>=0);
  
  
  auto mspace_id = H5Screate_simple(1,mem_dims,NULL);
  std::vector<int>buff_ = {-1,-2,-3};
  std::vector<char>buff = {static_cast<char>(buff_[0])};
  char* _buff = buff.data();

  
   auto status = H5Dwrite(dset_id,H5T_NATIVE_CHAR,mspace_id,
  			 dspace_id,xf_id,_buff);

   
  auto flush_err = H5Fflush(dset_id,H5F_SCOPE_LOCAL);
  assert(flush_err>=0);
  */
  MPI_Barrier(MPI_COMM_WORLD);
  // H5Sclose(mspace_id);
  
  H5Sclose(dspace_id);
  H5Dclose(dset_id);
  H5Pclose(dplist_id);

  
  //now the extension in the loop.

  std::vector<int>num_list;
  srand(time(NULL));
  hsize_t trial_offset=0;
  for(int i =0;i<size*_iter;i++){
    int random_number = rand()%100;
    num_list.push_back(random_number);
  }
  std::cout<<"size of the vector "<<num_list.size()<<std::endl;
  for(int i=0;i<num_list.size();i++)std::cout<<" Content "<<num_list[i]<<std::endl;
   for(int i = 0;i<_iter;i++){
    // MPI_Barrier(MPI_COMM_WORLD);//maybe we can synchronize before starting to write
    int random_num;
    random_num = num_list[rank+i*size];
    std::cout<<"Random number "<<random_num<<" "<<rank<<std::endl;
    std::vector<char>char_buff = {static_cast<char>(random_num)};
    auto curr_size = static_cast<unsigned long long>(char_buff.size());
    auto _curr_size = static_cast<int>(curr_size);
    
    int tot_size;
    MPI_Scan(&_curr_size,&tot_size,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    // MPI_Barrier(MPI_COMM_WORLD);
    auto _dataset_id = H5Dopen(lumi,dname,H5P_DEFAULT);
    auto _xf_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(_xf_id,H5FD_MPIO_COLLECTIVE);
    auto _dspace_id = H5Dget_space(_dataset_id);

    auto ndims = H5Sget_simple_extent_ndims(_dspace_id);
    
    hsize_t curr_dims[ndims];
    H5Sget_simple_extent_dims(_dspace_id,curr_dims,NULL);

    int tot_dims;
    hsize_t _tot_size = static_cast<hsize_t>(tot_size);
    int _curr_dims = static_cast<int>(curr_dims[0]);
    
#ifdef DEBUG 
    std::cout<<"curr dims "<<_curr_dims<<" total size "<<tot_size
	     <<" total dimensions "<<ndims<<" rank "<<rank<<std::endl;
#endif
    
    MPI_Scan(&_curr_dims,&tot_dims,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    int max_dims=0;
    MPI_Reduce(&_curr_dims,&max_dims,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);

    int tot_buff_size=0;
    int int_buff_size = char_buff.size();
    MPI_Scan(&int_buff_size,&tot_buff_size,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    int max_buff_size=0;
    MPI_Reduce(&tot_buff_size,&max_buff_size,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);

    
    int parcel[2] = {max_dims,max_buff_size};
    if(rank==0){
      for(int j=1;j<size;j++)
	MPI_Send(&parcel,2,MPI_INT,j,0,MPI_COMM_WORLD);
    }
    if(rank!=0){
      MPI_Recv(&parcel,2,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

    }
    
    max_dims = parcel[0];
    max_buff_size = parcel[1];

#ifdef DEBUG
    std::cout<<" Max DSpace Size "<<max_dims<<" Max Buff Size "<<max_buff_size<<" rank "<<rank<<std::endl;
#endif    
    //   hsize_t new_dims[1] = {trial_offset+curr_dims[0]+_tot_size};
    //  hsize_t new_dims[1] = {curr_dims[0]+max_dims};
    hsize_t new_dims[1];
    if(i==0)
      new_dims[0] = {curr_dims[0]+_tot_size};
    else
      new_dims[0] = {static_cast<hsize_t>(curr_dims[0]+max_buff_size)};
    
    //extend the data-set
    auto status_id = H5Dset_extent(_dataset_id,new_dims);
    _dspace_id = H5Dget_space(_dataset_id); 
    assert(status_id>=0);
    //test if the new dimension is updated or not....
    hsize_t new_curr_dims[ndims];
    H5Sget_simple_extent_dims(_dspace_id,new_curr_dims,NULL);

    /*    
#ifdef DEBUG
    std::cout<<" Size of the dataspace after extension "<<new_curr_dims[0]<<std::endl;
#endif
    */
    H5S_seloper_t op = H5S_SELECT_SET;
    hsize_t offset[1];
    if(i==0){
      offset[0] = {_tot_size-1};
    }
    else if(i==1){
      // offset[0] = {static_cast<hsize_t>(trial_offset+curr_dims[0])};//{curr_dims[0]};
      offset[0] = {new_dims[0]-static_cast<hsize_t>(int_buff_size)};
    }
    else{
      offset[0] = {new_dims[0]-static_cast<hsize_t>(int_buff_size)};
    }

    //  hsize_t offset[1]= {curr_dims[0]};
    hsize_t count[1] = {1};
    hsize_t buff_size[1] = {curr_size};
    
#ifdef DEBUG
    std::cout<<"offset "<<offset[0]<<" new_dims "<<new_dims[0]
	     <<" buffer size "<<buff_size[0]
	     <<" current size "<<curr_dims[0]<<std::endl;
#endif


    auto space_status = H5Sselect_hyperslab(_dspace_id,op,
					    offset,NULL,count,buff_size);

    
    auto _mspace_id = H5Screate_simple(1,buff_size,NULL);
    char* __buff = char_buff.data();
    std::vector<char>char_num_list;
    for(int i=0;i<num_list.size();i++)char_num_list.push_back(static_cast<char>(num_list[i]));
    //char* __buff = char_num_list.data();
    //  MPI_Barrier(MPI_COMM_WORLD);

     auto _status = H5Dwrite(_dataset_id,H5T_NATIVE_CHAR,_mspace_id,
   			    _dspace_id,_xf_id,__buff);
    assert(_status>=0);
#ifdef DEBUG
    if(rank==2)
      std::cout<<"Write Status "<<_status<<" Rank "<<rank<<std::endl;

#endif
    
#ifdef DEBUG
  std::time_t result = std::time(nullptr);
  std::cout<<" Data "<<random_num<< " For Rank "<<rank<<" At Offset "<<offset[0]<<" at "<<result<<std::endl;
#endif


  //  auto flush_err = H5Fflush(_dataset_id,H5F_SCOPE_LOCAL);
  //  assert(flush_err>=0);
  MPI_Barrier(MPI_COMM_WORLD);

#ifdef DEBUG
    std::cout<<"Trial Offset is "<<trial_offset<<std::endl;
#endif
    
    trial_offset = curr_dims[0];

    H5Pclose(_xf_id);
    H5Sclose(_dspace_id);
    H5Sclose(_mspace_id);
    H5Dclose(_dataset_id);

    
#ifdef DEBUG
    std::cout<<"********************END OF THE FOR LOOP**************"<<" "<<rank<<std::endl; 
#endif

    // MPI_Barrier(MPI_COMM_WORLD);
   }
  
  
  //END OF THE TEST LINE

   //   close everything here...
  
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

