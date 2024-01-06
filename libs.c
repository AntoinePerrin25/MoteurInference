#include "libs.h"
//#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
//#include <crtdbg.h>
#include <stdio.h>
#include <string.h>

// Question 1
Regle *creerRegleVide(void)
{
    Regle *r = calloc(1, sizeof(*r)); // allocation dynamique
    return r;
}
Proposition *creerProposition(const char *valeur)
{
    Proposition *p = calloc(1, sizeof(*p) + strlen(valeur)); // allocation dynamique
    strcpy(p->valeur, valeur);                              // copie de la valeur
    return p;
}
void ajouterProposition(BF *bf, Proposition *proposition)
{
    for (Proposition *p = bf->proposition; p != NULL; p = p->suivante)
    { // Pour chaque proposition de la base de fait
        if (strcmp(p->valeur, proposition->valeur) == 0)
        { // On vérifie si la proposition est dans la base de fait
            return;
        }
    }
    if (bf->proposition == NULL)
    { // Si la base de fait est vide, on ajoute la proposition
        bf->proposition = proposition;
    }
    else
    { // Sinon on ajoute la proposition à la fin de la base de fait
        Proposition *derniereProp = bf->proposition;
        while (derniereProp->suivante != NULL)
        {
            derniereProp = derniereProp->suivante;
        }
        derniereProp->suivante = proposition;
    }
    return;
}
void ajouterPremisse(Regle *regle, const char *valeur)
{
    Proposition *p = creerProposition(valeur); // Création de la proposition

    if (regle->premisses == NULL)
    { // Si c'est la première proposition on l'ajoute directement
        regle->premisses = p;
    }
    else
    { // Sinon on l'ajoute à la fin de la liste
        Proposition *Premisse = regle->premisses;
        while (Premisse->suivante != NULL)
        {
            Premisse = Premisse->suivante;
        }
        Premisse->suivante = p;
    }
    return;
}
void ajouterConclusion(Regle *regle, const char *valeur)
{
    Proposition *nouvelleProposition = creerProposition(valeur); // Création de la proposition

    if (regle->conclusions == NULL)
    { // Si c'est la première proposition on l'ajoute directement
        regle->conclusions = nouvelleProposition;
    }
    else
    { // Sinon on l'ajoute à la fin de la liste
        Proposition *derniereProposition = regle->conclusions;

        while (derniereProposition->suivante != NULL)
        {
            derniereProposition = derniereProposition->suivante;
        }
        derniereProposition->suivante = nouvelleProposition;
    }
    return;
}
int test_appartenance_premisse(const Proposition *premisse, const char *valeur)
{ // Recursivement
    if (premisse == NULL)
    { // Si la prémisse est vide, la proposition n'appartient pas à la prémisse
        return 0; 
    }
    else if (strcmp(premisse->valeur, valeur) == 0)
    {
        return 1; // La proposition appartient à la prémisse
    }
    else
    { // On continue la recherche recursivement dans la prémisse suivante
        return test_appartenance_premisse(premisse->suivante, valeur);
    }
}
void supprimerPropositionPremisse(Regle *regle, const char *valeur)
{
    if (regle->premisses == NULL)
    {
        return; // La prémisse est vide, il n'y a rien à supprimer
    }
    Proposition *precedente = NULL;
    Proposition *courante = regle->premisses;

    while (courante != NULL)
    { // On parcourt les prémisses de la règle
        if (strcmp(courante->valeur, valeur) == 0)
        {
            if (precedente == NULL)
            {
                // Si la prémisse à supprimer est la première, mettre à jour le pointeur de début
                regle->premisses = courante->suivante;
            }
            else
            {
                precedente->suivante = courante->suivante;
            }
            free(courante); // Libérer la mémoire de la prémisse
            return;
        }

        precedente = courante; // On passe à la prémisse suivante
        courante = courante->suivante;
    }
}
void supprimerPropositionConclusion(Regle *regle, const char *valeur)
{
    Proposition *precedente = NULL;
    Proposition *courante;

    for (courante = regle->conclusions; courante != NULL; courante = courante->suivante)
    { // On parcourt les conclusions de la règle
        if (strcmp(courante->valeur, valeur) == 0)
        { // Si la conclusion à supprimer est trouvée
            if (precedente == NULL)
            {
                regle->conclusions = courante->suivante;
            }
            else
            {
                precedente->suivante = courante->suivante;
            }

            free(courante); // Libérer la mémoire de la conclusion
            return;
        }

        precedente = courante; // On passe à la conclusion suivante
    }
}
int test_regle_vide(Regle *regle)
{ // explicit
    if (regle->premisses == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
Proposition *premiereProposition(Regle *regle)
{ // explicit
    return regle->premisses;
}
Proposition *premiereConclusion(Regle *regle)
{ // explicit
    return regle->conclusions;
}

// Question 2
BC *creerBC(void)
{
    BC *bc = calloc(1, sizeof(*bc)); // allocation dynamique
    return bc;
}
void ajouterRegle(BC *bc, Regle *regle)
{
    if (bc->regle == NULL)
    { // Si la première n'existe pas, on l'ajoute directement
        bc->regle = regle;
    }
    else
    { // Sinon on l'ajoute à la fin de la liste
        BC *derniereBC = bc;
        while (derniereBC->suivante != NULL)
        {
            derniereBC = derniereBC->suivante;
        }
        derniereBC->suivante = creerBC();    // On crée une nouvelle base de connaissance
        derniereBC->suivante->regle = regle; // On ajoute la règle à la nouvelle base de connaissance
    }
}
Regle *Tete_BC(BC *bc)
{ // explicit
    return bc->regle;
}

// Question 3
void moteur(BC *base_connaissances, BF *base_faits, FILE *conclusionstxt)
{
    for (Proposition *proposition = base_faits->proposition; proposition != NULL; proposition = proposition->suivante)
    { // On prend chaque proposition de la base de fait
        for (BC *bc = base_connaissances; bc != NULL; bc = bc->suivante)
        { // On prend chaque base de connaissance
            Regle *regle = bc->regle;
            if (regle != NULL)
            { // On prend chaque règle de la base de connaissance
                if (test_appartenance_premisse(regle->premisses, proposition->valeur))
                { // Si la proposition est dans la prémisse de la règle
                    supprimerPropositionPremisse(regle, proposition->valeur);
                    if (test_regle_vide(regle))
                    { // Si la prémisse est vide
                        Proposition *conclusion = regle->conclusions;
                        while(conclusion != NULL)
                        { // On ajoute chaque conclusion de la règle à la base de fait
                            Proposition *next = conclusion->suivante;
                            printf("La conclusion est : %s\n", conclusion->valeur);
                            ajouterProposition(base_faits, creerProposition(conclusion->valeur));
                            if (conclusionstxt != NULL)
                            {
                                fprintf(conclusionstxt, "%s\n", conclusion->valeur);
                            }
                            supprimerPropositionConclusion(regle, conclusion->valeur);
                            conclusion = next; // On passe à la conclusion suivante
                        }
                    }
                }
            }
        }
    }
}


// Autres fonctions utilisées
void lireFichierSauvegarde(FILE *fichier, BC *bc, BF *bf)
{
    char ligne[256];
    int ligneCourante = 0;

    while (fgets(ligne, sizeof(ligne), fichier))
    { // On lit chaque ligne du fichier
        ligneCourante++;
        switch (ligne[0])
        { // On regarde le premier caractère de la ligne
            case '#':
            case '\n':
                continue;
            case 'R':
            case 'r':
            { // Si c'est une règle
                char *colonne = strchr(ligne, ':'); // On cherche le premier ':' de la ligne
                if (colonne == NULL)
                {
                    fprintf(stderr, "Erreur ligne %d : ':' manquant dans la règle\n", ligneCourante);
                    continue;
                }
                char *premisses = strtok(colonne + 1, ";"); // On récupère les prémisses
                if (premisses == NULL)
                {
                    fprintf(stderr, "Erreur ligne %d : premisses manquantes dans la règle\n", ligneCourante);
                    continue;
                }
                char *premisse;
                char *conclusions = strtok(NULL, ";\n"); // On récupère les conclusions
                char *conclusion;

                Regle *regle = creerRegleVide(); // On crée la règle
                ajouterRegle(bc, regle);        // On ajoute la règle à la base de connaissance
                // On modifiera la règle de la base de connaissance ensuite

                for (premisse = strtok(premisses, ","); // On récupère chaque prémisse
                    premisse != NULL;
                    premisse = strtok(NULL, ","))
                { // On ajoute chaque prémisse à la règle
                    while (strchr(" \t", premisse[0])) // On prend la prémisse sans les espaces et les tabulations
                    {
                        premisse++;
                    }
                    ajouterPremisse(regle, premisse);
                }

                for (conclusion = strtok(conclusions, ","); // On récupère chaque conclusion
                    conclusion != NULL;
                    conclusion = strtok(NULL, ","))
                {
                    while (strchr(" \t", conclusion[0]))    // On prend la conclusion sans les espaces et les tabulations
                    {
                        conclusion++;
                    }
                    ajouterConclusion(regle, conclusion);
                }
                break;
            }
            case 'F':
            case 'f':
            { // Si c'est un fait
                char *colonne = strchr(ligne, ':'); // On cherche le premier ':' de la ligne
                if (colonne == NULL)
                {
                    fprintf(stderr, "Erreur ligne %d : ':' manquant dans le fait\n", ligneCourante);
                    continue;
                }
                for (char *fait = strtok(colonne + 1, ",\n"); // On récupère chaque fait
                    fait != NULL;
                    fait = strtok(NULL, ",\n"))
                {
                    while (strchr(" \t", fait[0]))            // On prend le fait sans les espaces et les tabulations
                    {
                        fait++;
                    }
                    ajouterProposition(bf, creerProposition(fait));
                }
                break;
            }
            default:
                fprintf(stderr, "Danger ligne %d : Premier caractère '%c' invalide\n", ligneCourante, ligne[0]);
                break;
        }
    }
    fclose(fichier);
}
BF *creerBF(void)
{
    BF *bf = calloc(1, sizeof(*bf)); // allocation dynamique
    return bf;
}
void afficherBC(const BC *bc, FILE *conclusionstxt)
{
    int index = 0;
    if (bc == NULL || bc->regle == NULL)
    {
        if (conclusionstxt)
        {
            fprintf(conclusionstxt, "Base de connaissances vide\n");
        }
        printf("Base de connaissances vide\n");
    }
    while (bc != NULL && bc->regle != NULL)
    { // On affiche chaque règle de la base de connaissance
        if (conclusionstxt)
        { // Si on a un fichier de sortie, on l'utilise
            afficherRegle(bc->regle, &index, conclusionstxt);
        }
        else
        {
            afficherRegle(bc->regle, &index, NULL);
        }
        bc = bc->suivante;
    }
}
void afficherBF(const BF *bf, FILE *conclusionstxt)
{
    for (Proposition *proposition = bf->proposition; // On affiche chaque proposition de la base de fait
        proposition != NULL;
        proposition = proposition->suivante)
    {
        if (conclusionstxt)
        { // Si on a un fichier de sortie, on l'utilise
            fprintf(conclusionstxt, "%s", proposition->valeur);
        }
        printf("'%s'", proposition->valeur);

        if (proposition->suivante != NULL)
        { // On ajoute une virgule si ce n'est pas la dernière proposition
            if (conclusionstxt)
            {
                fprintf(conclusionstxt, ", ");
            }
            printf(", ");
        }
    }

    if (conclusionstxt)
    {
        fprintf(conclusionstxt, "\n");
    }
    printf("\n");
}
void afficherRegle(const Regle *regle, int *index, FILE *conclusionstxt)
{
    if (regle == NULL || (regle->premisses == NULL && regle->conclusions == NULL))
    { // Si la règle est vide, on ne l'affiche pas
        return;
    }
    if (conclusionstxt)
    { // Si on a un fichier de sortie, on l'utilise
        fprintf(conclusionstxt, "R : ");
    }
    printf("Regle %d: ", ++ (*index));
    for (Proposition *premisse = regle->premisses; premisse != NULL; premisse = premisse->suivante)
    { // On affiche chaque prémisse de la règle
        if (conclusionstxt)
        {
            fprintf(conclusionstxt, "%s", premisse->valeur);
        }
        printf("'%s'", premisse->valeur);
        if (premisse->suivante != NULL)
        { // On ajoute une virgule si ce n'est pas la dernière prémisse
            if (conclusionstxt)
            {
                fprintf(conclusionstxt, ", ");
            }
            printf(", ");
        }
    }
    if (conclusionstxt)
    {   
        fprintf(conclusionstxt, "; ");
    }
    printf("; ");

    for (Proposition *conclusion = regle->conclusions; conclusion != NULL; conclusion = conclusion->suivante)
    { // On affiche chaque conclusion de la règle
        if (conclusionstxt)
        {
            fprintf(conclusionstxt, "%s", conclusion->valeur);
        }
        printf("'%s'", conclusion->valeur);
        if (conclusion->suivante != NULL)
        {
            if (conclusionstxt)
            {
                fprintf(conclusionstxt, ", ");
            }
            printf(", ");
        }
    }
    if (conclusionstxt)
    {
        fprintf(conclusionstxt, "\n");
    }
    printf("\n");
}

void supprimerReglesVides(BC *base_connaissances)
{ // non fonctionnelle
    BC *bc = base_connaissances;
    BC *precedente = NULL;

    while (bc != NULL)
    {
        if (bc->regle->premisses == NULL)
        {
            BC suivante = *bc->suivante;

            free(bc->suivante);
            free(bc->regle);
            *bc = suivante;

        }
        else
        {
            precedente = bc;
            bc = bc->suivante;
        }
    }
}
