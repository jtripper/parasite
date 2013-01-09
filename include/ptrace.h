/*
 *  parasite -- ptrace.h
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


#ifndef _PTRACE_H
#define _PTRACE_H

#include "includes.h"

typedef struct{
  long addr;
  long data;
}breakpoint;

int create_process(char *arguments[]);
void attach(int pid);
void detach(int pid);
void cont(int pid);
void step(int pid);
long peekdata(void *addr, int pid);
long pokedata(void *addr, void *data, int pid);
struct user_regs_struct *get_regs(int pid);
void set_regs(int pid, struct user_regs_struct *regs);
breakpoint *set_breakpoint(void *addr, int pid);
void rm_breakpoint(void *addr, int pid, breakpoint *bp);

#endif
