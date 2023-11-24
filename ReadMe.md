## Notes projet :

### questions :
 - faut-il libéré tout l'espace mémoire ou juste les str associé dans `IdentiteLiberer(tIdentite Identite)`:
    ```c
    //libere l'espace memoire occupe par Identite
    void IdentiteLiberer(tIdentite Identite){
        free(Identite->Nom);
        free(Identite->Prenom);
    }
    ``` 



    

### structure fichiers :
fichier lus par la fonction ``tIdentite IdentiteLiref(FILE *f)`` :
```c
tIdentite IdentiteLiref(FILE *f){
    int Identifiant ; 
    char *Nom,*Prenom, Sexe, DateNaissance [LG_DATE +1];
    if(!feof(f))
        fscanf(f,"%d",&Identifiant);
    if(!feof(f))
        fscanf(f,"%s",Nom);
    if(!feof(f))
        fscanf(f,"%s",Prenom);
    if(!feof(f))
        fscanf(f,"%c",&Sexe);
    if(!feof(f))
        fscanf(f,"%c",DateNaissance);
    else return NULL;
}
```
```txt
identifiant
nom
prénom
sexe
date de naissance
```
le fichier est composer d'au plus 80 caractéres par lignes.