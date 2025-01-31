#include "TFile.h"
#include "RooArgSet.h"
#include "RooDataSet.h"
#include <fstream>
#include <vector>
#include <string>

#define SAVE_RAW

void merge() {
    // Read in the list of input files
    const char* fileListFileName = "all_cands_list.txt";
    const char* outputFileName = "merged_dataset.root";
    const char* outputFileName_raw = "merged_dataset_raw.root";
    std::vector<std::string> inputFileNames;
    std::ifstream fileList(fileListFileName);
    std::string line;
    while (std::getline(fileList, line)) {
        inputFileNames.push_back(line);
    }
    fileList.close();

    // RooDatSet to read and merge: Jpsi_1_mass_cut_data, Jpsi_2_mass_cut_data, Ups_mass_cut_data, Pri_mass_cut_data
    // Also the errors: Jpsi_1_massErr_cut_data, Jpsi_2_massErr_cut_data, Ups_massErr_cut_data, Pri_massErr_cut_data
    // If SAVE_RAW is defined, also read and merge: Jpsi_1_mass_data, Jpsi_2_mass_data, Ups_mass_data, Pri_mass_data
    // Also the errors: Jpsi_1_massErr_data, Jpsi_2_massErr_data, Ups_massErr_data, Pri_massErr_data

    RooArgSet* argSet_Jpsi_1 = nullptr;
    RooArgSet* argSet_Jpsi_2 = nullptr;
    RooArgSet* argSet_Ups = nullptr;
    RooArgSet* argSet_Pri = nullptr;

    RooArgSet* argSet_Jpsi_1_err = nullptr;
    RooArgSet* argSet_Jpsi_2_err = nullptr;
    RooArgSet* argSet_Ups_err = nullptr;
    RooArgSet* argSet_Pri_err = nullptr;

    #ifdef SAVE_RAW
    RooArgSet* argSet_Jpsi_1_raw = nullptr;
    RooArgSet* argSet_Jpsi_2_raw = nullptr;
    RooArgSet* argSet_Ups_raw = nullptr;
    RooArgSet* argSet_Pri_raw = nullptr;

    RooArgSet* argSet_Jpsi_1_err_raw = nullptr;
    RooArgSet* argSet_Jpsi_2_err_raw = nullptr;
    RooArgSet* argSet_Ups_err_raw = nullptr;
    RooArgSet* argSet_Pri_err_raw = nullptr;
    #endif

    RooDataSet* merged_Jpsi_1 = nullptr;
    RooDataSet* merged_Jpsi_2 = nullptr;
    RooDataSet* merged_Ups = nullptr;
    RooDataSet* merged_Pri = nullptr;

    RooDataSet* merged_Jpsi_1_err = nullptr;
    RooDataSet* merged_Jpsi_2_err = nullptr;
    RooDataSet* merged_Ups_err = nullptr;
    RooDataSet* merged_Pri_err = nullptr;

    #ifdef SAVE_RAW
    RooDataSet* merged_Jpsi_1_raw = nullptr;
    RooDataSet* merged_Jpsi_2_raw = nullptr;
    RooDataSet* merged_Ups_raw = nullptr;
    RooDataSet* merged_Pri_raw = nullptr;

    RooDataSet* merged_Jpsi_1_err_raw = nullptr;
    RooDataSet* merged_Jpsi_2_err_raw = nullptr;
    RooDataSet* merged_Ups_err_raw = nullptr;
    RooDataSet* merged_Pri_err_raw = nullptr;
    #endif

    // Iterate over all input files
    for (const auto& inputFileName : inputFileNames) {
        // Open the file
        TFile* file = new TFile(inputFileName.c_str(), "READ");
        if (!file || file->IsZombie()) {
            std::cerr << "Error opening file " << inputFileName << std::endl;
            continue;
        }

        // Read the RooDataSet from the file
        RooDataSet* Jpsi_1_dataSet = (RooDataSet*)file->Get("Jpsi_1_mass_cut_data");
        RooDataSet* Jpsi_2_dataSet = (RooDataSet*)file->Get("Jpsi_2_mass_cut_data");
        RooDataSet* Ups_dataSet = (RooDataSet*)file->Get("Ups_mass_cut_data");
        RooDataSet* Pri_dataSet = (RooDataSet*)file->Get("Pri_mass_cut_data");

        RooDataSet* Jpsi_1_dataSet_err = (RooDataSet*)file->Get("Jpsi_1_massErr_cut_data");
        RooDataSet* Jpsi_2_dataSet_err = (RooDataSet*)file->Get("Jpsi_2_massErr_cut_data");
        RooDataSet* Ups_dataSet_err = (RooDataSet*)file->Get("Ups_massErr_cut_data");
        RooDataSet* Pri_dataSet_err = (RooDataSet*)file->Get("Pri_massErr_cut_data");

        #ifdef SAVE_RAW
        RooDataSet* Jpsi_1_dataSet_raw = (RooDataSet*)file->Get("Jpsi_1_mass_data");
        RooDataSet* Jpsi_2_dataSet_raw = (RooDataSet*)file->Get("Jpsi_2_mass_data");
        RooDataSet* Ups_dataSet_raw = (RooDataSet*)file->Get("Ups_mass_data");
        RooDataSet* Pri_dataSet_raw = (RooDataSet*)file->Get("Pri_mass_data");

        RooDataSet* Jpsi_1_dataSet_err_raw = (RooDataSet*)file->Get("Jpsi_1_massErr_data");
        RooDataSet* Jpsi_2_dataSet_err_raw = (RooDataSet*)file->Get("Jpsi_2_massErr_data");
        RooDataSet* Ups_dataSet_err_raw = (RooDataSet*)file->Get("Ups_massErr_data");
        RooDataSet* Pri_dataSet_err_raw = (RooDataSet*)file->Get("Pri_massErr_data");
        #endif

        if (!Jpsi_1_dataSet || !Jpsi_2_dataSet || !Ups_dataSet || !Pri_dataSet) {
            std::cerr << "Error reading RooDataSet from file " << inputFileName << std::endl;
            file->Close();
            delete file;
            continue;
        }

        // If SAVE_RAW is defined, check the raw datasets
        #ifdef SAVE_RAW
        if (!Jpsi_1_dataSet_raw || !Jpsi_2_dataSet_raw || !Ups_dataSet_raw || !Pri_dataSet_raw) {
            std::cerr << "Error reading raw RooDataSet from file " << inputFileName << std::endl;
            file->Close();
            delete file;
            continue;
        }
        #endif


        // If being read for the first time, get the RooArgSet from the dataset

        if (!argSet_Jpsi_1) {
            argSet_Jpsi_1 = (RooArgSet*)Jpsi_1_dataSet->get();
            argSet_Jpsi_2 = (RooArgSet*)Jpsi_2_dataSet->get();
            argSet_Ups = (RooArgSet*)Ups_dataSet->get();
            argSet_Pri = (RooArgSet*)Pri_dataSet->get();

            argSet_Jpsi_1_err = (RooArgSet*)Jpsi_1_dataSet_err->get();
            argSet_Jpsi_2_err = (RooArgSet*)Jpsi_2_dataSet_err->get();
            argSet_Ups_err = (RooArgSet*)Ups_dataSet_err->get();
            argSet_Pri_err = (RooArgSet*)Pri_dataSet_err->get();
        }

        #ifdef SAVE_RAW
        if (!argSet_Jpsi_1_raw) {
            argSet_Jpsi_1_raw = (RooArgSet*)Jpsi_1_dataSet_raw->get();
            argSet_Jpsi_2_raw = (RooArgSet*)Jpsi_2_dataSet_raw->get();
            argSet_Ups_raw = (RooArgSet*)Ups_dataSet_raw->get();
            argSet_Pri_raw = (RooArgSet*)Pri_dataSet_raw->get();

            argSet_Jpsi_1_err_raw = (RooArgSet*)Jpsi_1_dataSet_err_raw->get();
            argSet_Jpsi_2_err_raw = (RooArgSet*)Jpsi_2_dataSet_err_raw->get();
            argSet_Ups_err_raw = (RooArgSet*)Ups_dataSet_err_raw->get();
            argSet_Pri_err_raw = (RooArgSet*)Pri_dataSet_err_raw->get();
        }
        #endif

        if(!merged_Jpsi_1) {
            merged_Jpsi_1 = new RooDataSet("Jpsi_1_mass_cut_data", "Jpsi_1_mass_cut_data", *argSet_Jpsi_1);
            merged_Jpsi_2 = new RooDataSet("Jpsi_2_mass_cut_data", "Jpsi_2_mass_cut_data", *argSet_Jpsi_2);
            merged_Ups = new RooDataSet("Ups_mass_cut_data", "Ups_mass_cut_data", *argSet_Ups);
            merged_Pri = new RooDataSet("Pri_mass_cut_data", "Pri_mass_cut_data", *argSet_Pri);

            merged_Jpsi_1_err = new RooDataSet("Jpsi_1_massErr_cut_data", "Jpsi_1_massErr_cut_data", *argSet_Jpsi_1_err);
            merged_Jpsi_2_err = new RooDataSet("Jpsi_2_massErr_cut_data", "Jpsi_2_massErr_cut_data", *argSet_Jpsi_2_err);
            merged_Ups_err = new RooDataSet("Ups_massErr_cut_data", "Ups_massErr_cut_data", *argSet_Ups_err);
            merged_Pri_err = new RooDataSet("Pri_massErr_cut_data", "Pri_massErr_cut_data", *argSet_Pri_err);
        }

        #ifdef SAVE_RAW
        if(!merged_Jpsi_1_raw) {
            merged_Jpsi_1_raw = new RooDataSet("Jpsi_1_mass_data", "Jpsi_1_mass_data", *argSet_Jpsi_1_raw);
            merged_Jpsi_2_raw = new RooDataSet("Jpsi_2_mass_data", "Jpsi_2_mass_data", *argSet_Jpsi_2_raw);
            merged_Ups_raw = new RooDataSet("Ups_mass_data", "Ups_mass_data", *argSet_Ups_raw);
            merged_Pri_raw = new RooDataSet("Pri_mass_data", "Pri_mass_data", *argSet_Pri_raw);

            merged_Jpsi_1_err_raw = new RooDataSet("Jpsi_1_massErr_data", "Jpsi_1_massErr_data", *argSet_Jpsi_1_err_raw);
            merged_Jpsi_2_err_raw = new RooDataSet("Jpsi_2_massErr_data", "Jpsi_2_massErr_data", *argSet_Jpsi_2_err_raw);
            merged_Ups_err_raw = new RooDataSet("Ups_massErr_data", "Ups_massErr_data", *argSet_Ups_err_raw);
            merged_Pri_err_raw = new RooDataSet("Pri_massErr_data", "Pri_massErr_data", *argSet_Pri_err_raw);
        }
        #endif

        merged_Jpsi_1->append(*Jpsi_1_dataSet);
        merged_Jpsi_2->append(*Jpsi_2_dataSet);
        merged_Ups->append(*Ups_dataSet);
        merged_Pri->append(*Pri_dataSet);

        merged_Jpsi_1_err->append(*Jpsi_1_dataSet_err);
        merged_Jpsi_2_err->append(*Jpsi_2_dataSet_err);
        merged_Ups_err->append(*Ups_dataSet_err);
        merged_Pri_err->append(*Pri_dataSet_err);

        #ifdef SAVE_RAW
        merged_Jpsi_1_raw->append(*Jpsi_1_dataSet_raw);
        merged_Jpsi_2_raw->append(*Jpsi_2_dataSet_raw);
        merged_Ups_raw->append(*Ups_dataSet_raw);
        merged_Pri_raw->append(*Pri_dataSet_raw);

        merged_Jpsi_1_err_raw->append(*Jpsi_1_dataSet_err_raw);
        merged_Jpsi_2_err_raw->append(*Jpsi_2_dataSet_err_raw);
        merged_Ups_err_raw->append(*Ups_dataSet_err_raw);
        merged_Pri_err_raw->append(*Pri_dataSet_err_raw);
        #endif

        // 关闭文件并删除
        file->Close();
        delete file;
    }

    puts("Complete merging.");
    // save the merged datasets

    TFile* outputFile = nullptr
    #ifdef SAVE_RAW
    TFile* outputFile_raw = nullptr;
    #endif

    if(merged_Jpsi_1) {
        outputFile = new TFile(outputFileName, "RECREATE");
    }

    #ifdef SAVE_RAW
    if(merged_Jpsi_1_raw) {
        outputFile_raw = new TFile(outputFileName_raw, "RECREATE");
    }
    #endif

    if(outputFile){
        if (merged_Jpsi_1) {
            merged_Jpsi_1->Write();
            merged_Jpsi_2->Write();
            merged_Ups->Write();
            merged_Pri->Write();
            merged_Jpsi_1_err->Write();
            merged_Jpsi_2_err->Write();
            merged_Ups_err->Write();
            merged_Pri_err->Write();
        }
        outputFile->Close();
        delete outputFile;
    }

    #ifdef SAVE_RAW
    if(outputFile_raw){
        if (merged_Jpsi_1_raw) {
            merged_Jpsi_1_raw->Write();
            merged_Jpsi_2_raw->Write();
            merged_Ups_raw->Write();
            merged_Pri_raw->Write();
            merged_Jpsi_1_err_raw->Write();
            merged_Jpsi_2_err_raw->Write();
            merged_Ups_err_raw->Write();
            merged_Pri_err_raw->Write();
        }
        outputFile_raw->Close();
        delete outputFile_raw;
    }
    #endif


    delete merged_Jpsi_1;
    delete merged_Jpsi_2;
    delete merged_Ups;
    delete merged_Pri;
}
