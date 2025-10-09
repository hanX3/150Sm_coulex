// str is tree name 
void b2hist(int run, int win, int jump, TString str)
{
  TRandom3 *rndm = new TRandom3((Long64_t)time(0)); 

  //
  double cut_spider = 20000; // keV
  double cut_s3 = 20000; // keV

  //
  TFile *fi = TFile::Open(Form("../../rootfile/data%04d_build_%dns_jump_%dns.root",run,win,jump));
  if(fi->IsZombie()){
    std::cout << "open file run " << run << " error!" << std::endl;
    delete fi;

    return ;
  }

  TTree *tr = (TTree*)fi->Get(Form("tr_%s",str.Data()));
  raw *rd = new raw(tr); 

  //
  TFile *fo = new TFile(Form("../../rootfile/data%04d_build_%dns_jump_%dns_hist_%s.root",run,win,jump,str.Data()), "recreate");
  
  //
  double spider_r_min[8] = {4.63, 14.29, 22.99, 31.69, 40.39, 49.09, 57.79, 66.49};
  double spider_r_max[8] = {14.09, 22.79, 31.49, 40.19, 48.89, 57.59, 66.29, 75.20};
  double spider_phi_min[12], spider_phi_max[12];
  for(int i=0;i<12;i++){
    spider_phi_min[i] = 285.+30.*i;
    spider_phi_max[i] = 285.+30.*(i+1);
  }

  double s3_r_min[24], s3_r_max[24];
  double s3_phi_min[32], s3_phi_max[32];
  for(int i=0;i<24;i++){
    s3_r_min[i] = 34.-i;
    s3_r_max[i] = 35.-i;
  }
  for(int i=0;i<32;i++){
    s3_phi_min[i] = 11.25*i;
    s3_phi_max[i] = 11.25*(i+1);
  }

  vector<Long64_t> v_ge_ts;
  vector<Long64_t> v_spider_ts;
  vector<Long64_t> v_s3_sector_ts;
  vector<Long64_t> v_s3_ring_ts;
  vector<Long64_t> v_s3_ts;
  vector<Long64_t> v_si_ts;

  //
  TH1D *hpg, *hgg, *hpp;
  TH1D *h_spider_ge, *h_s3_ge;
  TH1D *h_spider_spider, *h_spider_s3, *h_s3_s3;
  TH2D *hh_spider_spot, *hh_s3_sector_spot, *hh_s3_ring_spot;

  // time diff
  hpg = new TH1D("hpg", "", 150, -3000, 3000);
  hgg = new TH1D("hgg", "", 150, -3000, 3000);
  hpp = new TH1D("hpp", "", 150, -3000, 3000);
  h_spider_ge = new TH1D("h_spider_ge", "", 300, -3000, 3000);
  h_s3_ge = new TH1D("h_s3_ge", "", 300, -3000, 3000);
  h_spider_spider = new TH1D("h_spider_spider", "", 300, -3000, 3000);
  h_s3_s3 = new TH1D("h_s3_s3", "", 300, -3000, 3000);
  // beam spot
  hh_spider_spot = new TH2D("hh_spider_spot", "", 320,-80,80,320,-80,80);
  hh_s3_sector_spot = new TH2D("hh_s3_sector_spot", "", 720,-36,36,720,-36,36);
  hh_s3_ring_spot = new TH2D("hh_s3_ring_spot", "", 360,-36,36,360,-36,36);

  double spider_r, spider_angle, spider_x, spider_y;
  double s3_r, s3_angle, s3_x, s3_y;

  cout <<  rd->GetEntries() << endl;
  for(Long64_t i=0;i<rd->GetEntries();i++){
    if(i%100000==0) cout << i << "/" << rd->GetEntries() << endl;

    rd->GetEntry(i);

    v_ge_ts.clear();
    v_spider_ts.clear();
    v_s3_sector_ts.clear();
    v_s3_ring_ts.clear();
    v_s3_ts.clear();
    v_si_ts.clear();

    //
    for(int j=0;j<rd->n_spider;j++){
      if(rd->spider_energy[j]<cut_spider) continue;
      v_spider_ts.push_back(rd->spider_ts[j]);
    }
    for(int j=0;j<rd->n_s3_sector;j++){
      if(rd->s3_sector_energy[j]<cut_s3) continue;
      v_s3_sector_ts.push_back(rd->s3_sector_ts[j]);
    }
    for(int j=0;j<rd->n_s3_ring;j++){
      if(rd->s3_ring_energy[j]<cut_s3) continue;
      v_s3_ring_ts.push_back(rd->s3_ring_ts[j]);
    }
  
    v_s3_ts.insert(v_s3_ts.end(), v_s3_sector_ts.begin(), v_s3_sector_ts.end());
    v_s3_ts.insert(v_s3_ts.end(), v_s3_ring_ts.begin(), v_s3_ring_ts.end());

    v_si_ts.insert(v_si_ts.end(), v_spider_ts.begin(), v_spider_ts.end());
    v_si_ts.insert(v_si_ts.end(), v_s3_ts.begin(), v_s3_ts.end());

    //
    for(int j=0;j<v_ge_ts.size();j++){
      for(int jj=0;jj<v_spider_ts.size();jj++){
        hpg->Fill(v_spider_ts[jj]-v_ge_ts[j]);
        h_spider_ge->Fill(v_spider_ts[jj]-v_ge_ts[j]);
      }
      for(int jj=0;jj<v_s3_ts.size();jj++){
        hpg->Fill(v_s3_ts[jj]-v_ge_ts[j]);
        h_s3_ge->Fill(v_s3_ts[jj]-v_ge_ts[j]);
      }
    }

    //
    if(v_ge_ts.size()>1){
      for(int j=0;j<v_ge_ts.size();j++){
        for(int jj=j+1;jj<v_ge_ts.size();jj++){
          if(rndm->Uniform(0,1)>0.5) hgg->Fill(v_ge_ts[j]-v_ge_ts[jj]);
          else hgg->Fill(v_ge_ts[jj]-v_ge_ts[j]);
        }
      }
    }

    //
    if(v_spider_ts.size()>1){
      for(int j=0;j<v_spider_ts.size();j++){
        for(int jj=j+1;jj<v_spider_ts.size();jj++){
          if(rndm->Uniform(0,1)>0.5) h_spider_spider->Fill(v_spider_ts[j]-v_spider_ts[jj]);
          else h_spider_spider->Fill(v_spider_ts[jj]-v_spider_ts[j]);
        }
      }
    }

    //
    if(v_s3_ts.size()>1){
      for(int j=0;j<v_s3_ts.size();j++){
        for(int jj=j+1;jj<v_s3_ts.size();jj++){
          if(rndm->Uniform(0,1)>0.5) h_s3_s3->Fill(v_s3_ts[j]-v_s3_ts[jj]);
          else h_s3_s3->Fill(v_s3_ts[jj]-v_s3_ts[j]);
        }
      }
    }

    //
    if(v_si_ts.size()){
      for(int j=0;j<v_si_ts.size();j++){
        for(int jj=j+1;jj<v_si_ts.size();jj++){
          if(rndm->Uniform(0,1)>0.5) hpp->Fill(v_si_ts[j]-v_si_ts[jj]);
          else hpp->Fill(v_si_ts[jj]-v_si_ts[j]);
        }
      }
    }

    //
    for(int j=0;j<rd->n_spider;j++){
      if(rd->spider_energy[j]<cut_spider) continue;

      spider_r = spider_r_min[rd->spider_ring_id[j]-1] + rndm->Uniform(0,1)*(spider_r_max[rd->spider_ring_id[j]-1]-spider_r_min[rd->spider_ring_id[j]-1]);
      spider_angle = spider_phi_min[rd->spider_sector_id[j]-1] + rndm->Uniform(0,1)*(spider_phi_max[rd->spider_sector_id[j]-1]-spider_phi_min[rd->spider_sector_id[j]-1]); 

      spider_x = spider_r*std::cos(spider_angle/180.*TMath::Pi());
      spider_y = spider_r*std::sin(spider_angle/180.*TMath::Pi());

      hh_spider_spot->Fill(spider_x, spider_y);
    }

    //
    for(int j=0;j<rd->n_s3_sector;j++){
      if(rd->s3_sector_energy[j]<cut_s3) continue;

      s3_r = 11. + rndm->Uniform(0,1)*24.;
      s3_angle = s3_phi_min[rd->s3_sector_id[j]-1] + rndm->Uniform(0,1)*(s3_phi_max[rd->s3_sector_id[j]-1]-s3_phi_min[rd->s3_sector_id[j]-1]); 

      s3_x = s3_r*std::cos(s3_angle/180.*TMath::Pi());
      s3_y = s3_r*std::sin(s3_angle/180.*TMath::Pi());

      hh_s3_sector_spot->Fill(s3_x, s3_y);
    }

    //
    for(int j=0;j<rd->n_s3_ring;j++){
      if(rd->s3_ring_energy[j]<cut_s3) continue;

      s3_r = s3_r_min[rd->s3_ring_id[j]-1] + rndm->Uniform(0,1)*(s3_r_max[rd->s3_ring_id[j]-1]-s3_r_min[rd->s3_ring_id[j]-1]); 
      s3_angle = rndm->Uniform(0,1)*360.; 

      s3_x = s3_r*std::cos(s3_angle/180.*TMath::Pi());
      s3_y = s3_r*std::sin(s3_angle/180.*TMath::Pi());

      hh_s3_ring_spot->Fill(s3_x, s3_y);
    }
  }

  //
  cout << "start spider\n";
  TH1D *h_n_spider;
  TH1D *h_spider_sector_id, *h_spider_ring_id;
  TH1D *h_spider_energy;
  TH2D *hh_spider_sector_energy, *hh_spider_ring_energy;
  TH2D* hh_spider_energy;
  TH1D *h_spider_sector_energy_single[12], *h_spider_ring_energy_single[8];

  h_n_spider = new TH1D("h_n_spider", "", 96, 0, 96);
  h_spider_sector_id = new TH1D("h_spider_sector_id", "", 20, 0, 20);
  h_spider_ring_id = new TH1D("h_spider_ring_id", "", 10, 0, 10);
  h_spider_energy = new TH1D("h_spider_energy", "", 1000, 0, 100000);
  hh_spider_sector_energy = new TH2D("hh_spider_sector_energy", "", 12, 0, 12, 1000, 0, 100000);
  hh_spider_ring_energy = new TH2D("hh_spider_ring_energy", "", 8, 0, 8, 1000, 0, 100000);
  hh_spider_energy = new TH2D("hh_spider_energy", "", 96, 0, 96, 1000, 0, 100000);
  for(int i=0;i<12;i++){
    h_spider_sector_energy_single[i] = new TH1D(TString::Format("h_spider_sector%02d_energy",i+1).Data(), "", 1000, 0, 100000);
    tr->Draw(TString::Format("spider_energy>>%s",h_spider_sector_energy_single[i]->GetName()).Data(), TString::Format("spider_sector_id==%d",i+1).Data(), "goff");
  }
  for(int i=0;i<8;i++){
    h_spider_ring_energy_single[i] = new TH1D(TString::Format("h_spider_ring%02d_energy",i+1).Data(), "", 1000, 0, 100000);
    tr->Draw(TString::Format("spider_energy>>%s",h_spider_ring_energy_single[i]->GetName()).Data(), TString::Format("spider_ring_id==%d",i+1).Data(), "goff");
  }

  TString str_cut_spider = TString::Format("spider_energy>%f", cut_spider);

  tr->Draw(TString::Format("n_spider>>%s",h_n_spider->GetName()).Data(), str_cut_spider.Data(), "goff");
  tr->Draw(TString::Format("spider_sector_id>>%s",h_spider_sector_id->GetName()).Data(), str_cut_spider.Data(), "goff");
  tr->Draw(TString::Format("spider_ring_id>>%s",h_spider_ring_id->GetName()).Data(), str_cut_spider.Data(), "goff");
  tr->Draw(TString::Format("spider_energy>>%s",h_spider_energy->GetName()).Data(), "", "goff");
  tr->Draw(TString::Format("spider_energy:spider_sector_id>>%s",hh_spider_sector_energy->GetName()).Data(), "", "goff");
  tr->Draw(TString::Format("spider_energy:spider_ring_id>>%s",hh_spider_ring_energy->GetName()).Data(), "", "goff");
  tr->Draw(TString::Format("spider_energy:(spider_ring_id-1)*12+spider_sector_id>>%s",hh_spider_energy->GetName()).Data(), "", "goff");

  //
  cout << "start s3\n";
  TH1D *h_n_s3_sector, *h_n_s3_ring;
  TH1D *h_s3_sector_id, *h_s3_ring_id;
  TH1D *h_s3_sector_energy, *h_s3_ring_energy;
  TH2D *hh_s3_sector_energy, *hh_s3_ring_energy;
  TH1D *h_s3_sector_energy_single[32], *h_s3_ring_energy_single[24];

  h_n_s3_sector = new TH1D("h_n_s3_sector", "", 32, 0, 32);
  h_n_s3_ring = new TH1D("h_n_s3_ring", "", 24, 0, 24);
  h_s3_sector_id = new TH1D("h_s3_sector_id", "", 40, 0, 40);
  h_s3_ring_id = new TH1D("h_s3_ring_id", "", 30, 0, 30);
  h_s3_sector_energy = new TH1D("h_s3_sector_energy", "", 1000, 0, 100000);
  h_s3_ring_energy = new TH1D("h_s3_ring_energy", "", 1000, 0, 100000);
  hh_s3_sector_energy = new TH2D("hh_s3_sector_energy", "", 40,0,40, 1000,0,100000);
  hh_s3_ring_energy = new TH2D("hh_s3_ring_energy", "", 30,0,30, 1000,0,100000);
  for(int i=0;i<32;i++){
    h_s3_sector_energy_single[i] = new TH1D(TString::Format("h_s3_sector%02d_energy",i+1).Data(), "", 1000, 0, 100000);
    tr->Draw(TString::Format("s3_sector_energy>>%s",h_s3_sector_energy_single[i]->GetName()).Data(), TString::Format("s3_sector_id==%d",i+1).Data(), "goff");
  }
  for(int i=0;i<24;i++){
    h_s3_ring_energy_single[i] = new TH1D(TString::Format("h_s3_ring%02d_energy",i+1).Data(), "", 1000, 0, 100000);
    tr->Draw(TString::Format("s3_ring_energy>>%s",h_s3_ring_energy_single[i]->GetName()).Data(), TString::Format("s3_ring_id==%d",i+1).Data(), "goff");
  }

  TString str_cut_s3_sector = TString::Format("s3_sector_energy>%f", cut_s3);
  TString str_cut_s3_ring = TString::Format("s3_ring_energy>%f", cut_s3);

  tr->Draw(TString::Format("n_s3_sector>>%s",h_n_s3_sector->GetName()).Data(), str_cut_s3_sector.Data(), "goff");
  tr->Draw(TString::Format("n_s3_ring>>%s",h_n_s3_ring->GetName()).Data(), str_cut_s3_ring.Data(), "goff");
  tr->Draw(TString::Format("s3_sector_id>>%s",h_s3_sector_id->GetName()).Data(), str_cut_s3_sector.Data(), "goff");
  tr->Draw(TString::Format("s3_ring_id>>%s",h_s3_ring_id->GetName()).Data(), str_cut_s3_ring.Data(), "goff");
  tr->Draw(TString::Format("s3_sector_energy>>%s",h_s3_sector_energy->GetName()).Data(), "", "goff");
  tr->Draw(TString::Format("s3_ring_energy>>%s",h_s3_ring_energy->GetName()).Data(), "", "goff");
  tr->Draw(TString::Format("s3_sector_energy:s3_sector_id>>%s",hh_s3_sector_energy->GetName()).Data(), "", "goff");
  tr->Draw(TString::Format("s3_ring_energy:s3_ring_id>>%s",hh_s3_ring_energy->GetName()).Data(), "", "goff");
  //
  cout << "start ge spectra\n"; 
  map<int, int> map_ge_ring_sector = {
    {1, 6},
    {2, 8},
    {3, 12},
    {4, 8},
    {5, 6}
  };

  TH1D *h_ge[40], *h_ge_spider[40], *h_ge_s3[40], *h_ge_spider_cut[40], *h_ge_s3_cut[40];
  int k = 0;
  for(int i=1;i<=5;i++){
    for(int j=1;j<=map_ge_ring_sector[i];j++){
      h_ge[k] = new TH1D(TString::Format("h_ge_ring%d_sector%d",i,j),"",4096,0,4096);
      h_ge_spider[k] = new TH1D(TString::Format("h_ge_ring%d_sector%d_spider",i,j),"",4096,0,4096);
      h_ge_s3[k] = new TH1D(TString::Format("h_ge_ring%d_sector%d_s3",i,j),"",4096,0,4096);
      h_ge_spider_cut[k] = new TH1D(TString::Format("h_ge_ring%d_sector%d_spider_cut",i,j),"",4096,0,4096);
      h_ge_s3_cut[k] = new TH1D(TString::Format("h_ge_ring%d_sector%d_s3_cut",i,j),"",4096,0,4096);

      //
      TString cut_str = TString::Format("ge_ring_id==%d && ge_sector_id==%d", i, j);
      tr->Draw(TString::Format("ge_energy>>%s",h_ge[k]->GetName()).Data(), cut_str.Data(),"goff");
      h_ge[k]->SetTitle(cut_str);
      cout << h_ge[k]->GetName() << endl;
      //
      TString cut_str_spider = TString::Format("ge_ring_id==%d && ge_sector_id==%d && n_spider>0", i, j);
      tr->Draw(TString::Format("ge_energy>>%s",h_ge_spider[k]->GetName()).Data(), cut_str_spider.Data(),"goff");
      h_ge_spider[k]->SetTitle(cut_str_spider);
      cout << h_ge_spider[k]->GetName() << endl;
      //
      TString cut_str_s3 = TString::Format("ge_ring_id==%d && ge_sector_id==%d && (n_s3_ring+n_s3_sector)>0", i, j);
      tr->Draw(TString::Format("ge_energy>>%s",h_ge_s3[k]->GetName()).Data(), cut_str_s3.Data(),"goff");
      h_ge_s3[k]->SetTitle(cut_str_s3);
      //
      TString cut_str_spider_cut = TString::Format("ge_ring_id==%d && ge_sector_id==%d && n_spider>0 &&spider_energy>%f", i, j, cut_spider);
      tr->Draw(TString::Format("ge_energy>>%s",h_ge_spider_cut[k]->GetName()).Data(), cut_str_spider_cut.Data(),"goff");
      h_ge_spider_cut[k]->SetTitle(cut_str_spider_cut);
      //
      TString cut_str_s3_cut = TString::Format("ge_ring_id==%d && ge_sector_id==%d && (n_s3_ring+n_s3_sector)>0 &&(s3_ring_energy>%f||s3_sector_energy>%f)", i, j, cut_s3, cut_s3);
      tr->Draw(TString::Format("ge_energy>>%s",h_ge_s3_cut[k]->GetName()).Data(), cut_str_s3_cut.Data(),"goff");
      h_ge_s3_cut[k]->SetTitle(cut_str_s3_cut);
      k++;
    }
  }

  //
  fo->cd();

  hh_spider_spot->Write();
  hh_s3_sector_spot->Write();
  hh_s3_ring_spot->Write();

  hpg->Write();
  hgg->Write();
  hpp->Write();
  h_spider_ge->Write();
  h_s3_ge->Write();
  h_spider_spider->Write();
  h_s3_s3->Write();
  
  //
  h_n_spider->Write();
  h_spider_sector_id->Write();
  h_spider_ring_id->Write();
  h_spider_energy->Write();
  hh_spider_sector_energy->Write();
  hh_spider_ring_energy->Write();
  hh_spider_energy->Write();
  for(int i=0;i<12;i++){
    h_spider_sector_energy_single[i]->Write();
  }
  for(int i=0;i<8;i++){
    h_spider_ring_energy_single[i]->Write();
  }

  //
  h_n_s3_sector->Write();
  h_n_s3_ring->Write();
  h_s3_sector_id->Write();
  h_s3_ring_id->Write();
  h_s3_sector_energy->Write();
  h_s3_ring_energy->Write();
  hh_s3_sector_energy->Write();
  hh_s3_ring_energy->Write();
  for(int i=0;i<32;i++){
    h_s3_sector_energy_single[i]->Write();
  }
  for(int i=0;i<24;i++){
    h_s3_ring_energy_single[i]->Write();
  }

  //
  k = 0;
  for(int i=1;i<=5;i++){
    for(int j=1;j<=map_ge_ring_sector[i];j++){
      h_ge[k]->Write();
      h_ge_spider[k]->Write();
      h_ge_s3[k]->Write();
      h_ge_spider_cut[k]->Write();
      h_ge_s3_cut[k]->Write();
      
      k++;
    }
  }

  //
  fo->Close();
  fi->Close();
}
