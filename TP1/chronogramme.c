#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sorted_job_list.h"

typedef struct Tab *Taskset;

/*Structure pour notre Matrice*/
struct Tab
{
    int nb_de_lignes; /*Nombre de tâche*/
    int nb_de_colonnes;
    int **Matrice;
    /*
   A->Matrice[i][0] = Ci
   A->Matrice[i][1] = Di
   A->Matrice[i][2] = Ti
   */
};

/*Allocation de l'espace mémoire pour notre Matrice*/
void init_tableau(Taskset A)
{
    int **tab = (int **)malloc(sizeof(int *) * A->nb_de_lignes);
    int i, j;

    if (tab == NULL)
    {
        printf("\nErreur: L'allocation de la mémoire à échoué.\n");
        exit(1);
    }
    for (i = 0; i < A->nb_de_lignes; i++)
    {
        tab[i] = (int *)malloc(sizeof(int) * (A->nb_de_colonnes));
        if (tab[i] == NULL)
        {
            printf("\nErreur: L'allocation de la mémoire à échoué.\n");
            exit(1);
        }
        for (j = 0; j < A->nb_de_colonnes; j++)
        {
            tab[i][j] = 0;
        }
    }
    A->Matrice = tab;
}

/*On libére la mémoire qui a été alloué par notre fonction init_tableau()*/
void libere_memoire(Taskset A)
{
    int i;
    for (i = 0; i < A->nb_de_lignes; i++)
        free(A->Matrice[i]);
    free(A->Matrice);
    free(A);
}

/*Cette fonction permet de créer une matrice vide avec le nombre de ligne/colonne souhaité*/
Taskset TableauVide(int n, int m)
{
    Taskset A = malloc(sizeof(A));
    A->nb_de_lignes = n;
    A->nb_de_colonnes = m;
    init_tableau(A);
    return A;
}

/*Permet d'afficher le contenu de notre Matrice*/
void AfficheTableau(Taskset A)
{
    int a, b;
    printf("\n--------------------------------------------------------\n");
    for (a = 0; a < A->nb_de_lignes; a++)
    {
        for (b = 0; b < A->nb_de_colonnes; b++)
        {
            printf("%3d", A->Matrice[a][b]);
        }
        printf("\n");
    }
    printf("--------------------------------------------------------\n");
}

/*Permet de lire le contenu d'un fichier et de l'enregistrer dans une matrice*/
Taskset LireTableau(const char *nom)
{
    Taskset A;

    int a, b, c;
    int ligne, colonne = 3, k = 0;
    FILE *fichier = NULL;
    fichier = fopen(nom, "r"); /*ouvre le fichier en lecture*/
    if (fichier == NULL)
    {
        printf("\nLe ficher n'a pas pu être ouvert. Fin du programme \n");
        exit(0);
    }
    if (fichier != NULL)
    {
        fscanf(fichier, "%d", &ligne); /*On récupère le nombre de ligne de notre matrice grâce à la première ligne de notre fichier*/
        A = TableauVide(ligne, colonne);
        while (fscanf(fichier, "%d %d %d", &a, &b, &c) != EOF)
        {                         /*tant que le fichier n'est pas vide*/
            A->Matrice[k][0] = a; /*Durée d'exécution pire cas - Ci*/
            A->Matrice[k][1] = b; /*Echéance relative - Di*/
            A->Matrice[k][2] = c; /*Période - Ti*/
            k++;
        }
    }
    fclose(fichier); /*ferme le fichier*/

    return A;
}

/*Algorithme FP (Priorité Fixe)*/
void FP(int duree, Taskset A)
{
    /*
   A->Matrice[i][0] = Ci
   A->Matrice[i][1] = Di
   A->Matrice[i][2] = Ti
   */

    int i, j, k, comp, nb_de_taches = A->nb_de_lignes; /*Le nombre de tâche correspond au nombre de ligne*/
    int tab[nb_de_taches];

    for (i = 0; i < nb_de_taches; i++)
    {
        /*printf("\n");*/
        tab[i] = A->Matrice[i][0]; /*Ci*/
    }
    for (j = 1; j <= duree; j++)
    {
        k = -1;
        for (i = 0; i < nb_de_taches; i++)
        {
            if (A->Matrice[i][0] /*Ci*/ > 0)
            {
                k = i;
                break;
            }
        }
        if (k != -1)
        {
            printf("%d ", k + 1);
            A->Matrice[k][0] /*Ci*/--;
        }
        else
        {
            printf("0 ");
        }
        for (i = 0; i < nb_de_taches; i++)
        {
            comp = (int)ceil((j / A->Matrice[i][2]) * A->Matrice[i][2]); /* (j/Ti)*Ti */
            if (j == comp)
            {
                if (A->Matrice[i][0] /*Ci*/ == 0 || j == 0)
                {
                    A->Matrice[i][0] /*Ci*/ += tab[i];
                }
            }
        }
    }
    printf("\n");
}

/*Algorithme EDF (Priorité Dynamique)*/
void EDF(int duree, Taskset A)
{
    /*
   A->Matrice[i][0] = Ci
   A->Matrice[i][1] = Di
   A->Matrice[i][2] = Ti
   */

    int i, j, nb_de_taches = A->nb_de_lignes; /*Le nombre de tâche correspond au nombre de ligne*/
    SortedJobList list = create_empty_list();

    for (i = 0; i < nb_de_taches; i++)
    {
        add_job(&list, (i + 1) /*Numéro de tâche*/, A->Matrice[i][0] /*Ci*/, A->Matrice[i][1] /*Di*/);
    }
    for (j = 1; j <= duree; j++)
    {
        printf("%d ", schedule_first(&list));
        for (i = 0; i < nb_de_taches; i++)
        {
            if (j == (int)ceil((j / A->Matrice[i][2]) * A->Matrice[i][2]))
            {
                add_job(&list, (i + 1) /*Numéro de tâche*/, A->Matrice[i][0] /*Ci*/, A->Matrice[i][1] /*Di*/ + (j - 1));
            }
        }
    }
    free_list(&list);
    printf("\n");
}

int main(int argc, char *argv[])
{
    Taskset A;
    const char *nom_de_fichier = argv[1];
    int duree = atoi(argv[3]);

    if ((strcmp(argv[2], "FP") == 0) || (strcmp(argv[2], "fp") == 0))
    {
        A = LireTableau(nom_de_fichier);
        AfficheTableau(A);
        FP(duree, A);
        libere_memoire(A);
    }
    if ((strcmp(argv[2], "EDF") == 0) || (strcmp(argv[2], "edf") == 0))
    {
        A = LireTableau(nom_de_fichier);
        AfficheTableau(A);
        EDF(duree, A);
        libere_memoire(A);
    }
    else
    {
        printf("Vous n'avez pas choisi le bon algorithme.\n");
        exit(10);
    }

    return 0;
}