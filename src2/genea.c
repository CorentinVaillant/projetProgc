#include "genea.h"
#include <stdlib.h>


tArbre ArbreCreer(void){
    return malloc(sizeof(struct sArbre));
}

void ArbreAfficher(tArbre Arbre){
    
    tIdentite fPere = Arbre->pPremiere->pPere;
    tIdentite fMere = Arbre->pPremiere->pMere;
    
    printf("\tPère :");
    if(!fPere)
        printf("inconnu");
    else
        IdentiteAfficher(fPere->Identifiant);
    printf("\tMère :");
    if(!fMere)
        printf("inconnu");
    else
        IdentiteAfficher(fMere->Identifiant);
    
    
}