/**
 * \file commande_ls.h
 *
 * \brief Header commande_ls.c
 */

/**
 * Code inclu uniquement si _COMMANDE_LS_H n'existe pas
 * (pour éviter les inclusions multiples)
 */

#ifndef _COMMANDE_LS_H

/**
 * \def _COMMANDE_LS_H
 */
#define _COMMANDE_LS_H
#include "divers.h" // inclusion du fichier divers.h (header)
#include "parse.h"  // inclusion du fichier parse.h (header)

#include <dirent.h>
#include <sys/types.h>

/**
 * \def fonction t_bool  ActionLS (parse_info *info, int debut, int nbArg)
 * (commandes_internes.c)
 */
t_bool  ActionLS (parse_info *info, int debut, int nbArg);

#endif //_COMMANDE_LS_H




