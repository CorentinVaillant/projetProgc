#include "genea.h"
#include <stdlib.h>

#define ROUGE "\x1b[31m"
#define BLANC   "\x1b[0m"

//prives

static pFiche trouverIdArbre(tArbre abr,int id){
    pFiche fiche = abr->pPremiere;
    while (fiche && (fiche->Identite->Identifiant != id)){
        fiche = fiche->pSuivante;
    }
    return fiche;
    
}

static unsigned int LongueurString(char String[]){
    char cara = String[0];
    int longueur=0;
    while (cara!='\0'){
        longueur++;
        cara = String[longueur];
    }
    return longueur;
}



int ajoutString(char **string, char *ajout) {
    unsigned int nbChar = LongueurString(*string) + LongueurString(ajout);
    *string = realloc(*string, (nbChar + 1) * sizeof(char));
    if (!*string) {
        perror("AÏE AÏE AÏE !");
        return -1;
    }
    
    snprintf(*string + LongueurString(*string), nbChar + 1, "%s", ajout);

    return nbChar;
}

static void ArbreAfficherAscendantsRec(tArbre Arbre, int Identifiant,unsigned int niveau){
    pFiche pId = trouverIdArbre(Arbre,Identifiant);
    if(!pId){
        perror("aaah ascendant"); //mettre erreur
        return ;
    }
    IdentiteAfficher(pId->Identite);
    printf("\n");
    if(pId->pPere){
        for(int i = 0 ; i<niveau ; i++)    printf("\t");
        printf("\tPère : ");
        ArbreAfficherAscendantsRec(Arbre,pId->pPere->Identite->Identifiant,niveau+1);
    }
    if(pId->pMere){
        for(int i = 0 ; i<niveau ; i++) printf("\t");
        printf("\tMère : ");
        ArbreAfficherAscendantsRec(Arbre,pId->pMere->Identite->Identifiant,niveau+1);   
    }
}

//publiques
tArbre ArbreCreer(void){
    return malloc(sizeof(struct sArbre));;
}

pFiche CreerFiche(tIdentite Identite, pFiche pPere, pFiche pMere, pFiche pSuivante){
    pFiche id = malloc(sizeof(struct sFiche));
    if(!id) return NULL;
    id->Identite = Identite;
    id->pPere = pPere;
    id->pMere = pMere;
    id->pSuivante = pSuivante;
    return id;
}

void ArbreAfficher(tArbre Arbre){

    if(!Arbre->pPremiere || !Arbre->pDerniere){
        fprintf(stderr,"%sERREUR%s : Pointeur invalide :\n\tArbre->pPremiere = %p \n\tArbre->pDerniere = %p\n",
        ROUGE,
        BLANC,
        Arbre->pPremiere,
        Arbre->pDerniere);
        return ;
    }
        
    pFiche id = Arbre->pPremiere;

    if(!(id->pSuivante))printf("AZAAAAAAAAH§!!!"); //remplacer par erreur 

    while (id){
        //affichage de l'identité
        if(!id->Identite) printf("inconnu");
        else IdentiteAfficher(id->Identite);
        
        //affichage du père
        printf("\n\tPère : ");
        if(!(id->pPere) || !(id->pPere->Identite)) printf("inconnu");
        else IdentiteAfficher(id->pPere->Identite);

        //affichage de la mère
        printf("\n\tMère : ");
        if(!(id->pMere) || !(id->pMere->Identite)) printf("inconnu");
        else IdentiteAfficher(id->pMere->Identite);
        printf("\n");

        if(id == id->pSuivante){
            fprintf(stderr,"%sERREUR%s : l'identité pointe vers elle même",ROUGE,BLANC);
            return ;}

        id = id->pSuivante;
        fflush(stdout);
    }
}

void ArbreAjouterPersonne(tArbre Arbre, tIdentite Identite){
    
    if(!Identite){
        fprintf(stderr,"%sERREUR%s : Identite = NULL",ROUGE,BLANC);
        return ;
    }
    
    pFiche pId = calloc(1,sizeof(struct sFiche));

    if(!pId){
        fprintf(stderr,"%sERREUR%s : echec dans l'allouage de struct sFiche* pId !",ROUGE,BLANC);
        return ;
    }
    pId->Identite = Identite;

    Arbre->pDerniere->pSuivante = pId;
    Arbre->pDerniere = pId;
}

void ArbreLiberer(tArbre Arbre){
    pFiche id = Arbre->pPremiere;

    if(!(id->pSuivante))printf("AZAAAAAAAAH§!!!"); //remplacer par erreur 

    while (id){
        pFiche idSuivant = id->pSuivante;
        IdentiteLiberer(id->Identite);
        free(id->Identite);
        free(id);
        id = idSuivant;    
    }
    free(Arbre);
    
}

tArbre ArbreLirePersonnesFichier(char Fichier[]){
    FILE *fichier = fopen(Fichier,"rt");
    if(!fichier) return NULL;
    tArbre abr = ArbreCreer();
    tIdentite id = IdentiteLiref(fichier);
    abr->pPremiere = CreerFiche(id,NULL,NULL,NULL);
    abr->pDerniere = abr->pPremiere;
    id = IdentiteLiref(fichier);
    
    while (id!=NULL){
        // abr->pPremiere = CreerFiche(id,NULL,NULL,abr->pPremiere);
        abr->pDerniere->pSuivante = CreerFiche(id,NULL,NULL,NULL);
        abr->pDerniere = abr->pDerniere->pSuivante;
        id = IdentiteLiref(fichier);
    }
    fclose(fichier);
    return abr;
}

void ArbreAjouterLienParente(tArbre Arbre, int IdEnfant, int IdParent, char Parente){
    pFiche pEnfant = trouverIdArbre(Arbre,IdEnfant);
    pFiche pParent = trouverIdArbre(Arbre,IdParent);

    if(!pEnfant || !pParent){
        printf("%sERROR:%s id enfant ou id parent non trouvé : \npEnfant =%p \npParent =%p\n",
        ROUGE,BLANC,pEnfant,pParent);
        return;
    }

    if (Parente == 'M')
        pEnfant->pMere = pParent;
    else if (Parente == 'P')
        pEnfant->pPere = pParent;
    else
        printf("%sERROR:%s lien de parente inconnus : %c\n",ROUGE,BLANC,Parente);
}

int ArbreLireLienParentef(FILE *f, int *pIdEnfant, int *pIdParent, char *pParente){
    return fscanf(f,"%d %d %c",pIdEnfant,pIdParent,pParente);
}

tArbre ArbreLireLienParenteFichier(tArbre Arbre, char Fichier[]){ //à modifié pour copier arbre et revoyer arbre 
    FILE *f = fopen(Fichier,"rt");
    if(!f){
        perror("AAAh");// insérer erreur
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
        perror("AAAAAAH!:!!!!");//mettre erreur !
        return;
    }

    char *homme = calloc(sizeof(char),77);
    char *femme = calloc(sizeof(char),27 );
    char *lien  = calloc(sizeof(char),24 );
    sprintf(homme,"\n\tnode [ shape = box , color = blue , fontname = \" Arial \" , fontsize =10];\n");
    sprintf(femme,"\n\tnode [ color = green ];\n");
    sprintf(lien ,"\n\tedge [ dir = none ];\n");

    pFiche id = Arbre->pPremiere;
    while (id){
        char buffer[100];
        sprintf(buffer,"\t%d [label=\"%s\\n%s\\n%s\"]\n",
        id->Identite->Identifiant,
        id->Identite->Nom,
        id->Identite->Prenom,
        id->Identite->DateNaissance);
            
        if(id->Identite->Sexe=='M')
            ajoutString(&homme,buffer);
        else
            ajoutString(&femme,buffer);
        
        if(id->pMere){
            char buffer[50];
            sprintf(buffer,"\t%d -> %d;\n",
            id->pMere->Identite->Identifiant,
            id->Identite->Identifiant);
            ajoutString(&lien,buffer);
        }
        if(id->pPere){
            char buffer[50];
            sprintf(buffer,"\t%d -> %d;\n",
            id->pPere->Identite->Identifiant,
            id->Identite->Identifiant);
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
    pFiche pId = trouverIdArbre(Arbre,Identifiant);
    if(!pId){
        perror("aaah ascendant"); //mettre erreur
        return ;
    }
    FILE *f = fopen(Fichier,"wt");
    if(!f){
        perror("AAAAAAH!:!!!!");//mettre erreur !
        return;
    }
    fprintf(f,"digraph {\n\trankdir = \" BT \" ;\n\nnode [shape=box,fontname=\"Arial\",fontsize =10];\n\nedge [dir=none];");


}