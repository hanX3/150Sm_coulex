//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May 14 10:02:45 2025 by ROOT version 6.24/02
// from TTree tr_event/timewindow_1000
// found on file: ../rootfile/data0472_build_1000ns.root
//////////////////////////////////////////////////////////

#ifndef raw_h
#define raw_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class raw {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           n_ge;
   Short_t         ge_sid[9];   //[n_ge]
   Short_t         ge_ch[9];   //[n_ge]
   Short_t         ge_ring_id[9];   //[n_ge]
   Short_t         ge_sector_id[9];   //[n_ge]
   Double_t        ge_energy[9];   //[n_ge]
   Long64_t        ge_ts[9];   //[n_ge]
   Int_t           n_spider;
   Short_t         spider_sid[13];   //[n_spider]
   Short_t         spider_ch[13];   //[n_spider]
   Short_t         spider_ring_id[13];   //[n_spider]
   Short_t         spider_sector_id[13];   //[n_spider]
   Double_t        spider_energy[13];   //[n_spider]
   Long64_t        spider_ts[13];   //[n_spider]
   Int_t           n_s3_sector;
   Short_t         s3_sector_sid[28];   //[n_s3_sector]
   Short_t         s3_sector_ch[28];   //[n_s3_sector]
   Short_t         s3_sector_id[28];   //[n_s3_sector]
   Double_t        s3_sector_energy[28];   //[n_s3_sector]
   Long64_t        s3_sector_ts[28];   //[n_s3_sector]
   Int_t           n_s3_ring;
   Short_t         s3_ring_sid[12];   //[n_s3_ring]
   Short_t         s3_ring_ch[12];   //[n_s3_ring]
   Short_t         s3_ring_id[12];   //[n_s3_ring]
   Double_t        s3_ring_energy[12];   //[n_s3_ring]
   Long64_t        s3_ring_ts[12];   //[n_s3_ring]

   // List of branches
   TBranch        *b_n_ge;   //!
   TBranch        *b_ge_sid;   //!
   TBranch        *b_ge_ch;   //!
   TBranch        *b_ge_ring_id;   //!
   TBranch        *b_ge_sector_id;   //!
   TBranch        *b_ge_energy;   //!
   TBranch        *b_ge_ts;   //!
   TBranch        *b_n_spider;   //!
   TBranch        *b_spider_sid;   //!
   TBranch        *b_spider_ch;   //!
   TBranch        *b_spider_ring_id;   //!
   TBranch        *b_spider_sector_id;   //!
   TBranch        *b_spider_energy;   //!
   TBranch        *b_spider_ts;   //!
   TBranch        *b_n_s3_sector;   //!
   TBranch        *b_s3_sector_sid;   //!
   TBranch        *b_s3_sector_ch;   //!
   TBranch        *b_s3_sector_id;   //!
   TBranch        *b_s3_sector_energy;   //!
   TBranch        *b_s3_sector_ts;   //!
   TBranch        *b_n_s3_ring;   //!
   TBranch        *b_s3_ring_sid;   //!
   TBranch        *b_s3_ring_ch;   //!
   TBranch        *b_s3_ring_id;   //!
   TBranch        *b_s3_ring_energy;   //!
   TBranch        *b_s3_ring_ts;   //!

   raw(TTree *tree=0);
   virtual ~raw();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   virtual Long64_t GetEntries();
};

#endif

#ifdef raw_cxx
raw::raw(TTree *tree) : fChain(0) 
{
   Init(tree);
}

raw::~raw()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t raw::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t raw::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void raw::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("n_ge", &n_ge, &b_n_ge);
   fChain->SetBranchAddress("ge_sid", ge_sid, &b_ge_sid);
   fChain->SetBranchAddress("ge_ch", ge_ch, &b_ge_ch);
   fChain->SetBranchAddress("ge_ring_id", ge_ring_id, &b_ge_ring_id);
   fChain->SetBranchAddress("ge_sector_id", ge_sector_id, &b_ge_sector_id);
   fChain->SetBranchAddress("ge_energy", ge_energy, &b_ge_energy);
   fChain->SetBranchAddress("ge_ts", ge_ts, &b_ge_ts);
   fChain->SetBranchAddress("n_spider", &n_spider, &b_n_spider);
   fChain->SetBranchAddress("spider_sid", spider_sid, &b_spider_sid);
   fChain->SetBranchAddress("spider_ch", spider_ch, &b_spider_ch);
   fChain->SetBranchAddress("spider_ring_id", spider_ring_id, &b_spider_ring_id);
   fChain->SetBranchAddress("spider_sector_id", spider_sector_id, &b_spider_sector_id);
   fChain->SetBranchAddress("spider_energy", spider_energy, &b_spider_energy);
   fChain->SetBranchAddress("spider_ts", spider_ts, &b_spider_ts);
   fChain->SetBranchAddress("n_s3_sector", &n_s3_sector, &b_n_s3_sector);
   fChain->SetBranchAddress("s3_sector_sid", s3_sector_sid, &b_s3_sector_sid);
   fChain->SetBranchAddress("s3_sector_ch", s3_sector_ch, &b_s3_sector_ch);
   fChain->SetBranchAddress("s3_sector_id", s3_sector_id, &b_s3_sector_id);
   fChain->SetBranchAddress("s3_sector_energy", s3_sector_energy, &b_s3_sector_energy);
   fChain->SetBranchAddress("s3_sector_ts", s3_sector_ts, &b_s3_sector_ts);
   fChain->SetBranchAddress("n_s3_ring", &n_s3_ring, &b_n_s3_ring);
   fChain->SetBranchAddress("s3_ring_sid", s3_ring_sid, &b_s3_ring_sid);
   fChain->SetBranchAddress("s3_ring_ch", s3_ring_ch, &b_s3_ring_ch);
   fChain->SetBranchAddress("s3_ring_id", s3_ring_id, &b_s3_ring_id);
   fChain->SetBranchAddress("s3_ring_energy", s3_ring_energy, &b_s3_ring_energy);
   fChain->SetBranchAddress("s3_ring_ts", s3_ring_ts, &b_s3_ring_ts);
   Notify();
}

Bool_t raw::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void raw::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t raw::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
Long64_t raw::GetEntries()
{
  return fChain->GetEntriesFast();
}
#endif // #ifdef raw_cxx
