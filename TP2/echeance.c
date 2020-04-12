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
    double charge = 0.0;
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

int weight(Taskset A, int i, int t)
{
    int n, w = 0;

    for (n = 0; n < i; n++)
    {
        w = w + (int)(ceil(t / A->Matrice[n][2])) * A->Matrice[n][0];
    }

    return w;
}

int get_busy_period(Taskset A, int i)
{
    int w, t = 1;
    w = weight(A, i, t);

    while (w != t)
    {
        t = w;
        w = weight(A, i, t);
    }

    return w;
}

int get_nb_critical_job(Taskset A, int i, int bp)
{
    return (int)floor(bp / A->Matrice[i][2]);
}

int get_response_time(Taskset A, int i, int k)
{

    int response_time, j;
    k = (1 + (int)floor(k / A->Matrice[i][2])) * A->Matrice[i][0];
    response_time = k;

    for (j = 0; j < i - 1; j++)
    {
        response_time = response_time + (int)ceil(k / A->Matrice[j][2]) * A->Matrice[j][0];
        k = response_time;
    }

    return response_time;
}

int get_worst_case_response_time(Taskset A, int i)
{

    int j, busy, critical, worst = -1, response = -1;
    busy = get_busy_period(A, i);
    critical = get_nb_critical_job(A, i, busy);

    for (j = 0; j < critical; j++)
    {
        response = get_response_time(A, i, critical);
        response = response - critical;

        if (response > A->Matrice[i][1])
        {
            break;
        }

        if (response >= worst)
        {
            worst = response;
        }
    }

    return worst;
}

int main(int argc, char *argv[])
{
    Taskset A;
    int echeance, busy, critical, response, worst;
    int tache = 0;
    const char *nom_de_fichier = argv[1];

    A = LireTaskset(nom_de_fichier);
    printf("\nContenu du fichier :");
    AfficheTaskset(A);

    echeance = test_load(A);
    printf("\ntest_load = %d\n", echeance);

    busy = get_busy_period(A, tache);
    printf("Tâche %d : get_busy_period = %d\n", tache + 1, busy);

    critical = get_nb_critical_job(A, tache, busy);
    printf("Tâche %d : get_nb_critical_job = %d\n", tache + 1, critical);

    response = get_response_time(A, tache, 1);
    printf("Tâche %d : get_response_time = %d\n", tache + 1, response);

    worst = get_worst_case_response_time(A, tache);
    printf("Tâche %d : get_worst_case_response_time = %d\n", tache + 1, worst);

    libere_memoire(A);
    return 0;
}