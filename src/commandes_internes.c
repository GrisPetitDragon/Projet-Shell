/**
 * \file commandes_internes.c
 *
 * \brief Code des fonctions appelées lors d'un appel à une fonction interne
 *
 * Ce fichier contient le code des fonctions appelées
 * lors d'un appel à une fonction interne du shell
 * dans la ligne de commande
 */

#include "divers.h" // inclusion du fichier divers.h (header)
#include "commandes_internes.h" // inclusion du fichier commandes_internes.h (header)

/**
 * \fn t_bool ActionSET (parse_info *info, int debut, int nbArg)
 * \brief ecrire_variable()
 *        - affectation d'une valeur à une variable d'environnement 
 *        - suppression de la valeur d'une variable d'environnement
 *
 * \param *info (parse_info) Pointeur sur l'analyse de la ligne de commande (parse_info)
 * \param debut (int)
 * \param nbArg (int) Nombre d'arguments
 *
 * \return (t_bool) faux
 */
t_bool ActionSET (parse_info *info, int debut, int nbArg) {

  /* si l'appel est correctement ecrit, on a :
   * arguments[0] == "set"
   * arguments[1] == "nom_variable"
   * arguments[2] == "="
   * arguments[3] == "valeur"
   * nbArg == 4
   *
   * ou
   * arguments[0] == "set"
   * arguments[1] == "nom_variable"
   * arguments[2] == "="
   * nbArg == 3
   *
   */

  /**
   * Cas où la commande est mal écrite
   * - Le premier argument ne correspond pas à l'argument "set"
   * - Le troisième argument ne correspond pas à l'opérateur "="
   */
  if (!EST_EGAL(info->ligne_cmd[debut], "set") && !EST_EGAL(info->ligne_cmd[debut+2], "=")) {
    printf("Impossible de mettre a jour la variable, appel incorrect\n");
    return faux;
  }

  /**
   * \bug POURQUOI CETTE PARTIE n'est pas dans le ELSE de la boucle précédente
   *      QUID du cas où il y a 3 ou 4 arguments sans symbole "="
   */
  if (nbArg == 4) {
    /**
     * S'il y a bien quatre arguments :
     * On remplit la variable de arg[1] par celle de arg[3]
     * grâce à l'appel de la fonction ecrire_variable()
     */
    return ecrire_variable (info->ligne_cmd[debut+1], info->ligne_cmd[debut+3]);
  } else if (nbArg == 3) {
    /**
     * S'il n'y a que trois arguments :
     * On remplit la variable de arg[1] par NULL
     * grâce à l'appel de la fonction ecrire_variable()
     */
    return ecrire_variable (info->ligne_cmd[debut+1], NULL);
  } else {
    /**
     * Sinon on ne fait rien
     */ 
    return faux;
  }
}

/**
 * \fn t_bool ActionCD (parse_info *info, int debut, int nbArg)
 * \brief Processus cd présent sur un système Unix
 *        - cd arg : déplace le rép. courant vers le rép. arg (lire_variable())
 *        - cd : (sans arg) déplace le rép. courant vers le rép. USERPROFILE
 *        - si plusieurs arguments, le nom du rép. contient des espaces
 *
 * \param *info (parse_info) Pointeur sur l'analyse de la ligne de commande (parse_info)
 * \param debut (int)
 * \param nbArg (int) Nombre d'arguments
 *
 * \return (t_bool) faux si le répertoir n'a pas pu être modifié, vrai sinon
 */
t_bool ActionCD (parse_info *info, int debut, int nbArg) {
  /** 
   * Bien définir les valeurs des variables avant leur utilisation
   */ 
  char def[MAX_PATH];
  char dirName[MAX_PATH];
  int i;

  /* le cas nbArg==1 a un comportement par defaut qui mene au
   * repertoire HOME utilisateur
   *
   * Si nbArg>=1 il faut aggreger tous les arguments en un seul chemin
   * (le repertoire de destination contient des espaces)
   */
  if (nbArg == 1) {
    lire_variable ("USERPROFILE", def, sizeof (def));
    /**
     * chdir permet de modifier le répertoire courant
     */
    if (chdir (def) == -1) {
      printf ("Impossible de changer vers le repertoire '%s' \n", def );
      return faux;
    }
  } else {
    /**
     * char *strcpy(char *dest, const char *src)
     * strcpy permet de copier src dans dest
     * \return (char) dest
     *
     * \ici permet d'initialiser diName
     */ 
    strcpy(dirName,"");

    for(i=1; i<nbArg; i++) {
      /**
       * size_t strlen(const char *str)
       * strlen permet de connaître la taille de str
       * \return (size_t) la taille du tableau
       * 
       * \ici permet de savoir si la dirName existe
       */

      /**
       * char *strcat(char *dest, const char *src)
       * strcat permet de concaténer src à dest
       * \return (char) dest (avec la valeur de src à la fin)
       *
       * \ici permet d'ajouter un espace à ligne si la dirName n'est pas nulle
       *      (dès le second tour de boucle)
       */ 
      if(strlen(dirName)!=0) strcat(dirName," ");

      /**
       * Ajoute la commande à dirName (au fur et à mesure des tours de boucles)
       */
      strcat(dirName, info->ligne_cmd[debut+i]);
    }

    /**
     * Cas où le répertoire ne peut pas être modifié
     */
    if (chdir (dirName) == -1) {
      printf ("Impossible de changer vers le repertoire '%s'\n", dirName);
      /**
       * \return faux si le répertoire ne peut pas être modifié
       */
      return faux;
    }
  }
  /**
   * \return vrai si le répertoire peut être modifié
   */
  return vrai;
}