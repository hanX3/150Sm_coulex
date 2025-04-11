#include <sys/stat.h>
#include <sys/types.h>

//
void correction_single(ofstream &fo, int run, int sector, int ring, double rob=0.6)
{
  mkdir("fig", 0777);
  mkdir(TString::Format("./fig/%04d", run).Data(), 0777);

  //
  ifstream fi_ring_cor(TString::Format("correction_ring_%04d.txt",run).Data());
  if(!fi_ring_cor){
    cout << "can not open ring correction file." << endl;
    return;
  }

  std::map<int, std::vector<double>> map_ring_cor;
  std::string line;
  std::getline(fi_ring_cor, line);

  int id;
  double p0, p1;
  while(1){
    fi_ring_cor >> id >> p0 >> p1;
    if(!fi_ring_cor.good()) break;

    map_ring_cor[id] = {p0, p1};
  }

  for(auto &it : map_ring_cor){
    cout << it.first << " " << it.second[0] << " " << it.second[1] << endl;
  }
  fi_ring_cor.close();

  //
  TFile *fi = TFile::Open(TString::Format("../rootfile/si/data%04d_build_s3_200ns.root", run).Data());
  if(fi->IsZombie()){
    cout << "can not open rootfile." << endl;
    return;
  }

  TTree *tr = (TTree*)fi->Get("tr_s3");
  TH2D *hh1 = new TH2D(TString::Format("hh1_sector%02d_ring%02d",sector,ring).Data(), "", 8192, 0, 65536, 8192, 0, 65536);
  TCanvas *c1 = new TCanvas(TString::Format("c1_for_sector_sector%02d_ring%02d",sector,ring).Data(), "", 0, 0, 600, 400);
  TCanvas *c2 = new TCanvas(TString::Format("c2_for_sector_sector%02d_ring%02d",sector,ring).Data(), "", 0, 0, 600, 400);
  TCanvas *c3 = new TCanvas(TString::Format("c3_for_sector_sector%02d_ring%02d",sector,ring).Data(), "", 0, 0, 600, 400);
  TGraph *gr;

  int n = 0;
  TString draw_str = TString::Format("%lf+%lf*s3_ring_energy:s3_sector_energy>>%s", map_ring_cor[ring][0], map_ring_cor[ring][1], hh1->GetName());
  TString cut_str = TString::Format("n_s3_sector==1 && n_s3_ring==1 && s3_sector_id==%d && s3_ring_id==%d && s3_sector_energy>20000", sector, ring);
  cout << draw_str << endl;
  cout << cut_str << endl;
  cout << endl;

  c1->cd();
  n = tr->Draw(draw_str.Data(), cut_str.Data(), "colz");
  cout << "n " << n << endl;
  gr = new TGraph(n, tr->GetV2(), tr->GetV1());

  c2->cd();
  gr->Draw("ap");

  gr->Fit("pol1", TString::Format("ROB=%lf",rob).Data(), "same", 0, 65536);
  TF1 *tf = (TF1*)gROOT->GetListOfFunctions()->FindObject("pol1");

  cout << sector << " " << ring << " " << tf->GetParameter(0) << " " << tf->GetParameter(1) << endl;

  // check
  c3->cd();
  TH2D *hh2 = new TH2D(TString::Format("hh2_sector%02d_ring%02d",sector,ring).Data(), "", 1000, -5000, 5000, 8192, 0, 65536);
  draw_str = TString::Format("%lf+%lf*s3_sector_energy:(%lf+%lf*s3_sector_energy-%lf-%lf*s3_ring_energy)>>%s", tf->GetParameter(0), tf->GetParameter(1), tf->GetParameter(0), tf->GetParameter(1), map_ring_cor[ring][0], map_ring_cor[ring][1], hh2->GetName());
  cut_str = TString::Format("n_s3_sector==1 && n_s3_ring==1 && s3_sector_id==%d && s3_ring_id==%d", sector, ring);
  tr->Draw(draw_str.Data(), cut_str.Data(), "colz");

  c1->SaveAs(TString::Format("./fig/%04d/%s.png", run, c1->GetName()));
  c2->SaveAs(TString::Format("./fig/%04d/%s.png", run, c2->GetName()));
  c3->SaveAs(TString::Format("./fig/%04d/%s.png", run, c3->GetName()));

  fi->Close();

  fo << sector << "    " << tf->GetParameter(0) << "    " << tf->GetParameter(1) << endl;
}

//
void correction_sector(int run)
{
  gROOT->SetBatch(1);

  ofstream fo(TString::Format("correction_sector_%04d.txt",run).Data(), std::ios::app);
  fo << " sector  p0                p1" << endl;

  for(int i=1;i<=32;i++){
    correction_single(fo, run, i, 12);
  }

  fo.close();
}
