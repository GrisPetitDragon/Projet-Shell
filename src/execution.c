/**
 * \file execution.c
 *
 * \brief Code des fonctions permettant de parcourir la ligne de commande
 *
 * Ce fichier contient les fonctions permettant de parcourir 
 * la ligne de commande (execution_ligne_cmd) et de lancer
 * les commandes individuelles (execution_cmd)
 */

#include "execution.h" // inclusion du fichier execution.h (header)
#include "commandes_internes.h" // inclusion du fichier commandes_internes.h (header)
#include "commandes_externes.h" // inclusion du fichier commandes_externes.h (header)

/**
 * \fn void execution_ligne_cmd(parse_info *info)
 * \brief Effectuer un parcours de la ligne de commande pour isoler chacune des commandes
 *
 * \param *info (parse_info) Pointeur sur l'analyse de la ligne de commande (parse_info)
 *
 * \return (void)
 */
void execution_ligne_cmd(parse_info *info) {

  int i, j, n, nb_arg;
  int resultat;

  /* cet appel est a supprimer et n'est la que pour illustrer l'existence de la structure de donnees */
  printf("Appel a affiche_parse_info (%s %d) a supprimer\n", __FILE__, __LINE__);

  /**
   * Fonction déclarée dans parse.c
   */
  affiche_parse_info(info);

  i = 0;
  /**
   * Pour tous les arguments inclus dans l'objet info (paramètre en entrée de la fonction)
   */
  while (i<info->nb_arg) {

    /**
     * Pour rappel, ces champs servent à passer une éventuelle redirection
     * à la fonction d'exécution d'une commande
     */
    strcpy(info->entree, "");
    strcpy(info->sortie, "");

    /* n aura la valeur de la fin de ligne de commande, ou du debut
     * de la prochaine commande a executer
     * \to-know : à quoi correspond la valeur de retour de detection_fin_commande() ?
     */
    n = detection_fin_commande(info, i);

    /**
     * Initialisation de j pour ne pas modifier i dans la boucle while suivante
     */
    j = i;

    /** 
     *  Initialisation de nb_arg à 0 
     */
    nb_arg = 0;

    while (j<n) {
      /* Attention cas non detecte arguments après une redirection :
       * commande arg1 < redirection arg2
       * il s'agit a priori d'une erreur de syntaxe du shell
       */

      switch(info->modificateur[j]) {
        case REDIRECTION_ENTREE:
          /**
           * Modification de entree --> avec la partie d'indice j
           * de la ligne de commande entrée
           */
          strcpy(info->entree, info->ligne_cmd[j]);
          break;
        case REDIRECTION_SORTIE:
          /**
           * Modification de entree --> avec la partie d'indice j
           * de la ligne de commande entrée
           */
          strcpy(info->sortie, info->ligne_cmd[j]);
          break;
        case ARRIERE_PLAN:
          /**
           * Modification du modificateur d'indice i
           * qui prend la valeur ARRIERE_PLAN
           */
          info->modificateur[i]=ARRIERE_PLAN;
          break;
        default:
          /**
           * Le cas par défaut permet de compter le nombre
           * d'arguments
           */
           nb_arg++;
      }
      /**
       * Incrémentation de j pour passé sur tous les 
       * modificateurs de info
       */
      j++;
    }

    /* cet appel est a supprimer et n'est la que pour illustrer l'existence de la structure de donnees */
    printf("Appel a affiche_commande (%s %d) a supprimer\n", __FILE__, __LINE__);
    affiche_commande(info, i);
    printf("\n");

    /**
     * \ici j = n (condition de la boucle précédente)
     */ 
    if(info->modificateur[j]==TUBE) {
      /* il faut traiter (par simplification uniquement pour deux commandes)
       * le cas de la communication via un tube
       */
    } else {
      resultat = execution_cmd(info, i, nb_arg);
    }

    if(info->modificateur[j]==COMMENTAIRE) {
      /* il suffit de sortir de la fonction, on est arrive a un commentaire
       * le reste de la ligne de commande n'est pas a traiter
       */
    }

    /* a ce stade, on a le resultat de la commande, et on sait s'il faut traiter la commande
     * suivante en sequence ou avec une condition
     */

    /* traitement post-execution de la partie concernant info->modificateur[j] != RIEN */
    switch(info->modificateur[j]) {
      case EXECUTION:
        break;

      case EXECUTION_SI:
        /* la/les commande suivante va être directement traitee ici :
         * i.e. ne sera pas executee
         */
        if(!resultat) {/* si on doit passer a la commande suivante... */
          while (j<info->nb_arg && \
                  (info->modificateur[j]!=EXECUTION && \
                  info->modificateur[j]!=EXECUTION_SINON)) {
            j++;
          }
        }
        break;
      case EXECUTION_SINON:
        /* la/les commande suivante va être directement traitee ici :
         * i.e. ne sera pas executee
         */
        if (resultat) { /* si on doit passer a la commande suivante... */

        }
        break;
      default :
        /* cas impossible a priori */
        break;
    }

    info->modificateur[j] = AUTRE; /* il suffit d'executer la commande suivante en sequence */
    /**
     * Finalement i reprend la valeur de j
     */
    i = j;
    /**
     * Si i < info->nbArg : on refait un tour de boucle
     */
  }
}

/**
 * \fn t_bool execution_cmd(parse_info *info, int debut, int nb_arg)
 * \brief Déterminer la fonction à lancer
 *
 * \param *info (parse_info) Pointeur sur l'analyse de la ligne de commande (parse_info)
 * \param debut (int)
 * \param nbArg (int) Nombre d'arguments
 *
 * \return (t_bool) Description de la valeur de retour
 */
t_bool execution_cmd(parse_info *info, int debut, int nb_arg) {
  /**
   * Selon la valeur de info->ligne_cmd[debut], 
   * on détermine la fonction à exécuter.
   * Les valeurs possibles : echo, cd, ls, set, sinon
   * la fonction exécuter est ActionEXEC
   */
  if (EST_EGAL(info->ligne_cmd[debut], "echo")) {
    return ActionECHO (info, debut, nb_arg);
  } else if (EST_EGAL (info->ligne_cmd[debut], "cd")) {
    return ActionCD (info, debut, nb_arg);
  } else if (EST_EGAL (info->ligne_cmd[debut], "ls")) {
    return ActionLS (info, debut, nb_arg);
  } else if (EST_EGAL (info->ligne_cmd[debut], "set")) {
    return ActionSET (info, debut, nb_arg);
  } else {
    return ActionEXEC (info, debut, nb_arg);
  }
}
