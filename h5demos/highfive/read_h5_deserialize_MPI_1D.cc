#include "serialize_dataprods.h"

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
//#include "H5Cpp.h"

//using c++ api has its own limitations.....
//the c++ api for the hdf5
//#include "H5Cpp.h"
//see example below for the cpp api:
//https://support.hdfgroup.org/HDF5/doc/cpplus_RM/readdata_8cpp-example.html
 
//The idea is to deserialize the 1D data-set first.

void hdf5_read_ds(std::string hfilename){
  // I want to start by reading in the data-sets
  
  hid_t file_id = H5Fopen(hfilename.c_str(),H5F_ACC_RDONLY,H5P_DEFAULT);

  //lets try with one data-set...this will have to go into an argument later....
  std::string _name = "Q2";
  std::string ds_name = "/run/lumi/"+_name;
  std::string info_name = ds_name+"_info";
  std::string offset_name = ds_name+"_offset";
  
  hid_t pds_id, info_id,offset_id;
  auto ds_info_id = H5Dopen(file_id,info_name.c_str(),info_id);
  auto dspace_info = H5Dget_space(ds_info_id);
  hsize_t dims_info[1];
  H5Sget_simple_extent_dims(dspace_info,dims_info,NULL);
  char data_type[dims_info[0]];

  assert(H5Dread(ds_info_id,H5T_NATIVE_CHAR,H5S_ALL,
		 H5S_ALL,H5P_DEFAULT,data_type)>=0);
  
  std::cout<<data_type<<" data type "<<std::endl;

  auto ds_offset_id = H5Dopen(file_id,offset_name.c_str(),offset_id);
  auto dspace_offset = H5Dget_space(ds_offset_id);

  auto ndims_offset = H5Sget_simple_extent_ndims(dspace_offset);
  hsize_t dims_offset[ndims_offset];

  H5Sget_simple_extent_dims(dspace_offset,dims_offset,NULL);

  int offset_data[dims_offset[0]];
  assert(H5Dread(ds_offset_id,H5T_NATIVE_INT,H5S_ALL,
		 H5S_ALL,H5P_DEFAULT,offset_data)>=0);
  
  
  auto ds_id = H5Dopen(file_id,ds_name.c_str(),pds_id);

  auto dspace = H5Dget_space(ds_id);

  auto ndims = H5Sget_simple_extent_ndims(dspace);
  hsize_t dims[ndims];

  H5Sget_simple_extent_dims(dspace,dims,NULL);

  char data[dims[0]];
  auto status = H5Dread(ds_id,H5T_NATIVE_CHAR,H5S_ALL,
			H5S_ALL,H5P_DEFAULT,data);


  int tot_vals = dims_offset[0];
  std::cout<<"Total events are "<<tot_vals<<" "<<offset_data[39]<<std::endl;
  for(int i=0;i<tot_vals;i++){
    std::vector<char>vals;
    int offset = offset_data[i];
    int size;
    if(i==tot_vals-1)size = dims[0] - offset_data[tot_vals-1];
    else
      size = offset_data[i+1]-offset_data[i];
    //  std::cout<<" size "<<size<<" "<<offset_data[i]<<" "<<dims_offset[0]<<" "<<i<<std::endl;
    
    for(int j=offset;j<size+offset;j++){
      vals.push_back(data[j]);      
    }
    double _val = 0.0;
    char *buff = vals.data();
    memcpy(&_val,buff,sizeof(double));
    std::cout<<i<<" "<<_val<<std::endl;
    vals.clear();
    
    
  }
    

}

  
int main(int argc, char* argv[]) {
  if(argc!=2){
    std::cout<<" hdf5 filename required "<<std::endl;
    return 0;
  }
  auto h5name = argv[1];
  hdf5_read_ds(h5name);
  return 0;
}

