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
#include "TH1D.h"
#include "TChain.h"
#include "TH2D.h"
#include "TFile.h"
#include "hdf5.h"
#include "h5_setup_mpi.h"
#include "mpi.h"
#include "H5FDmpio.h"
//#include "H5Cpp.h"
using namespace HighFive;
using product_t = std::vector<char>;

#define RANK 2 //This is not MPI RANK
#define NAME_LENGTH 17
#define DATASETNAME "CCQENu"
//#define GROUPNAME "lumi"
#define NX 8
#define NY 4
#define CH_NX 2
#define CH_NY 2


void ConvertHDF5_MPI(char* input_file_dir,int batch,int run_num,int lumi_num,std::string const& outname ){
  int argc; char **argv;
  int flag;
  //just the boiler plate from the example......
  //need to understand what each of these variables are needed for.....
  
  hid_t file_id;      //File ID
  hid_t filespace,memspace;
  //hid_t plist_id;
  int i;
  herr_t status;
  hid_t fapl_id;     //File access property list
  hid_t run,lumi;   //Group ID
  hid_t dset_id;    // DataSet ID
  hid_t chunk_id;   //Chunk ID
  hid_t file_space_id;     // File DataSpace ID
  hsize_t file_dims[RANK];
  hsize_t max_dims[RANK];
  hsize_t chunk_dims[RANK];
  hsize_t dimsf[2];
  herr_t ret;     // Generic return value (whatever it means...)
  std::vector<product_t>products;
  char groupname[16];
  std::string ntuple_name = "CCQENu";


  
  MPI_Initialized(&flag);
  if(!flag)MPI_Init(&argc,&argv);
  //also put the global variables....

  int global_size,global_rank;
  
  MPI_Comm_size(MPI_COMM_WORLD,&global_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&global_rank);

  
  
  std::cout<<"MPI RANK "<<global_rank<<" MPI SIZE "<<global_size<<std::endl;
  //exit(1);
  if(IsMotherRank()){
    std::cout<<"This is the Mother "<<std::endl;

  }
  //lets try to put the barrier and see if I can get rid of the error::
  MPI_Barrier(MPI_COMM_WORLD);
  //now lets try to setup the file access property list with parallel I/O access

  auto plist_id = H5Pcreate (H5P_FILE_ACCESS);
  //make sure that we have created the file access property list...
  assert(plist_id>0);
  std::cout<<"File access property id is "<<fapl_id<<std::endl;
  //Below function only works if HDF5-Parallel is implemented.
    ret = H5Pset_fapl_mpio(plist_id,MPI_COMM_WORLD,MPI_INFO_NULL);
    assert(ret>=0);
    
    auto H5FILE_NAME = outname.c_str();
    file_id = H5Fcreate(H5FILE_NAME, H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
    //auto _hfile = H5::H5File(H5FILE_NAME,H5F_ACC_TRUNC,H5P_DEFAULT,plist_id);
    //okay I cannot use the CPP functions. (ABOVE)
    /*
    chunk_dims[0] = CH_NX;
    chunk_dims[1] = CH_NY;

    dimsf[0] = NX;
    dimsf[1] = NY;

    max_dims[0] = H5S_UNLIMITED;
    max_dims[1] = H5S_UNLIMITED;
    //filespace = H5Screate_simple(RANK, dimsf, max_dims);//NULL); 
    //memspace  = H5Screate_simple(RANK, chunk_dims, max_dims);//NULL);
    */

    auto run_name = "/run";
    auto scalar_id = H5Screate(H5S_SCALAR);
    run = H5Gcreate(file_id,run_name,H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT);
    auto scalar_attr = H5Acreate(run,run_name,H5T_NATIVE_INT,scalar_id,H5P_DEFAULT,H5P_DEFAULT);
    //give the value to the attribue...
    /*
    void **_buff;
     *_buff = new char[sizeof(char)];
    //i guess do the memory copy
     std::memcpy(static_cast<char*>(*_buff),&run_num,sizeof run_num);
    */
    auto att_status = H5Awrite(scalar_attr,H5T_NATIVE_INT,&run_num);
    //okay inside group create lumi....
    auto lumi_name = "lumi";
    lumi = H5Gcreate(run,lumi_name,H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT);
    //create the attribute for lumi....
    auto lumi_att_id = H5Screate(H5S_SCALAR);
    auto lumi_attr = H5Acreate(lumi,lumi_name,H5T_NATIVE_INT,lumi_att_id,H5P_DEFAULT,H5P_DEFAULT);
    //all of these has to be done for each data-sets
    /*
    plist_id = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_chunk(plist_id, RANK, chunk_dims);
    dset_id = H5Dcreate(lumi, DATASETNAME, H5T_NATIVE_INT, filespace,
			H5P_DEFAULT, plist_id, H5P_DEFAULT);
    */    
    //now the root thing
    
    
    auto f = TFile::Open(input_file_dir);
    auto e = (TTree*)f->Get(ntuple_name.c_str());
    auto l = e->GetListOfBranches();
    //get the total number of branches....
    
    int ievt = 0;
    int nbatch = 0;
    int round = 0;
    auto nentries = e->GetEntriesFast();
    auto tot_branches = l->GetEntriesFast();
    //tot_branches=1;
    auto classes = return_classes(l);
    auto dset_names = return_dsetnames(l);
    std::cout<<"Total Number of Entries are "<<nentries<<" Total Branches "<<tot_branches<<std::endl;
    nentries = 10;
    SetTotalBranches(tot_branches);
    while(ievt<nentries){
      e->GetEntry(ievt);
      for(Long64_t jentry=0;jentry<tot_branches;++jentry){
	auto b = dynamic_cast<TBranch*>((*l)[jentry]);
	auto dataprod_name = b->GetName();
	if(classes[jentry]==nullptr){
	  // std::cout<<"Name of the Branch (null) "<<dataprod_name<<" "<<ievt<<std::endl;
	  auto blob = return_fundamental_blobs(b);
	  products.push_back(blob);

	}
	else{
	  //  std::cout<<"Name of the branch(not null) "<<dataprod_name<<" "<<ievt<<std::endl;
	  auto blob = return_blob(b,classes[jentry]);
	   products.push_back(blob);

	}

      }
      nbatch++;
      if(nbatch==batch||ievt==nentries-1){
	int rank = GetMPILocalRank();
	
	//if(rank%2==0)
	std::cout<<"Rank of the MPI "<<GetMPILocalRank()<<" "<<ievt<<std::endl;
	write_1D_chars_MPI(products,dset_names,batch,nbatch,round,lumi,global_rank,global_size);
	
	/*
	else{
	  if(rank%2!=0)
	    write_1D_chars_MPI(products,dset_names,batch,nbatch,round,lumi); 
	}
	*/
	nbatch=0;
	products.clear();
	++round;
      }

      //  if(ievt==nentries-1)CloseAllDataSets();
      ++ievt;

    }
  
    

    //close the dataspaces of files and attributes

    
    H5Sclose(scalar_id);
    H5Sclose(lumi_att_id);
    
    H5Pclose(plist_id);
    // H5Sclose(filespace);
    H5Gclose(lumi);
    H5Gclose(run);

    //close the attributes...
    H5Aclose(scalar_attr);
    H5Aclose(lumi_attr);

    //in the end close the file.
    H5Fclose(file_id);
    MPI_Barrier(MPI_COMM_WORLD);
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

  ConvertHDF5_MPI(input_filename,batch_size,run_num,lumi_num,outname);
  // MPI_Finalize(); //for some reason, I cannot finalize the mpich routine
  return 1;
  }
