#include "../src2/genea.h"

int main(int argc, char **argv){
    tArbre abr = ArbreLirePersonnesFichier(argv[1]);
    ArbreLireLienParenteFichier(abr,argv[2]);
    ArbreAfficher(abr);
    ArbreLiberer(abr);
    return 0;
}