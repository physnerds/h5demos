#include "time.h"
#include <ctime>
#include "Token.h"
#include <string>
#include <sstream>
#include <cstddef>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <cassert>
#include "hdf5.h"
#include "mpi.h"
#include "H5FDmpio.h"
#include "OffsetInfo.h"


void test_parToken(){
    int flag;
    MPI_Initialized(&flag);
    int rank,size;
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    
    //I am pretty sure I need to revisit the chunking info....
    hsize_t dimsf[1] = {static_cast<hsize_t>(size)};
    hsize_t chunk_dims[1] = {static_cast<hsize_t>(size)};
    hsize_t max_dims[1] = {H5S_UNLIMITED};
    hsize_t mem_dims[1] = {1};
    
    auto plist_id = H5Pcreate(H5P_FILE_ACCESS);
    assert(plist_id>=0);
    auto ret = H5Pset_fapl_mpio(plist_id,MPI_COMM_WORLD,MPI_INFO_NULL);
    assert(ret>=0);
    auto filename = "test_file_token.h5";
    auto file_id = H5Fcreate(filename,H5F_ACC_TRUNC,H5P_DEFAULT,plist_id);
    
    
    //lets just create a data-set to store the fake tokens....
    auto dspace_id_offset = H5Screate_simple(1,dimsf,max_dims);
    auto dplist_id_offset = H5Pcreate(H5P_DATASET_CREATE);
    
    H5Pset_chunk(dplist_id_offset,1,chunk_dims);
    
    auto dname_offset = "offset_test";
    auto dset_id = H5Dcreate(file_id,dname_offset,H5T_NATIVE_INT,dspace_id_offset,
                             H5P_DEFAULT,dplist_id_offset,H5P_DEFAULT);
     
      
    
    H5Sclose(dspace_id_offset);
    H5Dclose(dset_id);    
    H5Pclose(dplist_id_offset);
    //H5Dclose(dset_id);
    //H5Pclose(plist_id);
    
    //okay I need to simulate the 2 data-set scenario at least I think...
    std::vector<int>num_list1;
    std::vector<int>num_list2;
    srand(time(NULL));
    int _iter = 5;
    for(int i = 0;i<size*_iter;i++){
        int random_number1 = rand()%100;
        int random_number2 = rand()%100;
        num_list1.push_back(random_number1);
        num_list2.push_back(random_number2);
               
    }
    
    if(rank==0){
     for(int k = 0;k<num_list1.size();k++)std::cout<<k<<" "<<num_list1[k]<<" "<<num_list2[k]<<std::endl;     
    }
    
    std::cout<<"Number of events in the fake data-set "<<num_list1.size()<<std::endl;
    std::vector<std::string>dsetnames = {"dset1","dset2"};


   
   //std::cout<<" Token check "<<token_stuff<<std::endl;
   MPI_Barrier(MPI_COMM_WORLD);
   //okay now just mimic the data-set filling part...
    
   for(int i = 0;i<_iter;i++){
       std::vector<int>dset_offset;
       for(int j = 0;j<dsetnames.size();j++){
           auto _name = dsetnames[j];
           //for now the random numbers are the dataset contents....
           //remember that we are mimicking that one set of random number
           //corresponds to one data-set and another set to the another data-set.
           int random_num;
           if(j==0)random_num = num_list1[rank+i*size];
           if(j==1)random_num = num_list2[rank+i*size];
           

           dset_offset.push_back(random_num);
       }
    std::cout<<"Size of the Dset-Offset is "<<dset_offset.size()<<" "<<
        dset_offset[0]<<" "<<dset_offset[1]<<std::endl;
   //okay so we have collected the pseudo offset info for all the dset for a given data-set. Now time to write in the token.
   
    auto buff_offset = dset_offset.data();
    
    int curr_size = dset_offset.size();
    int tot_size;
    
    MPI_Scan(&curr_size,&tot_size,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    
    //now open the data-set.
     std::cout<<"File id is "<<file_id<<std::endl;
       
    dset_id = H5Dopen(file_id,dname_offset,H5P_DEFAULT);
    
    assert(dset_id>=0);
    auto xf_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(xf_id,H5FD_MPIO_COLLECTIVE);
    auto _dspace_id = H5Dget_space(dset_id);
    
    auto ndims= H5Sget_simple_extent_ndims(_dspace_id);
    std::cout<<"Dimension of the data-set "<<ndims<<std::endl;
   // exit(1);
    hsize_t curr_dims[ndims];
    H5Sget_simple_extent_dims(_dspace_id,curr_dims,NULL);
    //if(i==0)curr_dims[0] = 0;
    MPI_Scan(&curr_size,&tot_size,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    int max_dims=0;
    MPI_Reduce(&tot_size,&max_dims,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);
    MPI_Bcast(&max_dims,1,MPI_INT,0,MPI_COMM_WORLD);
   // if(curr_dims[0]==0)
    
    hsize_t new_dims[1];
    hsize_t count[1] = {1};   
    hsize_t buff_size[1] = {static_cast<hsize_t>(curr_size)};
    if(i==0)new_dims[0] = {static_cast<hsize_t>(max_dims)};//{curr_dims[0]};//+tot_size};
    else
        new_dims[0] = {curr_dims[0]+max_dims};
  
       auto status_id = H5Dset_extent(dset_id,new_dims);
       //this is required to ensure that the new dimension is updated in the data-space.
       _dspace_id = H5Dget_space(dset_id);
       
       hsize_t offset[1];
       
       if(i==0)
           offset[0] = static_cast<hsize_t>(tot_size-curr_size);
       else
           offset[0] = static_cast<hsize_t>(curr_dims[0]+tot_size-curr_size);

       
       
       auto space_status = H5Sselect_hyperslab(_dspace_id,H5S_SELECT_SET,
                                               offset,NULL,count,buff_size);

       auto _mspace_id = H5Screate_simple(1,buff_size,NULL);
       
       auto _status = H5Dwrite(dset_id,H5T_NATIVE_INT,_mspace_id,
   			    _dspace_id,xf_id,buff_offset);
       std::cout<<"Wrote "<<dset_offset[0]<<" "<<dset_offset[1]<<" offset "<<offset[0]<<" size before write "<<curr_dims[0]<<" size after write "<<new_dims[0]<<" "<<rank<<std::endl;
       
       H5Pclose(xf_id);
       H5Sclose(_dspace_id);
       H5Sclose(_mspace_id);
       H5Dclose(dset_id);
   
   }
    H5Pclose(plist_id);
    H5Fclose(file_id);
    
    
}

int main(int argc, char* argv[]){
    int err = MPI_Init(&argc,&argv);
    test_parToken();
    return 1;
    
}
