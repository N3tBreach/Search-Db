#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_RESULTS 10
#define MAX_THREADS 10

typedef struct {
    const char* file_path;
    const char* search_term;
    int* results_found;
} thread_data_t;

pthread_mutex_t result_mutex;  

void* search_in_file_thread(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    FILE* file = fopen(data->file_path, "r");
    if (file == NULL) {
        return NULL;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL) {
        pthread_mutex_lock(&result_mutex);  
        if (*(data->results_found) >= MAX_RESULTS) {
            pthread_mutex_unlock(&result_mutex);
            break;
        }
        pthread_mutex_unlock(&result_mutex);

        if (strstr(line, data->search_term) != NULL) {
            pthread_mutex_lock(&result_mutex);  
            if (*(data->results_found) < MAX_RESULTS) {
                printf("Found: %s\n", line);
                (*(data->results_found))++;
            }
            pthread_mutex_unlock(&result_mutex);
        }
    }
    fclose(file);
    return NULL;
}

void search_files(const char* directory_path, const char* search_term) {
    DIR* dir = opendir(directory_path);
    if (dir == NULL) {
        printf("Ne peut pas ouvrir: %s\n", directory_path);
        return;
    }

    struct dirent* entry;
    struct stat file_stat;
    int results_found = 0;
    pthread_t threads[MAX_THREADS];  
    int thread_count = 0;
    
    while ((entry = readdir(dir)) != NULL && results_found < MAX_RESULTS) {
        char file_path[1024];
        snprintf(file_path, sizeof(file_path), "%s/%s", directory_path, entry->d_name);

        
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        
        if (stat(file_path, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
            const char* file_name = entry->d_name;
            if (strstr(file_name, ".txt") || strstr(file_name, ".csv") || strstr(file_name, ".sql")) {
               
                thread_data_t* data = (thread_data_t*)malloc(sizeof(thread_data_t));
                data->file_path = strdup(file_path);  
                data->search_term = search_term;
                data->results_found = &results_found;

                
                pthread_create(&threads[thread_count++], NULL, search_in_file_thread, (void*)data);

                
                if (thread_count >= MAX_THREADS) {
                    for (int i = 0; i < thread_count; i++) {
                        pthread_join(threads[i], NULL);
                    }
                    thread_count = 0;  
                }
            }
        }
    }

    
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    closedir(dir);
}

int main() {
    pthread_mutex_init(&result_mutex, NULL);  

    char directory_path[256] = "C:/Users/nicol/Desktop/db"; 
    char search_term[256];

    printf("Entre ce que tu veux chercher (ou 'q' pour quitter): ");
    fgets(search_term, sizeof(search_term), stdin);
    search_term[strcspn(search_term, "\n")] = 0;  

    while (strcmp(search_term, "q") != 0) {
        search_files(directory_path, search_term);

        printf("Entre ce que tu veux chercher (ou 'q' pour quitter): ");
        fgets(search_term, sizeof(search_term), stdin);
        search_term[strcspn(search_term, "\n")] = 0;
    }

    pthread_mutex_destroy(&result_mutex);  
    return 0;
}
