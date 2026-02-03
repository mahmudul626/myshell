#include "../include/myshell.h"


int already_exists(char *matches[], int count, const char *name) {
    for (int i = 0; i < count; i++) {
        if (strcmp(matches[i], name) == 0) return 1;
    }
    return 0;
}

int compare_strings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void auto_complete(char *buffer, size_t *i, size_t size, int last_char) {
    char *last_word = buffer;
    int is_command = 1;

    // Find last word
    for (int j = (int)(*i) - 1; j >= 0; j--) {
        if (buffer[j] == ' ') {
            last_word = &buffer[j + 1];
            is_command = 0;
            break;
        }
    }

    size_t lw_len = strlen(last_word);
    if (lw_len == 0 && !is_command) return;

    char *matches[512];
    int match_count = 0;

    if (is_command) {
        // PATH command completion
        char *path_env = getenv("PATH");
        if (path_env) {
            char *path_dup = strdup(path_env);
            char *dir_path = strtok(path_dup, ":");
            while (dir_path != NULL && match_count < 512) {
                DIR *d = opendir(dir_path);
                if (d) {
                    struct dirent *ent;
                    while ((ent = readdir(d)) != NULL) {
                        if (strncmp(ent->d_name, last_word, lw_len) == 0) {
                            if (!already_exists(matches, match_count, ent->d_name)) {
                                matches[match_count++] = strdup(ent->d_name);
                            }
                        }
                        if (match_count >= 512) break;
                    }
                    closedir(d);
                }
                dir_path = strtok(NULL, ":");
            }
            free(path_dup);
        }
    } else {
        // Determine path and prefix
        char dir_path[1024];
        char prefix[256];
        const char *slash = strrchr(last_word, '/');
        if (slash) {
            strncpy(dir_path, last_word, slash - last_word + 1);
            dir_path[slash - last_word + 1] = '\0';
            strcpy(prefix, slash + 1);
        } else {
            strcpy(dir_path, ".");
            strcpy(prefix, last_word);
        }

        DIR *dir = opendir(dir_path);
        if (dir) {
            struct dirent *entry;
            while ((entry = readdir(dir)) != NULL && match_count < 512) {
                if (strncmp(entry->d_name, prefix, strlen(prefix)) == 0) {
                    if (!already_exists(matches, match_count, entry->d_name)) {
                        matches[match_count++] = strdup(entry->d_name);
                    }
                }
            }
            closedir(dir);
        }

        
        if (strncmp(prefix, ".", strlen(prefix)) == 0) {
            if (!already_exists(matches, match_count, ".")) matches[match_count++] = strdup(".");
            if (!already_exists(matches, match_count, "..")) matches[match_count++] = strdup("..");
        }
    }

    if (match_count == 1) {
        // one match → autocomplete
        char *name = matches[0];
        char full_path[1024];

        if (strrchr(last_word, '/')) {
            snprintf(full_path, sizeof(full_path), "%.*s%s", (int)(strrchr(last_word,'/') - last_word + 1), last_word, name);
        } else {
            snprintf(full_path, sizeof(full_path), "%s", name);
        }

        struct stat st;
        if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode)) {
            strcat(full_path, "/"); // if directory  /
        }

        for (size_t j = lw_len; j < strlen(full_path) && *i < size - 1; j++) {
            buffer[(*i)++] = full_path[j];
            putchar(full_path[j]);
        }
        fflush(stdout);
    } else if (match_count > 1 && last_char == '\t') {
        // multy match → display neatly
        qsort(matches, match_count, sizeof(char *), compare_strings);
        printf("\n");

        if (match_count > 50) {
            printf("Display all %d possibilities? (y or n) ", match_count);
            fflush(stdout);
            int ch = getchar();
            if (ch != 'y' && ch != 'Y') {
                printf("\n");
                goto cleanup;
            }
            printf("\n");
        }

        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        int max_len = 0;
        for (int j = 0; j < match_count; j++) {
            int len = strlen(matches[j]);
            if (len > max_len) max_len = len;
        }
        max_len += 2;
        int cols = w.ws_col / max_len;
        if (cols == 0) cols = 1;

        for (int j = 0; j < match_count; j++) {
            printf("%-*s", max_len, matches[j]);
            if ((j + 1) % cols == 0) printf("\n");
        }
        if (match_count % cols != 0) printf("\n");

        cleanup:
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("\033[1;32mmyShell:\033[1;34m%s\033[0m$ %.*s", cwd, (int)(*i), buffer);
        fflush(stdout);
    }

    for (int j = 0; j < match_count; j++) free(matches[j]);
}
