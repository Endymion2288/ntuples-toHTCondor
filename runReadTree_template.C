#include "/afs/cern.ch/user/c/chiw/cernbox/JpsiJpsiUps/ParticleCand/src/ParticleCand.C"
#include "/afs/cern.ch/user/c/chiw/cernbox/JpsiJpsiUps/ParticleCand/src/2022-2024temp-20cands-20bin-27Jan2025-02/ReadTree.C"

void runReadTree(){
    TChain *chain = new TChain("mkcands/X_data","");
    chain->Add("JOB_INPUT_FILE");
    ReadTree myReadTree(chain);
    myReadTree.Loop();
}
