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

tArbre ArbreLireLienParenteFichier(tArbre Arbre, char Fichier[]){
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
        //fseek(f,-1,SEEK_CUR);

        if (pos == ftell(f)){
            printf("ERREUR !!!!\n");
            return NULL;
        }
        pos = ftell(f);
    }
    return Arbre;
}