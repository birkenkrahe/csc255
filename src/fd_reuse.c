#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

static void die(const char *msg) {
  perror(msg);
  _exit(1);
}

int main(void) {
  printf("PID: %d\n", getpid());  // printed to terminal before stdout is closed

  int fd_a = open("a.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
  if (fd_a < 0) die("open a.txt");

  int fd_b = open("b.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
  if (fd_b < 0) die("open b.txt");

  // Close stdout (FD 1). The next open should reuse FD 1.
  if (close(1) < 0) die("close stdout");

  int fd_c = open("c.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
  if (fd_c < 0) die("open c.txt");

  dprintf(2, "Note: fd_a=%d, fd_b=%d, fd_c=%d (stderr is FD 2)\n", fd_a, fd_b, fd_c);

  const char *msgA = "Written to a.txt via fd_a\n";
  const char *msgB = "Written to b.txt via fd_b\n";
  const char *msgC = "This is stdout, but it goes to c.txt now!\n";

  if (write(fd_a, msgA, strlen(msgA)) < 0) die("write a.txt");
  if (write(fd_b, msgB, strlen(msgB)) < 0) die("write b.txt");

  // Write to FD 1 (stdout) — but stdout now points to c.txt
  if (write(1, msgC, strlen(msgC)) < 0) die("write c.txt (fd 1)");

  sleep(45); // give time to inspect

  return 0;
}
