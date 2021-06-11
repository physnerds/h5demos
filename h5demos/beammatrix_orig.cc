#include "dkmeta.h"
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

void beammatrix(std::string input_file_dir ){
std::cout<<"At plotdk2nu now"<<std::endl;
int iread=0;
std::string input_user = "bashyal8";
std::string version = "v3r4p2";
std::string physics_list = "QGSP_BERT";
std::string macro = "DUNECDR_Referenceorig";
std::string current = "neutrino";

// std::string input_flux_dir = "g4lbne_v3r5p4_beta_QGSP_BERT_OptimizedEngineeredSept2017Review_14_neutrino_00031.dk2nu.root";
  //"/pnfs/dune/scratch/users/"+input_user+"/fluxfiles/g4lbne/"+version+"/"+physics_list+"/"+macro+"/"+current+"/flux/*.dk2nu.root";
  //std::cout<<"input files at "<<input_flux_dir<<std::endl;

bsim::Dk2Nu* dk2nu = new bsim::Dk2Nu;
//bsim::DkMeta* dkmeta = new bsim::DkMeta;
std::string ntuple_name = "dk2nuTree";
TChain* fChain = new TChain(ntuple_name.c_str());
fChain->Add(input_file_dir.c_str());

fChain->SetBranchAddress("dk2nu",&dk2nu);
//fChain->SetBranchAddress("dkmeta",&dkmeta);


TH1D *hnumu = new TH1D("numu_flux_dk2nu","numu_flux_dk2nu",50,0,5);
TH2D *hfar = new TH2D("farmatrix","farmatrix",50,0,5,50,0,5);
TH2D *beammatrix = new TH2D("BeamMatrix","BeamMatrix",50,0,5,50,0,5);
TH1D *hnear = new TH1D("Near_Flux","Near_Flux",50,0,5);
hnumu->Sumw2();
hnear->Sumw2();
beammatrix->Sumw2();

hnumu->GetXaxis()->SetTitle("#nu Energy(GeV)");
hnumu->GetYaxis()->SetTitle("Weighted Neutrino Flux");

hnear->GetXaxis()->SetTitle("#nu Energy(GeV)");
hnear->GetYaxis()->SetTitle("Weighted Neutrino Flux");

beammatrix->GetXaxis()->SetTitle("Near Detector Energy");
beammatrix->GetYaxis()->SetTitle("Far Detector Energy");

Long64_t nentries = fChain->GetEntries();
for (Long64_t jentry=0;jentry<nentries;jentry++){
fChain->GetEntry(jentry);
++iread;

if(iread % 10000 ==0)std::cout<<" Reading Entry "<<iread<<std::endl;

//double nuE = dk2nu->nuray[2].E;
//double fluxwght = dk2nu->nuray[2].wgt*decay.nimpwt;
hnumu->Fill(dk2nu->nuray[2].E,(dk2nu->nuray[2].wgt)*(dk2nu->decay.nimpwt/3.1415));
hnear->Fill(dk2nu->nuray[1].E,(dk2nu->nuray[1].wgt)*(dk2nu->decay.nimpwt/3.1415));
hfar->Fill(dk2nu->nuray[1].E,dk2nu->nuray[2].E,(dk2nu->nuray[2].wgt)*(dk2nu->decay.nimpwt/3.1415));
}
std::string filename = version+"_"+physics_list+"_"+macro+".root";
TFile *f = new TFile(filename.c_str(),"recreate");

for(int i = 0;i<hfar->GetNbinsX()+1;i++){
	for(int j = 0;j<hfar->GetNbinsY()+1;j++){
	   if(hnear->GetBinContent(i) != 0)
	   	beammatrix->SetBinContent(i,j,hfar->GetBinContent(i,j)/hnear->GetBinContent(i));
		}
	}


hnumu->Write();
hfar->Write();
hnear->Write();
beammatrix->Write();

f->Close();
}


int main(int argc, char* argv[]){
  auto input_filename = argv[1];
  beammatrix(input_filename);
  return 1;
}
