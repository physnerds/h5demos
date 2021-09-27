#ifndef H5_WRITE_1D_CHARS_MPI_H
#define H5_WRITE_1D_CHARS_MPI_H

#include "utilities.h"
#include "serialize_dataprods.h"
#include <ctime>
//#include "mpi.h"
//#include "h5_setup_mpi.h
#define DEBUG
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
int CreateDataSets(std::string branch_name,int buff_size,hid_t lumi_id){
  int mpi_rank,mpi_size;
  MPI_Comm_size(MPI_COMM_WORLD,&mpi_size);
  MPI_Comm_size(MPI_COMM_WORLD,&mpi_rank);
  hsize_t dimsf[1],
    max_dims[1];  

  dimsf[0]=0;
  max_dims[0]=H5S_UNLIMITED;
  hsize_t chunk_dims[1] = {static_cast<hsize_t>(buff_size*mpi_size)};
  auto dspace_id = H5Screate_simple(1,dimsf,max_dims);
  auto dplist_id = H5Pcreate(H5P_DATASET_CREATE);

  //This is a bit challenging. To guarantee the smooth IO all the IO processes should use same chunk dims.
  H5Pset_chunk(dplist_id,1,chunk_dims);
  auto dset_id = H5Dcreate(lumi_id,branch_name.c_str(),H5T_NATIVE_CHAR,dspace_id,
			   H5P_DEFAULT,dplist_id,H5P_DEFAULT);
  /*
  auto plist_id = H5Pcreate(H5P_DATASET_XFER);
  H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
  */  
  MPI_Barrier(MPI_COMM_WORLD);
  // H5Pclose(plist_id);
  H5Sclose(dspace_id);
  H5Dclose(dset_id);
  H5Pclose(dplist_id);

  return mpi_rank;
}

//***********************************************************************************//

void WriteBufferAndSendMessage(hid_t dataset_id,hid_t mspace_id,hid_t dspace_id,hid_t xf_id, char* _buff_,int mpi_rank, int mpi_size){
  std::cout<<"This is Rank "<<mpi_rank<<std::endl;
  if(mpi_rank==0){
    std::cout<<" Stuck before the Send And Write "<<mpi_rank<<std::endl;
    auto status = H5Dwrite(dataset_id,H5T_NATIVE_CHAR,
			   mspace_id,dspace_id,xf_id, _buff_);
    std::cout<<"Status of Write "<<status<<" "<<mpi_rank<<std::endl;
    assert(status>=0);
    std::cout<<"Stuck Right after Write "<<mpi_rank<<std::endl;
    H5Pclose(xf_id);
    H5Sclose(dspace_id);
    H5Sclose(mspace_id);
    H5Dclose(dataset_id);
    std::cout<<" Stuck before the Send "<<mpi_rank<<std::endl;
    MPI_Send(HSendBuffer,0,MPI_CHAR,1,MPI_ANY_TAG,MPI_COMM_WORLD);
    std::cout<<" Stuck after the Send "<<mpi_rank<<std::endl;
    delete[] HSendBuffer; 
  }
  else{
    char* buff;
    MPI_Status mpi_status;
    std::cout<<" Stuck before the Probe "<<mpi_rank<<std::endl;
    MPI_Probe(mpi_rank-1,MPI_ANY_TAG,MPI_COMM_WORLD,&mpi_status);
    std::cout<<" Stuck after the Probe "<<mpi_rank<<std::endl;
    MPI_Recv(buff,0,MPI_CHAR,mpi_rank-1,MPI_ANY_TAG,
	     MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    std::cout<<" Stuck After the Wait "<<mpi_rank<<std::endl;
    auto status = H5Dwrite(dataset_id,H5T_NATIVE_CHAR,
			       mspace_id,dspace_id,xf_id,HSendBuffer);
    assert(status>=0);

    H5Pclose(xf_id);
    H5Sclose(dspace_id);
    H5Sclose(mspace_id);
    H5Dclose(dataset_id);
    if(mpi_rank<mpi_size-2)
      MPI_Send(HSendBuffer,0,MPI_CHAR,1,
	       mpi_size-1,MPI_COMM_WORLD);
    std::cout<<" Stuck after the Send "<<mpi_rank<<std::endl;
    delete[] HSendBuffer;

  }

}

//***********************************************************************************//

void WriteBufferAndSendMessageSerial(hid_t dataset_id,hid_t mspace_id,hid_t dspace_id,hid_t xf_id, char* _buff_,int mpi_rank, int mpi_size){
  
  for(int i=0;i<mpi_size;i++){
      if(i==mpi_rank){
      auto status = H5Dwrite(dataset_id,H5T_NATIVE_CHAR,
			     mspace_id,dspace_id,xf_id, _buff_);

      assert(status>=0);
       }
  }

}

//***********************************************************************************//

int WriteDataSets(std::string branch_name,std::vector<char>buff,hid_t lumi_id,int ievt, int tentry,int mpi_rank,int mpi_size){
  //  int mpi_rank,mpi_size;
  //  MPI_Comm_size(MPI_COMM_WORLD,&mpi_size);
  //  MPI_Comm_rank(MPI_COMM_WORLD,&mpi_rank);
  //I think it is better to do everything in one go.
  auto curr_size = static_cast<unsigned long long>(buff.size());
  int _curr_size = static_cast<int>(curr_size);
  
  hsize_t buff_size[1] = {static_cast<hsize_t>(curr_size)};
  auto dataset_id = H5Dopen(lumi_id,branch_name.c_str(),H5P_DEFAULT);
  std::string offset_name = branch_name+"_offset";
  
#ifdef EXTRAS
  auto dataset_offset = H5Dopen(lumi_id,offset_name.c_str(),H5P_DEFAULT);
  std::string size_name = branch_name+"_size";
  auto dataset_size = H5Dopen(lumi_id,size_name.c_str(),H5P_DEFAULT);
#endif
  
  //do i need to do this each time I write buffer or it can be a one time thing

  auto xf_id = H5Pcreate(H5P_DATASET_XFER);

#ifdef EXTRAS
  auto xf_id_sz = H5Pcreate(H5P_DATASET_XFER);
  auto xf_id_offset = H5Pcreate(H5P_DATASET_XFER);
#endif
  

  H5Pset_dxpl_mpio(xf_id,H5FD_MPIO_COLLECTIVE);

#ifdef EXTRAS
  H5Pset_dxpl_mpio(xf_id_sz,H5FD_MPIO_COLLECTIVE);
  H5Pset_dxpl_mpio(xf_id_offset,H5FD_MPIO_COLLECTIVE);
#endif
  
  auto _dspace_id = H5Dget_space(dataset_id);

  
#ifdef EXTRAS
  auto _dspace_offset = H5Dget_space(dataset_offset);
  auto _dspace_size = H5Dget_space(dataset_size);
#endif
  //okay the number of dims are going to be same for the meta-datasets as well.
  auto ndims = H5Sget_simple_extent_ndims(_dspace_id);

  hsize_t curr_dims[ndims];
  hsize_t curr_dims_sz[ndims];
  hsize_t curr_dims_offset[ndims];
  H5Sget_simple_extent_dims(_dspace_id,curr_dims,NULL);

#ifdef EXTRAS

  H5Sget_simple_extent_dims(_dspace_size,curr_dims_sz,NULL);
  H5Sget_simple_extent_dims(_dspace_offset,curr_dims_offset,NULL);

#endif
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
  hsize_t offset[1];
  hsize_t new_dims_sz[1];
  hsize_t new_dims_offset[1];
  hsize_t sz_offset[1];

#ifdef DEBUG
  std::cout<<" Scan Result "<<parcel[0]<<" "<<parcel[1]<<" "
	   <<mpi_rank<<std::endl;

#endif
  
 
  if(ievt==0){
    new_dims[0] = {_tot_size};
    offset[0] = {_tot_size-buff_size[0]};
    new_dims_sz[0] = {static_cast<hsize_t>(mpi_rank+1)};
    sz_offset[0] = {static_cast<hsize_t>(mpi_rank)};
  }
  
  else{
    new_dims[0] = {static_cast<hsize_t>(curr_dims[0]+parcel[1])};
    offset[0] = {new_dims[0]-static_cast<hsize_t>(int_buff_size)};
    new_dims_sz[0] = {curr_dims_sz[0]+static_cast<hsize_t>(mpi_size)};
    //  sz_offset[0] = {new_dims_sz[0]+static_cast<hsize_t>(mpi_size-1)};
    sz_offset[0] = {new_dims_sz[0]-1};
  }

#ifdef DEBUG
  std::cout<<" Size "<<_tot_size<<" Curr Dims "<<curr_dims[0]<<" Buffer "
	   <<int_buff_size<<" ievt "<<ievt<<" Branch name "<<branch_name
	   <<" rank "<<mpi_rank<<std::endl;
#endif


#ifdef DEBUG
  std::cout<<" Offset: New Dims "<<new_dims_sz[0]<<" offset "<<sz_offset[0]
	   <<" rank "<<mpi_rank<<" ievt "<<ievt<<std::endl;
#endif  
  
#ifdef DEBUG
  std::cout<<" new dimension "<<new_dims[0]
	   <<" offset "<<offset[0]<<" max buff size "<<max_buff_size
	   <<" rank "<<mpi_rank<<std::endl;
#endif
  hsize_t count[1] = {1}; //This is what we will put for now...
  auto status_id = H5Dset_extent(dataset_id,new_dims);
#ifdef EXTRAS
  auto status_sz = H5Dset_extent(dataset_size,new_dims_sz);
  auto status_offset = H5Dset_extent(dataset_offset,new_dims_sz);
#endif
  //always remember after the extension, need to do this one more time:
   _dspace_id = H5Dget_space(dataset_id);

   
#ifdef EXTRAS
   _dspace_offset = H5Dget_space(dataset_offset);
   _dspace_size = H5Dget_space(dataset_size);
#endif
   
  auto space_status = H5Sselect_hyperslab(_dspace_id,H5S_SELECT_SET,
					  offset,NULL,count,buff_size);
#ifdef EXTRAS
  
  space_status = H5Sselect_hyperslab(_dspace_offset,H5S_SELECT_SET,
				     sz_offset,NULL,count,count);

  space_status = H5Sselect_hyperslab(_dspace_size,H5S_SELECT_SET,
				     sz_offset,NULL,count,count);

#endif
  
  auto _mspace_id = H5Screate_simple(1,buff_size,NULL);
  assert(H5Sselect_all(_mspace_id)>=0);

#ifdef EXTRAS  
  auto _mspace_sz = H5Screate_simple(1,count,NULL);
  auto _mspace_offset= H5Screate_simple(1,count,NULL);
#endif

  char *__buff = buff.data();
  std::vector<char>__buff_sz_ = {static_cast<char>(_curr_size)};
  std::vector<char>__buff_offset_ = {static_cast<char>(offset[0])};

  char *__buff_offset = __buff_offset_.data();
  char *__buff_sz = __buff_sz_.data();

#ifdef DEBUG
  std::cout<<" Size of Offset Buffer "<<__buff_offset_.size()<<
    " Size of Sz Buffer "<<__buff_sz_.size()<<std::endl;
#endif

 MPI_Scan(&_curr_dims,&tot_dims,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
 max_dims=0;
 MPI_Reduce(&_curr_dims,&max_dims,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);

#ifdef DEBUG
  std::cout<<branch_name<<" DSet ID "<<dataset_id<<" "<<_dspace_id<<" "<<mpi_rank<<" "<<tentry<<" "<<std::endl;
#endif

#ifdef TEST
  WriteBufferAndSendMessage(dataset_id,_mspace_id,
			    _dspace_id,xf_id,__buff,
			    mpi_rank,mpi_size);

#endif
  
#ifndef TEST
  auto _status = H5Dwrite(dataset_id,H5T_NATIVE_CHAR,_mspace_id,
  			  _dspace_id,xf_id,__buff);
  assert(_status>=0);
#endif
  
#ifdef EXTRAS  
  _status = H5Dwrite(dataset_offset,H5T_NATIVE_CHAR,_mspace_offset,
  		     _dspace_offset,xf_id,__buff_offset);
  assert(_status>=0);
  
  _status = H5Dwrite(dataset_size,H5T_NATIVE_CHAR,_mspace_sz,
		     _dspace_size,xf_id,__buff_sz);
  assert(_status>=0);
#endif  

  
#ifdef DEBUG
  //  std::time_t final_time = std::time(nullptr);
  if(branch_name=="Q2"||branch_name=="cos_theta"||branch_name=="muon_p")
    std::cout<<"Written "<<branch_name<<" entry "<<tentry
	     <<" "<<offset[0]<<" "<<new_dims[0]
	     <<" "<<std::endl;
#endif
  // MPI_Barrier(MPI_COMM_WORLD);

#ifdef DEBUG

  H5Sget_simple_extent_dims(_dspace_id,curr_dims,NULL);
  std::cout<<" Branch Name "<<branch_name<<" Final Dims "
	   <<curr_dims[0]<<" "<<tentry<<" "<<mpi_rank<<std::endl;
  
#endif

#ifndef TEST
  
  assert(H5Pclose(xf_id)>=0);
#ifdef EXTRAS
  H5Pclose(xf_id_sz);
  H5Pclose(xf_id_offset);
#endif  
  assert(H5Sclose(_dspace_id)>=0);
#ifdef EXTRAS  
  H5Sclose(_dspace_offset);
  H5Sclose(_dspace_size);
#endif  
  assert(H5Sclose(_mspace_id)>=0);
#ifdef EXTRAS  
  H5Sclose(_mspace_sz);
  H5Sclose(_mspace_offset);
  H5Dclose(dataset_offset);
  H5Dclose(dataset_size);
#endif  
  assert(H5Dclose(dataset_id)>=0);
  
#endif 
  
  // MPI_Barrier(MPI_COMM_WORLD);
  //okay Maybe I can wait here....
  /*
  if(mpi_rank==0){
    MPI_Request requests[mpi_size-1];
    for(int i = 1;i<mpi_size;i++){
      MPI_Isend(&i,1,MPI_INT,i,mpi_rank,MPI_COMM_WORLD,&requests[i-1]);
    }
    MPI_Waitall(mpi_size-1,requests,MPI_STATUSES_IGNORE);
  }
  else{
    int received;
    MPI_Recv(&received, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  */
  return dataset_id;
}

//***********************************************************************************//

void write_1D_chars_MPI(std::vector<product_t> const& products, 
			std::vector<std::string> const& ds_names, 
			long unsigned int batch, 
			long unsigned int nbatch, 
			long unsigned int round,
			hid_t lumi_id,int mpi_rank,
			int mpi_size, int ievt, int tentry) 
{
  auto num_prods = ds_names.size();
#ifdef DEBUG
  std::cout<<" Tot Number of Products "<<num_prods<<" ievt "<<ievt<<" Entry "<<tentry<<std::endl;
#endif
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
      continue;
      
    }
    else{
#ifdef DEBUG
      std::cout<<"The sum of Products are "<<sum_prods<<" "<<ievt<<" "<<tentry
	       <<" "<<mpi_rank<<" "<<nbatch<<std::endl;
#endif

    }
    std::vector<char>sz_vector = {char(tmp1.size())};
    auto szds_name = ds_name+"_sz";
    auto offset_name = ds_name+"_offset";
    auto status = WriteDataSets(ds_name,tmp1,lumi_id,ievt,tentry,mpi_rank,mpi_size);
    MPI_Barrier(MPI_COMM_WORLD);
  }
}

void SetTotalBranches(int nbranch){
  __tot_branches=nbranch;
}
//***********************************************************************************//

#endif
