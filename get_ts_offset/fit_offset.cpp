// Ge vs. Ge
void fit_offset()
{
  ofstream fo;
  fo.open("ts.offset");
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
    
      c_ge[(i-2)*16+j]->SaveAs(TString::Format("c_ge_%02d_%02d.png",i,j).Data());

      ts = fg->GetParameter(1);
      chi2 = fg->GetChisquare();
    
      sprintf(str, "%3d  %3d  %2d  %13.2f  %10.2f", 0, i, j, ts, chi2);
      fo << str << endl;
      if(fg) delete fg;
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
      c_si[(i-2)*16+j]->SaveAs(TString::Format("c_si_%02d_%02d.png",i,j).Data()); 

      ts = fg->GetParameter(1);
      chi2 = fg->GetChisquare();
    
      sprintf(str, "%3d  %3d  %2d  %13.2f  %10.2f", 1, i, j, ts, chi2);
      fo << str << endl;
      if(fg) delete fg;
    }
  }

  fo.close();
}

