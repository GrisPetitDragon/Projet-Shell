/**
 * \file commandes_externes.c
 *
 * \brief Code de la fonction d'exécution d'une commande non-reconnue
 *
 * Ce fichier contient le code de la fonction permettant
 * d'exécuter une commande non reconnue comme une 
 * commande interne
 *
 * \ici Il faut utiliser exec()
 */

#include "divers.h" // inclusion du fichier divers.h (header)
#include "commandes_externes.h" // inclusion du fichier header

/**
 * \fn t_bool ActionEXEC (parse_info *info, int debut, int nbArg)
 *
 * \param *info (parse_info) Pointeur sur l'analyse de la ligne de commande (parse_info)
 * \param debut (int)
 * \param nbArg (int) Nombre d'arguments
 *
 * \return (t_bool) Description de la valeur de retour
 */

t_bool  ActionEXEC (parse_info *info, int debut, int nbArg) {
  char ligne[CHAINE_MAX];
  t_bool premierPlan; // Exécution de la commande en premier plan ou en arrière plan
  int i;

  /**
   * char *strcpy(char *dest, const char *src)
   * strcpy permet de copier src dans dest
   * \return (char) dest
   *
   * \ici permet d'initialiser ligne
   */ 
  strcpy(ligne, "");


  for(i=0; i<nbArg; i++) {

    /**
     * size_t strlen(const char *str)
     * strlen permet de connaître la taille de str
     * \return (size_t) la taille du tableau
     * 
     * \ici permet de savoir si la ligne existe
     */ 
    if(strlen(ligne)!=0) {

      /**
       * char *strcat(char *dest, const char *src)
       * strcat permet de concaténer src à dest
       * \return (char) dest (avec la valeur de src à la fin)
       *
       * \ici permet d'ajouter un espace à ligne si la variable n'est pas nulle
       */ 
      strcat(ligne," ");
    }

    /**
     * \ici permet d'ajouter toutes les parties de la ligne de commande à la variable ligne
     */
    strcat(ligne, info->ligne_cmd[debut+i]);
  }
  
    // On crée un second processus pour ne pas quitter notre shell 
    pid_t pid_fils = fork();

    if (pid_fils != -1) { // le fork s'est bien passé

      if (pid_fils == 0) { // code du fils
        printf("code du fils\n");

        /** 
         * TODO : Dans le cas de plusieurs arguments 'pstree -p 838' par exemple,
         * il faut passer les arguments de la forme "pstree", "-p", "838"
         * Fonction équivalente pour passer ligne en arg ?
         * execlp(info->ligne_cmd[debut],ligne, NULL);
         */ 

        /** 
         * TODO : Processus en arrière plan à chaque fois ?
         * Exemple de gedit : on revient au shell, une fois que gedit est fermé
         */

        execlp(info->ligne_cmd[debut], info->ligne_cmd[debut], NULL);
        printf("Commande non exécutée\n");
      } else { // code du père
        printf("code du père\n");
        wait(&pid_fils);
      }
    } else { // le fork a échoué
      return 0;
    }



  /**
   * \ici permet de savoir si la commande doit être exécuter en premier-plan ou en arrière-plan
   */
  premierPlan = (info->modificateur[debut]!=ARRIERE_PLAN);

  printf("execution d'une commande externe  (%s %d) a ecrire :\n%s\n", __FILE__, __LINE__, ligne);

  /**
   * (void) variable
   * syntaxe utilisée pour ne pas générer d'erreur lors de la compilation,
   * à cause de la non utilisation de la variable
   *
   * \ici pour la variable premierPlan
   */
  (void) premierPlan;

  /**
   * \ici la fonction ActionEXEC retourne toujours faux
   */ 
  return faux;
}