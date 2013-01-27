fork:
  push $0x39
  pop %rax
  syscall

  xor %rdi, %rdi
  cmp %rdi, %rax
  je child

parent:
  push $0x3e
  pop %rax
  
  mov %rsi, %rdi
  push $0xc
  pop %rsi

  syscall
  ret

child:
