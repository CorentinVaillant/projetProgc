#include "../src2/genea.h"

void test(void){
    //test fonction de bases
    printf("Test fonction de bases :\n");
    tArbre abr1 = ArbreCreer();
    ArbreAfficher(abr1);
    tIdentite mario = IdentiteCreer(0,"bros","mario",'M',"09/07/1981");
    tIdentite luigi = IdentiteCreer(0,"bros","luigi",'M',"14/03/1983");
    ArbreAjouterPersonne(abr1,mario);
    ArbreAjouterPersonne(abr1,luigi);
    ArbreAfficher(abr1);
    ArbreLiberer(abr1); abr1 = NULL;
    printf("--->OK<---\n");
    //test fichiers
    printf("Test fonction fichier :\n");
    tArbre abr2 = ArbreLirePersonnesFichier("./arbres/arbre10.ind");
    ArbreLireLienParenteFichier(abr2,"./arbres/arbre10.par");
    ArbreAfficher(abr2);
    printf("--->OK<---\n");
    //test niveau 3
    printf("Test fonction niveau 3 :\n");
    ArbreEcrireGV(abr2,"arbre10.dot");
    ArbreEcrireAscendantsGV(abr2,4,"ascendants4.dot");
    ArbreLiberer(abr2); abr2 = NULL;
    printf("--->OK<---\n");


}


int main(int argc, char **argv){
    test();
    return 0;
}