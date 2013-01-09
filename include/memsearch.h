/*
 *  parasite -- memsearch.h 
 *  (C) 2012 jtRIPper
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 1, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef _MEMSEARCH_H
#define _MEMSEARCH_H

#include "includes.h"

#define ELF_HEADER 0x400130

typedef struct {
  void *function;
  void *ret;
} function;

void *find_function(char *func, int pid);
function *find_function_ret(char *function, char *next_function, int pid);
long func(int pid, char *first_function, char *next_function, int num_args, ...);

#endif
