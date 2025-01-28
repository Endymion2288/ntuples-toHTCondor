#define ReadTree_cxx
//#define SHOW_DEBUG
#ifdef SHOW_DEBUG
#define VERBOSE
#endif

#define ALLOW_OVERLAP

// #define DRAW_RAW

#define CUT_MUON_ID_SOFT

#define CUT_UPS

#define CUT_UPS_MUON_PT

#define CUT_UPS_MUON_MEDIUM

#define _ON_LXPLUS_

#ifdef _ON_LXPLUS_
#include "/afs/cern.ch/user/c/chiw/cernbox/JpsiJpsiUps/ParticleCand/src/ParticleCand.C"
#endif

#ifdef _ON_HEPTHU_
#include "src/ParticleCand.C"
#endif

#include "JOB_DIR/ReadTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <memory>

// Include the header file for the roofit.
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooGaussian.h"
#include "RooArgList.h"
#include "RooFitResult.h"
#include "RooChebychev.h"

void ReadTree::Loop()
{
    if (fChain == 0) return;

    Long64_t nentries = fChain->GetEntriesFast();

    const unsigned int nBin = 40;
    const unsigned int nBin_cut = 20;
    const unsigned int nCandsAllowed = 20;

    // Use Roofit to draw the plot with proper error bars.
    // May try fitting later.
    #ifdef DRAW_RAW
    // Define mass histograms for Jpsi, Ups and Pri. Using Roofit.
    RooRealVar Jpsi1_mass_var("Jpsi1_mass", "Jpsi1_mass", 2.5, 3.5);
    RooRealVar Jpsi2_mass_var("Jpsi2_mass", "Jpsi2_mass", 2.5, 3.5);
    RooRealVar Ups_mass_var("Ups_mass","Ups_mass", 8.0, 12.0);
    RooRealVar Pri_mass_var("Pri_mass","Pri_mass", 0.0, 100.);
    #endif
    // Define mass histograms for Jpsi, Ups and Pri passing the cut. Using Roofit.
    RooRealVar Jpsi1_mass_cut_var("Jpsi1_mass_cut", "Jpsi1_mass_cut", 2.5, 3.5);
    RooRealVar Jpsi2_mass_cut_var("Jpsi2_mass_cut", "Jpsi2_mass_cut", 2.5, 3.5);
    RooRealVar Ups_mass_cut_var("Ups_mass_cut","Ups_mass_cut", 8.0, 12.0);
    RooRealVar Pri_mass_cut_var("Pri_mass_cut","Pri_mass_cut", 0.0, 100.0);

    #ifdef DRAW_RAW
    // Define dataset for Jpsi, Ups and Pri. Using Roofit.
    RooDataSet Jpsi1_mass_data("Jpsi1_mass_data", "Jpsi1_mass_data", RooArgList(Jpsi1_mass_var));
    RooDataSet Jpsi2_mass_data("Jpsi2_mass_data", "Jpsi2_mass_data", RooArgList(Jpsi2_mass_var));
    RooDataSet Ups_mass_data("Ups_mass_data", "Ups_mass_data", RooArgList(Ups_mass_var));
    RooDataSet Pri_mass_data("Pri_mass_data", "Pri_mass_data", RooArgList(Pri_mass_var));
    #endif
    // Define dataset for Jpsi, Ups and Pri passing the cut. Using Roofit.
    RooDataSet Jpsi1_mass_cut_data("Jpsi1_mass_cut_data", "Jpsi1_mass_cut_data", RooArgList(Jpsi1_mass_cut_var));
    RooDataSet Jpsi2_mass_cut_data("Jpsi2_mass_cut_data", "Jpsi2_mass_cut_data", RooArgList(Jpsi2_mass_cut_var));
    RooDataSet Ups_mass_cut_data("Ups_mass_cut_data", "Ups_mass_cut_data", RooArgList(Ups_mass_cut_var));
    RooDataSet Pri_mass_cut_data("Pri_mass_cut_data", "Pri_mass_cut_data", RooArgList(Pri_mass_cut_var));


    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;
        // if (Cut(ientry) < 0) continue;

        // Initialize the total set of candidates.
        std::vector<std::shared_ptr<ParticleCand> > CandList_cut;
        ParticleCand tempCand;
        ParticleCand::PartIdxList_t tempList;
        double temp_massChi2;

        const double vtxProb_Thr = 0.05;

        // Marker
        #ifdef SHOW_DEBUG
        printf("\n>>>>> Begin new event %lld <<<<<\n", jentry);
        #endif

        // Display event number once 500 events.
        if(jentry % 500 == 0){
            printf("Processing event %lld\n", jentry);
        }
        // Loop over all candidates.
        size_t nCands = Jpsi_1_mass->size();

        if(nCands == 0) continue;

        // Calculate muon pT, eta, phi from px, py and pz.
        std::vector<double> mu_pT;
        std::vector<double> mu_eta;
        std::vector<double> mu_phi;
        
        for(Long64_t iMuon=0; iMuon < nMu; iMuon++){
            // Calculate pT, eta, phi from px, py and pz.
            double pT  = sqrt(muPx->at(iMuon) * muPx->at(iMuon) + muPy->at(iMuon) * muPy->at(iMuon));
            double eta = 0.5 * log((pT + muPz->at(iMuon)) / (pT - muPz->at(iMuon)));
            double phi = atan2(muPy->at(iMuon), muPx->at(iMuon));
            mu_pT.push_back(pT);
            mu_eta.push_back(eta);
            mu_phi.push_back(phi);
        }

        #ifdef DRAW_RAW

        std::vector<std::shared_ptr<ParticleCand> > CandList_raw;

        // Loop exclusively for raw candidates.
        for (Long64_t iCand=0; iCand < nCands; iCand++) {
            // Exclude failed fitting.
            if(Pri_mass->at(iCand) <= 0.0) continue;
            // Register the candidate final state muons.
            tempList.push_back(Jpsi_1_mu_1_Idx->at(iCand));
            tempList.push_back(Jpsi_1_mu_2_Idx->at(iCand));
            tempList.push_back(Jpsi_2_mu_1_Idx->at(iCand));
            tempList.push_back(Jpsi_2_mu_2_Idx->at(iCand));
            tempList.push_back(Ups_mu_1_Idx->at(iCand));
            tempList.push_back(Ups_mu_2_Idx->at(iCand));
            tempCand.AddParticle(ParticleCand::PartType::Muon, tempList);
            // Prevent underflow or overflow of masses.
            if(Jpsi_1_mass->at(iCand) < 2.5 || Jpsi_1_mass->at(iCand) > 3.5){
                continue;
            }
            if(Jpsi_2_mass->at(iCand) < 2.5 || Jpsi_2_mass->at(iCand) > 3.5){
                continue;
            }
            if(Ups_mass->at(iCand) < 8.0 || Ups_mass->at(iCand) > 12.0){
                continue;
            }
            if(Pri_mass->at(iCand) < 0.0 || Pri_mass->at(iCand) > 100.0){
                continue;
            }
            // Calculate Chi2 from massDiff and massErr of Jpsi and Ups.
            temp_massChi2 =   (Jpsi_1_massDiff->at(iCand) / Jpsi_1_massErr->at(iCand))
                            * (Jpsi_1_massDiff->at(iCand) / Jpsi_1_massErr->at(iCand))
                            + (Jpsi_2_massDiff->at(iCand) / Jpsi_2_massErr->at(iCand))
                            * (Jpsi_2_massDiff->at(iCand) / Jpsi_2_massErr->at(iCand))
                            + (Ups_massDiff->at(iCand) / Ups_massErr->at(iCand))
                            * (Ups_massDiff->at(iCand) / Ups_massErr->at(iCand));
            tempCand.SetScore(temp_massChi2);
            tempCand.SetId(iCand);
            CandList_raw.push_back(std::make_shared<ParticleCand>(tempCand));
            tempList.clear();
            tempCand.Clear();
        }

        // Sort the raw candidates by the score.
        std::sort(CandList_raw.begin(), CandList_raw.end(), [](const std::shared_ptr<ParticleCand>& a, 
                                                               const std::shared_ptr<ParticleCand>& b){
            return a->GetScore() < b->GetScore();
        });

        #endif

        // Loop exclusively for candidates passing the cut.
        for (Long64_t iCand=0; iCand < nCands; iCand++) {
            // Exclude failed fitting.
            if(Pri_mass->at(iCand) <= 0.0) continue;
            // Further cuts are applied here.
            bool passCut = true;

            // Register the candidate final state muons.
            tempList.push_back(Jpsi_1_mu_1_Idx->at(iCand));
            tempList.push_back(Jpsi_1_mu_2_Idx->at(iCand));
            tempList.push_back(Jpsi_2_mu_1_Idx->at(iCand));
            tempList.push_back(Jpsi_2_mu_2_Idx->at(iCand));
            tempList.push_back(Ups_mu_1_Idx->at(iCand));
            tempList.push_back(Ups_mu_2_Idx->at(iCand));
            tempCand.AddParticle(ParticleCand::PartType::Muon, tempList);

            #ifdef CUT_MUON_ID_LOOSE
            // Check by muIsPatLooseMuon
            for(auto idx : tempList){
                if(!muIsPatLooseMuon->at(idx)){
                    passCut = false;
                    break;
                }
            }

            #endif

            #ifdef CUT_MUON_ID_SOFT
            // Check by muIsPatSoftMuon
            for(auto idx : tempList){
                if(!muIsPatSoftMuon->at(idx)){
                    passCut = false;
                    break;
                }
            }
            
            #ifdef CUT_UPS_MUON_MEDIUM
            if(!muIsPatMediumMuon->at(Ups_mu_1_Idx->at(iCand))){
                passCut = false;
            }
            if(!muIsPatMediumMuon->at(Ups_mu_2_Idx->at(iCand))){
                passCut = false;
	        }
            #endif

            #endif

            if(!passCut){
                continue;
            }
            
            // Prevent underflow or overflow of masses.
            if(Jpsi_1_mass->at(iCand) < 2.5 || Jpsi_1_mass->at(iCand) > 3.5){
                continue;
            }
            if(Jpsi_2_mass->at(iCand) < 2.5 || Jpsi_2_mass->at(iCand) > 3.5){
                continue;
            }
            if(Ups_mass->at(iCand) < 8.0 || Ups_mass->at(iCand) > 12.0){
                continue;
            }
            // Cut from 3J
            // Muons that decay from Jpsi and Ups:
            // require pT > 3.5GeV/c for abs(eta) < 1.2 and pT > 2.5GeV/c for 1.2 < abs(eta) < 2.4
            for(auto idx : tempList){
                if(mu_pT[idx] < 3.5 && abs(mu_eta[idx]) < 1.2){
                    passCut = false;
                    break;
                }
                if(mu_pT[idx] < 2.5 && abs(mu_eta[idx]) > 1.2 && abs(mu_eta[idx]) < 2.4){
                    passCut = false;
                    break;
                }
                if(abs(mu_eta[idx]) > 2.4){
                    passCut = false;
                    break;
                }
            }

            if(!passCut){
                continue;
            }

            #ifdef CUT_UPS
            // For Upsilon: try a loose cut. pT > 6GeV/c and abs(eta) < 2.4
            if(Ups_pt->at(iCand) <= 6.0 || abs(Ups_eta->at(iCand)) > 2.4){
                continue;
            }
            #endif

            // For Jpsi: require pT > 6GeV/c and abs(eta) < 2.4
            if(Jpsi_1_pt->at(iCand) <= 6.0 || abs(Jpsi_1_eta->at(iCand)) > 2.4){
                continue; 
            }
            if(Jpsi_2_pt->at(iCand) <= 6.0 || abs(Jpsi_2_eta->at(iCand)) > 2.4){
                continue;
            }

            #ifdef CUT_UPS_MUON_PT
            // Additional cut for muons from Ups: pT > 4 GeV/c
            if(mu_pT[Ups_mu_1_Idx->at(iCand)] <= 4.0 || mu_pT[Ups_mu_2_Idx->at(iCand)] <= 4.0){
                continue;
            }
            #endif

            // Calculate Chi2 from massDiff and massErr of Jpsi and Ups.
            temp_massChi2 =   (Jpsi_1_massDiff->at(iCand) / Jpsi_1_massErr->at(iCand))
                            * (Jpsi_1_massDiff->at(iCand) / Jpsi_1_massErr->at(iCand))
                            + (Jpsi_2_massDiff->at(iCand) / Jpsi_2_massErr->at(iCand))
                            * (Jpsi_2_massDiff->at(iCand) / Jpsi_2_massErr->at(iCand))
                            + (Ups_massDiff->at(iCand) / Ups_massErr->at(iCand))
                            * (Ups_massDiff->at(iCand) / Ups_massErr->at(iCand));
            tempCand.SetScore(temp_massChi2);
            tempCand.SetId(iCand);
            CandList_cut.push_back(std::make_shared<ParticleCand>(tempCand));
            tempList.clear();
            tempCand.Clear();
        }
        // Sort all candidates for later use.
        std::sort(CandList_cut.begin(), CandList_cut.end(), [](const std::shared_ptr<ParticleCand>& a, 
                                                               const std::shared_ptr<ParticleCand>& b){
            return a->GetScore() < b->GetScore();
        });

        #ifdef ALLOW_OVERLAP
        // Allow overlapping candidates.
        // Add the top-10 candidates to the RooDataSet directly.
        for(size_t iCand=0; iCand < CandList_cut.size(); iCand++){
            if(iCand >= nCandsAllowed){
                break;
            }
            // Set the mass.
            Jpsi1_mass_cut_var.setVal(Jpsi_1_mass->at(CandList_cut[iCand]->GetId()));
            Jpsi2_mass_cut_var.setVal(Jpsi_2_mass->at(CandList_cut[iCand]->GetId()));
            Ups_mass_cut_var.setVal(Ups_mass->at(CandList_cut[iCand]->GetId()));
            Pri_mass_cut_var.setVal(Pri_mass->at(CandList_cut[iCand]->GetId()));
            // Set the mass error.
            Jpsi1_mass_cut_var.setError(Jpsi_1_massErr->at(CandList_cut[iCand]->GetId()));
            Jpsi2_mass_cut_var.setError(Jpsi_2_massErr->at(CandList_cut[iCand]->GetId()));
            Ups_mass_cut_var.setError(Ups_massErr->at(CandList_cut[iCand]->GetId()));
            Pri_mass_cut_var.setError(Pri_massErr->at(CandList_cut[iCand]->GetId()));
            // Add the data to the dataset.
            Jpsi1_mass_cut_data.add(RooArgSet(Jpsi1_mass_cut_var));
            Jpsi2_mass_cut_data.add(RooArgSet(Jpsi2_mass_cut_var));
            Ups_mass_cut_data.add(RooArgSet(Ups_mass_cut_var));
            Pri_mass_cut_data.add(RooArgSet(Pri_mass_cut_var));
        }

        #ifdef DRAW_RAW
        // Top-10 candidates are added to the RooDataSet directly.
        for(size_t iCand=0; iCand < CandList_raw.size(); iCand++){
            if(iCand >= nCandsAllowed){
                break;
            }
            // Set the mass.
            Jpsi1_mass_var.setVal(Jpsi_1_mass->at(CandList_raw[iCand]->GetId()));
            Jpsi2_mass_var.setVal(Jpsi_2_mass->at(CandList_raw[iCand]->GetId()));
            Ups_mass_var.setVal(Ups_mass->at(CandList_raw[iCand]->GetId()));
            Pri_mass_var.setVal(Pri_mass->at(CandList_raw[iCand]->GetId()));
            // Set the mass error.
            Jpsi1_mass_var.setError(Jpsi_1_massErr->at(CandList_raw[iCand]->GetId()));
            Jpsi2_mass_var.setError(Jpsi_2_massErr->at(CandList_raw[iCand]->GetId()));
            Ups_mass_var.setError(Ups_massErr->at(CandList_raw[iCand]->GetId()));
            Pri_mass_var.setError(Pri_massErr->at(CandList_raw[iCand]->GetId()));
            // Add the data to the dataset.
            Jpsi1_mass_data.add(RooArgSet(Jpsi1_mass_var));
            Jpsi2_mass_data.add(RooArgSet(Jpsi2_mass_var));
            Ups_mass_data.add(RooArgSet(Ups_mass_var));
            Pri_mass_data.add(RooArgSet(Pri_mass_var));
        }
        #endif


        #else
        // Greedy algorithm to find a non-overlapping combination.
        #ifdef DRAW_RAW
        std::vector<std::shared_ptr<ParticleCand> > SelectedCands_raw;
        for(auto& cand : CandList_raw){
            if(SelectedCands_raw.empty()){
                SelectedCands_raw.push_back(cand);
            }
            else{
                bool isOverlap = false;
                for(auto& selCand : SelectedCands_raw){
                    if(cand->Overlap(*selCand)){
                        isOverlap = true;
                        break;
                    }
                }
                if(!isOverlap){
                    SelectedCands_raw.push_back(cand);
                }
            }
        }
        #endif


        std::vector<std::shared_ptr<ParticleCand> > SelectedCands_cut;
        for(auto& cand : CandList_cut){
            if(SelectedCands_cut.empty()){
                SelectedCands_cut.push_back(cand);
            }
            else{
                bool isOverlap = false;
                for(auto& selCand : SelectedCands_cut){
                    if(cand->Overlap(*selCand)){
                        isOverlap = true;
                        break;
                    }
                }
                if(!isOverlap){
                    SelectedCands_cut.push_back(cand);
                }
            }
        }

        #ifdef DRAW_RAW
        // Draw the selected candidates. Fill the dataset.
        for(auto& cand : SelectedCands_raw){
            // Set the mass.
            Jpsi1_mass_var.setVal(Jpsi_1_mass->at(cand->GetId()));
            Jpsi2_mass_var.setVal(Jpsi_2_mass->at(cand->GetId()));
            Ups_mass_var.setVal(Ups_mass->at(cand->GetId()));
            Pri_mass_var.setVal(Pri_mass->at(cand->GetId()));
            // Set the mass error.
            Jpsi1_mass_var.setError(Jpsi_1_massErr->at(cand->GetId()));
            Jpsi2_mass_var.setError(Jpsi_2_massErr->at(cand->GetId()));
            Ups_mass_var.setError(Ups_massErr->at(cand->GetId()));
            Pri_mass_var.setError(Pri_massErr->at(cand->GetId()));
            // Add the data to the dataset.
            Jpsi1_mass_data.add(RooArgSet(Jpsi1_mass_var));
            Jpsi2_mass_data.add(RooArgSet(Jpsi2_mass_var));
            Ups_mass_data.add(RooArgSet(Ups_mass_var));
            Pri_mass_data.add(RooArgSet(Pri_mass_var));
        }
        #endif

        // Draw the selected candidates passing the cut.
        for(auto& cand : SelectedCands_cut){
            // Set the mass.
            Jpsi1_mass_cut_var.setVal(Jpsi_1_mass->at(cand->GetId()));
            Jpsi2_mass_cut_var.setVal(Jpsi_2_mass->at(cand->GetId()));
            Ups_mass_cut_var.setVal(Ups_mass->at(cand->GetId()));
            Pri_mass_cut_var.setVal(Pri_mass->at(cand->GetId()));
            // Set the mass error.
            Jpsi1_mass_cut_var.setError(Jpsi_1_massErr->at(cand->GetId()));
            Jpsi2_mass_cut_var.setError(Jpsi_2_massErr->at(cand->GetId()));
            Ups_mass_cut_var.setError(Ups_massErr->at(cand->GetId()));
            Pri_mass_cut_var.setError(Pri_massErr->at(cand->GetId()));
            // Add the data to the dataset.
            Jpsi1_mass_cut_data.add(RooArgSet(Jpsi1_mass_cut_var));
            Jpsi2_mass_cut_data.add(RooArgSet(Jpsi2_mass_cut_var));
            Ups_mass_cut_data.add(RooArgSet(Ups_mass_cut_var));
            Pri_mass_cut_data.add(RooArgSet(Pri_mass_cut_var));
        }

        #endif
    }
    // Display the histograms.
    #ifdef DRAW_RAW
    // Mass histograms are drawn using Roofit.
    TCanvas* c1 = new TCanvas("c1", "c1", 1600, 1200);
    c1->Divide(2,2);
    RooPlot* frame1 = Jpsi1_mass_var.frame();
    RooPlot* frame2 = Jpsi2_mass_var.frame();
    RooPlot* frame3 = Ups_mass_var.frame();
    RooPlot* frame4 = Pri_mass_var.frame();
    Jpsi1_mass_data.plotOn(frame1);
    Jpsi2_mass_data.plotOn(frame2);
    Ups_mass_data.plotOn(frame3);
    Pri_mass_data.plotOn(frame4);
    // Draw the histograms.
    c1->cd(1); frame1->Draw();
    c1->cd(2); frame2->Draw();
    c1->cd(3); frame3->Draw();
    c1->cd(4); frame4->Draw();
    c1->SaveAs("mass_raw.png");
    #endif
    // Mass histograms passing the cut are drawn using Roofit.
    TCanvas* c2 = new TCanvas("c2", "c2", 1600, 1200);
    c2->Divide(2,2);
    RooPlot* frame5 = Jpsi1_mass_cut_var.frame();
    RooPlot* frame6 = Jpsi2_mass_cut_var.frame();
    RooPlot* frame7 = Ups_mass_cut_var.frame();
    RooPlot* frame8 = Pri_mass_cut_var.frame();
    Jpsi1_mass_cut_data.plotOn(frame5);
    Jpsi2_mass_cut_data.plotOn(frame6);
    Ups_mass_cut_data.plotOn(frame7);
    Pri_mass_cut_data.plotOn(frame8);
    // Draw the histograms.
    c2->cd(1); frame5->Draw();
    c2->cd(2); frame6->Draw();
    c2->cd(3); frame7->Draw();
    c2->cd(4); frame8->Draw();
    // Save png file.
    c2->SaveAs("mass_cut.png");

    // Save cut candidates to a root file.
    TFile* f = new TFile("JOB_DIR/Candidates.root", "RECREATE");
    #ifdef DRAW_RAW
    Jpsi1_mass_data.Write();
    Jpsi2_mass_data.Write();
    Ups_mass_data.Write();
    Pri_mass_data.Write();
    #endif
    Jpsi1_mass_cut_data.Write();
    Jpsi2_mass_cut_data.Write();
    Ups_mass_cut_data.Write();
    Pri_mass_cut_data.Write();
    f->Close();
}

