#ifndef H5_WRITE_CHARS_H
#define H5_WRITE_CHARS_H


//#include "h5_write_1D_chars.h"
#include "h5_write_1D_chars_MPI.h"
#include "serialize_dataprods.h"
#include "utilities.h"
#include "HDFCxx.h"

#include <string>
#include <cstddef>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <unistd.h>
#include <algorithm>
#include "TH1D.h"
#include "TChain.h"
#include "TH2D.h"
#include "TFile.h"
#include "hdf5.h"
#include "mpi.h"
#include "H5FDmpio.h"

using product_t = std::vector<char>;
using namespace cce::tf;

void ConvertHDF5_MPI(char* input_file_dir,int batch,int run_num,int lumi_num,std::string const& outname ){
  
  //start-time
  initial_time = std::time(nullptr);
 
  //MPI-Initializations if not initialized
  int argc; char **argv;
  int flag;
  MPI_Initialized(&flag);
  if(!flag)MPI_Init(&argc,&argv);

  int global_size,global_rank;
    
  MPI_Comm_size(MPI_COMM_WORLD,&global_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&global_rank);
 
  std::cout<<"MPI RANK "<<global_rank<<" MPI SIZE "<<global_size<<std::endl;
    
  MPI_Info info;
  MPI_Info_create(&info);
    
  hid_t file_id;      //File ID
  hid_t run,lumi;   //Group ID



   //create property lists of the hdf5 create attributes and groups etc
   auto plist_id = H5Pcreate (H5P_FILE_ACCESS);

   auto ret = H5Pset_fapl_mpio(plist_id,MPI_COMM_SELF,info);
    assert(ret>=0);
   // H5Pset_fapl_mpio( file_access, MPI_COMM_SELF, MPI_INFO_NULL ) 
    auto H5FILE_NAME = outname.c_str();
    file_id = H5Fcreate(H5FILE_NAME, H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);

    auto run_name = "/run";
    auto scalar_id = H5Screate(H5S_SCALAR);
    run = H5Gcreate(file_id,run_name,H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT);
    auto scalar_attr = H5Acreate(run,run_name,H5T_NATIVE_INT,scalar_id,H5P_DEFAULT,H5P_DEFAULT);

    auto att_status = H5Awrite(scalar_attr,H5T_NATIVE_INT,&run_num);
    //okay inside group create lumi....
    auto lumi_name = "lumi";
    lumi = H5Gcreate(run,lumi_name,H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT);
    //create the attribute for lumi....
    auto lumi_att_id = H5Screate(H5S_SCALAR);
    auto lumi_attr = H5Acreate(lumi,lumi_name,H5T_NATIVE_INT,lumi_att_id,H5P_DEFAULT,H5P_DEFAULT);

    //create the DataSets based on the Input ROOT File

    std::vector<product_t>products;
    std::string ntuple_name = "CollectionTree";
    auto f = TFile::Open(input_file_dir); 
    auto e = (TTree*)f->Get(ntuple_name.c_str());
    auto l_old = e->GetListOfBranches();
    //I want to test with a smaller set of branches...
    int new_length = l_old->GetEntriesFast();
    
    auto l = new TObjArray(new_length);
    int st_val = 0;
      for( int i=st_val; i<new_length; ++i) {
          
          l->AddAt(l_old->At(i),i-st_val);
      }
      
     std::cout<<"Size of new branch array "<<l->GetEntriesFast()<<std::endl;

    //get the total number of branches.... 

    auto nentries = e->GetEntriesFast();
    auto tot_branches = l->GetEntriesFast();
    auto dset_names = return_dsetnames(l);
    nentries = 1000;
    std::cout<<"Total Number of Entries are "<<nentries<< " Total Branches"<< tot_branches<<" "<<dset_names.size()<<std::endl;
   tot_branches = dset_names.size(); 
  
    CreateDataSets(l,ntuple_name,batch, lumi);
    
    //also create the data-set for the Token
    std::vector<int>dummy_token={0};
    auto token_info = CreateEventInfo("TokenInfo",dummy_token,lumi,true);

    //End of Data Set Creation
    
    int nbatch = 0;
    int round = 0;

    int niter = nentries/(global_size);
    int remainder = nentries%(global_size);
    int new_remainder = remainder;
    //if(new_remainder<global_size) new_remainder=global_size;
    if(remainder!=0)
      niter = niter+1; //just add the remainder back and then later on put empty buffers on the extra entries.
    int batch_status = nentries%(batch*global_size);//mpi ranks maybe not have same events all the time..need correction
    std::vector<int>_meta = {batch,global_size,int(nentries)}; //please do not change the order of this info. Ordering matters when reading back the info.
    auto event_info = CreateEventInfo("eventinfo", _meta, lumi);
    
    std::cout<<"Total Entries "<<nentries<<" "<<niter*global_size+remainder
	     <<" Iter "<<niter<<" Remainder "<<remainder<<std::endl;
    MPI_Barrier(MPI_COMM_WORLD);
    
    
   //Write data-products into HDF5  
  
    for(int i=0;i<niter;i++){
      auto j = global_rank+global_size*i;
      e->GetEntry(j);
      
        
      std::vector<product_t> temp_products = ReturnBlobs(l,tot_branches);
        if(products.size()==0)products.reserve(temp_products.size());
      else
          products.resize(products.size()+temp_products.size());
      copy(temp_products.begin(),temp_products.end(),back_inserter(products));
        
      temp_products.clear();
      int next_j = global_rank+global_size*(i+1);
      nbatch++;
      if(next_j<nentries){
	if(nbatch==batch||j==nentries-1){
	  write_1D_chars_MPI(products,dset_names,batch,nbatch,
        			     round,lumi,global_rank,global_size,i,j,false);
	  
	  
	  nbatch=0;
	  products.clear();
	  ++round;
	}
      }
      else{
	write_1D_chars_MPI(products,dset_names,batch,nbatch,
			   round,lumi,global_rank,global_size,i,j,false);
	nbatch=0;
	products.clear();
	++round;
	  
      }
    }
   
    MPI_Barrier(MPI_COMM_WORLD);
    
    H5Sclose(scalar_id);
    H5Sclose(lumi_att_id);
    H5Pclose(plist_id);

    H5Gclose(lumi);
    H5Gclose(run);

    //close the attributes...
    H5Aclose(scalar_attr);
    H5Aclose(lumi_attr);


    //lets check the number of open obj id in case there are any....
    size_t obj_id = H5Fget_obj_count(file_id,H5F_OBJ_ALL);
    std::cout<<"Total Open Objects are "<<obj_id<<" in rank "<<global_rank<<std::endl;

    obj_id = H5Fget_obj_count(file_id,H5F_OBJ_GROUP);
    std::cout<<"Total Open groups are "<<obj_id<<" in rank "<<global_rank<<std::endl;
    
    obj_id = H5Fget_obj_count(file_id,H5F_OBJ_DATATYPE);
    std::cout<<"Total Open datatypes are "<<obj_id<<" in rank "<<global_rank<<std::endl;

    obj_id = H5Fget_obj_count(file_id,H5F_OBJ_DATASET);
    std::cout<<"Total Open datasets are "<<obj_id<<" in rank "<<global_rank<<std::endl;    
    
    obj_id = H5Fget_obj_count(file_id,H5F_OBJ_ATTR);
    std::cout<<"Total Open attributes are "<<obj_id<<" in rank "<<global_rank<<std::endl;
    obj_id = H5Fget_obj_count(file_id,H5F_OBJ_FILE);
    std::cout<<"Total Open files are "<<obj_id<<" in rank "<<global_rank<<std::endl;


    MPI_Barrier(MPI_COMM_WORLD);

    
    std::cout<<"END OF THE CODE "<<global_rank<<std::endl;
}


#endif
int main(int argc, char* argv[]){

  if(argc!=4){
    std::cerr << "Please specify input file, batch-size and outputfilename\n";
    return 1;
}

  auto run_num = 1;
  auto lumi_num = 1;
  auto input_filename = argv[1];
  auto batch_size = atoi(argv[2]);
  auto outname = argv[3];

  
  int ierr,rank;
  ierr = MPI_Init(&argc,&argv);
  if(ierr!=0){
    std::cout << "\n";
    std::cout << "BONES - Fatal error!\n";
    std::cout << "  MPI_Init returned ierr = " << ierr << "\n";
    exit ( 1 );
  }

  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  double st_time = MPI_Wtime();
  ConvertHDF5_MPI(input_filename,batch_size,run_num,lumi_num,outname);
  double end_time = MPI_Wtime();
  MPI_Barrier(MPI_COMM_WORLD);
  
  std::cout<<" Rank "<<rank<<" Processing Time: "<<end_time-st_time<<std::endl;
  //MPI_Finalize();
  return 1;
  }


