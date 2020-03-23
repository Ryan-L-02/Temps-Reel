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
        { /*tant que le fichier n'est pas vide*/
            A->Matrice[k][0] = a; /*Durée d'exécution pire cas*/
            A->Matrice[k][1] = b; /*Echéance relative*/
            A->Matrice[k][2] = c; /*Période*/
            k++;
        }
    }
    fclose(fichier); /*ferme le fichier*/

    return A;
}

int main(int argc, char *argv[])
{
    /*Taskset A;
    const char *nom_de_fichier = argv[1];*/

    return 0;
}