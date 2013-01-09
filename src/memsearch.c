/*
 *  parasite -- memsearch.c
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

#include "../include/memsearch.h"
#include "../include/ptrace.h"
#include "../include/helper.h"

extern void *find_function(char *func, int pid) {
  char tmp[9], *data = (char*)malloc(1024);
  int counter = 0;

  long libc = peekdata((void*)ELF_HEADER, pid);
  libc += peekdata((void*)(ELF_HEADER + 0x10), pid);
  libc = peekdata((void*)libc + 0x28, pid);

  for (libc; (int)peekdata((void*)libc, pid) != 0x464c457f; libc--);

  long tmp_libc = libc;
  tmp_libc += (int)peekdata((void*)(tmp_libc + 0x4c * 4), pid);

  for (tmp_libc; (char)peekdata((void*)tmp_libc, pid) != 0x5; tmp_libc += 0x10);

  long string_table = peekdata((void*)(tmp_libc + 0x8), pid);
  long symbol_table = peekdata((void*)(tmp_libc + 0x18), pid);

  for (;;) { 
    symbol_table += 0x18;
    tmp_libc = string_table + (short)peekdata((void*)symbol_table, pid);

    counter = 0;
    strcpy(data, "\0");

    do {
      ltostr(tmp, peekdata((void*)(tmp_libc + counter), pid));
      strncat(data, tmp, 30);
      if (strlen(data) >= 30)
        break;
      counter += 0x8;
    } while(strchr(tmp, '\0') == tmp + 8);

    if (strcmp(data, func) == 0) {
      libc += (int)peekdata((void*)(symbol_table + 0x8), pid);
      printf("[*] %s found at %8p\n", func, libc);
      return (void*)libc;
    }
  }

  return NULL;
}

extern function *find_function_ret(char *first_function, char *next_function, int pid) {
  char *tmp = (char*)malloc(9);
  function *func = (function *)malloc(sizeof(function));

  func->function = find_function(first_function, pid);
  void *ret = find_function(next_function, pid) - 2;

  for (ret; strchr((tmp = ltostr(tmp, peekdata(ret, pid))), '\xc3') == NULL; ret-=8);

  ret += strchr(ltostr(tmp, peekdata(ret, pid)), '\xc3') - tmp;
  func->ret = ret;

  free(tmp);
  return func;
}

extern long func(int pid, char *first_function, char *next_function, int num_args, ...) {
  va_list ap;
  int status;
  siginfo_t siginfo; 
  function *mmap = find_function_ret(first_function, next_function, pid);
  struct user_regs_struct *regs = get_regs(pid);

  regs->rip = (long)mmap->function;

  va_start(ap, num_args);
  for (int i=0; i < num_args; i++) {
    if (i == 0) 
      regs->rdi = va_arg(ap, long);
    if (i == 1) 
      regs->rsi = va_arg(ap, long);
    if (i == 2) 
      regs->rdx = va_arg(ap, long);
    if (i == 3) 
      regs->rcx = va_arg(ap, long);
    if (i == 4) 
      regs->r8 = va_arg(ap, long);
    if (i == 5) 
      regs->r9 = va_arg(ap, long);
  }

  set_regs(pid, regs);
  breakpoint *bp = set_breakpoint(mmap->ret, pid);
  cont(pid);

  for(;;)
  {
    waitpid(pid, &status, WUNTRACED);
    regs = get_regs(pid);

    ptrace(PTRACE_GETSIGINFO, pid, NULL, &siginfo);
    if (siginfo.si_signo == 5) {
      rm_breakpoint((void*)regs->rip - 1, pid, bp);
      break;
    }

    cont(pid);
  }

  return regs->rax;
}
