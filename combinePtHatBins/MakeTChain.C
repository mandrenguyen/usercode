{

  TChain *t[4];
  TFile *fout=new TFile("merged_pthat_pp_hiGoodTightTracks_Escale_tuneZ2.root","recreate");
  

  string inputDirName[4]= {
    "ic5calo",
    "ak3pf",
    "ak4pf",
    "ak5pf"
  };


  for(int idir=0;idir<4;idir++){

    char name[500];
    sprintf(name,"%s",inputDirName[idir].c_str());
    cout<<" name "<<name<<endl;
    TDirectory *outdir = fout->mkdir(name);
    outdir->cd(); 

    t[idir]=new TChain("t");
    sprintf(name,"$CMSSW_BASE/src/UserCode/MNguyen/combinePtHatBins/pthat15_pp_hiGoodTightTracks_Escale_tuneZ2.root/%s/t",inputDirName[idir].c_str());
    t[idir]->Add(name);
    sprintf(name,"$CMSSW_BASE/src/UserCode/MNguyen/combinePtHatBins/pthat50_pp_hiGoodTightTracks_Escale_tuneZ2.root/%s/t",inputDirName[idir].c_str());
    t[idir]->Add(name);
    sprintf(name,"$CMSSW_BASE/src/UserCode/MNguyen/combinePtHatBins/pthat80_pp_hiGoodTightTracks_Escale_tuneZ2.root/%s/t",inputDirName[idir].c_str());
    t[idir]->Add(name);

    t[idir]->Write();
    
    
  }
  
  
  fout->Close();
  
}
