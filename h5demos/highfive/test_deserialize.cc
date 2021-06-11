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

void test_read_ds(std::string h5fname,std::string dsname,char* inroot){

  TFile outFile{"test_out.root","RECREATE"};
  TTree outEvents("dk2nuTree","dk2nuTree");
  
  auto f = TFile::Open(inroot);
  auto e = (TTree*)f->Get("dk2nuTree");
  auto l = e->GetListOfBranches();

  
  std::vector<TClass*>classes;
  classes.reserve(l->GetEntriesFast());
  std::vector<test_rw::BranchInfo>branchInfo;
  branchInfo.reserve(l->GetEntriesFast());

  for(int i=0;i<l->GetEntriesFast();++i){
    auto b = dynamic_cast<TBranch*>((*l)[i]);
    b->SetupAddresses();
    TClass* class_ptr=nullptr;
    EDataType type;
    b->GetExpectedType(class_ptr,type);
    classes.push_back(class_ptr);

    test_rw::BranchInfo bi;
    bi.name_ = b->GetName();
    bi.branch_ = outEvents.Branch(b->GetName(),b->GetClassName(), nullptr);
    bi.buffer_ = *reinterpret_cast<void**>(bi.branch_->GetAddress());    bi.name_ = b->GetName();
    bi.branch_ = outEvents.Branch(b->GetName(),b->GetClassName(), nullptr);
    bi.buffer_ = *reinterpret_cast<void**>(bi.branch_->GetAddress());
    branchInfo.push_back(bi);
    branchInfo.push_back(bi);
  }
    


  classes.reserve(l->GetEntriesFast());
  branchInfo.reserve(l->GetEntriesFast());

  auto sz_name = dsname+"_sz";
  //open the file...
  File file(h5fname,File::ReadOnly);
  std::vector<char>read_data;
  std::vector<char>read_data_sz;

  DataSet dk2nu = file.getDataSet(dsname);
  DataSet dk2nu_sz = file.getDataSet(sz_name);
  std::vector<std::string>names = file.listObjectNames();
  auto objtype = file.getObjectType("run");
  //std::cout<<"Object type for run "<<objtype<<std::endl;
  for(int i=0;i!=names.size();++i)std::cout<<"Names "<<names[i]<<std::endl;
  
  dk2nu.read(read_data);
  dk2nu_sz.read(read_data_sz);

  std::cout<<read_data.size()<<" "<<read_data_sz.size()<<std::endl;
  //  for(int i=0;i<read_data_sz.size();i++){

    TBufferFile readBufferFile{TBuffer::kRead};
    readBufferFile.SetBuffer(&read_data.front(),read_data.size(),kFALSE);
    classes[0]->ReadBuffer(readBufferFile,branchInfo[0].buffer_);
    readBufferFile.Reset();
    outEvents.Fill();
    // }
  outFile.Write();
}

int main(int argc,char* argv[]){
  auto h5name = argv[1];
  auto rname = argv[2];
  test_read_ds(h5name,"/run/lumi/dk2nu",rname);
}
