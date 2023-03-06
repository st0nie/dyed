#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // Check command-line arguments
  if (argc <= 1) {
    printf("Usage: %s [command]\n", argv[0]);
    return -1;
  }
  if (argv[1] == NULL || strcmp(argv[1], "") == 0) {
    printf("Error: Missing environment variable filename.\n");
    return -1;
  }

  // Get environment file name and command
  char *env_file = argv[1];
  char *cmd = argv[2];
  char **args = &argv[2];

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
  execvp(cmd, args);

  perror("Error executing command");
  return -1;
}
