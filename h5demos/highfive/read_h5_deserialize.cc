#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>
#include <highfive/H5DataType.hpp>
#include <highfive/H5File.hpp>

#include "serialize_dataprods.h"

namespace test_rw {
  struct BranchInfo {
    std::string name_;
    TBranch* branch_;
    void* buffer_ = nullptr;
  };
}

using namespace HighFive;

std::pair<long unsigned int, long unsigned int>getEventOffSets(long iEventIndex,std::string pname, File file){
  std::vector<long unsigned int>st_end;
  auto d = file.getDataSet(pname+"_sz");
  if(iEventIndex==0){
    d.read(st_end);
    st_end.insert(st_end.begin(),0);
  }
  else{
    d.select({static_cast<unsigned long int>(iEventIndex-1)},{2}).read(st_end);
  }
  return{st_end[0],st_end[1]};
  
}

//inroot is only used to construct the root file structure
//outFile is the output root file that we can look at for verification
//h5fname is the input hdf5 file we will be reading data from
//dsname is the dataset name we will be reading from
void
hdf5_read_ds(std::string h5fname, std::string dsname, char* inroot) {
  //read corresponding root file to create the structure
  auto f = TFile::Open(inroot);
  auto e = (TTree*)f->Get("CCQENu");
  auto l = e->GetListOfBranches();

  //create output root file
  
  TFile outFile{"test_out.root","RECREATE"};
  TTree outEvents("CCQENu","CCQENu");
  std::vector<test_rw::BranchInfo> branchInfo;

  std::vector<TClass*> classes;
  classes.reserve(l->GetEntriesFast());
  branchInfo.reserve(l->GetEntriesFast());
  std::cout<<"Tot Branches "<<l->GetEntriesFast()<<std::endl;
  for( int i=0; i< l->GetEntriesFast(); ++i) {
    auto b = dynamic_cast<TBranch*>((*l)[i]);
    b->SetupAddresses();
    TClass* class_ptr=nullptr;
    EDataType type;
    b->GetExpectedType(class_ptr,type);
    if(class_ptr==nullptr)continue;
    std::cout<<b->GetName()<<std::endl;
    std::cout<<b->GetClassName()<<std::endl;
    classes.push_back( class_ptr);

    test_rw::BranchInfo bi;
    bi.name_ = b->GetName();
    bi.branch_ = outEvents.Branch(b->GetName(),b->GetClassName(), nullptr);
    bi.buffer_ = *reinterpret_cast<void**>(bi.branch_->GetAddress());
    branchInfo.push_back(bi);
  }


  File file(h5fname, File::ReadOnly);
  //std::vector<std::vector<char>> read_data;
  //This is a 1D data set....
  std::vector<char>read_data;
  std::vector<int>read_sz;
  std::vector<int>read_offset;
  //std::vector<std::vector<int>>buffer_info;
  std::map<int,std::pair<unsigned int,unsigned int>>_index;
  DataSet dataset = file.getDataSet(dsname);
  auto sz = dataset.getSpace().getDimensions()[0];
  std::cout<<"Size of the DataSet "<<dsname<<" "<<sz<<std::endl;
   
  dataset.read(read_data);
    HighFive::Group lumi_(file.getGroup("/run/lumi"));
  //  HighFive::Group lumi_(file.getGroup("/Lumi"));
  for(auto p : lumi_.listObjectNames()){
    std::cout<<" objects "<<p<<" "<<std::endl;
    if(p.find("sz")!=std::string::npos){
      std::cout<<"Reading in "<<p<<std::endl;
       DataSet dataset_sz = file.getDataSet("/run/lumi/"+p);
      // DataSet dataset_sz = file.getDataSet("/Lumi/"+p);
      //dataset_sz.select({80},{2}).read(read_sz);
       dataset_sz.read(read_sz);
    }
    if(p.find("offset")!=std::string::npos){
      std::cout<<"Reading in "<<p<<std::endl;
         DataSet dataset_offset = file.getDataSet("/run/lumi/"+p);
      //  DataSet dataset_offset = file.getDataSet("/Lumi/"+p);

    }
  }
  /*
  for(int i=0;i!=10;i++){
    //need to figure out the size of each buffer....
    
    std::cout<<_index[i].first<<" "<<_index[i].second<<" "<<i<<std::endl;


  }
  */

  //std::cout<<"Size of dk2nu_sz "<<read_sz.size()<< " "<<read_sz[0]<<" "<<read_sz[1]<<" "<<read_sz[2]<<std::endl;

  //okay what read_sz has is the length of the arrays....
  //First one starts from 0 to read_sz[i]
  //buffer starts at i*read_sz[i]
  //buffer ends at i*read_sz[i]+read_sz[i]
 // for(size_t i=0;i!=read_data.size();i++)std::cout<<"reading "<<i<<read_data[i]<<std::endl;
  //std::cout << "read " << read_data.size() << " items\n";
  //loop over all events and then for this branch write to the out file



  
   TBufferFile readBufferFile{TBuffer::kRead};
   //readBufferFile.SetBuffer(&read_data.front(),read_data.size(),kFALSE);
   std::cout<<read_data.size()<<" size of read_data"<<std::endl;
   // exit(1);
   //start commenting out here
   auto offset=0;
   for(int i=0;i<10;++i){
     std::vector<char>read_dat;
      std::pair<long unsigned int, long unsigned int>pairs = getEventOffSets(i,dsname,file);
     // std::pair<long unsigned int, long unsigned int>pairs = getEventOffSets(i,"/Lumi/dk2nu",file);
     std::pair<long unsigned int, long unsigned int>prev_pairs={0,0};
     if(i>0){
        prev_pairs = getEventOffSets(i-1,dsname,file);
       // prev_pairs =  getEventOffSets(i-1,"/Lumi/dk2nu",file);

     }
     auto begin = pairs.first+offset;
     auto end = pairs.first+pairs.second+offset;
     std::cout<<"first and second "<<pairs.first<<" "<<pairs.second<<std::endl;
     std::cout<<begin<<" "<<end<<" "<<offset<<std::endl;
     offset += prev_pairs.second;
     dataset.select({begin},{end-begin}).read(read_dat);
     readBufferFile.SetBuffer(&read_dat.front(),read_dat.size(),kFALSE);
     classes[0]->ReadBuffer(readBufferFile,branchInfo[0].buffer_);
     readBufferFile.Reset();
     outEvents.Fill();
     read_dat.clear();
   }
   
  outFile.Write();
}


int main(int argc, char* argv[]) {
  auto h5name = argv[1];
  auto rname = argv[2];
    hdf5_read_ds(h5name, "/run/lumi/prong_part_E", rname);
  // hdf5_read_ds(h5name,"/Lumi/dk2nu",rname);
  return 0;
}
