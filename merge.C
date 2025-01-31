#include "TFile.h"
#include "RooArgSet.h"
#include "RooDataSet.h"
#include <fstream>
#include <vector>
#include <string>

void merge() {
    // 读取文件列表
    const char* fileListFileName = "all_cands_list.txt";
    const char* outputFileName = "merged_dataset.root";
    std::vector<std::string> inputFileNames;
    std::ifstream fileList(fileListFileName);
    std::string line;
    while (std::getline(fileList, line)) {
        inputFileNames.push_back(line);
    }
    fileList.close();

    // 文件中包含Jpsi1_mass_cut_data, Jpsi2_mass_cut_data, Ups_mass_cut_data, Pri_mass_cut_data这四个数据集需要合并
    // 首先，创建一个RooArgSet，其中包含各个数据集的变量

    RooArgSet* argSet_Jpsi1 = nullptr;
    RooArgSet* argSet_Jpsi2 = nullptr;
    RooArgSet* argSet_Ups = nullptr;
    RooArgSet* argSet_Pri = nullptr;

    // 创建新的RooDataSet来保存合并后的数据
    RooDataSet* merged_Jpsi1 = nullptr;
    RooDataSet* merged_Jpsi2 = nullptr;
    RooDataSet* merged_Ups = nullptr;
    RooDataSet* merged_Pri = nullptr;

    // 遍历所有输入文件
    for (const auto& inputFileName : inputFileNames) {
        // 打开文件
        TFile* file = new TFile(inputFileName.c_str(), "READ");
        if (!file || file->IsZombie()) {
            std::cerr << "Error opening file " << inputFileName << std::endl;
            continue;
        }

        // 从文件中读取RooDataSet
        // 文件中包含Jpsi1_mass_cut_data, Jpsi2_mass_cut_data, Ups_mass_cut_data, Pri_mass_cut_data这四个数据集需要合并
        RooDataSet* Jpsi1_dataSet = (RooDataSet*)file->Get("Jpsi1_mass_cut_data");
        RooDataSet* Jpsi2_dataSet = (RooDataSet*)file->Get("Jpsi2_mass_cut_data");
        RooDataSet* Ups_dataSet = (RooDataSet*)file->Get("Ups_mass_cut_data");
        RooDataSet* Pri_dataSet = (RooDataSet*)file->Get("Pri_mass_cut_data");

        // 任意一个不存在，则跳过并报错
        if (!Jpsi1_dataSet || !Jpsi2_dataSet || !Ups_dataSet || !Pri_dataSet) {
            std::cerr << "Error reading RooDataSet from file " << inputFileName << std::endl;
            file->Close();
            delete file;
            continue;
        }

        // 如果是第一个文件，使用它的RooArgSet作为合并后的RooDataSet的RooArgSet

        if (!argSet_Jpsi1) {
            argSet_Jpsi1 = (RooArgSet*)Jpsi1_dataSet->get();
            argSet_Jpsi2 = (RooArgSet*)Jpsi2_dataSet->get();
            argSet_Ups = (RooArgSet*)Ups_dataSet->get();
            argSet_Pri = (RooArgSet*)Pri_dataSet->get();
        }


        // 将当前数据集的内容添加到合并后的数据集中
        if(!merged_Jpsi1) {
            merged_Jpsi1 = new RooDataSet("Jpsi1_mass_cut_data", "Jpsi1_mass_cut_data", *argSet_Jpsi1);
            merged_Jpsi2 = new RooDataSet("Jpsi2_mass_cut_data", "Jpsi2_mass_cut_data", *argSet_Jpsi2);
            merged_Ups = new RooDataSet("Ups_mass_cut_data", "Ups_mass_cut_data", *argSet_Ups);
            merged_Pri = new RooDataSet("Pri_mass_cut_data", "Pri_mass_cut_data", *argSet_Pri);
        }

        merged_Jpsi1->append(*Jpsi1_dataSet);
        merged_Jpsi2->append(*Jpsi2_dataSet);
        merged_Ups->append(*Ups_dataSet);
        merged_Pri->append(*Pri_dataSet);

        // 关闭文件并删除
        file->Close();
        delete file;
    }

    puts("Complete merging.");
    // 如果合并后的数据集不为空，则保存到新的文件中
    if (merged_Jpsi1) {
        TFile* outputFile = new TFile(outputFileName, "RECREATE");
        merged_Jpsi1->Write();
        merged_Jpsi2->Write();
        merged_Ups->Write();
        merged_Pri->Write();
        outputFile->Close();
        delete outputFile;
    }

    // 清理
    delete merged_Jpsi1;
    delete merged_Jpsi2;
    delete merged_Ups;
    delete merged_Pri;
}
