#include "genea.h"

void test(void){
    tIdentite famille[4] = {
        IdentiteCreer(1,"pig","pepa",'F',"01/04/2030"),
        IdentiteCreer(2,"pig","george",'M',"06/01/2032"),
        IdentiteCreer(3,"pig","papa",'M',"06/06/0006"),
        IdentiteCreer(4,"hog","mama",'F',"77/77/0077")};

    printf("\x1b[31mfamille :\x1b[0m");
    for(int i=0 ; i<4 ; i++){
        printf("\n\t");
        IdentiteAfficher(famille[i]);
    }printf("\n");


    pFiche pPapa = CreerFiche(famille[2],NULL,NULL,NULL);
    pFiche pMama = CreerFiche(famille[3],NULL,NULL,pPapa);
    pFiche pPepa = CreerFiche(famille[0],pPapa,pMama,pMama);
    pFiche pGeorge = CreerFiche(famille[1],pPapa,pMama,pPepa);

    tArbre abrAsaucisse = ArbreCreer();
    abrAsaucisse->pPremiere = pGeorge;
    abrAsaucisse->pDerniere = pPapa;

    printf("\x1b[31mArbreAfficher\n\x1b[0m");
    ArbreAfficher(abrAsaucisse);

    printf("\x1b[31mAjout de papi pig à la famille \n\x1b[0m");
    tIdentite papi = IdentiteCreer(0,"pig","papi",'M',"00/00/0000");
    printf("papi pig : "); IdentiteAfficher(papi); printf("\n");
    ArbreAjouterPersonne(abrAsaucisse,papi);
    
    

    printf("\x1b[31mArbreAfficher\n\x1b[0m");
    ArbreAfficher(abrAsaucisse);

    ArbreLiberer(abrAsaucisse);

    printf("--OK--\n");

}


int main(){
    test();   
    
    ArbreAfficher(ArbreLirePersonnesFichier("arbres/arbre4.ind")); 
    return 0;
}