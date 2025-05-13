#include <sys/stat.h>
#include <sys/types.h>

//
void draw_sample(int run)
{
  mkdir(TString::Format("./fig/%04d", run).Data(), 0777);

  vector<std::string> v_dir = {
    "cid0_sid02",
    "cid0_sid03",
    "cid0_sid04",
    "cid0_sid05",
    "cid0_sid05",
    "cid1_sid02",
    "cid1_sid03",
    "cid1_sid04",
    "cid1_sid06",
    "cid1_sid07",
    "cid1_sid08",
    "cid1_sid09",
    "cid1_sid10",
    "cid1_sid11",
    "cid1_sid12"
  };

  //
  TFile *fi =  TFile::Open(TString::Format("../rootfile/data%04d_cali_hist.root",run).Data());
  if(fi->IsZombie()){
    cout << "cannot open the file." << std::endl;
    return;
  }
  
  //
  TDirectory *dir;
  TH1D *h;
  for(unsigned int i=0;i<v_dir.size();i++){
    dir = fi->GetDirectory(v_dir[i].c_str()); 
    for(int j=0;j<16;j++){
      h = (TH1D*)dir->Get(TString::Format("h_%s_ch%02d",v_dir[i].c_str(),j).Data());
      if(!h){
        // cout << "can not find hist." << endl;
        continue;
      }
      
      h->Draw();
      if(i>4) gPad->SetLogy();
      gPad->SaveAs(TString::Format("./fig/%04d/%s_ch%02d.png",run,v_dir[i].c_str(),j).Data());
    }
  }

  fi->Close();
}
