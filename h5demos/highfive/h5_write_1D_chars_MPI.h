#ifndef H5_WRITE_1D_CHARS_MPI_H
#define H5_WRITE_1D_CHARS_MPI_H

#include "utilities.h"
#include "serialize_dataprods.h"
#include <ctime>
#include "mpi.h"
#include "hdf5.h"
#include "H5FDmpio.h"

#include <string>
#include <cstddef>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <numeric>

//#include "h5_setup_mpi.h
#define DEBUG 0
//#define TEST
//#define EXTRAS

std::time_t initial_time;
using product_t = std::vector<char>;
char *HSendBuffer=0;

struct dataset_info{
  // Things I would like to save from the data space easily
  std::string name; //name of the dataset
  hid_t dset_id; //identifier of the data-set
  hid_t dspace_id; //identifier of the data-space
  hid_t dprop_id; //identifier of the property of the data-set
  hid_t memspace_id;
  hid_t plist_id;

};

std::vector<char>
flatten_MPI(std::vector<std::vector<char>> const& tmp) {
  std::vector<char> tmp1 = tmp[0];
  for (int i = 1; i<tmp.size(); ++i)
     tmp1.insert(end(tmp1), begin(tmp[i]), end(tmp[i]));
  return tmp1;
}

//*******************************************************************************
//these variables to save info....
std::map<std::string,dataset_info>list_createInfo;
std::map<std::string, dataset_info>size_createInfo;
int __tot_branches=0;
bool Writing_Data=false;
//end of these variables

//*********************************************************************************

template<typename T>
std::vector<char>GetMemcpyArray(T _val){
  char arr_val[sizeof(_val)];
  memcpy(arr_val,&_val,sizeof(_val));
  std::vector<char>blob;
  blob.insert(blob.begin(),std::begin(arr_val),std::end(arr_val));
  return blob;
}

//*********************************************************************************//
//only create the data-sets nothing else.
int CreateDataSets(TBranch *branch,int buff_size,hid_t lumi_id){
  int mpi_size, mpi_rank;
  MPI_Comm_size(MPI_COMM_WORLD,&mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&mpi_rank);
  hsize_t dimsf[1],
    max_dims[1];
  std::string branch_name = branch->GetName();
  std::string offset_name = branch_name+"_offset";
  std::string branch_info = branch_name+"_info";

  auto branch_type = return_datatype(branch);
  
  hid_t dset_id;
  hid_t offset_id;
  hid_t info_id;
  dimsf[0]=0;
  max_dims[0]=H5S_UNLIMITED;
  
  hsize_t chunk_dims[1] = {static_cast<hsize_t>(buff_size*mpi_size)};
  hsize_t chunk_dims_offset[1] = {static_cast<hsize_t>(mpi_size)};
  hsize_t chunk_dims_info[1] = {static_cast<hsize_t>(sizeof(branch_type))};
  auto dspace_id = H5Screate_simple(1,dimsf,max_dims);
  auto dspace_offset_id = H5Screate_simple(1,dimsf,max_dims);
  auto dspace_info_id = H5Screate_simple(1,dimsf,chunk_dims_info);
  
  auto dplist_id = H5Pcreate(H5P_DATASET_CREATE);
  auto dplist_offset_id = H5Pcreate(H5P_DATASET_CREATE);
  auto dplist_info_id = H5Pcreate(H5P_DATASET_CREATE);

  auto xf_id = H5Pcreate(H5P_DATASET_XFER);
  
  //This is a bit challenging. To guarantee the smooth IO all the IO processes should use same chunk dims.
  H5Pset_chunk(dplist_id,1,chunk_dims);
  dset_id = H5Dcreate(lumi_id,branch_name.c_str(),H5T_NATIVE_CHAR,dspace_id,
		      H5P_DEFAULT,dplist_id,H5P_DEFAULT);
  assert(dset_id>=0);
  
  H5Pset_chunk(dplist_offset_id,1,chunk_dims_offset);
  offset_id = H5Dcreate(lumi_id,offset_name.c_str(),H5T_NATIVE_INT,dspace_offset_id,
		      H5P_DEFAULT,dplist_offset_id,H5P_DEFAULT);  
  assert(offset_id>=0);
  H5Pset_chunk(dplist_info_id,1,chunk_dims_info);
  info_id = H5Dcreate(lumi_id,branch_info.c_str(),H5T_NATIVE_CHAR,dspace_info_id,
		      H5P_DEFAULT,dplist_info_id,H5P_DEFAULT);

  assert(info_id>=0);
  char *__buff = (char*)branch_type;
  hsize_t _buff_size[1] = {static_cast<hsize_t>(sizeof(branch_type))};

  hsize_t offset[1] = {0}; 
  hsize_t count[1] = {1};
  /*
  std::cout<<"offset "<<offset[0]<<" Buffer Size "<<_buff_size[0]
	   <<std::endl;
  */
  H5Dset_extent(info_id,_buff_size);
  dspace_info_id = H5Dget_space(info_id);
  auto space_status = H5Sselect_hyperslab(dspace_info_id,H5S_SELECT_SET,
					  offset,NULL,count,_buff_size);
  auto _mspace_id = H5Screate_simple(1,_buff_size,NULL);
  assert(H5Sselect_all(_mspace_id)>=0);

  auto _status = H5Dwrite(info_id,H5T_NATIVE_CHAR,_mspace_id,
			  dspace_info_id,xf_id,__buff);

  assert(_status>=0);
  assert(H5Pclose(xf_id)>=0);
  assert(H5Sclose(dspace_info_id)>=0);
  assert(H5Pclose(dplist_info_id)>=0);
  assert(H5Dclose(info_id)>=0);
  
  
  assert(H5Sclose(dspace_id)>=0);
  assert(H5Dclose(dset_id)>=0);
  assert(H5Pclose(dplist_id)>=0);
  
  assert(H5Sclose(dspace_offset_id)>=0);
  assert(H5Dclose(offset_id)>=0);
  assert(H5Pclose(dplist_offset_id)>=0);
  
  return mpi_size;
}
//************************************************************************************************//

int WriteDataSets(std::string branch_name,std::vector<char>buff,hid_t lumi_id,int ievt, int tentry,int mpi_rank,int mpi_size){
  // int _mpi_rank,_mpi_size;
  // MPI_Comm_size(MPI_COMM_WORLD,&mpi_size);
    //    MPI_Comm_rank(MPI_COMM_WORLD,&_mpi_rank);

  //I think it is better to do everything in one go.
  auto curr_size = static_cast<unsigned long long>(buff.size());
  int _curr_size = static_cast<int>(curr_size);
  
  hsize_t buff_size[1] = {static_cast<hsize_t>(curr_size)};
  auto dataset_id = H5Dopen(lumi_id,branch_name.c_str(),H5P_DEFAULT);
  std::string offset_name = branch_name+"_offset";
  auto dataset_offset = H5Dopen(lumi_id,offset_name.c_str(),H5P_DEFAULT);
  
  
  //do i need to do this each time I write buffer or it can be a one time thing

  auto xf_id = H5Pcreate(H5P_DATASET_XFER);
  auto xf_id_offset = H5Pcreate(H5P_DATASET_XFER);


  H5Pset_dxpl_mpio(xf_id,H5FD_MPIO_COLLECTIVE);
  H5Pset_dxpl_mpio(xf_id_offset,H5FD_MPIO_COLLECTIVE);  

  
  auto _dspace_id = H5Dget_space(dataset_id);
  auto _dspace_offset = H5Dget_space(dataset_offset);
  
  //okay the number of dims are going to be same for the meta-datasets as well.
  auto ndims = H5Sget_simple_extent_ndims(_dspace_id);
  
  hsize_t curr_dims[ndims];
  hsize_t curr_dims_offset[ndims];
  H5Sget_simple_extent_dims(_dspace_id,curr_dims,NULL);
  H5Sget_simple_extent_dims(_dspace_offset,curr_dims_offset,NULL);
  
  int tot_dims;
  int _curr_dims = static_cast<int>(curr_dims[0]);

  //probably need to scan 
  
  MPI_Scan(&_curr_dims,&tot_dims,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
  int max_dims=0;
  MPI_Reduce(&_curr_dims,&max_dims,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);

  int tot_buff_size=0;
  int int_buff_size=_curr_size;

  MPI_Scan(&int_buff_size,&tot_buff_size,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
  int max_buff_size=0;
  MPI_Reduce(&tot_buff_size,&max_buff_size,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);

  int parcel[2] = {max_dims,max_buff_size};

  MPI_Bcast(&parcel,2,MPI_INT,0,MPI_COMM_WORLD);
  
  hsize_t _tot_size = static_cast<hsize_t>(tot_buff_size);
  hsize_t new_dims[1];
  hsize_t new_dims_offset[1];
  hsize_t offset[1];
 
  if(ievt==0){
    new_dims_offset[0] = {static_cast<hsize_t>(mpi_size)};
    new_dims[0] = {static_cast<hsize_t>(parcel[1])};
    offset[0] = {static_cast<hsize_t>(tot_buff_size-int_buff_size)};
  }
  else{
    new_dims_offset[0] = {static_cast<hsize_t>(mpi_size+curr_dims_offset[0])};
    new_dims[0] = {static_cast<hsize_t>(curr_dims[0]+parcel[1])};
    offset[0] = {curr_dims[0]+static_cast<hsize_t>(tot_buff_size-int_buff_size)};
  }

  hsize_t offset_offset[1] = {static_cast<hsize_t>(mpi_rank)+curr_dims_offset[0]};
  hsize_t count[1] = {1}; //This is what we will put for now...
  auto status_id = H5Dset_extent(dataset_id,new_dims);
  auto status_offset = H5Dset_extent(dataset_offset,new_dims_offset);

  //get the data-space one more time......
  _dspace_id = H5Dget_space(dataset_id);
  _dspace_offset = H5Dget_space(dataset_offset);
   

  auto space_status = H5Sselect_hyperslab(_dspace_id,H5S_SELECT_SET,
					  offset,NULL,count,buff_size);

  space_status = H5Sselect_hyperslab(_dspace_offset,H5S_SELECT_SET,
				     offset_offset,NULL,count,count);

  
  auto _mspace_id = H5Screate_simple(1,buff_size,NULL);
  assert(H5Sselect_all(_mspace_id)>=0);

  auto _mspace_offset = H5Screate_simple(1,count,NULL);
  assert(H5Sselect_all(_mspace_offset)>=0);


  char *__buff = buff.data();
  std::vector<char>__buff_sz_ = {static_cast<char>(_curr_size)};
  std::vector<int>__buff_offset_ = {static_cast<int>(offset[0])};

  int *__buff_offset = __buff_offset_.data();
  char *__buff_sz = __buff_sz_.data();


  auto _status = H5Dwrite(dataset_id,H5T_NATIVE_CHAR,_mspace_id,
  			  _dspace_id,xf_id,__buff);
  assert(_status>=0);

  _status = H5Dwrite(dataset_offset,H5T_NATIVE_INT,_mspace_offset,
		     _dspace_offset,xf_id_offset,__buff_offset);
  

  
  assert(H5Pclose(xf_id)>=0);
  assert(H5Pclose(xf_id_offset)>=0);
  assert(H5Sclose(_dspace_id)>=0);
  assert(H5Sclose(_dspace_offset)>=0);
  assert(H5Sclose(_mspace_id)>=0);
  assert(H5Sclose(_mspace_offset)>=0);
  assert(H5Dclose(dataset_id)>=0);
  assert(H5Dclose(dataset_offset)>=0);
  return dataset_id;
}

//***********************************************************************************//
int WriteDataSetsSerial(std::string branch_name,std::vector<char>buff,hid_t lumi_id, int ievt, int tentry){
  int mpi_rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&mpi_rank);
  auto curr_size = static_cast<unsigned long long>(buff.size());
  
  
  hsize_t buff_size[1] = {static_cast<hsize_t>(curr_size)};
  hsize_t count[1] = {1};
  
  auto dataset_id = H5Dopen(lumi_id,branch_name.c_str(),H5P_DEFAULT);
  std::string offset_name = branch_name+"_offset";
  auto dataset_offset = H5Dopen(lumi_id,offset_name.c_str(),H5P_DEFAULT);

  
  auto xf_id_offset = H5Pcreate(H5P_DATASET_XFER);
  auto xf_id = H5Pcreate(H5P_DATASET_XFER);

  auto _dspace_offset = H5Dget_space(dataset_offset);
  auto _dspace_id = H5Dget_space(dataset_id);

  auto ndims = H5Sget_simple_extent_ndims(_dspace_id);

  hsize_t curr_dims[ndims];
  hsize_t curr_dims_offset[ndims];
  
  H5Sget_simple_extent_dims(_dspace_id,curr_dims,NULL);
  H5Sget_simple_extent_dims(_dspace_offset,curr_dims_offset,NULL);
  
  int _curr_dims = static_cast<int>(curr_dims[0]);
  hsize_t new_dims[1] = {curr_dims[0]+buff_size[0]};

  int _curr_dims_offset = static_cast<int>(curr_dims_offset[0]);
  hsize_t new_dims_offset[1] = {curr_dims_offset[0]+1};

 hsize_t offset_offset[1] = {curr_dims_offset[0]};
 
  
  auto status_id = H5Dset_extent(dataset_id,new_dims);
  auto status_offset = H5Dset_extent(dataset_offset,new_dims_offset);

  _dspace_offset = H5Dget_space(dataset_offset);
  _dspace_id = H5Dget_space(dataset_id);

  
  auto space_id = H5Sselect_hyperslab(_dspace_id,H5S_SELECT_SET,
				      curr_dims,NULL,count,buff_size);

  space_id = H5Sselect_hyperslab(_dspace_offset,H5S_SELECT_SET,
				     offset_offset,NULL,count,count);  

  auto _mspace_id = H5Screate_simple(1,buff_size,NULL);
  assert(H5Sselect_all(_mspace_id)>=0);

  auto _mspace_offset = H5Screate_simple(1,count, NULL);
  
  char *__buff = buff.data();
  auto _status = H5Dwrite(dataset_id,H5T_NATIVE_CHAR,_mspace_id,_dspace_id,
			  xf_id,__buff);

  std::vector<int>_buff_offset = {_curr_dims};
  int *__buff_offset = _buff_offset.data();
  _status = H5Dwrite(dataset_offset,H5T_NATIVE_INT,_mspace_offset,
		     _dspace_offset,xf_id_offset,__buff_offset);


  assert(H5Pclose(xf_id)>=0);
  assert(H5Sclose(_dspace_id)>=0);
  assert(H5Sclose(_mspace_id)>=0);
  assert(H5Dclose(dataset_id)>=0);

  assert(H5Pclose(xf_id_offset)>=0);
  assert(H5Sclose(_dspace_offset)>=0);
  assert(H5Sclose(_mspace_offset)>=0);
  assert(H5Dclose(dataset_offset)>=0);  
  return dataset_id;

}
//********************************************************************************//


void write_1D_chars_MPI(std::vector<product_t> const& products, 
			std::vector<std::string> const& ds_names, 
			long unsigned int batch, 
			long unsigned int nbatch, 
			long unsigned int round,
			hid_t lumi_id,int mpi_rank,
			int mpi_size, int ievt, int tentry,bool serial) 
{
  auto num_prods = ds_names.size();

  for(int prod_index = 0;prod_index<num_prods;++prod_index){
    auto prod_size = products[prod_index].size();
    auto tmp = get_prods(products,prod_index,num_prods);
    auto sizes = get_sizes(tmp);
    auto tmp1 = flatten_MPI(tmp);
    auto sizes1 = get_sizes1D(tmp1);
    auto ds_name = ds_names[prod_index];
    auto sum_prods = std::accumulate(sizes.begin(),sizes.end(),0);
    if(sum_prods==0){
#ifdef DEBUG
      std::cout<<" Found Empty sum_prods "<<ievt<<" "<<tentry<<" "
	       <<mpi_rank<<std::endl;
#endif
      
    }

    if(!serial)
    auto status = WriteDataSets(ds_name,tmp1,lumi_id,ievt,tentry,mpi_rank,mpi_size);
    else{
    auto  status = WriteDataSetsSerial(ds_name,tmp1,lumi_id,ievt,tentry);
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

void SetTotalBranches(int nbranch){
  __tot_branches=nbranch;
}
//***********************************************************************************//

#endif
