#include <sstream>
#include <vector>
#include <fstream>
//ROOT
#include <TROOT.h>
#include <TChain.h>
#include <TRandom3.h>
#include <TH1D.h>


// Dk2nu
//#ifndef __CINT__
#include "dk2nu.h"
#include "dkmeta.h"
//#endif // ifndef __ CINT__

void scanvolumelist(){
std::cout<<" At scanvolumelist now"<<std::endl;
int iread = 0;
std::string input_user = "bashyal8";
std::string version = "v3r5p4";
std::string physics_list = "QGSP_BERT";
//std::string macro = "OptimizedEngineeredJan2017_3";
std::string macro = "OptimizedEngineeredSept2017Review";
std::string current = "neutrino";

std::string input_flux_dir = 
"/pnfs/dune/scratch/users/"+input_user+"/fluxfiles/g4lbne/"+version+"/"+physics_list+"/"+macro+"/"+current+"/flux/*.dk2nu.root";

//get the dk2nu and dkmeta;
bsim::Dk2Nu* dk2nu = new bsim::Dk2Nu;
std::string ntuple_name = "dk2nuTree";
TChain *fChain = new TChain(ntuple_name.c_str());
fChain->Add(input_flux_dir.c_str());

fChain->SetBranchAddress("dk2nu",&dk2nu);

cout << "before reading any entries" << endl;
cout << *dk2nu << endl << endl;
//create two empty vector
std::vector<std::string>volname;
std::vector<std::string>matname;
std::string filename="outfile.txt";
std::ofstream fout;
//In the meantime, we need to find to find a way to scan through the unknown volume.....
//perhaps creating a complete list of volumes and materials via a csv file and then...
//going through the list and double checking will also be good.
//But to start with just scan the volumes and materials
int Nancestry = 3;

Long64_t nentries = fChain->GetEntries();
for(Long64_t jentry=0;jentry<nentries;jentry++){
fChain->GetEntry(jentry);
++iread;
if(iread%1000==0)std::cout<<"Reading Entry "<<iread<<std::endl;
if(iread>50000)break;
   for(int ii=1;ii<3;ii++){
      if(dk2nu->ancestor.size()==3&&ii==2)continue;
     // std::cout<<ii<<" "<<dk2nu->ancestor[ii].ivol<<" "<<dk2nu->ancestor[ii].imat<<" "<<std::endl;
      std::string volume = dk2nu->ancestor[ii].ivol;
      std::string material = dk2nu->ancestor[ii].imat;;
      bool parse = false;
      //size_t j =0;
      if(volname.size()==0)volname.push_back(volume);
      if(matname.size()==0)matname.push_back(material);
      for(size_t j= 0;j!= volname.size();j++){
      //std::cout<<" in the loop"<<std::endl;
        std::string contvol = volname.at(j);
        if(contvol==volume){
	  //std::cout<<"volume already in container "<<std::endl;
	break;}
	if(j>=volname.size()-1){
	parse=true;
      }
      }
      if(parse){
      volname.push_back(volume.c_str());
      matname.push_back(material.c_str());
  }
}
std::cout<<"size of volume container is "<<volname.size()<<std::endl;
}
fout.open(filename.c_str());
for(size_t i = 0;i!= volname.size();i++)fout<<volname.at(i)<<","<<matname.at(i)<<std::endl;
fout.close();
}
