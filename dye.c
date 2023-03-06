#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  char *env_file = "/tmp/dyed.env"; // 默认的环境文件路径

  // 检查命令行选项是否包含 -e 标志
  if (argc > 2 && strcmp(argv[1], "-e") == 0) {
    env_file = argv[2];
    argv += 2;  // 更新命令行参数列表，跳过 -e 标志和路径参数
    argc -= 2;
  }

  // 检查参数数量
  if (argc <= 1) {
    printf("Usage: %s [-e env_file] command [args]\n", argv[0]);
    return -1;
  }

  // Get command and args
  char *cmd = argv[1];
  char **args = &argv[1];

  // Open environment variable file
  FILE *fp = fopen(env_file, "r");
  if (fp == NULL) {
    perror("Error opening file");
    return -1;
  }

  // Read each line and set the environment variable
  char line[256];
  while (fgets(line, sizeof(line), fp)) {
    char *pos = strchr(line, '=');
    if (pos != NULL) {
      *pos = '\0';
      setenv(line, pos + 1, 1);
    }
  }

  // Execute the command
  char *new_args[] = { "/bin/sh", "-c", cmd, NULL };
  execvp(new_args[0], new_args);

  perror("Error executing command");
  return -1;
}
