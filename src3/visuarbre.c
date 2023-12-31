#include "../src2/genea.h"

int main(int argc, char **argv){
    if (argc != 4){
        printf("Veuillez mettre un nombre valide d'arguments : fichier-personnes fichier-liens-parente fichier-dot\n");
        return 0;
    }

    tArbre Arbre = ArbreLirePersonnesFichier(argv[1]);
    ArbreLireLienParenteFichier(Arbre,argv[2]);
    ArbreEcrireGV(Arbre,argv[3]);
    ArbreLiberer(Arbre);
    return 0;
}