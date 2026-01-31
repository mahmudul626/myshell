#include "../include/myshell.h"


void auto_complete(char *buffer, size_t *i, size_t size, int last_char) {
    // বাফারের শেষ শব্দটি খুঁজে বের করা (যেমন: ls /home/user/te -> শব্দ হলো 'te')
    char *last_word = buffer;
    for (int j = (int)(*i) - 1; j >= 0; j--) {
        if (buffer[j] == ' ') {
            last_word = &buffer[j + 1];
            break;
        }
    }

    size_t lw_len = strlen(last_word);
    
    // বর্তমান ডিরেক্টরি ওপেন করা
    DIR *dir = opendir("."); 
    if (!dir) return;

    struct dirent *entry;
    char *match = NULL;
    int match_count = 0;

    // ফাইলগুলোর সাথে বাফারের শব্দের মিল খোঁজা
    while ((entry = readdir(dir)) != NULL) {
        // লুকানো ফাইল বাদ দিয়ে strncmp দিয়ে চেক
        if (entry->d_name[0] != '.' && strncmp(entry->d_name, last_word, lw_len) == 0) {
            match = entry->d_name;
            match_count++;
        }
    }

    if (match_count == 1) {
        // ১. একটি মাত্র মিল থাকলে: অটো-কমপ্লিট
        size_t match_len = strlen(match);
        for (size_t j = lw_len; j < match_len && *i < size - 1; j++) {
            buffer[(*i)++] = match[j];
            putchar(match[j]);
        }
        fflush(stdout);
    } 
    else if (match_count > 1 && last_char == '\t') {
        // ২. একাধিক মিল থাকলে এবং ডাবল ট্যাব দিলে: লিস্ট দেখানো
        printf("\n");
        rewinddir(dir);
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] != '.' && strncmp(entry->d_name, last_word, lw_len) == 0) {
                printf("%s  ", entry->d_name);
            }
        }
        // আবার প্রম্পট ফিরিয়ে আনা (এখানে আপনার শেলের নাম ব্যবহার করুন)
        printf("\nmyShell> %.*s", (int)(*i), buffer);
        fflush(stdout);
    }

    closedir(dir);
}