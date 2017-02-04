/**
 * \file commandes_externes.h
 *
 * \brief Header commandes_externes.c
 */

/**
 * Code inclu uniquement si _COMMANDES_EXTERNES_H_ n'existe pas
 * (pour éviter les inclusions multiples)
 */
#ifndef _COMMANDES_EXTERNES_H_

/**
 * \def _COMMANDES_EXTERNES_H_
 */
#define _COMMANDES_EXTERNES_H_
#include "divers.h" // inclusion du fichier divers.h (header)
#include "parse.h"  // inclusion du fichier parse.h (header)

/**
 * \def fonction t_bool ActionEXEC (parse_info *info, int debut, int nbArg)
 * (commandes_externes.c)
 */ 
t_bool ActionEXEC (parse_info *info, int debut, int nbArg);

#endif
