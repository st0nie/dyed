#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define CONFIG_DIR "/.config/dye/"
#define OUTPUT_FILE "/tmp/dyed.env"

int main() {
  char *home_env = getenv("HOME"); // Get User's HOME environment variable
  char config_dir_path[BUF_SIZE];
  char output_file_path[BUF_SIZE];
  int config_dir_fd, output_fd;
  DIR *config_dir;
  struct dirent *entry;
  struct stat dir_stat;
  time_t last_mtime;

  strcpy(config_dir_path, home_env);
  strcat(config_dir_path, CONFIG_DIR); // Create .config/dye/ directory path

  strcpy(output_file_path, OUTPUT_FILE); // Create output file path

  int last_char_was_newline =
      1; // initialize to true to avoid printing empty line at beginning

  while (1) // Eternal Loop to Monitor and Read Configuration Files
  {
    // Check directory mtime to see if it has changed
    if (stat(config_dir_path, &dir_stat) < 0) {
      perror("stat config dir");
      exit(1);
    }

    if (dir_stat.st_mtime != last_mtime) {
      // Directory has changed, re-read configuration files and write to output
      last_mtime = dir_stat.st_mtime;

      // Open the config directory and convert the file descriptor to a
      // directory stream
      config_dir_fd = open(config_dir_path, O_RDONLY | O_DIRECTORY);
      if (config_dir_fd < 0) {
        perror("open config dir");
        exit(1);
      }

      config_dir = fdopendir(config_dir_fd);
      if (config_dir == NULL) {
        perror("fdopendir");
        exit(1);
      }

      // Open the output file, truncating it if it already exists
      output_fd = open(output_file_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
      if (output_fd < 0) {
        perror("open output file");
        closedir(config_dir);
        exit(1);
      }

      // Read each regular file in the config directory and write its contents
      // to the output file
      while ((entry = readdir(config_dir)) != NULL) {
        char file_path[BUF_SIZE];
        int file_fd, bytes_read, i;
        char buf[BUF_SIZE];

        if (entry->d_type != DT_REG) {
          continue; // skip non-regular files
        }
        size_t path_len = strnlen(config_dir_path, sizeof(file_path) - 2);
        snprintf(file_path, sizeof(file_path), "%.*s/%s", (int)path_len,
                 config_dir_path, entry->d_name);
        file_fd = open(file_path, O_RDONLY);
        if (file_fd < 0) {
          perror(file_path);
          continue;
        }

        if (!last_char_was_newline) {
          // Add newline to separate files if last character wasn't newline
          if (write(output_fd, "\n", 1) < 0) {
            perror("write");
            close(file_fd);
            continue;
          }
        }

        // Write file contents to output file, adding newline only at the
        // beginning of each file
        int first_char = 1;
        while ((bytes_read = read(file_fd, buf, sizeof(buf))) > 0) {
          for (i = 0; i < bytes_read; i++) {
            if (buf[i] == '\n') {
              if (!first_char || !last_char_was_newline) {
                if (write(output_fd, "\n", 1) < 0) {
                  perror("write");
                  close(file_fd);
                  break;
                }
                last_char_was_newline = 1;
              }
              first_char = 1;
            } else {
              first_char = 0;
              if (write(output_fd, &buf[i], 1) < 0) {
                perror("write");
                close(file_fd);
                break;
              }
              last_char_was_newline = 0;
            }
          }
        }

        close(file_fd);
      }

      // Close the config directory and the output file
      closedir(config_dir);
      close(output_fd);
    }

    // Sleep for 1 second before monitoring again
    sleep(1);
  }

  return 0;
}
