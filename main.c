#include "weiner.h"

// reads entire file into memory
static char* read_text_file(const char* path){
    FILE* f = fopen(path, "rb");
    if (!f){
        perror("fopen");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char* buf = (char*)malloc(size + 2);
    if (!buf){
        perror("malloc");
        fclose(f);
        return NULL;
    }

    fread(buf, 1, size, f);
    fclose(f);

    buf[size] = '\0';

    // remove trailing whitespace/newlines
    while (size > 0 && (buf[size-1] == '\n' || buf[size-1] == '\r' || buf[size-1] == ' ')){
        buf[size-1] = '\0';
        size--;
    }

    return buf;
}

// ensures a unique sentinel at the end of the text
static char* ensure_sentinel(char* text){
    size_t n = strlen(text);
    if (n == 0 || text[n-1] == '$') return text;

    char* out = (char*)malloc(n + 2);
    memcpy(out, text, n);
    out[n] = '$';
    out[n+1] = '\0';

    free(text);
    return out;
}

int main(int argc, char** argv){
    if (argc < 2){
        printf("Usage: %s <text_file>\n", argv[0]);
        return 1;
    }

    char* text = read_text_file(argv[1]);
    if (!text){
        printf("Error reading input file\n");
        return 1;
    }

    // ensure unique sentinel at the end
    text = ensure_sentinel(text);

    printf("Text size: %zu characters\n", strlen(text));

    // create suffix tree
    Suf_tree* st = new_suf_tree(text);
    if (!st){
        printf("Error creating suffix tree\n");
        free(text);
        return 1;
    }

    // measure construction time
    clock_t start = clock();
    weiner_construction(st);
    clock_t end = clock();

    double build_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Tree construction time: %.6f seconds\n", build_time);

    // search loop
    char factor[1024];
    while (1){
        printf("\nType a factor to search (or 'exit'): ");
        if (!fgets(factor, sizeof(factor), stdin)) break;

        factor[strcspn(factor, "\r\n")] = '\0';
        if (strcmp(factor, "exit") == 0) break;
        if (factor[0] == '\0') continue;

        start = clock();

        boolean found = search(st, factor);

        end = clock();
        double search_time = (double)(end - start) / CLOCKS_PER_SEC;

        printf("found = %d | time = %.6f seconds\n", found, search_time);
    }

    // cleanup tree freeing not implemented yet
    free(text);
    free(st);

    return 0;
}
