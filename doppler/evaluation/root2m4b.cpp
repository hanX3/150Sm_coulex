//
void root2m4b(string file_name, string h_name)
{
  TFile *fi = new TFile(file_name.c_str(), "READ");

  TH2I *h = (TH2I*)fi->Get(h_name.c_str());
  h->Print();

  ofstream fo;
  fo.open(TString::Format("%s.m4b",h_name.c_str()).Data(),std::ios::binary);

  double raw;
  int data;
  for(int i=0;i<4096;++i){
    for(int j=0;j<4096;++j){
	    raw = h->GetBinContent(i+1, j+1);
	    data = (int)raw;
	    fo.write((char*)&data,sizeof(int));
    }
  }
   
  fo.close();
  fi->Close();
  
  return;
}
