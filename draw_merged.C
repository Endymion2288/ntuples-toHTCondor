/****
 * Plotting the distribution of a few RooDataSet from merged_dataset.root .
 * Uses the RooPlot class to plot a histogram of the data.
 * The datasets are Jpsi1_mass_cut_data, Jpsi2_mass_cut_data, Ups_mass_cut_data, and Pri_mass_cut_data.
 * the elements of the datasets are the mass variable and the error on the mass variable.
*/

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

#include "TFile.h"
#include "TCanvas.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooPlot.h"

using namespace RooFit;

void draw_merged(){
    // Open the file
    TFile *file = TFile::Open("merged_dataset.root");
    if (!file) {
        std::cerr << "File not found." << std::endl;
        return;
    }

    // Get the datasets
    RooDataSet *Jpsi1_mass_cut_data = (RooDataSet*)file->Get("Jpsi1_mass_cut_data");
    RooDataSet *Jpsi2_mass_cut_data = (RooDataSet*)file->Get("Jpsi2_mass_cut_data");
    RooDataSet *Ups_mass_cut_data = (RooDataSet*)file->Get("Ups_mass_cut_data");
    RooDataSet *Pri_mass_cut_data = (RooDataSet*)file->Get("Pri_mass_cut_data");

    // Plot the data
    RooRealVar Jpsi1_mass_cut_var("Jpsi1_mass_cut", "Jpsi1_mass_cut", 2.5, 3.5);
    RooRealVar Jpsi2_mass_cut_var("Jpsi2_mass_cut", "Jpsi2_mass_cut", 2.5, 3.5);
    RooRealVar Ups_mass_cut_var("Ups_mass_cut","Ups_mass_cut", 8.0, 12.0);
    RooRealVar Pri_mass_cut_var("Pri_mass_cut","Pri_mass_cut", 0.0, 100.0);

    // Create the frame
    TCanvas *c = new TCanvas("c", "c", 1600, 1200);
    c->Divide(2, 2);
    RooPlot *frame1 = Jpsi1_mass_cut_var.frame(Title("Jpsi1_mass_cut"));
    RooPlot *frame2 = Jpsi2_mass_cut_var.frame(Title("Jpsi2_mass_cut"));
    RooPlot *frame3 = Ups_mass_cut_var.frame(Title("Ups_mass_cut"));
    RooPlot *frame4 = Pri_mass_cut_var.frame(Title("Pri_mass_cut"));

    // Plot the data
    Jpsi1_mass_cut_data->plotOn(frame1);
    Jpsi2_mass_cut_data->plotOn(frame2);
    Ups_mass_cut_data->plotOn(frame3);
    Pri_mass_cut_data->plotOn(frame4);

    // Draw the frame
    c->cd(1);
    frame1->Draw();
    c->cd(2);
    frame2->Draw();
    c->cd(3);
    frame3->Draw();
    c->cd(4);
    frame4->Draw();

    // Save the plot
    c->SaveAs("merged_dataset.png");

    // print out explicitly all mass values and their errors in the datasets
    // print one Jpsi+Jpsi+Upsilon candidate by one candidate
    printf("Jpsi1_mass_cut   \t Jpsi2_mass_cut   \t Ups_mass_cut   \t Pri_mass_cut\n");
    for (int i = 0; i < Jpsi1_mass_cut_data->numEntries(); i++){
        printf("%.3f +/- %.3f   \t %.3f +/- %.3f   \t %.3f +/- %.3f   \t %.3f +/- %.3f\n",
            Jpsi1_mass_cut_data->get(i)->getRealValue("Jpsi1_mass_cut"),
            Jpsi1_mass_cut_data->get(i)->getRealValue("Jpsi1_mass_cut")->getPropagatedError(),
            Jpsi2_mass_cut_data->get(i)->getRealValue("Jpsi2_mass_cut"),
            Jpsi2_mass_cut_data->get(i)->getRealValue("Jpsi2_mass_cut_error"),
            Ups_mass_cut_data->get(i)->getRealValue("Ups_mass_cut"),
            Ups_mass_cut_data->get(i)->getRealValue("Ups_mass_cut_error"),
            Pri_mass_cut_data->get(i)->getRealValue("Pri_mass_cut"),
            Pri_mass_cut_data->get(i)->getRealValue("Pri_mass_cut_error")
        );
    }

    

    // Close the file
    file->Close();
    delete file;

}

