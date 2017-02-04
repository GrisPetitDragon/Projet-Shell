/**
 * \file commandes_internes.h
 *
 * \brief Header commandes_internes.c
 */

/**
 * Code inclu uniquement si _COMMANDES_INTERNES_H_ n'existe pas
 * (pour éviter les inclusions multiples)
 */
#ifndef _COMMANDES_INTERNES_H_

/**
 * \def _COMMANDES_INTERNES_H_
 */
#define _COMMANDES_INTERNES_H_
#include "divers.h" // inclusion du fichier divers.h (header)
#include "parse.h"  // inclusion du fichier parse.h (header)

#include <dirent.h>
#include <sys/types.h>

#include "commande_ls.h"
#include "commande_echo.h"

/**
 * \def fonction t_bool  ActionSET (parse_info *info, int debut, int nbArg)
 * (commandes_internes.c)
 */ 
t_bool  ActionSET (parse_info *info, int debut, int nbArg);

/**
 * \def fonction t_bool  ActionCD (parse_info *info, int debut, int nbArg)
 * (commandes_internes.c)
 */ 
t_bool  ActionCD (parse_info *info, int debut, int nbArg);

#endif
