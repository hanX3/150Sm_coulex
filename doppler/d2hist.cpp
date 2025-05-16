//
void d2hist(TString str, int run)
{
  TRandom3 *rndm = new TRandom3((Long64_t)time(0)); 

  TFile *fi = TFile::Open(TString::Format("../rootfile/data%04d_doppler_200ns_%s.root",run,str.Data()));
  if(fi->IsZombie()){
    std::cout << "open file run " << run << " error!" << std::endl;
    delete fi;

    return ;
  }

  TFile *fo = new TFile(TString::Format("../rootfile/data%04d_doppler_200ns_%s_hist.root",run,str.Data()), "recreate");
  fo->cd();

  vector<string> v_str_name = {"event", "bg"};
  vector<string> v_str_e = {
    "e_raw", 
    "e_dc_p", 
    "e_dc_r", 
    "e_dc_p_10fs", 
    "e_dc_r_10fs", 
    "e_dc_p_100fs", 
    "e_dc_r_100fs", 
    "e_dc_p_1000fs", 
    "e_dc_r_1000fs"
  };

  //
  for(auto it=v_str_name.begin();it!=v_str_name.end();++it){
    TTree *tr = (TTree*)fi->Get(TString::Format("tr_%s",(*it).c_str()));

    TString str_cmd;

    TH1D *h[9];
    // all
    for(int i=0;i<9;i++){
      h[i] = new TH1D(TString::Format("%s_%s",(*it).c_str(),v_str_e[i].c_str()), "", 4096, 0, 4096);
      str_cmd = TString::Format("%s>>%s",v_str_e[i].c_str(),h[i]->GetName());
      tr->Draw(str_cmd.Data(), "", "goff");
      h[i]->Write();

      delete h[i];
    }

    // spider p
    for(int i=0;i<9;i++){
      h[i] = new TH1D(TString::Format("%s_%s_spider_p",(*it).c_str(),v_str_e[i].c_str()), "", 4096, 0, 4096);
      str_cmd = TString::Format("%s>>%s",v_str_e[i].c_str(),h[i]->GetName());
      tr->Draw(str_cmd.Data(), "who==1", "goff");
      h[i]->Write();

      delete h[i];
    }

    // s3 p
    for(int i=0;i<9;i++){
      h[i] = new TH1D(TString::Format("%s_%s_s3_p",(*it).c_str(),v_str_e[i].c_str()), "", 4096, 0, 4096);
      str_cmd = TString::Format("%s>>%s",v_str_e[i].c_str(),h[i]->GetName());
      tr->Draw(str_cmd.Data(), "who==2", "goff");
      h[i]->Write();

      delete h[i];
    }

    // spider p different ring ge
    for(int i=0;i<5;i++){
      for(int j=0;j<9;j++){
        h[j] = new TH1D(TString::Format("%s_%s_spider_p_ge_ring%d",(*it).c_str(),v_str_e[j].c_str(),i+1), "", 4096, 0, 4096);
        str_cmd = TString::Format("%s>>%s",v_str_e[j].c_str(),h[j]->GetName());
        tr->Draw(str_cmd.Data(), TString::Format("who==1 && rg==%d",i+1), "goff");
        h[j]->Write();

        delete h[j];
      }
    }

    // spider p different ring spider
    for(int i=0;i<8;i++){
      for(int j=0;j<9;j++){
        h[j] = new TH1D(TString::Format("%s_%s_spider_p_spider_ring%d",(*it).c_str(),v_str_e[j].c_str(),i+1), "", 4096, 0, 4096);
        str_cmd = TString::Format("%s>>%s",v_str_e[j].c_str(),h[j]->GetName());
        tr->Draw(str_cmd.Data(), TString::Format("who==1 && rp==%d",i+1), "goff");
        h[j]->Write();

        delete h[j];
      }
    }

    // spider p different sector spider
    for(int i=0;i<12;i++){
      for(int j=0;j<9;j++){
        h[j] = new TH1D(TString::Format("%s_%s_spider_p_spider_sector%d",(*it).c_str(),v_str_e[j].c_str(),i+1), "", 4096, 0, 4096);
        str_cmd = TString::Format("%s>>%s",v_str_e[j].c_str(),h[j]->GetName());
        tr->Draw(str_cmd.Data(), TString::Format("who==1 && sp==%d",i+1), "goff");
        h[j]->Write();

        delete h[j];
      }
    }
  }

  fo->Close();
  fi->Close();
}
