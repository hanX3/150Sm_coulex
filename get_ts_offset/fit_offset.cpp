#include <sys/stat.h>
#include <sys/types.h>

// Ge vs. Ge
void fit_offset(TString filename, int run)
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
  sprintf(str, "%3d  %3d  %2d  %13.2f  %10.2f", 0, 0, 0, 0., 0.);
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
      h_ge[(i-2)*16+j]->Draw();
      h_ge[(i-2)*16+j]->Fit("gaus", "", "", -300, 300);
      TF1 *fg = (TF1*)gROOT->GetListOfFunctions()->FindObject("gaus");
      c_ge[(i-2)*16+j]->SaveAs(TString::Format("./fig/%04d/c1_ge_%02d_%02d.png",run,i,j).Data());

      TF1 *fgg = new TF1("fgg", "gaus(0)+pol0(3)", fg->GetParameter(1)-1.5*fg->GetParameter(2), fg->GetParameter(1)+1.5*fg->GetParameter(2));
      fgg->SetParameter(0, fg->GetParameter(0));
      fgg->SetParameter(1, fg->GetParameter(1));
      fgg->SetParameter(2, fg->GetParameter(2));
      fgg->SetParameter(3, 10.);

      h_ge[(i-2)*16+j]->Draw();
      h_ge[(i-2)*16+j]->Fit("fgg", "R");
      
      ts = fgg->GetParameter(1);
      chi2 = fgg->GetChisquare();

      TLatex *latex = new TLatex();
      latex->SetTextSize(0.05);
      latex->SetTextColor(2);
      latex->DrawLatex(-800, 0.9*h_ge[(i-2)*16+j]->GetMaximum(), TString::Format("%.2f ns",ts).Data());

      c_ge[(i-2)*16+j]->SaveAs(TString::Format("./fig/%04d/c2_ge_%02d_%02d.png",run,i,j).Data());

      sprintf(str, "%3d  %3d  %2d  %13.2f  %10.2f", 0, i, j, ts, chi2);
      fo << str << endl;
      if(fg) delete fg;
      if(fgg) delete fgg;
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
      h_si[(i-2)*16+j]->Draw();
      h_si[(i-2)*16+j]->Fit("gaus", "", "", 200, 800);
      TF1 *fg = (TF1*)gROOT->GetListOfFunctions()->FindObject("gaus");
      c_si[(i-2)*16+j]->SaveAs(TString::Format("./fig/%04d/c1_si_%02d_%02d.png",run,i,j).Data()); 

      TF1 *fgg = new TF1("fgg", "gaus(0)+pol0(3)", fg->GetParameter(1)-1.5*fg->GetParameter(2), fg->GetParameter(1)+1.5*fg->GetParameter(2));
      fgg->SetParameter(0, fg->GetParameter(0));
      fgg->SetParameter(1, fg->GetParameter(1));
      fgg->SetParameter(2, fg->GetParameter(2));
      fgg->SetParameter(3, 10.);

      h_si[(i-2)*16+j]->Draw();
      h_si[(i-2)*16+j]->Fit("fgg", "R");
      
      ts = fg->GetParameter(1);
      chi2 = fg->GetChisquare();

      TLatex *latex = new TLatex();
      latex->SetTextSize(0.05);
      latex->SetTextColor(2);
      latex->DrawLatex(-800, 0.9*h_ge[(i-2)*16+j]->GetMaximum(), TString::Format("%.2f ns",ts).Data());

      c_si[(i-2)*16+j]->SaveAs(TString::Format("./fig/%04d/c2_si_%02d_%02d.png",run,i,j).Data());
    
      sprintf(str, "%3d  %3d  %2d  %13.2f  %10.2f", 1, i, j, ts, chi2);
      fo << str << endl;
      if(fg) delete fg;
      if(fgg) delete fgg;
    }
  }

  fo.close();
}

