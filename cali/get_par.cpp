
//
void get_par_single(ofstream &fo, int run, int cid, int sid, int ch)
{
  TString str_tmp, str_out;
  TH1D *h1;

  str_tmp = Form("../rootfile/ge/data%04d_hist.root", run);
  TFile *fi = new TFile(str_tmp.Data());
  if(fi==NULL){
    cout << "open file " << str_tmp.Data() << " error!!!" << endl;
    return -1;
  }

  gFile->cd(TString::Format("cid%d_sid%02d", cid, sid).Data());

  str_tmp = Form("h_cid%d_sid%02d_ch%02d", cid, sid, ch);
  cout << "analysis " << str_tmp << endl;
  h1 = (TH1D*)gDirectory->Get(str_tmp.Data());
  if(!h1){
    cout << "no data" << endl;
    str_out = Form("%4d  %4d  %4d  %16.9f  %16.9f  %16.9f  %16.9f", cid, sid, ch, 0., 0., 0., 0.);
    fo << str_out << endl;
    return;
  }

  double par[4] = {0., 0., 0., 0.};
  auto_cali(h1, par);
  str_out = Form("%4d  %4d  %4d  %16.9f  %16.9f  %16.9f  %16.9f", cid, sid, ch, par[0], par[1], par[2], par[3]);

  fo << str_out << endl;

  fi->Close();
}

//
void get_par_all(int run)
{
  ofstream fo("cali.txt", std::ios::app);
  fo << " cid   sid    ch                p0                p1                p2               chi" << endl;

  int cid = 0;
  for(int sid=2;sid<=5;sid++){
    for(int ch=0;ch<16;ch++){
      get_par_single(fo, run, cid, sid, ch);
    }
  }

  fo.close();
}
