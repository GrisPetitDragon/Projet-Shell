//
// Created by chloe on 12/01/17.
//

/**
 * \file commande_echo.h
 *
 * \brief Header commande_echo.c
 */

/**
 * Code inclu uniquement si _COMMANDE_ECHO_H n'existe pas
 * (pour éviter les inclusions multiples)
 */

#ifndef _COMMANDE_ECHO_H

/**
 * \def _COMMANDE_ECHO_H
 */
#define _COMMANDE_ECHO_H
#include "divers.h" // inclusion du fichier divers.h (header)
#include "parse.h"  // inclusion du fichier parse.h (header)

#include <dirent.h>
#include <sys/types.h>

/**
 * \def fonction t_bool  ActionECHO (parse_info *info, int debut, int nbArg)
 * (commandes_internes.c)
 */
t_bool  ActionECHO (parse_info *info, int debut, int nbArg);

#endif //_COMMANDE_ECHO_H