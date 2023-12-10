#include "../src2/genea.h"

int main(int argc, char **argv){
    if (argc != 5){
        printf("Veuillez mettre un nombre valide d'arguments : fichier-personnes fichier-liens-parente fichier-dot\n");
        return 0;
    }

    tArbre Arbre = ArbreLirePersonnesFichier(argv[1]);
    ArbreLireLienParenteFichier(Arbre,argv[2]);
    int id;
    sscanf(argv[3],"%d",&id);
    ArbreAfficherAscendants(Arbre,id);
    ArbreEcrireAscendantsGV(Arbre,id,argv[4]);
    return 0;
}