#define subjet_csv_cxx
#include "subjet_csv.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

void subjet_csv::Loop(string outputFileName)
{
  //   In a ROOT session, you can do:
  //      root> .L subjet_csv.C
  //      root> subjet_csv t
  //      root> t.GetEntry(12); // Fill t data members with entry number 12
  //      root> t.Show();       // Show values of entry 12
  //      root> t.Show(16);     // Read and show values of entry 16
  //      root> t.Loop();       // Loop on all entries
  //

  //     This is the loop skeleton where:
  //    jentry is the global entry number in the chain
  //    ientry is the entry number in the current Tree
  //  Note that the argument to GetEntry must be:
  //    jentry for TChain::GetEntry
  //    ientry for TTree::GetEntry and TBranch::GetEntry
  //
  //       To read only selected branches, Insert statements like:
  fChain->SetBranchStatus("*",0);  // disable all branches
  fChain->SetBranchStatus("subjetAK8_pruned_csv",1);  // activate branchname
  fChain->SetBranchStatus("subjetAK8_pruned_N",1);  // activate branchname
  // METHOD2: replace line
  //    fChain->GetEntry(jentry);       //read all branches
  //by  b_branchname->GetEntry(ientry); //read only this branch
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  bool debugFlag    = false;
  bool printThisGuy = false;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;++jentry) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (Cut(ientry) < 0) continue;
    if (debugFlag) {
      printThisGuy = false;
      for (uint iJet=0; iJet<subjetAK8_pruned_csv->size(); ++iJet) {  
        printThisGuy |= ( subjetAK8_pruned_csv->at(iJet).size() > 1 ) ;
      }
    }

    if (printThisGuy) cout << "Event "<< jentry << ":" << endl;
    for (uint iJet=0; iJet<subjetAK8_pruned_csv->size(); ++iJet) {

      cout << "   jet " << iJet << " has number of subjets: " << subjetAK8_pruned_N->at(iJet) << endl;
      if (printThisGuy) cout << "   jet " << iJet << endl;
      for (uint iSubjet=0; iSubjet<subjetAK8_pruned_csv->at(iJet).size(); ++iSubjet) {
        if (printThisGuy) cout << "      subjet " << iSubjet << " has csv value: " << subjetAK8_pruned_csv->at(iJet).at(iSubjet) << endl;
      }
      leadingSubjets bSubJets = getLeadingSubjets(subjetAK8_pruned_csv->at(iJet));
      if (printThisGuy) cout << "   The two highest csv values for all subjets in this jet are: " << bSubJets.leading << ", " << bSubJets.subleading << endl;
      passSubjetCuts subjetCutDecisions = getSubjetCutDecisions(bSubJets);
      if (debugFlag) {
        cout << "   The decisions are : " << endl;
        cout << "      loose, loose:   " << subjetCutDecisions.loose_loose << endl;
        cout << "      medium, loose:  " << subjetCutDecisions.medium_loose << endl;
        cout << "      tight, loose:   " << subjetCutDecisions.tight_loose << endl;
        cout << "      medium, medium: " << subjetCutDecisions.medium_medium << endl;
        cout << "      tight, medium:  " << subjetCutDecisions.tight_medium << endl;
        cout << "      tight, tight:   " << subjetCutDecisions.tight_tight << endl;
      }
    }
  }
}

subjet_csv::leadingSubjets subjet_csv::getLeadingSubjets(vector<float> prunedJet) {
  // Note: in miniaod, there are only two subjets stored since the declustering is done recursively and miniaod's declustering stops after splitting into two subjets
  leadingSubjets topCSVs;
  topCSVs.leading = -10.;
  topCSVs.subleading = -10.;
  for (uint iSubjet=0; iSubjet<prunedJet.size(); ++iSubjet) {  
    if (prunedJet.at(iSubjet)>topCSVs.leading) {
      topCSVs.subleading = topCSVs.leading;
      topCSVs.leading = prunedJet.at(iSubjet);
    } 
    else if (topCSVs.leading > prunedJet.at(iSubjet) && topCSVs.subleading < prunedJet.at(iSubjet)) {
      topCSVs.subleading = prunedJet.at(iSubjet);
    }
  }
  return topCSVs;
}

subjet_csv::passSubjetCuts subjet_csv::getSubjetCutDecisions(leadingSubjets subjets) {
  float looseWP  = 0.605;
  float mediumWP = 0.89;
  float tightWP  = 0.97;

  bool leadingIsLoose     = (subjets.leading    > looseWP);
  bool leadingIsMedium    = (subjets.leading    > mediumWP);
  bool leadingIsTight     = (subjets.leading    > tightWP);
  bool subleadingIsLoose  = (subjets.subleading > looseWP);
  bool subleadingIsMedium = (subjets.subleading > mediumWP);
  bool subleadingIsTight  = (subjets.subleading > tightWP);

  passSubjetCuts decisions;

  decisions.loose_loose    = leadingIsLoose   &&  subleadingIsLoose;
  decisions.medium_loose   = leadingIsMedium  &&  subleadingIsLoose;
  decisions.tight_loose    = leadingIsTight   &&  subleadingIsLoose;
  decisions.medium_medium  = leadingIsMedium  &&  subleadingIsMedium;
  decisions.tight_medium   = leadingIsTight   &&  subleadingIsMedium;
  decisions.tight_tight    = leadingIsTight   &&  subleadingIsTight;

  return decisions;
}
