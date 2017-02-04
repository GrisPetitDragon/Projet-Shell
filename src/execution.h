/**
 * \file execution.h
 *
 * \brief Header execution.c
 */

/**
 * Code inclu uniquement si _EXECUTION_H_ n'existe pas
 * (pour éviter les inclusions multiples)
 */
#ifndef _EXECUTION_H_

/**
 * \def _EXECUTION_H_
 */
#define _EXECUTION_H_
#include "divers.h" // inclusion du fichier divers.h (header)
#include "parse.h"  // inclusion du fichier parse.h (header)

/**
 * \def fonction void execution_ligne_cmd(parse_info *info)
 * (execution.c)
 */ 
void execution_ligne_cmd(parse_info *info);

/**
 * \def fonction t_bool execution_cmd(parse_info *info, int debut, int nb_arg)
 * (execution.c)
 */ 
t_bool execution_cmd(parse_info *info, int debut, int nb_arg);

#endif
