#include <stdlib.h>
#include <stdio.h>

// Définition de la structure d'une proposition
typedef struct Proposition
{
    struct Proposition *suivante; // Pointeur vers la proposition suivante
    char valeur[1]; // TOUJOURS EN DERNIER !!!
} Proposition;
// Définition de la structure d'une règle
typedef struct
{
    Proposition *premisses ; // Pointeur vers la première proposition dans la prémisse
    Proposition *conclusions; // Pointeur vers la première proposition dans la conclusion
} Regle;
// Définition de la structure liste chainée des bases de connaissances
typedef struct BC           // Base de connaissances
{
    Regle *regle;           // Pointeur vers la règle
    struct BC *suivante;    // Pointeur vers la base de connaissance suivante (règle suivante)
} BC;
// Définition de la structure liste chainée des bases de faits
typedef struct           // Base de faits   
{
    Proposition *proposition;   // Pointeur vers la proposition
} BF;

// Question 1
    Regle *creerRegleVide(void);
    Proposition *creerProposition(const char *valeur);
    void ajouterProposition(BF *bf, Proposition *proposition);
    void ajouterPremisse(Regle *regle, const char *valeur);
    void ajouterConclusion(Regle *regle, const char *valeur);
    int test_appartenance_premisse(const Proposition *premisse, const char *valeur);
    void supprimerPropositionPremisse(Regle *regle, const char *valeur);
    void supprimerPropositionConclusion(Regle *regle, const char *valeur);
    int test_regle_vide(Regle *regle);
    Proposition *premiereProposition(Regle *regle);
    Proposition *premiereConclusion(Regle *regle);

// Question 2
    BC *creerBC(void);
    void ajouterRegle(BC *bc, Regle *regle);
    Regle *Tete_BC(BC *bc);
// Question 3
    void moteur(BC *base_connaissances, BF *base_faits, FILE *conclusionstxt);

// Autres prototypes de fonctions utilisées
    BF *creerBF(void);
    void lireFichierSauvegarde(FILE *fichier, BC *bc, BF *bf);
    void afficherBC(const BC *bc, FILE *conclusionstxt);
    void afficherBF(const BF *bf, FILE *conclusionstxt);
    void afficherRegle                  (const Regle *regle, int *index, FILE *conclusionstxt);

    void supprimerReglesVides(BC *base_connaissances);
