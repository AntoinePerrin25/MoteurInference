#include <stdio.h>
#include <string.h>


#include "libs.h"
// cmake --build . && Debug\main.exe
// Ou avec les fichiers en paramètres
// cmake --build . && Debug\main.exe   ..\input.txt ..\output.txt

int main(int argc, char **argv)
{
    FILE *fichier = stdin;
    FILE *conclusionstxt;
    char *cheminconclusions;

    if (argc > 2)
    {
        printf("%d arguments\n", argc);
        if ((fichier = fopen(argv[1], "r")) == NULL)
        {
            fprintf(stderr, "Erreur: impossible d'ouvrir le fichier %s\n", argv[1]);
            return 1;
        }
        cheminconclusions = argv[2];
    }
    else
    {
        if ((fichier = fopen("..\\..\\save.txt", "r")) != NULL)
        {
            printf("cheminconclusions = ..\\..\\conclusions.txt\n");
            cheminconclusions = "..\\..\\conclusions.txt";
        }
        else if ((fichier = fopen("..\\save.txt", "r")) != NULL)
        {
            printf("cheminconclusions = ..\\conclusions.txt\n");
            cheminconclusions = "..\\conclusions.txt";
        }
        else if ((fichier = fopen("save.txt", "r")) != NULL)
        {
            printf("cheminconclusions = conclusions.txt\n");
            cheminconclusions = "conclusions.txt";
        }
        else
        {
            fprintf(stderr, "Erreur: impossible d'ouvrir le fichier save.txt\n");
            return 1;
        }
    }
    
    if ((conclusionstxt = fopen(cheminconclusions, "w")) == NULL)
    {
        fprintf(stderr, "Erreur: impossible d'ouvrir le fichier %s\n", cheminconclusions);
        return 1;
    }

    BC *base_connaissances = creerBC();
    BF *base_faits = creerBF();
    lireFichierSauvegarde(fichier, base_connaissances, base_faits);
    fclose(fichier);
    printf("---------\n");
    printf("Base de connaissances:\n");
    afficherBC(base_connaissances, NULL);
    printf("Base de faits:\n");
    afficherBF(base_faits, NULL);

    printf("-------\n");
    printf("Moteur d'inference :\n");
    moteur(base_connaissances, base_faits, conclusionstxt);
    printf("Base de faits:\n");
    fprintf(conclusionstxt, "Base de faits:\n");
    afficherBF(base_faits, conclusionstxt);
    printf("Base de connaissances:\n");
    //supprimerReglesVides(base_connaissances); Non Fonctionnelle
    fprintf(conclusionstxt, "Base de connaissances:\n");
    afficherBC(base_connaissances, conclusionstxt);
    printf("Fin du programme\n");
    fclose(conclusionstxt);

    return 0;
}

