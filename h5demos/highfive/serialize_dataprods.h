#ifndef SERIALIZE_DATAPRODS_H
#define SERIALIZE_DATAPRODS_H
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TClass.h"
#include "TBufferFile.h"
#include <iostream>
#include <string>
#include <vector>

using product_t = std::vector<char>;

std::vector<char>return_empty_buffer(TBranch *b){
  auto leaves = b->GetListOfLeaves();
  auto leaf = dynamic_cast<TLeaf*>((*leaves)[0]);
  std::vector<char>blob;
  blob.push_back('\0');

  return blob;
}

//****************************************************************//

std::vector<std::string> return_dsetnames(TObjArray* l) {
  std::vector<std::string> names;
  names.reserve(l->GetEntriesFast());
  for( int i=0; i< l->GetEntriesFast(); ++i) {
    auto b = dynamic_cast<TBranch*>((*l)[i]);
    b->SetupAddresses();
    TClass* class_ptr=nullptr;
    EDataType type;
    b->GetExpectedType(class_ptr,type);
    //   if(class_ptr==nullptr)continue; //need to uncomment it if we want to skip the null pointers....
    names.push_back(b->GetName());
  }
  return names;
}

std::vector<TClass*> return_classes(TObjArray* l) {
  std::vector<TClass*> classes;
  classes.reserve(l->GetEntriesFast());
  for( int i=0; i< l->GetEntriesFast(); ++i) {
    auto b = dynamic_cast<TBranch*>((*l)[i]);
    // std::cout<<b->GetName()<<std::endl;
    //std::cout<<b->GetClassName()<<std::endl;
    b->SetupAddresses();
    TClass* class_ptr=nullptr;
    EDataType type;
    b->GetExpectedType(class_ptr,type);
    //if(class_ptr==nullptr)continue;
    classes.push_back( class_ptr);
  }
  return classes;
}

std::vector<char> return_blob(TBranch* b, TClass* c) {
  TBufferFile bufferFile{TBuffer::kWrite};
  auto address = reinterpret_cast<void**>(b->GetAddress());
  c->WriteBuffer(bufferFile, *address);
  std::vector<char> blob(bufferFile.Buffer(), bufferFile.Buffer()+bufferFile.Length());
  return blob;
}


std::vector<char>return_fundamental_blobs(TBranch *b){
  TBufferFile bufferFile{TBuffer::kWrite};
  auto leaves = b->GetListOfLeaves();
  //Right now for the testing purpose, I will just assume there will be one leaf in the fundamental type branches....
  auto leaf = dynamic_cast<TLeaf*>((*leaves)[0]);
  auto c_type = leaf->GetTypeName();
  std::vector<char>blob;
  auto _length = leaf->GetLen();
  if(_length!=0){
    for(int i = 0;i<_length;i++){
      auto _val = leaf->GetValue(i);
      char arr_val[sizeof(_val)];
      memcpy(arr_val,&_val,sizeof(_val));
      blob.insert(blob.begin(),std::begin(arr_val),std::end(arr_val));
      // blob.push_back(arr_val);

    }
  }
  else{
    blob.push_back('\0');
    
  }
  return blob;
}

const char* return_datatype(TBranch *b){
  TClass *class_ptr = nullptr;
  EDataType type;
  b->GetExpectedType(class_ptr,type);
  if(class_ptr!=nullptr){
    return b->GetClassName();

  }
  else{
    auto leaves = b->GetListOfLeaves();
    auto leaf = dynamic_cast<TLeaf*>((*leaves)[0]);
    auto _val = leaf->GetTypeName();
    return _val;

  }
}

//*********************************************************************************
std::vector<product_t> ReturnBlobs(TObjArray* obj,int tot_branches,std::vector<TClass*>classes, int jentry, int tot_entries){
  //tree->GetEntry(ientry);
  std::vector<product_t>products;

  for(Long64_t jentry=0;jentry<tot_branches;++jentry){
    auto b = dynamic_cast<TBranch*>((*obj)[jentry]);

    if(jentry>=tot_entries){
      auto blob = return_empty_buffer(b);
      products.push_back(blob);
      return products;
    }
    else{ 
      if(classes[jentry]==nullptr){
	auto blob = return_fundamental_blobs(b);
	products.push_back(blob);
      }
      else{
	auto blob = return_blob(b,classes[jentry]);
	products.push_back(blob);
      }
    }
  }
  return products;
}

//******************************************************************************//

void cms_read() {
  auto f = TFile::Open("PoolOutputTest.root");
  auto e = (TTree*)f->Get("Events");
  auto l = e->GetListOfBranches();
  auto classes = return_classes(l);
  std::cout << "Total events: "<< e->GetEntriesFast() << std::endl;
  for(int ievt=0; ievt< e->GetEntriesFast();++ievt) {
    std::cout <<"event "<<ievt<<std::endl;
    e->GetEntry(ievt);
    for( int ib = 0; ib< l->GetEntriesFast(); ++ib) {
      auto b = dynamic_cast<TBranch*>((*l)[ib]);
      auto blob =  return_blob(b, classes[ib]);
      std::cout<<"   "<<b->GetName()<<" size "<<blob.size() <<std::endl;;
    }
  }
}
#endif
