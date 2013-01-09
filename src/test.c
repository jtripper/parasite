#include <stdio.h>

int main() {
  printf("%d\n", getpid());
  for(;;) {
    sleep(1);
  }
}
