# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>

static void do_cat(const char *path);
static void die(const char *s);

int
main(int argc, char *argv[])
{
  int i;
  if (argc < 2) {
    fprintf(stderr, "%s: file name not given\n", argv[0]);
  }
  for (i = 1; i < argc; i++) {
    do_cat(argv[i]);
  }
  exit(0);
}

#define BUFFER_SIZE 2048

static void
do_cat(const char *path)
{
  int fd;
  unsigned char buf[BUFFER_SIZE];
  int n;

  fd = open(path, O_RDONLY);
  if (fd < 0) die(path);

  for (;;) {
    // readの戻り値は読み込んだバイト数. 0はEOF, -1はエラー
    n = read(fd, buf, sizeof buf);

    if (n < 0) die(path);
    // EOF
    if (n == 0) break;

    // writeの戻り値は書き込んだバイト数. 0はEOF, -1はエラー
    if (write(STDOUT_FILENO, buf, n) < 0) die(path);
  }
  // closeの戻り値は0が成功, -1がエラー
  if (close(fd) < 0) die(path);
}

// エラー処理. perrorはerrnoの値を元にエラーメッセージを出力する
static void
die(const char *s)
{
  perror(s);
  exit(1);
}
