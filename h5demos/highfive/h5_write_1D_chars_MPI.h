#ifndef H5_WRITE_1D_CHARS_MPI_H
#define H5_WRITE_1D_CHARS_MPI_H

#include "utilities.h"
#include "serialize_dataprods.h"
#include <ctime>
#include "hdf5.h"
#include "H5FDmpio.h"

#include <string>
#include <cstddef>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <numeric>
#include <sstream>


std::time_t initial_time;
using product_t = std::vector<char>;



std::vector<char>
flatten_MPI(std::vector<std::vector<char>> const& tmp) {
  std::vector<char> tmp1 = tmp[0];
  for (int i = 1; i<tmp.size(); ++i)
     tmp1.insert(end(tmp1), begin(tmp[i]), end(tmp[i]));
  return tmp1;
}

//*******************************************************************************

template<typename T>
std::vector<char>GetMemcpyArray(T _val){
  char arr_val[sizeof(_val)];
  memcpy(arr_val,&_val,sizeof(_val));
  std::vector<char>blob;
  blob.insert(blob.begin(),std::begin(arr_val),std::end(arr_val));
  return blob;
}
//*******************************************************************************//
//int CreateEventInfo(std::string _name, int batch,int mpi_size, int tot_events,hid_t lumi_id){
int CreateEventInfo(std::string _name, std::vector<int>_meta, hid_t lumi_id, bool chunk=false){
    std::cout<<" Size of the EventInfo "<<_meta.size()<<std::endl;
  int *__buff = _meta.data();
  
  hsize_t chunk_dims[1] = {static_cast<hsize_t>(_meta.size())};

  hsize_t dimsf[1] = {chunk_dims[0]};
  
    hsize_t max_dims[1]={H5S_UNLIMITED};
    hid_t dspace_id;
    if(chunk)
        dspace_id = H5Screate_simple(1,dimsf,max_dims);
    else
        dspace_id = H5Screate_simple(1,dimsf,NULL);
  
  auto dplist_id = H5Pcreate(H5P_DATASET_CREATE);

  if(chunk)
      H5Pset_chunk(dplist_id,1,chunk_dims);
  //  auto xf_id = H5Pcreate(H5P_DATASET_XFER);

  auto dset_id = H5Dcreate(lumi_id,_name.c_str(),
			   H5T_NATIVE_INT,dspace_id,H5P_DEFAULT,
			   dplist_id,H5P_DEFAULT);


  hsize_t offset[1] = {0};
  hsize_t count[1] = {1};

  auto space_status = H5Sselect_hyperslab(dspace_id,H5S_SELECT_SET,
					  offset,NULL,count,chunk_dims);

  auto _mspace_id = H5Screate_simple(1,chunk_dims,NULL);


  auto _status = H5Dwrite(dset_id,H5T_NATIVE_INT,_mspace_id,
			  dspace_id,H5P_DEFAULT,__buff);

  assert(_status>=0);
  // assert(H5Pclose(xf_id)>=0);
  assert(H5Sclose(dspace_id)>=0);
  assert(H5Pclose(dplist_id)>=0);
  assert(H5Dclose(dset_id)>=0);

  return _status;

}
//***********************************************************************
std::string GetAttributes(int batch, std::string type){
    std::stringstream ss;
    ss<<batch;
    std::string _atr = ss.str()+","+type;
    return _atr;
}



//*********************************************************************************//

void SetDataSetRawChunkCache(hid_t dset_id,hsize_t chunk_size){
 //for now I am just going to hard code the number of objects and bytes in cache
 
     //Based on the documentation of H5Pset_chunk_cache();
    hsize_t max_object_in_chunk_cache = chunk_size*100;//12799u;

    hsize_t one_mb = 1024u*1024u; //1 MB
    //Total size of the raw data chunk cache for this data-set.
    hsize_t max_bytes_in_cache = 10u*one_mb; 
    hid_t dapl_id = H5Dget_access_plist(dset_id);
    H5Pset_chunk_cache(dapl_id,
                      max_object_in_chunk_cache,
                      max_bytes_in_cache,
                      H5D_CHUNK_CACHE_W0_DEFAULT);
}

//***********************************************************************************************//

//only create the data-sets nothing else.
int CreateDataSet(TBranch *branch,int buff_size,int batch_size, hid_t lumi_id){
  
  int mpi_size, mpi_rank;
  MPI_Comm_size(MPI_COMM_WORLD,&mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&mpi_rank);
  hsize_t dimsf[1],
    max_dims[1];
//  TClass *class_ptr = nullptr;
//  EDataType type;
//  branch->GetExpectedType(class_ptr,type);
 // if(class_ptr==nullptr)return mpi_size;

  std::string branch_name = branch->GetName();
  std::string branch_info = branch_name+"_info";


  auto branch_type = return_datatype(branch); 
 // std::cout<<"Single Creation "<<branch->GetName()<<" "<<branch_type<<std::endl;
  hid_t dset_id;
  hid_t offset_id;
  hid_t info_id;
  dimsf[0]=0;
  max_dims[0]=H5S_UNLIMITED;
  hsize_t chunk_size = static_cast<hsize_t>(buff_size*1500);
  hsize_t chunk_dims[1] = {chunk_size}; //okay i had to add that extra number after mpi-size

  
  auto dspace_id = H5Screate_simple(1,dimsf,max_dims);
  
  auto dplist_id = H5Pcreate(H5P_DATASET_CREATE);

  auto xf_id = H5Pcreate(H5P_DATASET_XFER);
  
  //This is a bit challenging. To guarantee the smooth IO all the IO processes should use same chunk dims.
    
  H5Pset_chunk(dplist_id,1,chunk_dims);
  dset_id = H5Dcreate(lumi_id,branch_name.c_str(),H5T_NATIVE_CHAR,dspace_id,
		      H5P_DEFAULT,dplist_id,H5P_DEFAULT);
 
    SetDataSetRawChunkCache(dset_id,chunk_size) ;
  //Create the Atributes:
//    auto attr_id = H5Acreate (dset_id, "Attr", H5T_STD_I32BE, dspace_id, H5P_DEFAULT, H5P_DEFAULT);
 //  H5Aclose (attr_id);


    assert(H5Pclose(xf_id)>=0);

    
    
    assert(H5Sclose(dspace_id)>=0);
    assert(H5Dclose(dset_id)>=0);
    assert(H5Pclose(dplist_id)>=0);
    

  return mpi_size;
}
//************************************************************************************************//


void CreateDataSets(TObjArray *l, std::string tree_name,int batch_size, hid_t lumi){
    
    auto tot_branches = l->GetEntriesFast();
    auto classes = return_classes(l);
    auto dset_names = return_dsetnames(l);
    
    for(Long64_t jentry=0;jentry<tot_branches;++jentry){
        auto branch = dynamic_cast<TBranch*>((*l)[jentry]);
        std::string branch_name = branch->GetName();
//	std::cout<<"Creating "<<branch_name<<" "<<classes[jentry]<<" "<<jentry<<std::endl;
	auto _class = return_class(branch);
        if(_class==nullptr){
            std::cout<<"Found nullptr "<<branch_name<<std::endl;
            auto blob = return_fundamental_blobs(branch);
            //DataSet for the actual branch
            //CreateDataSet(branch,blob.size(),batch_size, lumi);
          }
          else{
//   std::cout<<"Return blob "<<branch_name<<std::endl;
            auto blob = return_blob(branch,_class);
            CreateDataSet(branch,blob.size(),batch_size, lumi);
            
          }
 //  std::cout<<"Created "<<branch->GetName()<<std::endl; 
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    
}


//*********************************************************************************//

int WriteTokenInfo(hid_t lumi_id, int ievt,std::vector<int>&token_info){

    int rank;
    
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    
    int token_size = static_cast<int>(token_info.size());
    auto buff_offset = token_info.data();
    auto dset_id = H5Dopen(lumi_id,"TokenInfo", H5P_DEFAULT);
    assert(dset_id>=0);
    auto xf_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(xf_id,H5FD_MPIO_COLLECTIVE);
    auto _dspace_id = H5Dget_space(dset_id);
    assert(_dspace_id>=0);
    auto ndims = H5Sget_simple_extent_ndims(_dspace_id);
    
    hsize_t curr_dims[ndims];
    H5Sget_simple_extent_dims(_dspace_id,curr_dims,NULL);
    
    int tot_dims;
    int _curr_dims = token_size;//static_cast<int>(curr_dims[0]);
    
    MPI_Scan(&_curr_dims,&tot_dims,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    int max_dims = 0;
    MPI_Allreduce(&tot_dims,&max_dims,1,MPI_INT,MPI_MAX,MPI_COMM_WORLD);
    MPI_Bcast(&max_dims,1,MPI_INT,0,MPI_COMM_WORLD);

    hsize_t new_dims[1];
    hsize_t count[1] = {1};
    hsize_t buff_size[1] = {static_cast<hsize_t>(token_size)};
    
    new_dims[0] = {curr_dims[0]+static_cast<hsize_t>(max_dims)};
    auto status_id = H5Dset_extent(dset_id,new_dims);
    _dspace_id = H5Dget_space(dset_id);
    assert(_dspace_id>=0);
    hsize_t offset[1];
   
    if(ievt==0)
        offset[0] = static_cast<hsize_t>(tot_dims-token_size);
    else
        offset[0] = static_cast<hsize_t>(curr_dims[0]+tot_dims-token_size);
   

    
    
    auto space_status = H5Sselect_hyperslab(_dspace_id,H5S_SELECT_SET,
                                            offset,NULL,count,buff_size);

    auto _mspace_id = H5Screate_simple(1,buff_size,NULL);
    auto _status = 1;    
 //   std::cout<<"Token size "<<token_size<<" new dims "<<new_dims[0]
 //       <<" curr size "<<curr_dims[0]<<" Offset "<<offset[0]<<std::endl;      
    _status = H5Dwrite(dset_id,H5T_NATIVE_INT,_mspace_id,
                _dspace_id,xf_id,buff_offset);


        
    
    H5Pclose(xf_id);
    H5Sclose(_dspace_id);
    H5Sclose(_mspace_id);
    H5Dclose(dset_id);
    
    return _status;
    
    
}
std::vector<int> WriteDataSets(std::string branch_name,std::vector<char>const &buff,hid_t lumi_id,int ievt,int mpi_rank,int mpi_size){
  
  //I think it is better to do everything in one go.
  auto curr_size = static_cast<unsigned long long>(buff.size());
  int _curr_size = static_cast<int>(curr_size);
  
  hsize_t buff_size[1] = {static_cast<hsize_t>(curr_size)};
  auto dataset_id = H5Dopen(lumi_id,branch_name.c_str(),H5P_DEFAULT);
  std::string offset_name = branch_name+"_offset";

  assert(dataset_id>=0);
  auto xf_id = H5Pcreate(H5P_DATASET_XFER);


  H5Pset_dxpl_mpio(xf_id,H5FD_MPIO_COLLECTIVE);

  
  auto _dspace_id = H5Dget_space(dataset_id);
  
  auto ndims = H5Sget_simple_extent_ndims(_dspace_id);
  
  hsize_t curr_dims[ndims];
  H5Sget_simple_extent_dims(_dspace_id,curr_dims,NULL);
  
  int tot_dims;
  int _curr_dims = static_cast<int>(curr_dims[0]);

  //probably need to scan 
  
  MPI_Scan(&_curr_dims,&tot_dims,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
  int max_dims=0;
  MPI_Allreduce(&_curr_dims,&max_dims,1,MPI_INT,MPI_MAX,MPI_COMM_WORLD);

  int tot_buff_size=0;
  int int_buff_size=_curr_size;

  MPI_Scan(&int_buff_size,&tot_buff_size,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
  int max_buff_size=0;
  MPI_Allreduce(&tot_buff_size,&max_buff_size,1,MPI_INT,MPI_MAX,MPI_COMM_WORLD);

  int parcel[2] = {max_dims,max_buff_size};

  MPI_Bcast(&parcel,2,MPI_INT,0,MPI_COMM_WORLD);
  
  hsize_t _tot_size = static_cast<hsize_t>(tot_buff_size);
  hsize_t new_dims[1];
  hsize_t new_dims_offset[1];
  hsize_t offset[1];
 
  if(ievt==0){
  //  new_dims_offset[0] = {static_cast<hsize_t>(mpi_size)};
    new_dims[0] = {static_cast<hsize_t>(parcel[1])};
    offset[0] = {static_cast<hsize_t>(tot_buff_size-int_buff_size)};
  }
  else{
  //  new_dims_offset[0] = {static_cast<hsize_t>(mpi_size+curr_dims_offset[0])};
    new_dims[0] = {static_cast<hsize_t>(curr_dims[0]+parcel[1])};
    offset[0] = {curr_dims[0]+static_cast<hsize_t>(tot_buff_size-int_buff_size)};
  }


  hsize_t count[1] = {1}; //This is what we will put for now...
  hid_t _status = -1;
  hid_t _mspace_id;
 // while(_status<0){
  auto status_id = H5Dset_extent(dataset_id,new_dims);
  
  //assert(status_id>=0);
  _dspace_id = H5Dget_space(dataset_id);
    
// assert(_dspace_id>0);
  auto space_status = H5Sselect_hyperslab(_dspace_id,H5S_SELECT_SET,
					  offset,NULL,count,buff_size);

   _mspace_id = H5Screate_simple(1,buff_size,NULL);

  assert(space_status>=0);
  assert(_mspace_id>=0);

 
  const char *__buff = buff.data();
  std::vector<char>__buff_sz_ = {static_cast<char>(_curr_size)};

  char *__buff_sz = __buff_sz_.data();

   _status = H5Dwrite(dataset_id,H5T_NATIVE_CHAR,_mspace_id,
  			  _dspace_id,xf_id,__buff);


    assert(H5Pclose(xf_id)>=0);
    assert(H5Sclose(_dspace_id)>=0);
    assert(H5Sclose(_mspace_id)>=0);
    assert(H5Dclose(dataset_id)>=0);
    std::vector<int>_val = {static_cast<int>(_status),static_cast<int>(offset[0])}; 
  return _val;
}

//***********************************************************************************//
int WriteDataSetsSerial(std::string branch_name,std::vector<char>&buff,hid_t lumi_id, int tentry){
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

std::vector<int>WriteDataAndReturnStatus(std::vector<product_t> const& products, 
			std::vector<std::string> const& ds_names,                             
           long unsigned int round,
           hid_t lumi_id, int mpi_rank,
           int mpi_size,int st_val, int end_val){
   
    std::vector<int>token_info;
    int num_prods = ds_names.size();
    for(int prod_index = st_val; prod_index<end_val;++prod_index){
        auto prod_size = products[prod_index].size();
        auto tmp = get_prods_ver2(products,prod_index,end_val, num_prods);
        auto tmp1 = flatten_MPI(tmp);
        auto ds_name = ds_names[prod_index];
        
        std::vector<int>_status={-1,-1}; 
        _status = WriteDataSets(ds_name,tmp1,lumi_id,round,mpi_rank,mpi_size); 
       token_info.push_back(_status[1]);
    }
        
       return token_info;
}
    

//*********************************************************************************//

void write_1D_chars_MPI(std::vector<product_t> const& products, 
			std::vector<std::string> const& ds_names, 
			long unsigned int batch, 
			long unsigned int nbatch, 
			long unsigned int round,
			hid_t lumi_id,int mpi_rank,
			int mpi_size, int ievt, int tentry,bool serial=false) 
{

    
  std::vector<int>token_info;
  auto num_prods = ds_names.size();
  for(int prod_index = 0;prod_index<num_prods;++prod_index){

    auto prod_size = products[prod_index].size();
    auto tmp = get_prods(products,prod_index,num_prods);
    auto sizes = get_sizes(tmp);
    auto tmp1 = flatten_MPI(tmp);
   // auto sizes1 = get_sizes1D(tmp1);
    auto ds_name = ds_names[prod_index];
    
    std::vector<int>_status={-1,-1}; 

   _status = WriteDataSets(ds_name,tmp1,lumi_id,round,mpi_rank,mpi_size);

   token_info.push_back(_status[1]);
        

  }
  MPI_Barrier(MPI_COMM_WORLD);

  hid_t tk_status = -1;

  tk_status = WriteTokenInfo(lumi_id, round,token_info);

  MPI_Barrier(MPI_COMM_WORLD);
  token_info.clear();
}


//***********************************************************************************//

#endif
