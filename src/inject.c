/*
 *  parasite -- inject.c
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

#include "../include/ptrace.h"
#include "../include/memsearch.h"
#include "../include/inject.h"

#define WORD 4

extern struct user_regs_struct *inject(int pid, char *shellcode) {
  void **tmp = malloc(sizeof(void*));

  long exec_mem = func(pid, "mmap", "munmap", 6, 0x0, strlen(shellcode), 0x7, 0x22, 0x0, 0x0);

  struct user_regs_struct *regs = get_regs(pid);
  struct user_regs_struct *tmp_regs = (struct user_regs_struct*)malloc(sizeof(struct user_regs_struct));
  memcpy(tmp_regs, regs, sizeof(struct user_regs_struct));

  regs->rip = exec_mem;
  regs->rsi = getpid();

  for (int i=0; i < strlen(shellcode) / WORD + 1; i++) {
    memset(tmp, 0, WORD + 1);
    memcpy(tmp, shellcode + i * WORD, WORD);
    pokedata((void*)(exec_mem + i * WORD), *tmp, pid);
  }

  set_regs(pid, regs);

  return tmp_regs;
} 
