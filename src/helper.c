/*
 *  parasite -- helper.c
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

#include "../include/helper.h"

extern int argv_len(char *argv[]) {
  int argc;
  for(argc = 0; argv[argc] != NULL; argc++)
    continue;
  return argc;
}

extern char **create_argv(char *argv[]) {
  int argc = argv_len(argv) - ARGLEN;

  /* allocate array of char pointers */
  char **args = (char**)malloc(argc * sizeof(char*) + 1);

  /* allocate each char pointer and copy the coresponding argument into it */
  for (int i=0; i<argc; i++) {
    args[i] = (char*)malloc(sizeof(argv[i+ARGLEN] + 1));
    strcpy(args[i], argv[i+ARGLEN]);
  }

  args[argc] = NULL;

  return args;
}

extern void free_argv(char *argv[]) {
  int argc = argv_len(argv);
  /* free each argument */
  for (int i=0; i<argc; i++) {
    free(argv[i]);
  }

  /* free array of char pointers */
  free(argv);
}

extern char *ltostr(char *str, long l) {
  memset(str, 0, sizeof(long) + 1);
  memcpy(str, &l, sizeof(long));
  return str;
}

