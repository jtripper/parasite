fork:
  push $0x39
  pop %rax
  syscall

  xor %rdi, %rdi
  cmp %rdi, %rax
  je child

parent:
  ret

child:
