//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Oct  1 12:04:00 2025 by ROOT version 6.32.12
// from TTree tr_event/coincidence window [-200, 200] ns
// found on file: ../rootfile/si/data0472_build_s3_200ns_new.root
//////////////////////////////////////////////////////////

#ifndef s3_tree_h
#define s3_tree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class s3_tree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           n_s3_sector;
   Short_t         s3_sector_sid[2];   //[n_s3_sector]
   Short_t         s3_sector_ch[2];   //[n_s3_sector]
   Short_t         s3_sector_id[2];   //[n_s3_sector]
   Double_t        s3_sector_energy[2];   //[n_s3_sector]
   Long64_t        s3_sector_ts[2];   //[n_s3_sector]
   Int_t           n_s3_ring;
   Short_t         s3_ring_sid[2];   //[n_s3_ring]
   Short_t         s3_ring_ch[2];   //[n_s3_ring]
   Short_t         s3_ring_id[2];   //[n_s3_ring]
   Double_t        s3_ring_energy[2];   //[n_s3_ring]
   Long64_t        s3_ring_ts[2];   //[n_s3_ring]

   // List of branches
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

   s3_tree(TTree *tree=0);
   virtual ~s3_tree();
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

#ifdef s3_tree_cxx
s3_tree::s3_tree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   Init(tree);
}

s3_tree::~s3_tree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t s3_tree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t s3_tree::LoadTree(Long64_t entry)
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

void s3_tree::Init(TTree *tree)
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

bool s3_tree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return true;
}

void s3_tree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

Int_t s3_tree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

Long64_t s3_tree::GetEntries()
{
  return fChain->GetEntriesFast();
}
#endif // #ifdef s3_tree_cxx
