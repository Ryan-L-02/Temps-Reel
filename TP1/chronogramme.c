#include <stdio.h>
#include <stdlib.h>

typedef struct Tab *Tableau;

struct Tab
{
    int nb_de_lignes;
    int nb_de_colonnes;
    int **Matrice;
};

void init_tableau(Tableau A)
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

void libere_memoire(Tableau A)
{
    int i;
    for (i = 0; i < A->nb_de_lignes; i++)
        free(A->Matrice[i]);
    free(A->Matrice);
    free(A);
}

Tableau TableauVide(int n, int m)
{
    Tableau A = malloc(sizeof(A));
    A->nb_de_lignes = n;
    A->nb_de_colonnes = m;
    init_tableau(A);
    return A;
}

void AfficheTableau(Tableau A)
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

Tableau LireTableau(const char *nom)
{
    Tableau A;

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
        fscanf(fichier, "%d", &ligne);
        A = TableauVide(ligne, colonne);
        while (fscanf(fichier, "%d %d %d", &a, &b, &c) != EOF)
        { /*tant que le fichier n'est pas vide*/
            A->Matrice[k][0] = a;
            A->Matrice[k][1] = b;
            A->Matrice[k][2] = c;
            k++;
        }
    }
    fclose(fichier); /*ferme le fichier*/

    return A;
}

int main()
{
    /*TEST 1*/
    int ligne = 5, colonne = 3;
    Tableau A;
    A = TableauVide(ligne, colonne);
    AfficheTableau(A);

    /*TEST 2*/
    const char *nom = "exemple1.txt";
    A = LireTableau(nom);
    AfficheTableau(A);

    libere_memoire(A);
    return 0;
}