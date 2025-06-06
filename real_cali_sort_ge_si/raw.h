//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun May 11 16:43:22 2025 by ROOT version 6.24/02
// from TTree tree/GDDAQ Multi-Crate sort Data
// found on file: ../rootfile/data_C2_0472_0472.root
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
   Short_t         sr;
   Bool_t          pileup;
   Bool_t          outofr;
   Short_t         cid;
   Short_t         sid;
   Short_t         ch;
   UShort_t        evte;
   Long64_t        ts;
   Short_t         cfd;
   Bool_t          cfdft;
   Short_t         cfds;

   // List of branches
   TBranch        *b_sr;   //!
   TBranch        *b_pileup;   //!
   TBranch        *b_outofr;   //!
   TBranch        *b_cid;   //!
   TBranch        *b_sid;   //!
   TBranch        *b_ch;   //!
   TBranch        *b_evte;   //!
   TBranch        *b_ts;   //!
   TBranch        *b_cfd;   //!
   TBranch        *b_cfdft;   //!
   TBranch        *b_cfds;   //!

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

   fChain->SetBranchAddress("sr", &sr, &b_sr);
   fChain->SetBranchAddress("pileup", &pileup, &b_pileup);
   fChain->SetBranchAddress("outofr", &outofr, &b_outofr);
   fChain->SetBranchAddress("cid", &cid, &b_cid);
   fChain->SetBranchAddress("sid", &sid, &b_sid);
   fChain->SetBranchAddress("ch", &ch, &b_ch);
   fChain->SetBranchAddress("evte", &evte, &b_evte);
   fChain->SetBranchAddress("ts", &ts, &b_ts);
   fChain->SetBranchAddress("cfd", &cfd, &b_cfd);
   fChain->SetBranchAddress("cfdft", &cfdft, &b_cfdft);
   fChain->SetBranchAddress("cfds", &cfds, &b_cfds);
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
