#include <stdio.h>
#include <string.h>

#include "parse.h"
#include "divers.h"
#include "execution.h"
#include "commandes_internes.h"
#include "commandes_externes.h"

// Ce fichier est clairement commenté dans le sujet, à la page 8 (Exécution de plusieurs scripts). Je ne fais donc pas (trop) de commentaires en plus, ça n'apporterait rien.

int main (int argc, char *argv[]) { // arg c : le nb d'aguments reçus dans la ligne de de commande d'appel au shell. Argv[] : tableau contenant les chaînes de caractères correspondant à ces arguments


  char ligne_cmd[CHAINE_MAX]; // La ligne de commande que l'on va récupérer
  char verOS[CHAINE_MAX];
  FILE* entree;
  int i;
  parse_info *info;
  t_bool mode_interactif = (argc==1);

  /**
   * Définition de GNU.org : An interactive shell is one started without non-option arguments, 
   * unless -s is specified, without specifying the -c option, 
   * and whose input and error output are both connected to terminals (as determined by isatty(3)), 
   * or one started with the -i option.
   * An interactive shell generally reads from and writes to a user’s terminal.
   * The -s invocation option may be used to set the positional parameters when an interactive shell is started.
   * 
   * Donc si j'ai bien compris, un shell interactif est lancé avec une commande simple sans options, d'où le argc==1.
   * Si c'est un autre type de shell, il exécute des scripts contenant les instructions.
   **/


  if (mode_interactif) { // Si on est pas en train d'éxécuter du code, on affiche notre petit copyright
    lire_variable ("OS", verOS, sizeof (verOS)); // affiche la version de l'OS
    printf ("imrShell - %s\n", (char *) verOS);
    printf ("Programmation Système d'exploitation - IMR 1\n");
    fflush (stdout);
  }

  for(i=1; i<argc || mode_interactif; i++) {

    if (mode_interactif) {
      AfficheInvite ();
      entree = stdin;
    }
    else {
      /* Si le mode n'est pas interractif (il y a des arguments sur la ligne
       * de commande lors de l'appel au shell), alors il faut ouvrir les
       * fichiers en argument, un à un (argv[i]), puis interpreter leur contenu
       * dans la boucle suivante
       */
      (void) argv; // On ne fait rien : on attend l'itération suivante
    }


    while ((fgets (ligne_cmd, sizeof (ligne_cmd), entree) != 0) && !COMMENCE_PAR(ligne_cmd, "exit")) {
      info = parse(ligne_cmd);
      execution_ligne_cmd(info);

      if (mode_interactif) {

	AfficheInvite ();
      }
    }

    mode_interactif = faux;
  }

  return EXIT_SUCCESS;
}
