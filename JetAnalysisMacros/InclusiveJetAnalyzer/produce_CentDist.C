void produce_CentDist(char *infile="/castor/cern.ch/user/m/mnguyen//HIDATA/JetTrees/merged_jetTree_Data_v10.root",char *outfile="CentDist_Data_v10.root")
{
  
  TFile *fin = TFile::Open(infile);

  TTree *t = (TTree*)gDirectory->Get("inclusiveJetAnalyzer/icPu5patJets_tree");

  TH1F *h=new TH1F("h","h",40,0,40);

  t->Draw("bin>>h","jtpt[0]>120&&abs(jteta[0])<2");
//  t->Draw("bin>>h");
  
  TFile *fout = new TFile(outfile,"RECREATE");
  h->Write();
  fout->Close();


}
