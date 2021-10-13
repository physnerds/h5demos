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

std::vector<std::string>DataVector;

struct GetDataSetsInfo{
  GetDataSetsInfo(hid_t file_id,H5O_iterate_t op_func)
    :_file_id(file_id),_op_func(op_func)
  {
    auto _status = H5Ovisit (file_id, H5_INDEX_NAME, H5_ITER_NATIVE, op_func, NULL);
  }
  
  hid_t _file_id;
  H5O_iterate_t _op_func;
  std::string _filename;
  std::vector<std::string>dsetnames;
};


//***************************************************************************************//

herr_t op_func (hid_t loc_id, const char *name, const H5O_info_t *info, void *operator_data)
{
    if(name[0]!='.'){
      if(info->type==H5O_TYPE_DATASET){
	std::string str(name);
	std::size_t found_info = str.find("info");
	std::size_t found_size = str.find("_size");
	std::size_t found_offset = str.find("_offset");
	if(found_size!=std::string::npos){
	  std::cout<<"Skipping "<<str<<std::endl;
	}

	else if(found_info!=std::string::npos){
	  std::cout<<"Skipping "<<str<<std::endl;
	}
	else if(found_offset!=std::string::npos){
	  std::cout<<"Skipping "<<str<<std::endl;
	}
	else
	  DataVector.push_back(str);

      }
    }

    return 0;
}


//*****************************************************************************************//
template<typename T>
std::vector<T> DataArray(std::string hfilename,std::string ds_name,hid_t data_type){
  hid_t file_id = H5Fopen(hfilename.c_str(),H5F_ACC_RDONLY,H5P_DEFAULT);
  hid_t ds_id;
  auto ds_info_id = H5Dopen(file_id,ds_name.c_str(),H5P_DEFAULT);
  assert(ds_info_id>=0);
  auto dspace_info_id = H5Dget_space(ds_info_id);
  assert(dspace_info_id>=0);
  hsize_t dims_info[1];
  H5Sget_simple_extent_dims(dspace_info_id,dims_info,NULL);
  std::cout<<"dims info "<<dims_info[0]<<std::endl;
  T data_container[dims_info[0]];

  assert(H5Dread(ds_info_id,data_type,H5S_ALL,
		 H5S_ALL,H5P_DEFAULT,data_container)>=0);

  assert(H5Dclose(ds_info_id)>=0);
  std::vector<T>vec_container;
  for(int i=0;i<dims_info[0];i++)vec_container.push_back(data_container[i]);

  return vec_container;
}

//******************************************************************************
std::vector<char>GetBufferChunks(std::vector<int>offset_val,std::vector<char>_val,int start,int end){
  std::vector<char>chunks;

  for(int i=start;i<end;i++){
    chunks.push_back(_val[i]);
  }
  return chunks;
  
}

//*****************************************************************************//
template<typename T>
std::vector<T>GetValues(std::vector<int>offset_val,std::vector<char>_val,int batch_size,int tot_entries){
  std::vector<T> values;
  int buff_size = (offset_val[1]-offset_val[0])/batch_size;
  int tot_events = tot_entries;
  assert(_val.size()%buff_size==0);
  for(int i= 0;i<offset_val.size();i++){
    int start = offset_val[i];
    int end;
    
    if(i==offset_val.size()-1)end = _val.size();
    else
      end = offset_val[i+1];
    
    std::vector<char>buff_chunks = GetBufferChunks(offset_val,_val,start,end);

    for(int j=0;j<buff_chunks.size();j+=buff_size){
      std::vector<char>ind_buff;
      for(int k=j;k<j+buff_size;k++){
	ind_buff.push_back(buff_chunks[k]);
	
      }
      T final_val;
      char *buff_val = ind_buff.data();
      memcpy(&final_val,buff_val,sizeof(T));
      values.push_back(final_val);
      ind_buff.clear();
    }
    buff_chunks.clear();

  }
  std::cout<<"Total events recovered "<<values.size()<<std::endl;
  return values;

}

//******************************************************************************//
void hdf5_read_ds(std::string hfilename,std::string _name){
  // I want to start by reading in the data-sets
  


  //lets try with one data-set...this will have to go into an argument later....
  std::string ds_name = _name;
  std::string info_name = ds_name+"_info";
  std::string offset_name = ds_name+"_offset";
  std::string event_info = "/run/lumi/eventinfo";
  hid_t pds_id, info_id,offset_id;
  
  std::vector<char>_val = DataArray<char>( hfilename,ds_name,H5T_NATIVE_CHAR);

  std::vector<int> _info = DataArray<int>(hfilename,event_info,H5T_NATIVE_INT);

  std::vector<int>_val_offset = DataArray<int>(hfilename,
					       offset_name,H5T_NATIVE_INT);

  std::vector<char>_val_type = DataArray<char>(hfilename,info_name,
					       H5T_NATIVE_CHAR);

  std::cout<<" Type of Data is ";
  for(auto v:_val_type)std::cout<<v<<"";
  std::cout<<" \n"<<std::endl;
  
  std::cout<<_info[0]<<" Batch \n"
	   <<_info[1]<<" MPI-Size \n"
	   <<_info[2]<<" Total Events \n"<<std::endl;


  std::vector<double>values = GetValues<double>(_val_offset,_val,_info[0],_info[2]);


  for(int i=0;i<_info[2];i++)std::cout<<_name<<" "<<i<<" "<<values[i]<<std::endl;

}
//*******************************************************************************************//


void GetFileContent(std::string hfilename){
  auto file = H5Fopen(hfilename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
  GetDataSetsInfo dinfo = GetDataSetsInfo(file,op_func);

  for(size_t i=0;i!=DataVector.size();++i){
    hdf5_read_ds(hfilename,DataVector[i]);

  }
  
}
//***********************************************************//
int main(int argc, char* argv[]) {
  if(argc!=2){
    std::cout<<" hdf5 filename required "<<std::endl;
    return 0;
  }
  auto h5name = argv[1];
  //hdf5_read_ds(h5name);
  GetFileContent(h5name);
  
  return 0;
}

