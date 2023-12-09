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
        fprintf(stderr,"%s :pString pointe vers NULL !\n",ERREUR);
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
    return calloc(sizeof(struct sArbre),1); //créer un arbre vide initialiser à 0
}

void ArbreAfficher(tArbre Arbre){
    //vérifie que Arbre n'est pas NULL, affiche une erreur si oui
    if(!Arbre){ 
        fprintf(stderr,"%s : arbre NULL !",ERREUR);
        return ;
    }
    //vérifie que pPremiere et pDerniere ne pointent pas vers NULL, affiche erreur si oui
    if(!Arbre->pPremiere || !Arbre->pDerniere){
        fprintf(stderr,"%s : Pointeur NULL :\n\tArbre->pPremiere = %p \n\tArbre->pDerniere = %p\n",
        ERREUR,
        Arbre->pPremiere,
        Arbre->pDerniere);
        fflush(stderr);
        return ;
    }
    
    pFiche fiche = Arbre->pPremiere;

    //tant que fiche != NULL
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

        //vérifie que la fiche ne s'autoréférence pas en fiche suivantre, pour éviter une boucle infinie
        if(fiche == fiche->pSuivante){
            fprintf(stderr,"%s : l'identité pointe vers elle même",ERREUR);
            return ;}

        //passe à la fiche suivante
        fiche = fiche->pSuivante;
        fflush(stdout);
    }
}

void ArbreAjouterPersonne(tArbre Arbre, tIdentite Identite){
    //vérifie que Identite ou Arbre ne sont pas NULL
    if(!Identite || !Arbre){
        fprintf(stderr,"%s : Identite ou Arbre NULL \nIdentite :%p \nArbre :%p",ERREUR,Identite,Arbre);
        return ;
    }
    
    //si la personne est déjà dans l'arbre ne fais rien et renvoie l'arbre inchangé
    if(trouverIdArbre(Arbre,IdentiteIdentifiant(Identite))){
        return ;
    }

    pFiche fiche = calloc(1,sizeof(struct sFiche));
    //test de la réussite du calloc, affiche une erreur et retourne si non
    if(!fiche){
        fprintf(stderr,"%s : echec dans l'allouage de struct sFiche* pId !",ERREUR);
        return ;
    }
    fiche->Identite = Identite;
    //si l'abre est vide, mes l'identite en première et dernière fiche
    if(!Arbre->pPremiere){
        Arbre->pPremiere = fiche;
        Arbre->pDerniere = fiche;
    }
    //sinon mes l'identite en dernière position de l'arbre
    else{
        Arbre->pDerniere->pSuivante = fiche;
        Arbre->pDerniere = fiche;
    }
}

void ArbreLiberer(tArbre Arbre){
    pFiche fiche = Arbre->pPremiere;
    //libère toute les fiche tant qu'il y en a
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
    if(!fichier) //test de la réussite de l'ouverture du fichier
        return NULL; 
    tArbre abr = ArbreCreer();
    tIdentite identite = IdentiteLiref(fichier);
    abr->pPremiere = CreerFiche(identite,NULL,NULL,NULL);
    abr->pDerniere = abr->pPremiere;
    identite = IdentiteLiref(fichier);
    //tant qu'il y a une identetie à lire
    while (identite){
        abr->pDerniere->pSuivante = CreerFiche(identite,NULL,NULL,NULL);
        abr->pDerniere = abr->pDerniere->pSuivante;
        identite = IdentiteLiref(fichier);
    }
    fclose(fichier);
    return abr;
}

void ArbreAjouterLienParente(tArbre Arbre, int IdEnfant, int IdParent, char Parente){
    pFiche pEnfant = trouverIdArbre(Arbre,IdEnfant); //trouve l'enfant
    pFiche pParent = trouverIdArbre(Arbre,IdParent); //trouve le parent

    if(!pEnfant || !pParent){ //vérifie si il existent, sinon affiche une erreur et retourne
        printf("%s : id enfant ou id parent non trouvé : \npEnfant =%p \npParent =%p\n",
        ERREUR,pEnfant,pParent);
        return;
    }
    //assigne mère ou père selon Parente
    if (Parente == 'M')
        pEnfant->pMere = pParent;
    else if (Parente == 'P')
        pEnfant->pPere = pParent;
    else
        printf("%s : lien de parente inconnus : %c\n",ERREUR,Parente);
}

int ArbreLireLienParentef(FILE *f, int *pIdEnfant, int *pIdParent, char *pParente){
    return fscanf(f,"%d %d %c",pIdEnfant,pIdParent,pParente); //retourne la réussite du fscanf
}

tArbre ArbreLireLienParenteFichier(tArbre Arbre, char Fichier[]){
    FILE *f = fopen(Fichier,"rt");
    if(!f){ //vérification de la réussite de l'ouverture du fichier, affiche une erreur et renvoie NULL sinon
        fprintf(stderr,"%s : échec de l'ouverture de %s",ERREUR,Fichier);
        return NULL;}
    long int pos = ftell(f); //position dans le fichier
    while (!feof(f)){ //tant que la fin n'est pas atteinte
        //passe tout les '\n'
        while (fgetc(f) == (int)'\n') ;
        fseek(f,-1,SEEK_CUR);
        
        int idEnfant, idParent;
        char parente;
        ArbreLireLienParentef(f,&idEnfant,&idParent,&parente);
        ArbreAjouterLienParente(Arbre,idEnfant,idParent,parente);
        
        //passe tout les '\n' et ' '
        while (fgetc(f) == (int)' ') ;
        while (fgetc(f) == (int)'\n') ;
        fseek(f,-1,SEEK_CUR);

        //si la position n'avance plus
        if (pos == ftell(f)){
            fclose(f);
            return Arbre;
        }
    
        pos = ftell(f);
    }
    fclose(f);
    return Arbre;
}

//niveau 3

void ArbreEcrireGV(tArbre Arbre, char Fichier[]){
    FILE *f = fopen(Fichier,"wt");
    if(!f){ //vérification de la réussite de l'ouverture du fichier, affiche une erreur et retourne sinon
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
    while (id){ //tant que id!=NULL

        if(!id->Identite){
            fclose(f);
            free(homme);
            free(femme);
            free(lien);
            fprintf(stderr,"%s : la variable *pFiche(%p) pointe vers NULL", ERREUR,id);
            return ;
        }
        //variable temporaire pour contenir les caractères stockées dans le fichier
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

    //écriture dans le fichier
    fprintf(f,"digraph {\n\trankdir = \" BT \" ;");
    fprintf(f,"%s",homme);
    fprintf(f,"%s",femme);
    fprintf(f,"%s",lien);
    fprintf(f,"}");
    fclose(f);
    //libération des variables
    free(homme);
    free(femme);
    free(lien);

}


void ArbreAfficherAscendants(tArbre Arbre, int Identifiant){
    ArbreAfficherAscendantsRec(Arbre,Identifiant,0); //appel de la fonction récursive
}

void ArbreEcrireAscendantsGV(tArbre Arbre, int Identifiant, char Fichier[]){
    FILE *f = fopen(Fichier,"wt");
    if(!f){ //vérification de la réussite de l'ouverture du fichier, affiche une erreur et retourne si non 
        fprintf(stderr,"%s : échec de l'ouverture de %s",ERREUR,Fichier);
        return;
    }
    //écrit le début du fichier dans le fichier
    fprintf(f,"digraph {\n\trankdir = \" BT \" ;\n\nnode [shape=box,fontname=\"Arial\",fontsize =10];\n\nedge [dir=none];\n");
    //appel de la fonction récursive pour pouvoir écrire dans le fichier
    ArbreEcrireAscendantsGVRec(Arbre,Identifiant,f);
    //écrit la fin du fichier
    fprintf(f,"}");
    fclose(f);
}