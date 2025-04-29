#include <sys/stat.h>
#include <sys/types.h>

// Ge vs. Ge
void max_offset(TString filename, int run)
{
  mkdir(TString::Format("./fig/%04d", run).Data(), 0777);

  TFile *fi = TFile::Open(filename.Data());
  if(fi->IsZombie()){
    cout << "can not open rootfile." << endl;
    return;
  }

  ofstream fo;
  fo.open(TString::Format("./par/ts%04d.offset", run).Data());
  fo << "cid  sid  ch  ts_offset[ns]  chi_square" << endl;
  char str[1024];
  fo << str << endl;

  int sid, ch;
  double ts, chi2;
  
  // Ge
  TH1D *h_ge[5*16];
  TCanvas *c_ge[5*16];
  for(int i=2;i<=5;i++){
    for(int j=0;j<16;j++){
      h_ge[(i-2)*16+j] = (TH1D*)gFile->Get(TString::Format("ge_sid%d_ch%02d",i,j).Data());

      if(h_ge[(i-2)*16+j]->GetEntries()==0){
        sprintf(str, "%3d  %3d  %2d  %13.2f  %10.2f", 0, i, j, 0., 0.);
        fo << str << endl;
        continue;
      }

      c_ge[(i-2)*16+j] = new TCanvas(TString::Format("c_ge_%02d_%02d",i,j).Data(), "");
      c_ge[(i-2)*16+j]->cd();
      h_ge[(i-2)*16+j]->Rebin(2);
      h_ge[(i-2)*16+j]->Draw();

      ts = -1000+h_ge[(i-2)*16+j]->GetMaximumBin()*h_ge[(i-2)*16+j]->GetBinWidth(0);
      chi2 = 0;

      TLatex *latex = new TLatex();
      latex->SetTextSize(0.05);
      latex->SetTextColor(2);
      latex->DrawLatex(-800, 0.9*h_ge[(i-2)*16+j]->GetMaximum(), TString::Format("%d ns",(int)ts).Data());
      
      c_ge[(i-2)*16+j]->Update();
      c_ge[(i-2)*16+j]->SaveAs(TString::Format("./fig/%04d/c_ge_%02d_%02d.png",run,i,j).Data());
      sprintf(str, "%3d  %3d  %2d  %13.2f  %10.2f", 0, i, j, ts, chi2);
      fo << str << endl;

      if(latex) delete latex;
    }
  }

  // Si
  TH1D *h_si[11*16];
  TCanvas *c_si[11*16];
  for(int i=2;i<=12;i++){
    for(int j=0;j<16;j++){
      h_si[(i-2)*16+j] = (TH1D*)gFile->Get(TString::Format("si_sid%d_ch%02d",i,j).Data());

      if(h_si[(i-2)*16+j]->GetEntries()==0){
        sprintf(str, "%3d  %3d  %2d  %13.2f  %10.2f", 1, i, j, 0., 0.);
        fo << str << endl;
        continue;
      }

      c_si[(i-2)*16+j] = new TCanvas(TString::Format("c_si_%02d_%02d",i,j).Data(), "");
      c_si[(i-2)*16+j]->cd();
      h_si[(i-2)*16+j]->Rebin(2);
      h_si[(i-2)*16+j]->Draw();

      ts = -1000+h_si[(i-2)*16+j]->GetMaximumBin()*h_si[(i-2)*16+j]->GetBinWidth(0);
      chi2 = 0;

      TLatex *latex = new TLatex();
      latex->SetTextSize(0.04);
      latex->SetTextColor(2);
      latex->DrawLatex(-800, 0.9*h_si[(i-2)*16+j]->GetMaximum(), TString::Format("%d ns",(int)ts).Data());
      
      c_si[(i-2)*16+j]->Update();
      c_si[(i-2)*16+j]->SaveAs(TString::Format("./fig/%04d/c_si_%02d_%02d.png",run,i,j).Data()); 

      sprintf(str, "%3d  %3d  %2d  %13.2f  %10.2f", 1, i, j, ts, chi2);
      fo << str << endl;

      if(latex) delete latex;
    }
  }

  fo.close();
}

