//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jun 20 17:07:04 2025 by ROOT version 6.32.12
// from TTree tr_event/event doppler data
// found on file: ../rootfile/data0472_doppler_250ns_jump_250ns_e.root
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
   Int_t           hits;
   Bool_t          flag_spider_p;
   Bool_t          flag_spider_r;
   Bool_t          flag_s3_p;
   Bool_t          flag_s3_r;
   Int_t           rp;
   Int_t           sp;
   Int_t           rr;
   Int_t           sr;
   Int_t           rg[4];   //[hits]
   Int_t           sg[4];   //[hits]
   Double_t        e_raw[4];   //[hits]
   Double_t        e_dc_p[4];   //[hits]
   Double_t        e_dc_r[4];   //[hits]
   Double_t        e_dc_p_10fs[4];   //[hits]
   Double_t        e_dc_r_10fs[4];   //[hits]
   Double_t        e_dc_p_100fs[4];   //[hits]
   Double_t        e_dc_r_100fs[4];   //[hits]
   Double_t        e_dc_p_1000fs[4];   //[hits]
   Double_t        e_dc_r_1000fs[4];   //[hits]

   // List of branches
   TBranch        *b_hits;   //!
   TBranch        *b_flag_spider_p;   //!
   TBranch        *b_flag_spider_r;   //!
   TBranch        *b_flag_s3_p;   //!
   TBranch        *b_flag_s3_r;   //!
   TBranch        *b_rp;   //!
   TBranch        *b_sp;   //!
   TBranch        *b_rr;   //!
   TBranch        *b_sr;   //!
   TBranch        *b_rg;   //!
   TBranch        *b_sg;   //!
   TBranch        *b_e_raw;   //!
   TBranch        *b_e_dc_p;   //!
   TBranch        *b_e_dc_r;   //!
   TBranch        *b_e_dc_p_10fs;   //!
   TBranch        *b_e_dc_r_10fs;   //!
   TBranch        *b_e_dc_p_100fs;   //!
   TBranch        *b_e_dc_r_100fs;   //!
   TBranch        *b_e_dc_p_1000fs;   //!
   TBranch        *b_e_dc_r_1000fs;   //!

   raw(TTree *tree=0);
   virtual ~raw();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual bool     Notify();
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

   fChain->SetBranchAddress("hits", &hits, &b_hits);
   fChain->SetBranchAddress("flag_spider_p", &flag_spider_p, &b_flag_spider_p);
   fChain->SetBranchAddress("flag_spider_r", &flag_spider_r, &b_flag_spider_r);
   fChain->SetBranchAddress("flag_s3_p", &flag_s3_p, &b_flag_s3_p);
   fChain->SetBranchAddress("flag_s3_r", &flag_s3_r, &b_flag_s3_r);
   fChain->SetBranchAddress("rp", &rp, &b_rp);
   fChain->SetBranchAddress("sp", &sp, &b_sp);
   fChain->SetBranchAddress("rr", &rr, &b_rr);
   fChain->SetBranchAddress("sr", &sr, &b_sr);
   fChain->SetBranchAddress("rg", rg, &b_rg);
   fChain->SetBranchAddress("sg", sg, &b_sg);
   fChain->SetBranchAddress("e_raw", e_raw, &b_e_raw);
   fChain->SetBranchAddress("e_dc_p", e_dc_p, &b_e_dc_p);
   fChain->SetBranchAddress("e_dc_r", e_dc_r, &b_e_dc_r);
   fChain->SetBranchAddress("e_dc_p_10fs", e_dc_p_10fs, &b_e_dc_p_10fs);
   fChain->SetBranchAddress("e_dc_r_10fs", e_dc_r_10fs, &b_e_dc_r_10fs);
   fChain->SetBranchAddress("e_dc_p_100fs", e_dc_p_100fs, &b_e_dc_p_100fs);
   fChain->SetBranchAddress("e_dc_r_100fs", e_dc_r_100fs, &b_e_dc_r_100fs);
   fChain->SetBranchAddress("e_dc_p_1000fs", e_dc_p_1000fs, &b_e_dc_p_1000fs);
   fChain->SetBranchAddress("e_dc_r_1000fs", e_dc_r_1000fs, &b_e_dc_r_1000fs);
   Notify();
}

bool raw::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return true;
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
