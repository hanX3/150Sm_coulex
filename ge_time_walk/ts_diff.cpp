//
void ts_diff(int run, int win)
{
  double x_min=-5000., x_max=5000.;
  int n_bin = int(x_max-x_min);
  cout << "n_bin " << n_bin << " [" << x_min << ", " << x_max << "]" << endl;

  TRandom3 *rndm = new TRandom3((Long64_t)time(0)); 
  
  //
  TFile *fo = new TFile(TString::Format("../rootfile/data%04d_ge_tw_%dns_ts_diff.root",run,win).Data(), "recreate");
  
  TFile *fi = TFile::Open(TString::Format("../rootfile/data%04d_ge_tw_%dns.root",run,win).Data());
  if(fi->IsZombie()){
    std::cout << "open file run " << run << " error!" << std::endl;
    delete fi;

    return ;
  }

  //
  double cut_ge = 50.;
  double cut_spider = 20000;
  double cut_s3 = 30000;

  vector<Long64_t> v_ge_ts;
  vector<Long64_t> v_spider_ts;
  vector<Long64_t> v_spider_ts_lt;
  vector<Long64_t> v_spider_ts_gt;
  vector<Long64_t> v_s3_sector_ts;
  vector<Long64_t> v_s3_sector_ts_lt;
  vector<Long64_t> v_s3_sector_ts_gt;
  vector<Long64_t> v_s3_ring_ts;
  vector<Long64_t> v_s3_ring_ts_lt;
  vector<Long64_t> v_s3_ring_ts_gt;
  vector<Long64_t> v_s3_ts;
  vector<Long64_t> v_s3_ts_lt;
  vector<Long64_t> v_s3_ts_gt;
  vector<Long64_t> v_si_ts;
  vector<Long64_t> v_si_ts_lt;
  vector<Long64_t> v_si_ts_gt;

  //
  TTree *tr_event = (TTree*)fi->Get("tr_event");
  raw *rd_event = new raw(tr_event); 

  //
  TH1D *hpg_event, *hgg_event, *hpp_event;
  TH1D *h_spider_ge_event, *h_spider_ge_lt_event, *h_spider_ge_gt_event;
  TH1D *h_s3_ge_event, *h_s3_ge_lt_event, *h_s3_ge_gt_event;
  TH1D *h_spider_spider_event, *h_spider_spider_lt_event, *h_spider_spider_gt_event;
  TH1D *h_spider_s3_event, *h_spider_s3_lt_event, *h_spider_s3_gt_event;
  TH1D *h_s3_s3_event, *h_s3_s3_lt_event, *h_s3_s3_gt_event;

  hpg_event = new TH1D("hpg_event", "", n_bin, x_min, x_max);
  hgg_event = new TH1D("hgg_event", "", n_bin, x_min, x_max);
  hpp_event = new TH1D("hpp_event", "", n_bin, x_min, x_max);
  h_spider_ge_event = new TH1D("h_spider_ge_event", "", n_bin, x_min, x_max);
  h_spider_ge_lt_event = new TH1D("h_spider_ge_lt_event", "", n_bin, x_min, x_max);
  h_spider_ge_gt_event = new TH1D("h_spider_ge_gt_event", "", n_bin, x_min, x_max);
  h_s3_ge_event = new TH1D("h_s3_ge_event", "", n_bin, x_min, x_max);
  h_s3_ge_lt_event = new TH1D("h_s3_ge_lt_event", "", n_bin, x_min, x_max);
  h_s3_ge_gt_event = new TH1D("h_s3_ge_gt_event", "", n_bin, x_min, x_max);
  h_spider_spider_event = new TH1D("h_spider_spider_event", "", n_bin, x_min, x_max);
  h_spider_spider_lt_event = new TH1D("h_spider_spider_lt_event", "", n_bin, x_min, x_max);
  h_spider_spider_gt_event = new TH1D("h_spider_spider_gt_event", "", n_bin, x_min, x_max);
  h_spider_s3_event = new TH1D("h_spider_s3_event", "", n_bin, x_min, x_max);
  h_spider_s3_lt_event = new TH1D("h_spider_s3_lt_event", "", n_bin, x_min, x_max);
  h_spider_s3_gt_event = new TH1D("h_spider_s3_gt_event", "", n_bin, x_min, x_max);
  h_s3_s3_event = new TH1D("h_s3_s3_event", "", n_bin, x_min, x_max);
  h_s3_s3_lt_event = new TH1D("h_s3_s3_lt_event", "", n_bin, x_min, x_max);
  h_s3_s3_gt_event = new TH1D("h_s3_s3_gt_event", "", n_bin, x_min, x_max);

  //
  cout << rd_event->GetEntries() << endl;
  for(Long64_t i=0;i<rd_event->GetEntries();++i){
    if(i%1000000==0) cout << i << "/" << rd_event->GetEntries() << endl;

    rd_event->GetEntry(i);

    v_ge_ts.clear();
    v_spider_ts.clear();
    v_spider_ts_lt.clear();
    v_spider_ts_gt.clear();
    v_s3_sector_ts.clear();
    v_s3_sector_ts_lt.clear();
    v_s3_sector_ts_gt.clear();
    v_s3_ring_ts.clear();
    v_s3_ring_ts_lt.clear();
    v_s3_ring_ts_gt.clear();
    v_s3_ts.clear();
    v_s3_ts_lt.clear();
    v_s3_ts_gt.clear();
    v_si_ts.clear();
    v_si_ts_lt.clear();
    v_si_ts_gt.clear();

    //
    for(int j=0;j<rd_event->n_ge;++j){
      v_ge_ts.push_back(rd_event->ge_ts[j]);
    }
    if(v_ge_ts.size()==0) continue;

    for(int j=0;j<rd_event->n_spider;++j){
      v_spider_ts.push_back(rd_event->spider_ts[j]);

      if(rd_event->spider_energy[j]<cut_spider) v_spider_ts_lt.push_back(rd_event->spider_ts[j]);
      else v_spider_ts_gt.push_back(rd_event->spider_ts[j]);
    }
    for(int j=0;j<rd_event->n_s3_sector;++j){
      v_s3_sector_ts.push_back(rd_event->s3_sector_ts[j]);

      if(rd_event->s3_sector_energy[j]<cut_s3) v_s3_sector_ts_lt.push_back(rd_event->s3_sector_ts[j]);
      else v_s3_sector_ts_gt.push_back(rd_event->s3_sector_ts[j]);
    }
    for(int j=0;j<rd_event->n_s3_ring;++j){
      v_s3_ring_ts.push_back(rd_event->s3_ring_ts[j]);

      if(rd_event->s3_ring_energy[j]<cut_s3) v_s3_ring_ts_lt.push_back(rd_event->s3_ring_ts[j]);
      else v_s3_ring_ts_gt.push_back(rd_event->s3_ring_ts[j]);
    }
   
    v_s3_ts.insert(v_s3_ts.end(), v_s3_sector_ts.begin(), v_s3_sector_ts.end());
    v_s3_ts.insert(v_s3_ts.end(), v_s3_ring_ts.begin(), v_s3_ring_ts.end());

    v_s3_ts_lt.insert(v_s3_ts_lt.end(), v_s3_sector_ts_lt.begin(), v_s3_sector_ts_lt.end());
    v_s3_ts_lt.insert(v_s3_ts_lt.end(), v_s3_ring_ts_lt.begin(), v_s3_ring_ts_lt.end());

    v_s3_ts_gt.insert(v_s3_ts_gt.end(), v_s3_sector_ts_gt.begin(), v_s3_sector_ts_gt.end());
    v_s3_ts_gt.insert(v_s3_ts_gt.end(), v_s3_ring_ts_gt.begin(), v_s3_ring_ts_gt.end());

    v_si_ts.insert(v_si_ts.end(), v_spider_ts.begin(), v_spider_ts.end());
    v_si_ts.insert(v_si_ts.end(), v_s3_ts.begin(), v_s3_ts.end());

    v_si_ts_lt.insert(v_si_ts_lt.end(), v_spider_ts_lt.begin(), v_spider_ts_lt.end());
    v_si_ts_lt.insert(v_si_ts_lt.end(), v_s3_ts_lt.begin(), v_s3_ts_lt.end());

    v_si_ts_gt.insert(v_si_ts_gt.end(), v_spider_ts_gt.begin(), v_spider_ts_gt.end());
    v_si_ts_gt.insert(v_si_ts_gt.end(), v_s3_ts_gt.begin(), v_s3_ts_gt.end());
    
    //
    for(int j=0;j<v_ge_ts.size();++j){
      for(int jj=0;jj<v_spider_ts.size();++jj){
        hpg_event->Fill(v_spider_ts[jj]-v_ge_ts[j]);
        h_spider_ge_event->Fill(v_spider_ts[jj]-v_ge_ts[j]);
      }
      for(int jj=0;jj<v_s3_ts.size();++jj){
        hpg_event->Fill(v_s3_ts[jj]-v_ge_ts[j]);
        h_s3_ge_event->Fill(v_s3_ts[jj]-v_ge_ts[j]);
      }
    }

    //
    for(int j=0;j<v_ge_ts.size();++j){
      for(int jj=0;jj<v_spider_ts_lt.size();++jj){
        h_spider_ge_lt_event->Fill(v_spider_ts_lt[jj]-v_ge_ts[j]);
      }
    }

    //
    for(int j=0;j<v_ge_ts.size();++j){
      for(int jj=0;jj<v_spider_ts_gt.size();++jj){
        h_spider_ge_gt_event->Fill(v_spider_ts_gt[jj]-v_ge_ts[j]);
      }
    }

    //
    for(int j=0;j<v_ge_ts.size();++j){
      for(int jj=0;jj<v_s3_ts_lt.size();++jj){
        h_s3_ge_lt_event->Fill(v_s3_ts_lt[jj]-v_ge_ts[j]);
      }
    }

    //
    for(int j=0;j<v_ge_ts.size();++j){
      for(int jj=0;jj<v_s3_ts_gt.size();++jj){
        h_s3_ge_gt_event->Fill(v_s3_ts_gt[jj]-v_ge_ts[j]);
      }
    }

    //
    if(v_ge_ts.size()>1){
      for(int j=0;j<v_ge_ts.size();++j){
        for(int jj=j+1;jj<v_ge_ts.size();++jj){
          if(rndm->Uniform(0,1)>0.5) hgg_event->Fill(v_ge_ts[j]-v_ge_ts[jj]);
          else hgg_event->Fill(v_ge_ts[jj]-v_ge_ts[j]);
        }
      }
    }

    //
    if(v_si_ts.size()){
      for(int j=0;j<v_si_ts.size();++j){
        for(int jj=j+1;jj<v_si_ts.size();++jj){
          if(rndm->Uniform(0,1)>0.5) hpp_event->Fill(v_si_ts[j]-v_si_ts[jj]);
          else hpp_event->Fill(v_si_ts[jj]-v_si_ts[j]);
        }
      }
    }

    //
    if(v_spider_ts.size()>1){
      for(int j=0;j<v_spider_ts.size();++j){
        for(int jj=j+1;jj<v_spider_ts.size();++jj){
          if(rndm->Uniform(0,1)>0.5) h_spider_spider_event->Fill(v_spider_ts[j]-v_spider_ts[jj]);
          else h_spider_spider_event->Fill(v_spider_ts[jj]-v_spider_ts[j]);
        }
      }
    }

    //
    if(v_spider_ts_lt.size()>1){
      for(int j=0;j<v_spider_ts_lt.size();++j){
        for(int jj=j+1;jj<v_spider_ts_lt.size();++jj){
          if(rndm->Uniform(0,1)>0.5) h_spider_spider_lt_event->Fill(v_spider_ts_lt[j]-v_spider_ts_lt[jj]);
          else h_spider_spider_lt_event->Fill(v_spider_ts_lt[jj]-v_spider_ts_lt[j]);
        }
      }
    }

    //
    if(v_spider_ts_gt.size()>1){
      for(int j=0;j<v_spider_ts_gt.size();++j){
        for(int jj=j+1;jj<v_spider_ts_gt.size();++jj){
          if(rndm->Uniform(0,1)>0.5) h_spider_spider_gt_event->Fill(v_spider_ts_gt[j]-v_spider_ts_gt[jj]);
          else h_spider_spider_lt_event->Fill(v_spider_ts_gt[jj]-v_spider_ts_gt[j]);
        }
      }
    }

    //
    if(v_spider_ts.size()>0 && v_s3_ts.size()>0){
      for(int j=0;j<v_spider_ts.size();++j){
        for(int jj=0;jj<v_s3_ts.size();++jj){
          h_spider_s3_event->Fill(v_spider_ts[j]-v_s3_ts[jj]);
        }
      }
    }

    //
    if(v_spider_ts_lt.size()>0 && v_s3_ts_lt.size()>0){
      for(int j=0;j<v_spider_ts_lt.size();++j){
        for(int jj=0;jj<v_s3_ts_lt.size();++jj){
          h_spider_s3_lt_event->Fill(v_spider_ts_lt[j]-v_s3_ts_lt[jj]);
        }
      }
    }

    //
    if(v_spider_ts_gt.size()>0 && v_s3_ts_gt.size()>0){
      for(int j=0;j<v_spider_ts_gt.size();++j){
        for(int jj=0;jj<v_s3_ts_gt.size();++jj){
          h_spider_s3_gt_event->Fill(v_spider_ts_gt[j]-v_s3_ts_gt[jj]);
        }
      }
    }

    //
    if(v_s3_ts.size()>1){
      for(int j=0;j<v_s3_ts.size();++j){
        for(int jj=j+1;jj<v_s3_ts.size();++jj){
          if(rndm->Uniform(0,1)>0.5) h_s3_s3_event->Fill(v_s3_ts[j]-v_s3_ts[jj]);
          else h_s3_s3_event->Fill(v_s3_ts[jj]-v_s3_ts[j]);
        }
      }
    }

    //
    if(v_s3_ts_lt.size()>1){
      for(int j=0;j<v_s3_ts_lt.size();++j){
        for(int jj=j+1;jj<v_s3_ts_lt.size();++jj){
          if(rndm->Uniform(0,1)>0.5) h_s3_s3_lt_event->Fill(v_s3_ts_lt[j]-v_s3_ts_lt[jj]);
          else h_s3_s3_lt_event->Fill(v_s3_ts_lt[jj]-v_s3_ts_lt[j]);
        }
      }
    }

    //
    if(v_s3_ts_gt.size()>1){
      for(int j=0;j<v_s3_ts_gt.size();++j){
        for(int jj=j+1;jj<v_s3_ts_gt.size();++jj){
          if(rndm->Uniform(0,1)>0.5) h_s3_s3_lt_event->Fill(v_s3_ts_gt[j]-v_s3_ts_gt[jj]);
          else h_s3_s3_gt_event->Fill(v_s3_ts_gt[jj]-v_s3_ts_gt[j]);
        }
      }
    }
  }

  //
  TTree *tr_bg = (TTree*)fi->Get("tr_bg");
  raw *rd_bg = new raw(tr_bg); 

  //
  TH1D *hpg_bg, *hgg_bg, *hpp_bg;
  TH1D *h_spider_ge_bg, *h_spider_ge_lt_bg, *h_spider_ge_gt_bg;
  TH1D *h_s3_ge_bg, *h_s3_ge_lt_bg, *h_s3_ge_gt_bg;
  TH1D *h_spider_spider_bg, *h_spider_spider_lt_bg, *h_spider_spider_gt_bg;
  TH1D *h_spider_s3_bg, *h_spider_s3_lt_bg, *h_spider_s3_gt_bg;
  TH1D *h_s3_s3_bg, *h_s3_s3_lt_bg, *h_s3_s3_gt_bg;

  hpg_bg = new TH1D("hpg_bg", "", n_bin, x_min, x_max);
  hgg_bg = new TH1D("hgg_bg", "", n_bin, x_min, x_max);
  hpp_bg = new TH1D("hpp_bg", "", n_bin, x_min, x_max);
  h_spider_ge_bg = new TH1D("h_spider_ge_bg", "", n_bin, x_min, x_max);
  h_spider_ge_lt_bg = new TH1D("h_spider_ge_lt_bg", "", n_bin, x_min, x_max);
  h_spider_ge_gt_bg = new TH1D("h_spider_ge_gt_bg", "", n_bin, x_min, x_max);
  h_s3_ge_bg = new TH1D("h_s3_ge_bg", "", n_bin, x_min, x_max);
  h_s3_ge_lt_bg = new TH1D("h_s3_ge_lt_bg", "", n_bin, x_min, x_max);
  h_s3_ge_gt_bg = new TH1D("h_s3_ge_gt_bg", "", n_bin, x_min, x_max);
  h_spider_spider_bg = new TH1D("h_spider_spider_bg", "", n_bin, x_min, x_max);
  h_spider_spider_lt_bg = new TH1D("h_spider_spider_lt_bg", "", n_bin, x_min, x_max);
  h_spider_spider_gt_bg = new TH1D("h_spider_spider_gt_bg", "", n_bin, x_min, x_max);
  h_spider_s3_bg = new TH1D("h_spider_s3_bg", "", n_bin, x_min, x_max);
  h_spider_s3_lt_bg = new TH1D("h_spider_s3_lt_bg", "", n_bin, x_min, x_max);
  h_spider_s3_gt_bg = new TH1D("h_spider_s3_gt_bg", "", n_bin, x_min, x_max);
  h_s3_s3_bg = new TH1D("h_s3_s3_bg", "", n_bin, x_min, x_max);
  h_s3_s3_lt_bg = new TH1D("h_s3_s3_lt_bg", "", n_bin, x_min, x_max);
  h_s3_s3_gt_bg = new TH1D("h_s3_s3_gt_bg", "", n_bin, x_min, x_max);

  //
  cout << rd_bg->GetEntries() << endl;
  for(Long64_t i=0;i<rd_bg->GetEntries();++i){
    if(i%1000000==0) cout << i << "/" << rd_bg->GetEntries() << endl;

    rd_bg->GetEntry(i);

    v_ge_ts.clear();
    v_spider_ts.clear();
    v_spider_ts_lt.clear();
    v_spider_ts_gt.clear();
    v_s3_sector_ts.clear();
    v_s3_sector_ts_lt.clear();
    v_s3_sector_ts_gt.clear();
    v_s3_ring_ts.clear();
    v_s3_ring_ts_lt.clear();
    v_s3_ring_ts_gt.clear();
    v_s3_ts.clear();
    v_s3_ts_lt.clear();
    v_s3_ts_gt.clear();
    v_si_ts.clear();
    v_si_ts_lt.clear();
    v_si_ts_gt.clear();

    //
    for(int j=0;j<rd_bg->n_ge;++j){
      v_ge_ts.push_back(rd_bg->ge_ts[j]);
    }
    if(v_ge_ts.size()==0) continue;

    for(int j=0;j<rd_bg->n_spider;++j){
      v_spider_ts.push_back(rd_bg->spider_ts[j]);

      if(rd_bg->spider_energy[j]<cut_spider) v_spider_ts_lt.push_back(rd_bg->spider_ts[j]);
      else v_spider_ts_gt.push_back(rd_bg->spider_ts[j]);
    }
    for(int j=0;j<rd_bg->n_s3_sector;++j){
      v_s3_sector_ts.push_back(rd_bg->s3_sector_ts[j]);

      if(rd_bg->s3_sector_energy[j]<cut_s3) v_s3_sector_ts_lt.push_back(rd_bg->s3_sector_ts[j]);
      else v_s3_sector_ts_gt.push_back(rd_bg->s3_sector_ts[j]);
    }
    for(int j=0;j<rd_bg->n_s3_ring;++j){
      v_s3_ring_ts.push_back(rd_bg->s3_ring_ts[j]);

      if(rd_bg->s3_ring_energy[j]<cut_s3) v_s3_ring_ts_lt.push_back(rd_bg->s3_ring_ts[j]);
      else v_s3_ring_ts_gt.push_back(rd_bg->s3_ring_ts[j]);
    }
   
    v_s3_ts.insert(v_s3_ts.end(), v_s3_sector_ts.begin(), v_s3_sector_ts.end());
    v_s3_ts.insert(v_s3_ts.end(), v_s3_ring_ts.begin(), v_s3_ring_ts.end());

    v_s3_ts_lt.insert(v_s3_ts_lt.end(), v_s3_sector_ts_lt.begin(), v_s3_sector_ts_lt.end());
    v_s3_ts_lt.insert(v_s3_ts_lt.end(), v_s3_ring_ts_lt.begin(), v_s3_ring_ts_lt.end());

    v_s3_ts_gt.insert(v_s3_ts_gt.end(), v_s3_sector_ts_gt.begin(), v_s3_sector_ts_gt.end());
    v_s3_ts_gt.insert(v_s3_ts_gt.end(), v_s3_ring_ts_gt.begin(), v_s3_ring_ts_gt.end());

    v_si_ts.insert(v_si_ts.end(), v_spider_ts.begin(), v_spider_ts.end());
    v_si_ts.insert(v_si_ts.end(), v_s3_ts.begin(), v_s3_ts.end());

    v_si_ts_lt.insert(v_si_ts_lt.end(), v_spider_ts_lt.begin(), v_spider_ts_lt.end());
    v_si_ts_lt.insert(v_si_ts_lt.end(), v_s3_ts_lt.begin(), v_s3_ts_lt.end());

    v_si_ts_gt.insert(v_si_ts_gt.end(), v_spider_ts_gt.begin(), v_spider_ts_gt.end());
    v_si_ts_gt.insert(v_si_ts_gt.end(), v_s3_ts_gt.begin(), v_s3_ts_gt.end());
    
    //
    for(int j=0;j<v_ge_ts.size();++j){
      for(int jj=0;jj<v_spider_ts.size();++jj){
        hpg_bg->Fill(v_spider_ts[jj]-v_ge_ts[j]);
        h_spider_ge_bg->Fill(v_spider_ts[jj]-v_ge_ts[j]);
      }
      for(int jj=0;jj<v_s3_ts.size();++jj){
        hpg_bg->Fill(v_s3_ts[jj]-v_ge_ts[j]);
        h_s3_ge_bg->Fill(v_s3_ts[jj]-v_ge_ts[j]);
      }
    }

    //
    for(int j=0;j<v_ge_ts.size();++j){
      for(int jj=0;jj<v_spider_ts_lt.size();++jj){
        h_spider_ge_lt_bg->Fill(v_spider_ts_lt[jj]-v_ge_ts[j]);
      }
    }

    //
    for(int j=0;j<v_ge_ts.size();++j){
      for(int jj=0;jj<v_spider_ts_gt.size();++jj){
        h_spider_ge_gt_bg->Fill(v_spider_ts_gt[jj]-v_ge_ts[j]);
      }
    }

    //
    for(int j=0;j<v_ge_ts.size();++j){
      for(int jj=0;jj<v_s3_ts_lt.size();++jj){
        h_s3_ge_lt_bg->Fill(v_s3_ts_lt[jj]-v_ge_ts[j]);
      }
    }

    //
    for(int j=0;j<v_ge_ts.size();++j){
      for(int jj=0;jj<v_s3_ts_gt.size();++jj){
        h_s3_ge_gt_bg->Fill(v_s3_ts_gt[jj]-v_ge_ts[j]);
      }
    }

    //
    if(v_ge_ts.size()>1){
      for(int j=0;j<v_ge_ts.size();++j){
        for(int jj=j+1;jj<v_ge_ts.size();++jj){
          if(rndm->Uniform(0,1)>0.5) hgg_bg->Fill(v_ge_ts[j]-v_ge_ts[jj]);
          else hgg_bg->Fill(v_ge_ts[jj]-v_ge_ts[j]);
        }
      }
    }

    //
    if(v_si_ts.size()){
      for(int j=0;j<v_si_ts.size();++j){
        for(int jj=j+1;jj<v_si_ts.size();++jj){
          if(rndm->Uniform(0,1)>0.5) hpp_bg->Fill(v_si_ts[j]-v_si_ts[jj]);
          else hpp_bg->Fill(v_si_ts[jj]-v_si_ts[j]);
        }
      }
    }

    //
    if(v_spider_ts.size()>1){
      for(int j=0;j<v_spider_ts.size();++j){
        for(int jj=j+1;jj<v_spider_ts.size();++jj){
          if(rndm->Uniform(0,1)>0.5) h_spider_spider_bg->Fill(v_spider_ts[j]-v_spider_ts[jj]);
          else h_spider_spider_bg->Fill(v_spider_ts[jj]-v_spider_ts[j]);
        }
      }
    }

    //
    if(v_spider_ts_lt.size()>1){
      for(int j=0;j<v_spider_ts_lt.size();++j){
        for(int jj=j+1;jj<v_spider_ts_lt.size();++jj){
          if(rndm->Uniform(0,1)>0.5) h_spider_spider_lt_bg->Fill(v_spider_ts_lt[j]-v_spider_ts_lt[jj]);
          else h_spider_spider_lt_bg->Fill(v_spider_ts_lt[jj]-v_spider_ts_lt[j]);
        }
      }
    }

    //
    if(v_spider_ts_gt.size()>1){
      for(int j=0;j<v_spider_ts_gt.size();++j){
        for(int jj=j+1;jj<v_spider_ts_gt.size();++jj){
          if(rndm->Uniform(0,1)>0.5) h_spider_spider_gt_bg->Fill(v_spider_ts_gt[j]-v_spider_ts_gt[jj]);
          else h_spider_spider_lt_bg->Fill(v_spider_ts_gt[jj]-v_spider_ts_gt[j]);
        }
      }
    }

    //
    if(v_spider_ts.size()>0 && v_s3_ts.size()>0){
      for(int j=0;j<v_spider_ts.size();++j){
        for(int jj=0;jj<v_s3_ts.size();++jj){
          h_spider_s3_bg->Fill(v_spider_ts[j]-v_s3_ts[jj]);
        }
      }
    }

    //
    if(v_spider_ts_lt.size()>0 && v_s3_ts_lt.size()>0){
      for(int j=0;j<v_spider_ts_lt.size();++j){
        for(int jj=0;jj<v_s3_ts_lt.size();++jj){
          h_spider_s3_lt_bg->Fill(v_spider_ts_lt[j]-v_s3_ts_lt[jj]);
        }
      }
    }

    //
    if(v_spider_ts_gt.size()>0 && v_s3_ts_gt.size()>0){
      for(int j=0;j<v_spider_ts_gt.size();++j){
        for(int jj=0;jj<v_s3_ts_gt.size();++jj){
          h_spider_s3_gt_bg->Fill(v_spider_ts_gt[j]-v_s3_ts_gt[jj]);
        }
      }
    }

    //
    if(v_s3_ts.size()>1){
      for(int j=0;j<v_s3_ts.size();++j){
        for(int jj=j+1;jj<v_s3_ts.size();++jj){
          if(rndm->Uniform(0,1)>0.5) h_s3_s3_bg->Fill(v_s3_ts[j]-v_s3_ts[jj]);
          else h_s3_s3_bg->Fill(v_s3_ts[jj]-v_s3_ts[j]);
        }
      }
    }

    //
    if(v_s3_ts_lt.size()>1){
      for(int j=0;j<v_s3_ts_lt.size();++j){
        for(int jj=j+1;jj<v_s3_ts_lt.size();++jj){
          if(rndm->Uniform(0,1)>0.5) h_s3_s3_lt_bg->Fill(v_s3_ts_lt[j]-v_s3_ts_lt[jj]);
          else h_s3_s3_lt_bg->Fill(v_s3_ts_lt[jj]-v_s3_ts_lt[j]);
        }
      }
    }

    //
    if(v_s3_ts_gt.size()>1){
      for(int j=0;j<v_s3_ts_gt.size();++j){
        for(int jj=j+1;jj<v_s3_ts_gt.size();++jj){
          if(rndm->Uniform(0,1)>0.5) h_s3_s3_lt_bg->Fill(v_s3_ts_gt[j]-v_s3_ts_gt[jj]);
          else h_s3_s3_gt_bg->Fill(v_s3_ts_gt[jj]-v_s3_ts_gt[j]);
        }
      }
    }
  }

  // 
  TH1D *hpg_event_bg, *hgg_event_bg, *hpp_event_bg;
  TH1D *h_spider_ge_event_bg, *h_spider_ge_lt_event_bg, *h_spider_ge_gt_event_bg;
  TH1D *h_s3_ge_event_bg, *h_s3_ge_lt_event_bg, *h_s3_ge_gt_event_bg;
  TH1D *h_spider_spider_event_bg, *h_spider_spider_lt_event_bg, *h_spider_spider_gt_event_bg;
  TH1D *h_spider_s3_event_bg, *h_spider_s3_lt_event_bg, *h_spider_s3_gt_event_bg;
  TH1D *h_s3_s3_event_bg, *h_s3_s3_lt_event_bg, *h_s3_s3_gt_event_bg;
  
  hpg_event_bg = (TH1D*)hpg_event->Clone("hpg_event_bg");
  hgg_event_bg = (TH1D*)hgg_event->Clone("hgg_event_bg");
  hpp_event_bg = (TH1D*)hpp_event->Clone("hpp_event_bg");
  h_spider_ge_event_bg = (TH1D*)h_spider_ge_event->Clone("h_spider_ge_event_bg");
  h_spider_ge_lt_event_bg = (TH1D*)h_spider_ge_lt_event->Clone("h_spider_ge_lt_event_bg");
  h_spider_ge_gt_event_bg = (TH1D*)h_spider_ge_gt_event->Clone("h_spider_ge_gt_event_bg");
  h_s3_ge_event_bg = (TH1D*)h_s3_ge_event->Clone("h_s3_ge_event_bg");
  h_s3_ge_lt_event_bg = (TH1D*)h_s3_ge_lt_event->Clone("h_s3_ge_lt_event_bg");
  h_s3_ge_gt_event_bg = (TH1D*)h_s3_ge_gt_event->Clone("h_s3_ge_gt_event_bg");
  h_spider_spider_event_bg = (TH1D*)h_spider_spider_event->Clone("h_spider_spider_event_bg");
  h_spider_spider_lt_event_bg = (TH1D*)h_spider_spider_lt_event->Clone("h_spider_spider_lt_event_bg");
  h_spider_spider_gt_event_bg = (TH1D*)h_spider_spider_gt_event->Clone("h_spider_spider_gt_event_bg");
  h_spider_s3_event_bg = (TH1D*)h_spider_s3_event->Clone("h_spider_s3_event_bg");
  h_spider_s3_lt_event_bg = (TH1D*)h_spider_s3_lt_event->Clone("h_spider_s3_lt_event_bg");
  h_spider_s3_gt_event_bg = (TH1D*)h_spider_s3_gt_event->Clone("h_spider_s3_gt_event_bg");
  h_s3_s3_event_bg = (TH1D*)h_s3_s3_event->Clone("h_s3_s3_event_bg"); 
  h_s3_s3_lt_event_bg = (TH1D*)h_s3_s3_lt_event->Clone("h_s3_s3_lt_event_bg"); 
  h_s3_s3_gt_event_bg = (TH1D*)h_s3_s3_gt_event->Clone("h_s3_s3_gt_event_bg"); 

  hpg_event_bg->Add(hpg_bg, -1);
  hgg_event_bg->Add(hgg_bg, -1);
  hpp_event_bg->Add(hpp_bg, -1);
  h_spider_ge_event_bg->Add(h_spider_ge_bg, -1);
  h_spider_ge_lt_event_bg->Add(h_spider_ge_lt_bg, -1);
  h_spider_ge_gt_event_bg->Add(h_spider_ge_gt_bg, -1);
  h_s3_ge_event_bg->Add(h_s3_ge_bg, -1);
  h_s3_ge_lt_event_bg->Add(h_s3_ge_lt_bg, -1);
  h_s3_ge_gt_event_bg->Add(h_s3_ge_gt_bg, -1);
  h_spider_spider_event_bg->Add(h_spider_spider_bg, -1);
  h_spider_spider_lt_event_bg->Add(h_spider_spider_lt_bg, -1);
  h_spider_spider_gt_event_bg->Add(h_spider_spider_gt_bg, -1);
  h_spider_s3_event_bg->Add(h_spider_s3_bg, -1);
  h_spider_s3_lt_event_bg->Add(h_spider_s3_lt_bg, -1);
  h_spider_s3_gt_event_bg->Add(h_spider_s3_gt_bg, -1);
  h_s3_s3_event_bg->Add(h_s3_s3_bg, -1);
  h_s3_s3_lt_event_bg->Add(h_s3_s3_lt_bg, -1);
  h_s3_s3_gt_event_bg->Add(h_s3_s3_gt_bg, -1);

  //
  fo->cd();

  hpg_event->Write(); hgg_event->Write(); hpp_event->Write();
  h_spider_ge_event->Write(); h_spider_ge_lt_event->Write(); h_spider_ge_gt_event->Write();
  h_s3_ge_event->Write(); h_s3_ge_lt_event->Write(); h_s3_ge_gt_event->Write();
  h_spider_spider_event->Write(); h_spider_spider_lt_event->Write(); h_spider_spider_gt_event->Write();
  h_spider_s3_event->Write(); h_spider_s3_lt_event->Write(); h_spider_s3_gt_event->Write();
  h_s3_s3_event->Write(); h_s3_s3_lt_event->Write(); h_s3_s3_gt_event->Write();
  
  hpg_bg->Write(); hgg_bg->Write(); hpp_bg->Write();
  h_spider_ge_bg->Write(); h_spider_ge_lt_bg->Write(); h_spider_ge_gt_bg->Write();
  h_s3_ge_bg->Write(); h_s3_ge_lt_bg->Write(); h_s3_ge_gt_bg->Write();
  h_spider_spider_bg->Write(); h_spider_spider_lt_bg->Write(); h_spider_spider_gt_bg->Write();
  h_spider_s3_bg->Write(); h_spider_s3_lt_bg->Write(); h_spider_s3_gt_bg->Write();
  h_s3_s3_bg->Write(); h_s3_s3_lt_bg->Write(); h_s3_s3_gt_bg->Write();

  hpg_event_bg->Write(); hgg_event_bg->Write(); hpp_event_bg->Write();
  h_spider_ge_event_bg->Write(); h_spider_ge_lt_event_bg->Write(); h_spider_ge_gt_event_bg->Write();
  h_s3_ge_event_bg->Write(); h_s3_ge_lt_event_bg->Write(); h_s3_ge_gt_event_bg->Write();
  h_spider_spider_event_bg->Write(); h_spider_spider_lt_event_bg->Write(); h_spider_spider_gt_event_bg->Write();
  h_spider_s3_event_bg->Write(); h_spider_s3_lt_event_bg->Write(); h_spider_s3_gt_event_bg->Write();
  h_s3_s3_event_bg->Write(); h_s3_s3_lt_event_bg->Write(); h_s3_s3_gt_event_bg->Write();


  //
  fo->Close();
  fi->Close();
}
