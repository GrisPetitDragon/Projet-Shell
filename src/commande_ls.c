//
// Created by chloe on 12/01/17.
//

#include "commande_ls.h"

/**
 * \fn t_bool ActionLS (parse_info *info, int debut, int nbArg)
 * \brief Fonction définie et écrite en tant que commande interne
 *        (cf. sujet pour plus d'infos)
 *
 * \param *info (parse_info) Pointeur sur l'analyse de la ligne de commande (parse_info)
 * \param debut (int)
 * \param nbArg (int) Nombre d'arguments
 *
 * \return (t_bool) Description de la valeur de retour
 */
t_bool ActionLS (parse_info *info, int debut, int nbArg) {

    /**
     * \ici Tout est à faire !
     */

    DIR* dp;
    struct dirent *ep;
    dp = opendir("./");

    // Si le dossier n'est pas inconnu
    if (dp != NULL ) {
        // Tant que tous les fichiers du dossier n'ont pas été parcourus (inclus . et ..)
        while ( (ep = readdir(dp)) != NULL) {
            // On affiche le nom du fichier en cours (un répertoire, un programme est un fichier sur Linux)
            printf("%s\n", ep->d_name);
        }
    }


    /* Utilisation des parametres */
    /**
     * \ici (void) permet de compiler le code sans utiliser les variables
     * dans la fonction
     */
    (void) info;
    (void) debut;
    (void) nbArg;

    printf("Appel a actionLS (%s %d) a ecrire.\n",
           __FILE__,
           __LINE__);

    return faux;
}
