#ifndef H5_WRITE_1D_CHARS_MPI_H
#define H5_WRITE_1D_CHARS_MPI_H

#include "utilities.h"
#include "serialize_dataprods.h"
//#include "mpi.h"
//#include "h5_setup_mpi.h
#define DEBUG
using product_t = std::vector<char>;

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

//these variables to save info....
std::map<std::string,dataset_info>list_createInfo;
std::map<std::string, dataset_info>size_createInfo;
int __tot_branches=0;
bool Writing_Data=false;
//end of these variables

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

  MPI_Barrier(MPI_COMM_WORLD);
  H5Sclose(dspace_id);
  H5Dclose(dset_id);
  H5Pclose(dplist_id);

  return mpi_rank;
}

//***********************************************************************************//


//***********************************************************************************//

int WriteDataSets(std::string branch_name,std::vector<char>buff,hid_t lumi_id,int ievt){
  int mpi_rank,mpi_size;
  MPI_Comm_size(MPI_COMM_WORLD,&mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&mpi_rank);
  //I think it is better to do everything in one go.
  auto curr_size = static_cast<unsigned long long>(buff.size());
  int _curr_size = static_cast<int>(curr_size);
  
  hsize_t buff_size[1] = {static_cast<hsize_t>(curr_size)};
  auto dataset_id = H5Dopen(lumi_id,branch_name.c_str(),H5P_DEFAULT);
  std::string offset_name = branch_name+"_offset";
  auto dataset_offset = H5Dopen(lumi_id,offset_name.c_str(),H5P_DEFAULT);
  std::string size_name = branch_name+"_size";
  auto dataset_size = H5Dopen(lumi_id,size_name.c_str(),H5P_DEFAULT);

  //do i need to do this each time I write buffer or it can be a one time thing

  auto xf_id = H5Pcreate(H5P_DATASET_XFER);
  auto xf_id_sz = H5Pcreate(H5P_DATASET_XFER);
  auto xf_id_offset = H5Pcreate(H5P_DATASET_XFER);
  
  H5Pset_dxpl_mpio(xf_id,H5FD_MPIO_COLLECTIVE);
  
  auto _dspace_id = H5Dget_space(dataset_id);
  auto _dspace_offset = H5Dget_space(dataset_offset);
  auto _dspace_size = H5Dget_space(dataset_size);
  //okay the number of dims are going to be same for the meta-datasets as well.
  auto ndims = H5Sget_simple_extent_ndims(_dspace_id);

  hsize_t curr_dims[ndims];
  hsize_t curr_dims_sz[ndims];
  hsize_t curr_dims_offset[ndims];
  H5Sget_simple_extent_dims(_dspace_id,curr_dims,NULL);
  H5Sget_simple_extent_dims(_dspace_size,curr_dims_sz,NULL);
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
  hsize_t offset[1];
  hsize_t new_dims_sz[1];
  hsize_t new_dims_offset[1];
  hsize_t sz_offset[1];

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
  auto status_sz = H5Dset_extent(dataset_size,new_dims_sz);
  auto status_offset = H5Dset_extent(dataset_offset,new_dims_sz);

  //always remember after the extension, need to do this one more time:
   _dspace_id = H5Dget_space(dataset_id);
   _dspace_offset = H5Dget_space(dataset_offset);
   _dspace_size = H5Dget_space(dataset_size);
  
  auto space_status = H5Sselect_hyperslab(_dspace_id,H5S_SELECT_SET,
					  offset,NULL,count,buff_size);

  space_status = H5Sselect_hyperslab(_dspace_offset,H5S_SELECT_SET,
				     sz_offset,NULL,count,count);

  space_status = H5Sselect_hyperslab(_dspace_size,H5S_SELECT_SET,
				     sz_offset,NULL,count,count);

  auto _mspace_id = H5Screate_simple(1,buff_size,NULL);
  auto _mspace_sz = H5Screate_simple(1,count,NULL);
  auto _mspace_offset= H5Screate_simple(1,count,NULL);
  char *__buff = buff.data();
  std::vector<char>__buff_sz_ = {static_cast<char>(_curr_size)};
  std::vector<char>__buff_offset_ = {static_cast<char>(offset[0])};

  char *__buff_offset = __buff_offset_.data();
  char *__buff_sz = __buff_sz_.data();

#ifdef DEBUG
  std::cout<<" Size of Offset Buffer "<<__buff_offset_.size()<<
    " Size of Sz Buffer "<<__buff_sz_.size()<<std::endl;
#endif  
  auto _status = H5Dwrite(dataset_id,H5T_NATIVE_CHAR,_mspace_id,
  			  _dspace_id,xf_id,__buff);
  assert(_status>=0);
  
   _status = H5Dwrite(dataset_offset,H5T_NATIVE_CHAR,_mspace_offset,
  		     _dspace_offset,xf_id,__buff_offset);
  assert(_status>=0);

  _status = H5Dwrite(dataset_size,H5T_NATIVE_CHAR,_mspace_sz,
		     _dspace_size,xf_id,__buff_sz);
  assert(_status>=0);
  
  MPI_Barrier(MPI_COMM_WORLD);

  H5Pclose(xf_id);
  H5Pclose(xf_id_sz);
  H5Pclose(xf_id_offset);
  H5Sclose(_dspace_id);
  H5Sclose(_dspace_offset);
  H5Sclose(_dspace_size);
  H5Sclose(_mspace_id);
  H5Sclose(_mspace_sz);
  H5Sclose(_mspace_offset);
  H5Dclose(dataset_offset);
  H5Dclose(dataset_size);
  H5Dclose(dataset_id);
  return dataset_id;
}

//***********************************************************************************//

void write_1D_chars_MPI(std::vector<product_t> const& products, 
			std::vector<std::string> const& ds_names, 
			long unsigned int batch, 
			long unsigned int nbatch, 
			long unsigned int round,
			hid_t lumi_id,int mpi_rank,
			int mpi_size, int ievt) 
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
    if(sum_prods==0)continue;
    std::vector<char>sz_vector = {char(tmp1.size())};
    auto szds_name = ds_name+"_sz";
    auto offset_name = ds_name+"_offset";
    auto status = WriteDataSets(ds_name,tmp1,lumi_id,ievt);

  }
}

void SetTotalBranches(int nbranch){
  __tot_branches=nbranch;
}
//***********************************************************************************//

#endif
