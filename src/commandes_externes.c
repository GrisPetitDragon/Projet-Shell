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
        /** 
         * TODO : Dans le cas de plusieurs arguments 'pstree -p 838' par exemple,
         * il faut passer les arguments de la forme "pstree", "-p", "838"
         * Fonction équivalente pour passer ligne en arg ?
         * execlp(info->ligne_cmd[debut],ligne, NULL);
         * Ici on ne gère que les 5 premiers arguments : 1 commande + 4 arguments
         */ 

        /** 
         * TODO : résoudre le pb du wait
         *
         * Avec un wait
         * Exemple de gedit : on revient au shell uniquement une fois que gedit 
         * est fermé
         *
         * Mais sans wait(), il y a d'autres pb
         * Exemple de pstree : on ne revient jamais au shell sans intervention de l'utilisateur
         */

        // on gère les commandes jusqu'à 5 arguments (commande inclue)
        switch(nbArg) {
          case 1:
            execlp(info->ligne_cmd[debut], info->ligne_cmd[0], NULL);
            break;
          case 2 :
            execlp(info->ligne_cmd[debut], info->ligne_cmd[0], info->ligne_cmd[1], NULL);
            break;
          case 3:
            execlp(info->ligne_cmd[debut], info->ligne_cmd[0], info->ligne_cmd[1], info->ligne_cmd[2], NULL);
            break;
          case 4:
            execlp(info->ligne_cmd[debut], info->ligne_cmd[0], info->ligne_cmd[1], info->ligne_cmd[2], info->ligne_cmd[3], NULL);
            break;
          default:
            // on prend en compte les 5 premiers arguments (commande inclue)
            execlp(info->ligne_cmd[debut], info->ligne_cmd[0], info->ligne_cmd[1], info->ligne_cmd[2], info->ligne_cmd[3], info->ligne_cmd[4], NULL);
            break;
        }

        if (nbArg == 1 ) {
          execlp(info->ligne_cmd[debut], info->ligne_cmd[0], NULL);
        } else if (nbArg == 3 ) {
          execlp(info->ligne_cmd[debut], info->ligne_cmd[0], info->ligne_cmd[1], info->ligne_cmd[2], NULL);
        }
        

        /** 
         * Si la commande n'a pas pu s'exécutée, et uniquement dans ce cas 
         * (car on ne revient jamais d'un exec() - d'où le fork() )
         * Par exemple : 
         * - erreur du execlp() (mais pas de la commande exécutée)
         */
        printf("Commande non exécutée\n");
      } else { // code du père
        /** 
         * Le père attend le fils
         * Cela n'est pas utile dans tous les cas,
         * et pose des problèmes
         */ 
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