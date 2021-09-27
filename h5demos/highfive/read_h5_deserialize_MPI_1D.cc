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
 
//The idea is to deserialize the 1D data-set first.

void hdf5_read_ds(std::string hfilename){
  // I want to start by reading in the data-sets
  
  hid_t file_id = H5Fopen(hfilename.c_str(),H5F_ACC_RDONLY,H5P_DEFAULT);

  //lets try with one data-set...this will have to go into an argument later....
  std::string _name = "Q2";
  std::string ds_name = "/run/lumi/"+_name;
  hid_t pds_id;
  auto ds_id = H5Dopen(file_id,ds_name.c_str(),pds_id);

  auto dspace = H5Dget_space(ds_id);

  auto ndims = H5Sget_simple_extent_ndims(dspace);
  hsize_t dims[ndims];

  H5Sget_simple_extent_dims(dspace,dims,NULL);

  char data[dims[0]];
  auto status = H5Dread(ds_id,H5T_NATIVE_CHAR,H5S_ALL,
			H5S_ALL,H5P_DEFAULT,data);

  int size =8;
  int tot_vals = dims[0]/size;

  std::cout<<"Total events are "<<tot_vals<<std::endl;

  for(int i=0;i<tot_vals;i++){
    std::vector<char>vals;
    int offset = 8;
    for(int j=i*offset;j<size+i*offset;j++){
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

