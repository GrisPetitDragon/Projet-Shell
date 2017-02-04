#include <stdio.h>
#include <stdlib.h>
#include "divers.h"
#include "parse.h"

#define SEPARATEUR " \n"

parse_info *parse(char *ligne_cmd)
{
	parse_info *info; // Un pointeur vers la structure qui va recueillir toutes les informations contenues dans la igne de commande.
	char *tok; // Une chaine de caractères qui va succesivement contenir les différentes composantes de la ligne de commande (l'appel à une fonction proprement dit, paramètres, conditions, séparateurs, etc.)
	int i; // Cet entier correspond au numéro courant du segment de la ligne de commande que l'on est en train d'analyser.

	/* L'appel calloc initialise à zéro la structure de données,
	 * ainsi on est certain d'avoir des 'AUTRE' dans tout le tableau
	 * modificateur.
	 * La même remarque est valable pour les chemin entree et sortie. 
	 */
	info = calloc(1, sizeof(parse_info));

	tok = strtok(ligne_cmd, SEPARATEUR); // On "découpe" la ligne de commande reçue en paramètre selon les espaces (valeur de la constante SEPARATEUR)
	i = 0;
	while (tok) // Tant que l'on n'a pas traité tous les segments de la chaîne, càd tant qu'un segment (token) est défini...
	{

		/**
		 * On s'intéresse au contenu du segment. Il y a quatre cas de figure possibles :
		 *
		 * 1 : ce segment est un modificateur à lui tout seul, mais il a un lien direct avec le contenu du  token suivant. 
		 * C'est le cas pour les symboles <,>,>> et &, qui servent ou bien à rediriger les flux entrant et sortants, 
		 * ou bien à placer une commande en arrière plan.
		 *
		 * 2 : ce segment est un modificateur à lui tout seul aussi, mais il fait le lien entre l'instruction qui le précède 
		 * (le ou les tokens qui le précèdent) et celle qui le suit. C'est le cas pour les chaînes de caractères (&&, ||, |, ;)
		 * 
		 * 3: ce segment commence par un # : cela indique un  commmentaire;
		 *
		 *
		 * pour chaque token analysé,
		 * 1 : On regarde dans quel cas de figure sus-décrit on se trouve
		 * 2 : On dit à info : ce segment sert à effectuer telle action (attibution d'une valeur prédéfinie à la case courante du tableau modificateur)
		 * 3 : on conserve éventuellement un bout du token qui contient des informations dont on aura besoin pour effectuer le traitement 
		 * et afficher la ligne de commande après l'avoir complètement décomposée (voir plus en bas du fichier)
		 * eeeet... on recommence, jusqu'à ce qu'il n'y ait plus de token à lire, càd qu'on a parcouru toute la ligne de commande.
		 **/

		if (EST_EGAL(tok, "<"))
		{
			info->modificateur[i] = REDIRECTION_ENTREE;
		}
		else if (EST_EGAL(tok, ">>"))
		{
			info->modificateur[i] = REDIRECTION_SORTIE_AJOUT;
		}
		else if (EST_EGAL(tok, ">"))
		{
			info->modificateur[i] = REDIRECTION_SORTIE;
		}
		else if (EST_EGAL(tok, ";"))
		{
			info->modificateur[i] = EXECUTION;
		}
		else if (EST_EGAL(tok, "&&"))
		{
			info->modificateur[i] = EXECUTION_SI;
		}
		else if (EST_EGAL(tok, "||"))
		{
			info->modificateur[i] = EXECUTION_SINON;
		}
		else if (EST_EGAL(tok, "|"))
		{
			info->modificateur[i] = TUBE;
		}
		else if (EST_EGAL(tok, "&"))
		{
			info->modificateur[i] = ARRIERE_PLAN;
		}

		else if (EST_EGAL(tok, "#")) // Le token ne contient QUE le caractère #
		{
			/* Attention pour la prise en compte du commentaire : 
			 * ... # commande en arriere plan
			 * le commentaire doit être attaché à la suite
			 */
			i++;
			info->modificateur[i] = COMMENTAIRE;
			strcpy(info->ligne_cmd[i], ""); /* on copie par défaut une chaine vide qui sera remplacée dans les itérations suivantes */
		}
		else if (COMMENCE_PAR(tok, "#")) // Le token contient une chaîne de caractère dont le premier caractère est un #
		{
			/* Attention pour la prise en compte du commentaire : 
			 * ... #commande en arriere plan
			 * le commentaire doit être attaché au token courant
			 */
			i++;
			info->modificateur[i] = COMMENTAIRE;
			strcpy(info->ligne_cmd[i], &(tok[1]));
		}
		else if (COMMENCE_PAR(tok, "%")) // Remplacement de variable
		{
			/* Comme la substitution est faite directement, il est en réalité inutile
			 * de marquer la chaîne comme une variable.
			 * Ce ne serait pas le cas si on choisissait de faire la substitution de
			 * la variable par sa valeur lors de l'interprétation de la commande
			 */

			//info->modificateur[i] = VARIABLE;
			lire_variable(&(tok[1]), info->ligne_cmd[i], sizeof(info->ligne_cmd[i]));
			i++;
		}
		else // Si le segment ne contient pas un modificateur, ni une variable, il ne reste qu'une seule possibilité : il s'agit du nom d'une fonction ou d'une option
		{ 
			strcpy(info->ligne_cmd[i], tok);
			if (info->modificateur[i] != COMMENTAIRE) /* pour le commentaire l'incrément de i est déjà réalisé */
				i++;
		}

		/** 
		 * Le premier argument à NULL indique de garder la chaı̂ne d’origine, 
		 * et renvoie un pointeur sur le mot suivant.
		 * Cela revient à "incrémenter" tok, si on veut.
		 */
		tok = strtok(NULL, SEPARATEUR);
	}           

	info->nb_arg = i;

	return info;
}
/** 
 * detection_fin_de_commande renvoie l'indice du premier composant de la commande qui suit 
 * celle à laquelle on s'intéresse quand on appelle cette fonction. 
 * En gros, on lui donne une position de départ (logiquement, l'indice de début de la commande que l'on veut borner). 
 * La fonction va jusqu'au bout de la commande courante 
 * (elle fait progresser i jusqu'à ce qu'elle arrive au bout du nombre d'arguments de parse_info, 
 * ou jusqu'à ce que le modificateur du segment qu'elle considère concerne une autre commande que celle qui l'intéresse.
 * = Elle renvoie alors la position de i.
 **/
int detection_fin_commande(parse_info *info, int debut)
{
	int i;

	i = debut+1;
	while (i<info->nb_arg && \
			(info->modificateur[i]!=EXECUTION && \
					info->modificateur[i]!=EXECUTION_SI && \
					info->modificateur[i]!=EXECUTION_SINON && \
					info->modificateur[i]!=TUBE && \
					info->modificateur[i]!=COMMENTAIRE))
	{
		/* Les seuls éléments pouvant apparaître entre deux commandes
		 * sont ceux d'exécution conditionnelle, de tube ou de
		 * commentaire (ce qui vient après n'étant plus une commande)
		 */
		i++;
	}
	return i;
}

/** 
 * Cette fonction sert à afficher le contenu de parse info une fois que l'on a parsé la ligne de commande.
 * Maintenant qu'on a tronçonné notre ligne de commande (et donc nos commandes) en plein de segments, 
 * on peut afficher la ligne de commande initiale complète (c'est le but de cette fonction), 
 * ou une commande en particulier (c'est le but d'affiche_commande).
 * Si on veut reconstituer une commande, ou la ligne complète, 
 * il suffit d'afficher tous les segments contenus dans parse_info, un par un, 
 * et d'éventuellement faire réapparaître les opérateurs, dont on avait gardé que la sémantique 
 * (conserver le caractère en lui-même était superflu.)
 * Finalement, le but de cette fonction et de la suivant est de faire l'opération inverse de parse();
 **/
void affiche_parse_info(parse_info *info)
{
	int i;
	i = 0;

	while (i<info->nb_arg)
	{
		affiche_commande(info, i);
		i = detection_fin_commande(info, i);

		/* Les seuls éléments pouvant apparaître entre deux commandes
		 * sont ceux d'exécution conditionnelle, de tube ou de
		 * commentaire (ce qui vient après n'étant plus une commande)
		 */
		if (i<info->nb_arg && info->modificateur[i]==EXECUTION)
		{
			printf(";");
		} else if (i<info->nb_arg && info->modificateur[i]==EXECUTION_SI)
		{
			printf("&&");
		} else if (i<info->nb_arg && info->modificateur[i]==EXECUTION_SINON)
		{
			printf("||");
		} else if (i<info->nb_arg && info->modificateur[i]==TUBE)
		{
			printf("|");
		} else if (i<info->nb_arg && info->modificateur[i]==COMMENTAIRE)
		{
			printf("#");
		}
	}
	printf("\n");
}

/**
 * Ici, c'est exactement le même principe que pour la fonction précédente - 
 * sauf qu'on ne consière qu'une commande particuère à la fois ( donc une partie de la ligne de commande).
 * Pour cela, on se place à un indice de début i, et l'on fait progresser cet indice i jusqu'à une borne de fin 
 * que l'on aura trouvé grâce à la fonction detection_fin_de_commande.
 **/ 
void affiche_commande(parse_info *info, int debut)
{
	int i, n;

	n = detection_fin_commande(info, debut);

	i = debut;
	while (i<n)
	{
		/* Les seuls éléments pouvant apparaître dans une commande
		 * sont ceux de redirection ou d'exécution en arrière-plan
		 */

		// On affiche donc éventuellement l'opérande qui précise graphiquement l'action réalisée par la commande
		if (info->modificateur[i]==REDIRECTION_ENTREE)
		{
			printf("<");
		} else if (info->modificateur[i]==REDIRECTION_SORTIE)
		{
			printf(">");
		} else if (info->modificateur[i]==REDIRECTION_SORTIE_AJOUT)
		{
			printf(">>");
		} else if (info->modificateur[i]==ARRIERE_PLAN)
		{
			printf("&");
		}
		//Et, en tous les cas, on affiche un segment
		printf(" '%s' ", info->ligne_cmd[i]);

		i++;
	}
}
