void compute_p2c()
{
  gStyle->SetOptStat(0);

  vector<string> v_filename = {
    "data0472_0718_doppler_100ns_e_hist",
    "data0472_0718_doppler_250ns_jump_250ns_e_hist",
    "data0472_0718_doppler_200ns_jump_200ns_e_hist",
    "data0472_0718_doppler_150ns_jump_150ns_e_hist",
    "data0472_0718_doppler_100ns_jump_100ns_e_hist",
    "data0472_0718_doppler_50ns_jump_50ns_e_hist"
  };

  TFile *fi;
  TH1D *h1, *hh1;
  double p, c;
  for(int i=0;i<v_filename.size();++i){
    p = 0; c = 0;

    fi = TFile::Open(TString::Format("../rootfile/%s.root", v_filename[i].c_str()));
    if(fi->IsZombie()){
      cout << "wrong " << v_filename[i] << endl;
      return;
    }

    h1 = (TH1D*)fi->Get("event_e_dc_r_1000fs_spider_p");
    hh1 = (TH1D*)fi->Get("bg_e_dc_r_1000fs_spider_p");
    h1->Add(hh1, -1);

    for(int j=300;j<310;++j) c += h1->GetBinContent(j);
    for(int j=330;j<340;++j) p += h1->GetBinContent(j);

    cout << v_filename[i] << endl;
    cout << "peak " << p << " compton " << c << " p/c " << double(p)/c << endl;
    cout << endl;
    
    fi->Close();
  }
}
