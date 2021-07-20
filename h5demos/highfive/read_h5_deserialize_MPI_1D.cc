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
#include "ccqe_data.h"


//using c++ api has its own limitations.....
//the c++ api for the hdf5
//#include "H5Cpp.h"
//see example below for the cpp api:
//https://support.hdfgroup.org/HDF5/doc/cpplus_RM/readdata_8cpp-example.html
 
//The idea is to deserialize the 2D data-set first.

void hdf5_read_ds(std::string hfilename){
  // I want to start by reading in the data-sets
  
  hid_t file_id = H5Fopen(hfilename.c_str(),H5F_ACC_RDONLY,H5P_DEFAULT);

  //lets try with one data-set...this will have to go into an argument later....
  std::string ds_name = "/run/lumi/phi";
  std::string sz_name = "/run/lumi/phi_size";
  hid_t pds_id,psz_id;
  auto ds_id = H5Dopen(file_id,ds_name.c_str(),pds_id);
  
  auto sz_id = H5Dopen(file_id,sz_name.c_str(),psz_id);
  
  std::cout<<pds_id<<" "<<psz_id<<std::endl;
  
  //now get the dimension of the data-set...
  //first the dataspace.
  auto dspace = H5Dget_space(ds_id);

  auto ndims = H5Sget_simple_extent_ndims(dspace);

  //now get the dimensions..
  hsize_t dims[ndims];
  H5Sget_simple_extent_dims(dspace,dims,NULL);

  std::cout<<"ndims "<<ndims<<" "<<dims[0]<<std::endl;

  char data[dims[0]];
  //now read the dataset..
  auto status = H5Dread(ds_id,H5T_NATIVE_CHAR,H5S_ALL,
			H5S_ALL,H5P_DEFAULT,data);

  char *val ;
  int size = 8;
  int tot_vals = dims[0]/size;

  
  for(int i=0;i<dims[0];i+=size){
    std::vector<char>vals;
    for(int j=0;j<size;j++){
      //   std::cout<<i<<" "<<j<<std::endl;
      val = &data[j];
      vals.push_back(data[j+i]);
      
    }
    double phi_val = 0.0;
    char* buff_ = vals.data();
    memcpy(&phi_val,buff_,sizeof(double));

   std::cout<<phi_val<<" "<<i<<" "<<std::endl;
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

