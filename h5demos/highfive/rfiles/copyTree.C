void copyTree(){
  auto _file = new TFile("For_TMVA_DATA_minervame5A6A6B_kludged.root","READ");
  auto *t = (TTree*) _file->Get("ccqe_data");

  auto branches = t->GetListOfBranches();
  std::string _bn = "phi";
  std::string _bn2 = "Q2";
  auto tot_branches = branches->GetEntriesFast();
  for(Long64_t ibranch=0;ibranch<tot_branches;ibranch++){
    auto b = dynamic_cast<TBranch*>((*branches)[ibranch]);
    auto _name = b->GetName();
    std::cout<<_name<<std::endl;
    // if(_name==_bn||_name==_bn2)
      t->SetBranchStatus(_name,1);
    
      // else{
      //  t->SetBranchStatus(_name,0);
      // }

  }
  auto _newfile = new TFile("For_TMVA_DATA_minervame5A6A6B_kludged_small_stats.root","RECREATE");
  auto new_t = t->CloneTree(0);
  new_t->CopyEntries(t,100);

  _newfile->Write();

}
