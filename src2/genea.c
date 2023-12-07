#include "genea.h"
#include <stdlib.h>

#define ERREUR "\x1b[31mERREUR\x1b[0m"

//prives

// trouve une fiche dans un arbre à partir de l'identifiant de la variable tIdentite des struct sFiches, et renvoie le pointeur de la fiche
static pFiche trouverIdArbre(tArbre abr,int id){
    if(!abr) return NULL;
    pFiche fiche = abr->pPremiere;
    
    while (fiche && (fiche->Identite) && (IdentiteIdentifiant(fiche->Identite) != id)){
        fiche = fiche->pSuivante;
    }
    return fiche;
    
}

//renvoie la longeur d'une String
static unsigned int LongueurString(char String[]){
    char cara = String[0];
    int longueur=0;
    while (cara!='\0'){
        longueur++;
        cara = String[longueur];
    }
    return longueur;
}

//ajoute à la fin d'une variable string une autre, et réallou la mémoire pour pouvoire faire cela
static int ajoutString(char **pString, char *ajout) {
    unsigned int nbChar = LongueurString(*pString) + LongueurString(ajout);
    *pString = realloc(*pString, (nbChar + 1) * sizeof(char));
    if (!*pString) {
        fprintf(stderr,"%s :pString pointe vers NULL !\n");
        return -1;
    }
    
    snprintf(*pString + LongueurString(*pString), nbChar + 1, "%s", ajout);

    return nbChar;
}

//fonction récursive d'affiche de l'arbre ascendant
static void ArbreAfficherAscendantsRec(tArbre Arbre, int Identifiant,unsigned int niveau){
    pFiche pFi = trouverIdArbre(Arbre,Identifiant);
    if(!pFi){
        fprintf(stderr,"%s : Identifiant %d non trouvé !\n",ERREUR,Identifiant);
        return ;
    }
    IdentiteAfficher(pFi->Identite);
    printf("\n");
    if(pFi->pPere){
        for(int i = 0 ; i<niveau ; i++)    printf("\t");
        printf("\tPère : ");
        ArbreAfficherAscendantsRec(Arbre,IdentiteIdentifiant(pFi->pPere->Identite),niveau+1);
    }
    if(pFi->pMere){
        for(int i = 0 ; i<niveau ; i++) printf("\t");
        printf("\tMère : ");
        ArbreAfficherAscendantsRec(Arbre,IdentiteIdentifiant(pFi->pMere->Identite),niveau+1);   
    }
}

//fonction récurcive de l'écriture d'un arbre ascendant dans un fichier
static void ArbreEcrireAscendantsGVRec(tArbre Arbre, int Identifiant, FILE *fichier){
    pFiche pId = trouverIdArbre(Arbre,Identifiant);
    if(!pId){
        fprintf(stderr,"%s : identitifiant %d non trouvé",ERREUR,Identifiant);
        return ;
    }
    
    if (IdentiteSexe(pId->Identite) == 'M'){
        fprintf(fichier,"\n\tnode [color=blue];\n\t%d [label=\"%s\\n%s\\n%s\"];\n",
        IdentiteIdentifiant(pId->Identite),
        IdentiteNom(pId->Identite),
        IdentitePrenom(pId->Identite),
        IdentiteDateNaissance(pId->Identite));
    }
    else{
        fprintf(fichier,"\n\tnode [color=green];\n\t%d [label=\"%s\\n%s\\n%s\"];\n",
        IdentiteIdentifiant(pId->Identite),
        IdentiteNom(pId->Identite),
        IdentitePrenom(pId->Identite),
        IdentiteDateNaissance(pId->Identite));
    }

    if(pId->pPere){
        ArbreEcrireAscendantsGVRec(Arbre,pId->pPere->Identite->Identifiant,fichier);
        fprintf(fichier,"\t%d -> %d;\n", pId->pPere->Identite->Identifiant,IdentiteIdentifiant(pId->Identite));
    }
    if(pId->pMere){
        ArbreEcrireAscendantsGVRec(Arbre,pId->pMere->Identite->Identifiant,fichier);
        fprintf(fichier,"\t%d -> %d;\n", pId->pMere->Identite->Identifiant,IdentiteIdentifiant(pId->Identite));
    }

}

//fonction de création d'une fiche
static pFiche CreerFiche(tIdentite Identite, pFiche pPere, pFiche pMere, pFiche pSuivante){
    pFiche fiche = malloc(sizeof(struct sFiche));
    if(!fiche) return NULL;
    fiche->Identite = Identite;
    fiche->pPere = pPere;
    fiche->pMere = pMere;
    fiche->pSuivante = pSuivante;
    return fiche;
}



//publiques
tArbre ArbreCreer(void){
    return calloc(sizeof(struct sArbre),1);
}

void ArbreAfficher(tArbre Arbre){

    if(!Arbre){
        perror("arbres -> NULL\n");
        return ;
    }

    if(!Arbre->pPremiere || !Arbre->pDerniere){
        fprintf(stderr,"%s : Pointeur invalide :\n\tArbre->pPremiere = %p \n\tArbre->pDerniere = %p\n",
        ERREUR,
        Arbre->pPremiere,
        Arbre->pDerniere);
        fflush(stderr);
        return ;
    }
    
    pFiche fiche = Arbre->pPremiere;

    while (fiche){
        //affichage de l'identité
        if(!fiche->Identite) printf("inconnu");
        else IdentiteAfficher(fiche->Identite);
        
        //affichage du père
        printf("\n\tPère : ");
        if(!(fiche->pPere) || !(fiche->pPere->Identite)) printf("inconnu");
        else IdentiteAfficher(fiche->pPere->Identite);

        //affichage de la mère
        printf("\n\tMère : ");
        if(!(fiche->pMere) || !(fiche->pMere->Identite)) printf("inconnu");
        else IdentiteAfficher(fiche->pMere->Identite);
        printf("\n");

        if(fiche == fiche->pSuivante){
            fprintf(stderr,"%s : l'identité pointe vers elle même",ERREUR);
            return ;}

        fiche = fiche->pSuivante;
        fflush(stdout);
    }
}

void ArbreAjouterPersonne(tArbre Arbre, tIdentite Identite){
    
    if(!Identite){
        fprintf(stderr,"%s : Identite = NULL",ERREUR);
        return ;
    }
    
    pFiche fiche = calloc(1,sizeof(struct sFiche));

    if(!fiche){
        fprintf(stderr,"%s : echec dans l'allouage de struct sFiche* pId !",ERREUR);
        return ;
    }
    fiche->Identite = Identite;

    if(!Arbre->pPremiere){
        Arbre->pPremiere = fiche;
        Arbre->pDerniere = fiche;
    }
    else{
        Arbre->pDerniere->pSuivante = fiche;
        Arbre->pDerniere = fiche;
    }
}

void ArbreLiberer(tArbre Arbre){
    pFiche fiche = Arbre->pPremiere;

//pas d'erreur ?
    // if(!(fiche->pSuivante)){
    //     printf("AZAAAAAAAAH§!!!"); //remplacer par erreur 
    //     free(fiche);
    //     return ;
    //     }

    while (fiche){
        pFiche ficheSuivante = fiche->pSuivante;
        IdentiteLiberer(fiche->Identite);
        free(fiche->Identite);
        free(fiche);
        fiche = ficheSuivante;    
    }
    free(Arbre);
    
}

tArbre ArbreLirePersonnesFichier(char Fichier[]){
    FILE *fichier = fopen(Fichier,"rt");
    if(!fichier) return NULL;
    tArbre abr = ArbreCreer();
    tIdentite iden = IdentiteLiref(fichier);
    abr->pPremiere = CreerFiche(iden,NULL,NULL,NULL);
    abr->pDerniere = abr->pPremiere;
    iden = IdentiteLiref(fichier);
    
    while (iden){
        abr->pDerniere->pSuivante = CreerFiche(iden,NULL,NULL,NULL);
        abr->pDerniere = abr->pDerniere->pSuivante;
        iden = IdentiteLiref(fichier);
    }
    fclose(fichier);
    return abr;
}

void ArbreAjouterLienParente(tArbre Arbre, int IdEnfant, int IdParent, char Parente){
    pFiche pEnfant = trouverIdArbre(Arbre,IdEnfant);
    pFiche pParent = trouverIdArbre(Arbre,IdParent);

    if(!pEnfant || !pParent){
        printf("%s : id enfant ou id parent non trouvé : \npEnfant =%p \npParent =%p\n",
        ERREUR,pEnfant,pParent);
        return;
    }

    if (Parente == 'M')
        pEnfant->pMere = pParent;
    else if (Parente == 'P')
        pEnfant->pPere = pParent;
    else
        printf("%s : lien de parente inconnus : %c\n",ERREUR,Parente);
}

int ArbreLireLienParentef(FILE *f, int *pIdEnfant, int *pIdParent, char *pParente){
    return fscanf(f,"%d %d %c",pIdEnfant,pIdParent,pParente);
}

tArbre ArbreLireLienParenteFichier(tArbre Arbre, char Fichier[]){ //à modifié pour copier arbre et revoyer arbre 
    FILE *f = fopen(Fichier,"rt");
    if(!f){
        fprintf(stderr,"%s : échec de l'ouverture de %s",ERREUR,Fichier);
        return NULL;}
    long int pos = ftell(f);
    while (!feof(f)){
        //passe tout les '\n'
        while (fgetc(f) == (int)'\n') ;
        fseek(f,-1,SEEK_CUR);
        
        int idEnfant, idParent;
        char parente;
        ArbreLireLienParentef(f,&idEnfant,&idParent,&parente);
        ArbreAjouterLienParente(Arbre,idEnfant,idParent,parente);
        
        while (fgetc(f) == (int)' ') ;
        //passe tout les '\n'
        while (fgetc(f) == (int)'\n') ;
        fseek(f,-1,SEEK_CUR);

        if (pos == ftell(f))
            return Arbre;
    
        pos = ftell(f);
    }
    return Arbre;
}

//niveau 3

void ArbreEcrireGV(tArbre Arbre, char Fichier[]){
    FILE *f = fopen(Fichier,"wt");
    if(!f){
        fprintf(stderr,"%s : échec de l'ouverture de %s",ERREUR,Fichier);
        return;
    }
    //allocation de diférentes char[] pour pouvoir stocké l'écriture du fichier
    char *homme = calloc(sizeof(char),77);
    char *femme = calloc(sizeof(char),27 );
    char *lien  = calloc(sizeof(char),24 );
    sprintf(homme,"\n\tnode [ shape = box , color = blue , fontname = \" Arial \" , fontsize =10];\n");
    sprintf(femme,"\n\tnode [ color = green ];\n");
    sprintf(lien ,"\n\tedge [ dir = none ];\n");

    pFiche id = Arbre->pPremiere;
    while (id){

        if(!id->Identite){
            fclose(f);
            free(homme);
            free(femme);
            free(lien);
            perror("AAAAAh!");
            return ;
        }
        
        char buffer[100];
        sprintf(buffer,"\t%d [label=\"%s\\n%s\\n%s\"]\n",
        IdentiteIdentifiant(id->Identite),
        IdentiteNom(id->Identite),
        IdentitePrenom(id->Identite),
        IdentiteDateNaissance(id->Identite));
            
        if(id->Identite->Sexe=='M')
            ajoutString(&homme,buffer);
        else
            ajoutString(&femme,buffer);
        
        if(id->pMere){
            char buffer[50];
            sprintf(buffer,"\t%d -> %d;\n",
            IdentiteIdentifiant(id->pMere->Identite),
            IdentiteIdentifiant(id->Identite));
            ajoutString(&lien,buffer);
        }
        if(id->pPere){
            char buffer[50];
            sprintf(buffer,"\t%d -> %d;\n",
            IdentiteIdentifiant(id->pPere->Identite),
            IdentiteIdentifiant(id->Identite));
            ajoutString(&lien,buffer);
        }

        id = id->pSuivante;
    }


    fprintf(f,"digraph {\n\trankdir = \" BT \" ;");
    fprintf(f,"%s",homme);
    fprintf(f,"%s",femme);
    fprintf(f,"%s",lien);
    fprintf(f,"}");
    fclose(f);
    free(homme);
    free(femme);
    free(lien);

}


void ArbreAfficherAscendants(tArbre Arbre, int Identifiant){
    ArbreAfficherAscendantsRec(Arbre,Identifiant,0);
}

void ArbreEcrireAscendantsGV(tArbre Arbre, int Identifiant, char Fichier[]){
    FILE *f = fopen(Fichier,"wt");
    if(!f){
        fprintf(stderr,"%s : échec de l'ouverture de %s",ERREUR,Fichier);
        return;
    }
    fprintf(f,"digraph {\n\trankdir = \" BT \" ;\n\nnode [shape=box,fontname=\"Arial\",fontsize =10];\n\nedge [dir=none];\n");
    ArbreEcrireAscendantsGVRec(Arbre,Identifiant,f);
    fprintf(f,"}");
    fclose(f);
}