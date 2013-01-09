/*
 *  parasite -- ptrace.c 
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

extern int create_process(char *argv[]) {
  int argc, cpid;
  struct stat statp;

  for(argc=0; argv[argc] != NULL; argc++)
    continue;


  if(stat(argv[0], &statp) != 0){
    perror("stat()");
    exit(0);
  }

  if((cpid = fork()) == 0) {
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    if(execv(argv[0], argv) == -1) {
      perror("execv()");
      exit(0);
    }
  }

  return cpid;
}

extern void attach(int pid) {
  ptrace(PTRACE_ATTACH, pid, NULL, NULL);
}

extern void detach(int pid) {
  ptrace(PTRACE_DETACH, pid, NULL, NULL);
}

extern void cont(int pid) {
  ptrace(PTRACE_CONT, pid, NULL, NULL);
}

extern long peekdata(void *addr, int pid) {
  return ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
}

extern long pokedata(void *addr, void *data, int pid) {
  return ptrace(PTRACE_POKEDATA, pid, addr, data);
}

extern struct user_regs_struct *get_regs(int pid) {
  struct user_regs_struct *regs = (struct user_regs_struct *)malloc(sizeof(struct user_regs_struct));
  ptrace(PTRACE_GETREGS, pid, NULL, regs);
  return regs;
}

extern void set_regs(int pid, struct user_regs_struct *regs) {
  ptrace(PTRACE_SETREGS, pid, NULL, regs);
}

extern void step(int pid) {
  ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);
}

extern breakpoint *set_breakpoint(void *addr, int pid)
{
  breakpoint *bp = malloc(sizeof(breakpoint));;
  long data = peekdata(addr, pid);

  bp->data = data;
  bp->addr = (long)addr;

  data = (data & 0xffffffffffffff00) | 0xcc;
  pokedata(addr, (void*)data, pid);

  return bp;
}

extern void rm_breakpoint(void *addr, int pid, breakpoint *bp)
{
  pokedata(addr, (void*)bp->data, pid);
  struct user_regs_struct *regs = get_regs(pid);
  regs->rip--;
  set_regs(pid, regs);
}

