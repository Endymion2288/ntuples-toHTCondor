/****
 * Plotting the distribution of a few RooDataSet from merged_dataset.root .
 * Uses the RooPlot class to plot a histogram of the data.
 * The datasets are Jpsi_1_mass_cut_data, Jpsi_2_mass_cut_data, Ups_mass_cut_data, and Pri_mass_cut_data.
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

#define SAVE_RAW

using namespace RooFit;

void draw_merged(){
    // Open the file
    TFile *file = TFile::Open("merged_dataset.root");
    if (!file) {
        std::cerr << "File not found." << std::endl;
        return;
    }

    // Get the datasets
    RooDataSet *Jpsi_1_mass_cut_data = (RooDataSet*)file->Get("Jpsi_1_mass_cut_data");
    RooDataSet *Jpsi_2_mass_cut_data = (RooDataSet*)file->Get("Jpsi_2_mass_cut_data");
    RooDataSet *Ups_mass_cut_data = (RooDataSet*)file->Get("Ups_mass_cut_data");
    RooDataSet *Pri_mass_cut_data = (RooDataSet*)file->Get("Pri_mass_cut_data");
    // Along with the error on the mass variable
    RooDataSet *Jpsi_1_massErr_cut_data = (RooDataSet*)file->Get("Jpsi_1_massErr_cut_data");
    RooDataSet *Jpsi_2_massErr_cut_data = (RooDataSet*)file->Get("Jpsi_2_massErr_cut_data");
    RooDataSet *Ups_massErr_cut_data = (RooDataSet*)file->Get("Ups_massErr_cut_data");
    RooDataSet *Pri_massErr_cut_data = (RooDataSet*)file->Get("Pri_massErr_cut_data");

    // Raw datasets if needed
    #ifdef SAVE_RAW
    RooDataSet *Jpsi_1_mass_data = (RooDataSet*)file->Get("Jpsi_1_mass_data");
    RooDataSet *Jpsi_2_mass_data = (RooDataSet*)file->Get("Jpsi_2_mass_data");
    RooDataSet *Ups_mass_data = (RooDataSet*)file->Get("Ups_mass_data");
    RooDataSet *Pri_mass_data = (RooDataSet*)file->Get("Pri_mass_data");

    RooDataSet *Jpsi_1_massErr_data = (RooDataSet*)file->Get("Jpsi_1_massErr_data");
    RooDataSet *Jpsi_2_massErr_data = (RooDataSet*)file->Get("Jpsi_2_massErr_data");
    RooDataSet *Ups_massErr_data = (RooDataSet*)file->Get("Ups_massErr_data");
    RooDataSet *Pri_massErr_data = (RooDataSet*)file->Get("Pri_massErr_data");
    #endif

    // Plot the data
    RooRealVar Jpsi_1_mass_cut_var("Jpsi_1_mass_cut", "Jpsi_1_mass_cut", 2.5, 3.5);
    RooRealVar Jpsi_2_mass_cut_var("Jpsi_2_mass_cut", "Jpsi_2_mass_cut", 2.5, 3.5);
    RooRealVar Ups_mass_cut_var("Ups_mass_cut","Ups_mass_cut", 8.0, 12.0);
    RooRealVar Pri_mass_cut_var("Pri_mass_cut","Pri_mass_cut", 0.0, 100.0);

    // Raw variables if needed
    #ifdef SAVE_RAW
    RooRealVar Jpsi_1_mass_var("Jpsi_1_mass", "Jpsi_1_mass", 2.5, 3.5);
    RooRealVar Jpsi_2_mass_var("Jpsi_2_mass", "Jpsi_2_mass", 2.5, 3.5);
    RooRealVar Ups_mass_var("Ups_mass","Ups_mass", 8.0, 12.0);
    RooRealVar Pri_mass_var("Pri_mass","Pri_mass", 0.0, 100.0);
    #endif

    // Create a new dataset containing the mass variable and the error on the mass variable
    RooDataSet Jpsi_1_mass_with_Err_cut("Jpsi_1_mass_with_Err_cut", "Jpsi_1_mass_with_Err_cut", RooArgSet(Jpsi_1_mass_cut_var));
    RooDataSet Jpsi_2_mass_with_Err_cut("Jpsi_2_mass_with_Err_cut", "Jpsi_2_mass_with_Err_cut", RooArgSet(Jpsi_2_mass_cut_var));
    RooDataSet Ups_mass_with_Err_cut("Ups_mass_with_Err_cut", "Ups_mass_with_Err_cut", RooArgSet(Ups_mass_cut_var));
    RooDataSet Pri_mass_with_Err_cut("Pri_mass_with_Err_cut", "Pri_mass_with_Err_cut", RooArgSet(Pri_mass_cut_var));

    // Raw datasets if needed
    #ifdef SAVE_RAW
    RooDataSet Jpsi_1_mass_with_Err("Jpsi_1_mass_with_Err", "Jpsi_1_mass_with_Err", RooArgSet(Jpsi_1_mass_var));
    RooDataSet Jpsi_2_mass_with_Err("Jpsi_2_mass_with_Err", "Jpsi_2_mass_with_Err", RooArgSet(Jpsi_2_mass_var));
    RooDataSet Ups_mass_with_Err("Ups_mass_with_Err", "Ups_mass_with_Err", RooArgSet(Ups_mass_var));
    RooDataSet Pri_mass_with_Err("Pri_mass_with_Err", "Pri_mass_with_Err", RooArgSet(Pri_mass_var));
    #endif

    // Fill the datasets
    for (int i = 0; i < Jpsi_1_mass_data->numEntries(); i++){
        Jpsi_1_mass_cut_var.setVal(Jpsi_1_mass_data->get(i)->getRealValue("Jpsi_1_mass_cut"));
        Jpsi_2_mass_cut_var.setVal(Jpsi_2_mass_data->get(i)->getRealValue("Jpsi_2_mass_cut"));
        Ups_mass_cut_var.setVal(Ups_mass_data->get(i)->getRealValue("Ups_mass_cut"));
        Pri_mass_cut_var.setVal(Pri_mass_data->get(i)->getRealValue("Pri_mass_cut"));
        Jpsi_1_mass_cut_var.setError(Jpsi_1_massErr_data->get(i)->getRealValue("Jpsi_1_massErr"));
        Jpsi_2_mass_cut_var.setError(Jpsi_2_massErr_data->get(i)->getRealValue("Jpsi_2_massErr"));
        Ups_mass_cut_var.setError(Ups_massErr_data->get(i)->getRealValue("Ups_massErr"));
        Pri_mass_cut_var.setError(Pri_massErr_data->get(i)->getRealValue("Pri_massErr"));
        Jpsi_1_mass_with_Err.add(RooArgSet(Jpsi_1_mass_var));
        Jpsi_2_mass_with_Err.add(RooArgSet(Jpsi_2_mass_var));
        Ups_mass_with_Err.add(RooArgSet(Ups_mass_var));
        Pri_mass_with_Err.add(RooArgSet(Pri_mass_var));
    }

    // Raw datasets if needed
    #ifdef SAVE_RAW
    // Fill the datasets
    for (int i = 0; i < Jpsi_1_mass_data->numEntries(); i++){
        Jpsi_1_mass_var.setVal(Jpsi_1_mass_data->get(i)->getRealValue("Jpsi_1_mass"));
        Jpsi_2_mass_var.setVal(Jpsi_2_mass_data->get(i)->getRealValue("Jpsi_2_mass"));
        Ups_mass_var.setVal(Ups_mass_data->get(i)->getRealValue("Ups_mass"));
        Pri_mass_var.setVal(Pri_mass_data->get(i)->getRealValue("Pri_mass"));
        Jpsi_1_mass_var.setError(Jpsi_1_massErr_data->get(i)->getRealValue("Jpsi_1_massErr"));
        Jpsi_2_mass_var.setError(Jpsi_2_massErr_data->get(i)->getRealValue("Jpsi_2_massErr"));
        Ups_mass_var.setError(Ups_massErr_data->get(i)->getRealValue("Ups_massErr"));
        Pri_mass_var.setError(Pri_massErr_data->get(i)->getRealValue("Pri_massErr"));
        Jpsi_1_mass_with_Err.add(RooArgSet(Jpsi_1_mass_var));
        Jpsi_2_mass_with_Err.add(RooArgSet(Jpsi_2_mass_var));
        Ups_mass_with_Err.add(RooArgSet(Ups_mass_var));
        Pri_mass_with_Err.add(RooArgSet(Pri_mass_var));
    }
    #endif


    // Create the frame
    TCanvas *c = new TCanvas("c", "c", 1600, 1200);
    c->Divide(2, 2);
    RooPlot *frame1 = Jpsi_1_mass_cut_var.frame(Title("Jpsi_1_mass_cut"));
    RooPlot *frame2 = Jpsi_2_mass_cut_var.frame(Title("Jpsi_2_mass_cut"));
    RooPlot *frame3 = Ups_mass_cut_var.frame(Title("Ups_mass_cut"));
    RooPlot *frame4 = Pri_mass_cut_var.frame(Title("Pri_mass_cut"));

    // Plot the data
    Jpsi_1_mass_cut_data->plotOn(frame1);
    Jpsi_2_mass_cut_data->plotOn(frame2);
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

    // Draw for the raw datasets if needed
    #ifdef SAVE_RAW
    // Create the frame
    TCanvas *c_raw = new TCanvas("c_raw", "c_raw", 1600, 1200);
    c_raw->Divide(2, 2);
    RooPlot *frame5 = Jpsi_1_mass_var.frame(Title("Jpsi_1_mass"));
    RooPlot *frame6 = Jpsi_2_mass_var.frame(Title("Jpsi_2_mass"));
    RooPlot *frame7 = Ups_mass_var.frame(Title("Ups_mass"));
    RooPlot *frame8 = Pri_mass_var.frame(Title("Pri_mass"));
    // Plot the data
    Jpsi_1_mass_data->plotOn(frame5);
    Jpsi_2_mass_data->plotOn(frame6);
    Ups_mass_data->plotOn(frame7);
    Pri_mass_data->plotOn(frame8);
    // Draw the frame
    c_raw->cd(1);
    frame5->Draw();
    c_raw->cd(2);
    frame6->Draw();
    c_raw->cd(3);
    frame7->Draw();
    c_raw->cd(4);
    frame8->Draw();
    // Save the plot
    c_raw->SaveAs("merged_dataset_raw.png");
    #endif

    // print out explicitly all mass values and their errors in the datasets
    // print one Jpsi+Jpsi+Upsilon candidate by one candidate
    printf("Jpsi_1_mass_cut   \t Jpsi_2_mass_cut   \t Ups_mass_cut   \t Pri_mass_cut\n");
    for (int i = 0; i < Jpsi_1_mass_cut_data->numEntries(); i++){
        printf("%.3f +/- %.3f   \t %.3f +/- %.3f   \t %.3f +/- %.3f   \t %.3f +/- %.3f\n",
            Jpsi_1_mass_cut_data->get(i)->getRealValue("Jpsi_1_mass_cut"),
            Jpsi_1_massErr_cut_data->get(i)->getRealValue("Jpsi_1_massErr"),
            Jpsi_2_mass_cut_data->get(i)->getRealValue("Jpsi_2_mass_cut"),
            Jpsi_2_massErr_cut_data->get(i)->getRealValue("Jpsi_2_massErr"),
            Ups_mass_cut_data->get(i)->getRealValue("Ups_mass_cut"),
            Ups_massErr_cut_data->get(i)->getRealValue("Ups_massErr"),
            Pri_mass_cut_data->get(i)->getRealValue("Pri_mass_cut"),
            Pri_massErr_cut_data->get(i)->getRealValue("Pri_massErr"));
    }

    

    // Close the file
    file->Close();
    delete file;

}

