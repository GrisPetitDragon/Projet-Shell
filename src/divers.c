/**
 * \file divers.c
 *
 * \brief Code des fonctions utilitaires
 *
 * Ce fichier contient les fonctions utilitaires.
 * Son rôle est de factoriser les parties communes
 * des autres modules.
 */

#include "divers.h" // inclusion du fichier divers.h (header)

/**
 * \fn void AfficheInvite()
 * \brief Lire la variable d'environnement INV et affiche un invite de commande
 *		  correspondant à cette variable. Si INV n'est pas définie, la fonction
 *		  affiche uniquement les caractères "$ "
 *
 * \return (void)
 */
void AfficheInvite() {

  char invite[CHAINE_MAX];
  char chaine[CHAINE_MAX];
  char var[CHAINE_MAX];
  size_t i;
  char * dwRet;

  /**
   * Si la variable INV n'est pas NULL-> si on affiche soit l'utilisateur, soit la machine, soit le répertoire courant, soit d'autres trucs...
   */
  if (lire_variable ("INV", invite, sizeof (invite))) {

    /* Format reconnu :
     * \u : remplace par l'utilisateur
     * \h : remplace par le nom de machine
     * \p : remplace par le chemin courant
     */

  	/**
  	 * char *strcpy(char *dest, const char *src)
  	 * strcpy permet de copier src dans dest
  	 * \return (char) dest
  	 *
  	 * \ici chaine est vidée
  	 */
    strcpy(chaine, "");

    /**
     * Tant que i est inférieur à taille de invite
     */ 
    for(i=0; i<strlen(invite); i++) {
      /**
       * Si le caractère i de invite est "\" et 
       * qu'il ne s'agit pas du dernier caractère
       */
      if (invite[i]=='\\' && i<strlen(invite)-1) {
		i = i+1;
        //On regarde ce qu'il y a après ce "\"
		switch(invite[i]) {
			/**
			 * Cas utilisateur
			 */
			case 'u' :
			  lire_variable ("USERNAME", var, sizeof (var));
			  break;
			/**
			 * Cas nom de machine
			 */
			case 'h' :
			  lire_variable ("COMPUTERNAME", var, sizeof (var));
			  break;
			/**
			 * ???
			 */ 
			case 's' :
			  strcpy(var," ");
			  break;
			/**
			 * Cas chemin courant
			 */
			case 'p' :
			  /**
			   *getcwd permet d'obtenir le répertoire de travail courant
			   */ 
			  dwRet = getcwd (var, sizeof (var));

			  /**
			   * Si le répertoire courant est NULL
			   */
			  if (dwRet == NULL) {
			  	/**
			  	 * Ecrire dans stderr
			  	 */ 
			    fprintf (stderr, "Echec lors de l'appel a getcwd !\n");

			    /**
			     * Permet de flush stderr
			     */
			    fflush (stderr);
			  }
			  break;
			default :
			  /* cas impossible a priori */
			  break;
		}
	  }
	  else {
	  	/**
	  	 * Enregistre la valeur de invite[i] dans var dans le format %c
	  	 */
		sprintf(var, "%c", invite[i]);
	  }

	  /**
	   * Ajoute la chaîne de caractère var à la fin de chaine
	   * S'il ne reste pas assez de place dans chaine, alors
	   * var est tronquée
	   */
	  strncat(chaine, var, CHAINE_MAX - strlen(chaine));
    }

  }
  else {
  	/**
  	 * Si la variable INV est NULL, on l'initie à "$ "
  	 */
    strcpy(chaine, "$ ");
  }

  /**
   * Dans le terminal, on écrit la valeur de chaine
   */ 
  printf ("%s", chaine);

  /**
   * Permet de flush stdout
   */ 
  fflush (stdout);
}

/**
 * \fn t_bool ecrire_variable (char* nomVar, char* valeur)
 * \brief Permet de modifier une variable d'environnement
 *
 * \return (t_bool) Renvoie vrai si le traitement s'est bien passé, faux sinon
 */
t_bool ecrire_variable (char* nomVar, char* valeur) {

	/**
	 * int setenv(const char *name, const char *value, int overwrite)
	 * Change ou ajoute une variable d'environnement
	 */

	int success = setenv(nomVar, valeur, 1);

    if (success == 0) {
  		printf("La variable \"%s\" vaut maintenant \"%s\".", nomVar, valeur);
  		return vrai;
  	} else {
  		printf("La variable n'a pas pu être modifiée/créée.");
  		return faux;
  	}

	printf("Appel a ecrire_variable (%s %d) a ecrire avec \"%s\" et \"%s\". \n",
	 __FILE__,
	 __LINE__,
	 nomVar,
	 valeur);

  return faux;
}

/**
 * \fn t_bool lire_variable (char* nomVar, char* valeur)
 * \brief Permet de lire une variable d'environnement
 *
 * \return (t_bool) Renvoie vrai si le traitement s'est bien passé, faux sinon
 */
t_bool lire_variable (char* nomVar, char* valeur, int taille) {
  strcpy(valeur, "");

  /**
   * char *getenv(const char *name)
   * Lit la valeur de la variable d'environnement name
   */
  valeur = getenv(nomVar);

  printf("Appel a lire_variable (%s %d) a ecrire avec \"%s\", \"%s\" et %d. \n",
	 __FILE__,
	 __LINE__,
	 nomVar,
	 valeur,
	 taille);

  printf("%s : %s\n", nomVar, valeur);
  return faux;
}