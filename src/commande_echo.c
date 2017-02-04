//
// Created by chloe on 12/01/17.
//

#include "commande_echo.h"

/**
 * \fn t_bool ActionECHO (parse_info *info, int debut, int nbArg)
 * \brief Processus echo présent traditionnellement sur un système Unix
 *        - echo : mécanisme de redirection de la sortie
 *
 * \param *info (parse_info) Pointeur sur l'analyse de la ligne de commande (parse_info)
 * \param debut (int)
 * \param nbArg (int) Nombre d'arguments
 *
 * \return (t_bool) Description de la valeur de retour
 */
t_bool ActionECHO (parse_info *info, int debut, int nbArg) {

    int i;
    /**
     * Création de sortie de type FILE *
     */
    FILE *sortie;
    /* si l'appel est correctement ecrit, on a :
     * arguments[0] == "echo"
     * arguments[1..nbArg-1] = "..."
     *
     * Par de lecture, mais une ecriture redirigee possible
     */

    /**
     * Type FILE* spécial stdout
     * Correspond à l'endroit où les "choses" vont lorsqu'on
     * utilise printf()
     * https://www.cs.bu.edu/teaching/c/file-io/intro/
     *
     * Il s'agit de la sortie standard. Il n'y a pas de redirection.
     */
    sortie=stdout;

    // Si info->sortie n'est pas vide (redirection en entrée et en sortie d'une commande interne
    if(!EST_EGAL(info->sortie, ""))
    {
        // Permet aussi de créer le fichier s'il n'existe pas
        sortie = fopen(info->sortie, "w");
        printf("Le fichier est bien ouvert.");

        if (sortie == NULL) {
            /* Traitement du cas où le fichier n'est pas accessible en écriture */
            printf("Le fichier n'est pas accessible en écriture.");
        }
    }

    i = 1;
    /**
     * Ecrit l'ensemble de la ligne de commande dans sortie
     */
    while(i<nbArg)	{
        /**
         * La différence de fprintf() et printf()
         * fprintf() permet d'écrire dans un fichier (1er paramètre)
         */
        fprintf(sortie, "%s ", info->ligne_cmd[debut+i]);
        i++;
    }
    printf("\n");

    return vrai;
}