// #define ON_LXPLUS_
#define ON_HEPTHU_

#ifdef ON_LXPLUS_
#include "/afs/cern.ch/user/c/chiw/cernbox/JpsiJpsiUps/ParticleCand/src/ParticleCand.C"
#endif

#ifdef ON_HEPTHU_
#include "/home/storage0/users/chiwang/storage2/CMS-Analysis/JpsiJpsiUps/condor_job/ntuples-toHTCondor/includes/ParticleCand.C"
#endif

#include "JOB_DIR/ReadTree.C"

void runReadTree(){
    TChain *chain = new TChain("mkcands/X_data","");
    chain->Add("JOB_INPUT_FILE");
    ReadTree myReadTree(chain);
    myReadTree.Loop();
}
