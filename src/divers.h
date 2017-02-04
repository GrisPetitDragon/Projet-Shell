/**
 * \file divers.h
 *
 * \brief Header divers.c
 */

/**
 * Code inclu uniquement si _DIVERS_H_ n'existe pas
 * (pour éviter les inclusions multiples)
 */
#ifndef _DIVERS_H_

/**
 * \def _DIVERS_H_
 */
#define _DIVERS_H_

#include <stdio.h> // inclusion de la librairie <stdio.h>
#include <string.h> // inclusion de la librairie <string.h>
#include <unistd.h> // inclusion de la librairie <unistd.h>

#include <stdlib.h>

/**
 * Permet de vérifier que a commence par b
 * Si la valeur de retour est 0, alors la condition est vérifiée
 */ 
#define COMMENCE_PAR(a,b)	(strncmp((a), (b), strlen((b)))==0)

/**
 * Vérifie que la taille de a est b est la même
 * Vérifie que a commence par b
 * 
 * Si les deux conditions sont vérifiées,
 * alors a et b sont strictement égaux
 */
#define EST_EGAL(a,b)    	(strlen((a))==strlen((b)) && COMMENCE_PAR((a),(b)))

/**
 * \def EXIT_FAILURE (valeur : 1)
 */
#define EXIT_FAILURE 1

/**
 * \def EXIT_SUCCESS (valeur : 0)
 */
#define EXIT_SUCCESS 0


/**
 * \def EXIT_SUCCESS (valeur : 128)
 */
#define ARG_MAX 128

/**
 * \def CHAINE_MAX (valeur : 512)
 */
#define CHAINE_MAX 512

/**
 * \def MAX_PATH (valeur : 512)
 */
#define MAX_PATH 512

/**
 * \enum t_bool
 * \brief Défini un type booléen
 */
typedef enum {faux=0, vrai} t_bool ; 

/**
 * \def fonction void AfficheInvite()
 * (divers.c)
 */ 
void AfficheInvite();

/**
 * \def fonction t_bool ecrire_variable (char* nomVar, char* valeur)
 * (divers.c)
 */ 
t_bool ecrire_variable (char* nomVar, char* valeur);

/**
 * \def fonction t_bool lire_variable (char* nomVar, char* valeur, int taille)
 * (divers.c)
 */ 
t_bool lire_variable (char* nomVar, char* valeur, int taille);

#endif
