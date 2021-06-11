#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>
#include <highfive/H5DataType.hpp>
#include <highfive/H5File.hpp>

#include "serialize_dataprods.h"

//basically 

namespace test_rw {
  struct BranchInfo {
    std::string name_;
    TBranch* branch_;
    void* buffer_ = nullptr;
  };
}

using namespace HighFive;
//If offset data-set is filled properly, then for each event, it should give where the buffer starts and where it ends.
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

void hdf5_read_ds(std::string h5fname,std::string dsname,char* inroot){
  //read the root file to create the data structure....
  auto f = TFile::Open(inroot);
  auto e = (TTree*)f->Get("CCQENu");
  auto l = e->GetListOfBranches();

  //now we have to craete the output file....
  size_t index = 0;
  index = h5fname.find(".h5");
  if(index==std::string::npos){
    std::cerr<<"The hdf5 file "<<h5fname<<" Does not exist or end with .h5 "<<std::endl;
    exit(1);
  }
  std::string outfilename = h5fname;
  std::string suffix = ".root";
  outfilename.replace(index,suffix.length(),suffix);
  TFile outfile{outfilename.c_str(),"RECREATE"};
  TTree outEvents("CCQENu","CCQENu");
  std::vector<test_rw::BranchInfo>branchInfo;
  
  std::vector<TClass*>classes;
  classes.reserve(l->GetEntriesFast());
  branchInfo.reserve(l->GetEntriesFast());
  //So the total branches should all be filled if everything works out....
  std::cout<<" Total Number of branches in the template root file is "<<l->GetEntriesFast()<<std::endl;
  //Now save all the branch info...Note that a bunch of them will be null pointer (fundamentals)
  for(int i=0;i<l->GetEntriesFast();++i){
    auto b = dynamic_cast<TBranch*>((*l)[i]);
    b->SetupAddresses();
    TClass* class_ptr = nullptr;
    EDataType type;
    b->GetExpectedType(class_ptr,type); //If the branch is fundamental type, the pointer is still null
    //if(class_ptr==nullptr)continue;
    std::cout<<b->GetName()<<std::endl;
    classes.push_back(class_ptr);

    //okay now...create the branches for non-fundamental types.....
    test_rw::BranchInfo bi;
    bi.name_ = b->GetName();
    if(class_ptr!=nullptr){
      bi.branch_ = outEvents.Branch(b->GetName(),b->GetClassName(),nullptr);
      bi.buffer_ = *reinterpret_cast<void**>(bi.branch_->GetAddress());
      branchInfo.push_back(bi);
    }
    //and now for the fundamental types.....
    if(class_ptr==nullptr){
      //still need to test the address thing...
      std::cout<<"Null pointer class name "<<b->GetClassName()<<std::endl;
      auto leaves = b->GetListOfLeaves();
      //how to declare the branches....
      auto leaf = dynamic_cast<TLeaf*>((*leaves)[0]);
      auto c_type = leaf->GetTypeName();
      std::stringstream _type;
      _type<<c_type;
      auto _length = leaf->GetLen();
      if(_type.str()=="Int_t"){
	if(_length==0){
	  Int_t _val;
	  bi.branch_ = outEvents.Branch(b->GetName(),&_val);
	}
	else{
	  Int_t _val[_length];
	  std::stringstream ss;
	  ss<<_length;
	  std::string name = b->GetName()+"/["+ss.str()+"]";
	  bi.branch_ = outEvents.Branch(b->GetName(),_val,name.c_str());
	}
	bi.buffer_ = *reinterpret_cast<void**>(bi.branch_->GetAddress());
      }
    }
  }
}
 



int main(int argc, char* argv[]) {
  if(argc!=3){
    std::cout<<" hdf5 filename and template root filename required "<<std::endl;
    return 0;
  }
  auto h5name = argv[1];
  auto rname = argv[2];
    hdf5_read_ds(h5name, "/run/lumi", rname);
  return 0;
}
