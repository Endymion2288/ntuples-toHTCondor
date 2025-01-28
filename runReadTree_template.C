#define _ON_LXPLUS_

#ifdef _ON_LXPLUS_
#include "/afs/cern.ch/user/c/chiw/cernbox/JpsiJpsiUps/ParticleCand/src/ParticleCand.C"
#endif

#ifdef _ON_HEPTHU_
#include "src/ParticleCand.C"
#endif

#include "JOB_DIR/ReadTree.C"

void runReadTree(){
    TChain *chain = new TChain("mkcands/X_data","");
    chain->Add("JOB_INPUT_FILE");
    ReadTree myReadTree(chain);
    myReadTree.Loop();
}
