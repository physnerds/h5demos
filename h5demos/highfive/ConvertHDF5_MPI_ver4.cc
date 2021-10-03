#ifndef H5_WRITE_CHARS_H
#define H5_WRITE_CHARS_H

#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>
#include <highfive/H5DataType.hpp>
#include <highfive/H5File.hpp>


#include "h5_write_1D_chars.h"
#include "h5_write_1D_chars_MPI.h"
#include "serialize_dataprods.h"
#include "utilities.h"


//#include "dkmeta.h"
#include "dk2nu.h"

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
#include "h5_setup_mpi.h"
#include "mpi.h"
#include "H5FDmpio.h"
using product_t = std::vector<char>;

//#define EXTRAS
//#define WAIT
//#define BETA

//Chris:
//Simplify with the simple  numbers for multiple data-sets and then see if events are going whey they are supposed to be going.

void ConvertHDF5_MPI(char* input_file_dir,int batch,int run_num,int lumi_num,std::string const& outname ){
  int argc; char **argv;
  int flag;
  //just the boiler plate from the example......
  //need to understand what each of these variables are needed for.....
  
  hid_t file_id;      //File ID
  hid_t filespace,memspace;
  //hid_t plist_id;
  hid_t fapl_id;     //File access property list
  hid_t run,lumi;   //Group ID

  std::vector<product_t>products;
  char groupname[16];
  std::string ntuple_name = "ccqe_data";

  initial_time = std::time(nullptr);
  
  MPI_Initialized(&flag);
  if(!flag)MPI_Init(&argc,&argv);
  //also put the global variables....

  int global_size,global_rank;
  
  MPI_Comm_size(MPI_COMM_WORLD,&global_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&global_rank);

  MPI_Info info;
  MPI_Info_create(&info);
  
  std::cout<<"MPI RANK "<<global_rank<<" MPI SIZE "<<global_size<<std::endl;
  //exit(1);

  //lets try to put the barrier and see if I can get rid of the error::
  // MPI_Barrier(MPI_COMM_WORLD);
  //now lets try to setup the file access property list with parallel I/O access
  
  auto plist_id = H5Pcreate (H5P_FILE_ACCESS);
  //make sure that we have created the file access property list...
  assert(plist_id>0);
  std::cout<<"File access property id is "<<fapl_id<<std::endl;
  //Below function only works if HDF5-Parallel is implemented.
   auto  ret = H5Pset_fapl_mpio(plist_id,MPI_COMM_WORLD,info);
    assert(ret>=0);
    
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

    
    std::cout<<" Am I here Yet "<<std::endl;
    
    auto f = TFile::Open(input_file_dir);
    std::cout<<" Am I here Yet 2"<<std::endl; 
    auto e = (TTree*)f->Get(ntuple_name.c_str());
    auto l = e->GetListOfBranches();
    //get the total number of branches....
     std::cout<<" Am I here Yet 3"<<std::endl;  
    int nbatch = 0;
    int round = 0;
    auto nentries = e->GetEntriesFast();
    auto tot_branches = l->GetEntriesFast();
    auto classes = return_classes(l);
    auto dset_names = return_dsetnames(l);
    std::cout<<"Total Number of Entries are "<<nentries<<" Total Branches "<<tot_branches<<std::endl;
    /*
    std::vector<std::string>dset_names = {dset_names_[1],dset_names_[0]};
    tot_branches = 2;
    std::vector<TClass*>classes = {classes_[1],classes_[0]};
    */
    for(Long64_t jentry=0;jentry<tot_branches;++jentry)
      {
      //I guess we can take the buffer info from here....
	e->GetEntry(0);	
	auto branch = dynamic_cast<TBranch*>((*l)[jentry]);
	std::string branch_name = branch->GetName();
	if(classes[jentry]==nullptr){
	  auto blob = return_fundamental_blobs(branch);
	  //DataSet for the actual branch
	  CreateDataSets(branch_name,blob.size(),lumi);
	  //where does a particular buffer starts?
#ifdef EXTRAS
	  CreateDataSets(branch_name+"_offset",8,lumi);
	  //what is the length of that buffer?
	  CreateDataSets(branch_name+"_size",8,lumi);
#endif
	}
	else{
	  auto blob = return_blob(branch,classes[jentry]);
	  CreateDataSets(branch_name,blob.size(),lumi);
	  //where does a particular buffer starts?
#ifdef EXTRAS	  
	  auto sz_blob = GetMemcpyArray(blob.size());
	  CreateDataSets(branch_name+"_offset",sz_blob.size(),lumi);
	  //what is the length of that buffer?
	  CreateDataSets(branch_name+"_size",sz_blob.size(),lumi);
#endif
	  
	}
#ifdef WAIT	  
  if(global_rank==0){
    MPI_Request requests[global_size-1];
    for(int i = 1;i<global_size;i++){
      MPI_Isend(&i,1,MPI_INT,i,global_rank,MPI_COMM_WORLD,&requests[i-1]);
    }
    MPI_Waitall(global_size-1,requests,MPI_STATUSES_IGNORE);
  }
  else{
    int received;
    MPI_Recv(&received, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  
#endif
      }


    //Creating part is done....now the filling part.
      nentries = 114;
    int niter = nentries/global_size;
    int remainder = nentries%global_size;

    std::cout<<"Total Entries "<<nentries<<" "<<niter*global_size+remainder
	     <<" Iter "<<niter<<" Remainder "<<remainder<<std::endl;
    //exit(1);
    MPI_Barrier(MPI_COMM_WORLD);

    for(int i=0;i<niter;i++){
      auto j = global_rank+global_size*i;
      e->GetEntry(j);
      for(Long64_t jentry=0;jentry<tot_branches;++jentry){
	auto b = dynamic_cast<TBranch*>((*l)[jentry]);
	auto dataprod_name = b->GetName();
	std::cout<<"Event: "<<j<<" Name Read "<<dataprod_name<<" "
		 <<global_rank<<" "<<i<<std::endl;
	if(classes[jentry]==nullptr){
	  auto blob = return_fundamental_blobs(b);
	  products.push_back(blob);
	}
	else{
	  auto blob = return_blob(b,classes[jentry]);
	  products.push_back(blob);
	}
	//	write_1D_chars_MPI_2(products[0],dataprod_name,batch,round,lumi,global_rank,global_size,i,j);
      }
      nbatch++;
      if(nbatch==batch||j==niter*global_size-1){
#ifdef DEBUG
	std::cout<<" Writing for "<<i<<" "<<j<<" "<<global_rank
		 <<std::endl;
#endif
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

    //in the end close the file.

    //lets check the number of open obj id in case there are any....
#ifdef DEBUG
    std::cout<<" Done with IO "<<global_rank<<std::endl;
#endif
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
#ifdef DEBUG
    int tot_ranks= 0;
    MPI_Scan(&global_rank,&tot_ranks,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    std::cout<<" Total Ranks "<<tot_ranks<<" "<<global_rank<<std::endl;
#endif
    H5Fclose(file_id);
    MPI_Barrier(MPI_COMM_WORLD);
    //okay once everything is closed, maybe I can try to open the file in serial and do the IO stuff serially.
      if(remainder!=0){
      file_id = H5Fopen(H5FILE_NAME,H5F_ACC_RDWR,H5P_DEFAULT);
      //open the group run....
      
      run = H5Gopen(file_id,run_name,H5P_DEFAULT);
      lumi = H5Gopen(run,lumi_name,H5P_DEFAULT);
      //  auto dataset_id = H5Dopen(lumi,"phi",H5P_DEFAULT);
      for(Long64_t i = niter*global_size;i<nentries;i++){
	//	std::cout<<"Serial Reading entry "<<i<<" "<<niter*global_size<<" "
	//	 <<nentries<<std::endl;
	e->GetEntry(i);
	for(Long64_t jentry=0;jentry<tot_branches;++jentry){
	  auto b = dynamic_cast<TBranch*>((*l)[jentry]);
	  auto dataprod_name = b->GetName();
	  
	  if(classes[jentry]==nullptr){
	    auto blob = return_fundamental_blobs(b);
	    products.push_back(blob);
	    
	  }
	  else{
	    auto blob = return_blob(b,classes[jentry]);
	    products.push_back(blob);
	    
	  }
	}
	nbatch++;
	if(nbatch==batch||i==nentries-1){
	  write_1D_chars_MPI(products,dset_names,batch,nbatch,
			     round,lumi,global_rank,global_size,i,i,true);
	
	nbatch=0;
	products.clear();
	++round;

	}

      }
	
      H5Gclose(lumi);
      H5Gclose(run);
      H5Fclose(file_id);
      
      }
      


    
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

  int rank,size;
  
  int ierr;
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

