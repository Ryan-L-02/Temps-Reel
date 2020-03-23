#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Tab *Taskset;

/*Structure pour notre Matrice*/
struct Tab
{
    int nb_de_lignes; /*Nombre de tâche*/
    int nb_de_colonnes;
    int **Matrice;
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
Taskset TasksetVide(int n, int m)
{
    Taskset A = malloc(sizeof(A));
    A->nb_de_lignes = n;
    A->nb_de_colonnes = m;
    init_tableau(A);
    return A;
}

/*Permet d'afficher le contenu de notre Matrice*/
void AfficheTaskset(Taskset A)
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
Taskset LireTaskset(const char *nom)
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
        A = TasksetVide(ligne, colonne);
        while (fscanf(fichier, "%d %d %d", &a, &b, &c) != EOF)
        {                         /*tant que le fichier n'est pas vide*/
            A->Matrice[k][0] = a; /*Durée d'exécution pire cas*/
            A->Matrice[k][1] = b; /*Echéance relative*/
            A->Matrice[k][2] = c; /*Période*/
            k++;
        }
    }
    fclose(fichier); /*ferme le fichier*/

    return A;
}

int test_load(Taskset A)
{
    /*
   A->Matrice[i][0] = Ci
   A->Matrice[i][1] = Di
   A->Matrice[i][2] = Ti
   */

    int i;
    double charge;
    double nb_de_taches = A->nb_de_lignes;                              /*On converti le nombre de tâche en double*/
    double borne = nb_de_taches * (pow(2.0, 1.0 / nb_de_taches) - 1.0); /*Borne = n(2^(1/n)-1)*/

    for (i = 1; i < A->nb_de_lignes; i++)
    {
        charge = charge + (A->Matrice[i][0] / A->Matrice[i][2]); /*Charge = SOMME de (Ci/Ti)*/

        if (A->Matrice[i][1] > A->Matrice[i][2])
        {
            printf("On a Di > Ti.\n");
            return 10;
        }
    }
    if (charge <= borne)
    {
        return 1;
    }
    else if (charge < 1.0 && charge > borne)
    {
        return 0;
    }
    else
        return -1;
}

int main(int argc, char *argv[])
{
    Taskset A;
    int echeance;
    const char *nom_de_fichier = argv[1];

    A = LireTaskset(nom_de_fichier);
    AfficheTaskset(A);
    echeance = test_load(A);
    printf("Echéance = %d\n", echeance);

    libere_memoire(A);
    return 0;
}