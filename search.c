//
// Implements search algorithms (brute-force and B+ tree search)
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "record.h"
#include "block.h"
#include "search.h"

// Function to search for records within a range using B+ tree search
SearchResult brute_force_search(const char *data_filename, float lower, float upper, int *blocks_accessed, float *average_FG3_PCT_home, double *time_taken) {
    clock_t start = clock();

    FILE *data_file = fopen(data_filename, "rb");
    if (!data_file) {
        perror("fopen data_file");
        SearchResult empty = {NULL, 0};
        return empty;
    }

    // Determine file size
    fseek(data_file, 0, SEEK_END);
    long file_size = ftell(data_file);
    fseek(data_file, 0, SEEK_SET);

    int records_per_block = BLOCK_SIZE / sizeof(Record);
    int num_blocks = file_size / sizeof(Block);
    *blocks_accessed = num_blocks;

    SearchResult result;
    result.records = NULL;
    result.count = 0;

    float sum_FG3_PCT_home = 0.0;
    int capacity = 100;
    result.records = (Record *)malloc(sizeof(Record) * capacity);
    if (!result.records) {
        perror("malloc");
        fclose(data_file);
        return result;
    }

    for (int i = 0; i < num_blocks; i++) {
        Block block;
        fread(&block, sizeof(Block), 1, data_file);

        for (int j = 0; j < block.record_count; j++) {
            Record rec = block.records[j];
            if (rec.FG_PCT_home >= lower && rec.FG_PCT_home <= upper) {
                if (result.count >= capacity) {
                    capacity *= 2;
                    result.records = (Record *)realloc(result.records, sizeof(Record) * capacity);
                    if (!result.records) {
                        perror("realloc");
                        fclose(data_file);
                        SearchResult empty = {NULL, 0};
                        return empty;
                    }
                }
                result.records[result.count++] = rec;
                sum_FG3_PCT_home += rec.FG3_PCT_home;
            }
        }
    }

    fclose(data_file);

    *average_FG3_PCT_home = (result.count > 0) ? (sum_FG3_PCT_home / result.count) : 0.0;
    clock_t end = clock();
    *time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    return result;
}


// Function to report search statistics
void report_search_stats(int index_nodes_accessed, int data_blocks_accessed, float average_FG3_PCT_home, double retrieval_time, int brute_force_blocks, double brute_force_time) {
    printf("Search Statistics:\n");
    printf("-------------------\n");
    printf("Number of index nodes accessed: %d\n", index_nodes_accessed);
    printf("Number of data blocks accessed: %d\n", data_blocks_accessed);
    printf("Average FG3_PCT_home of returned records: %.3f\n", average_FG3_PCT_home);
    printf("Running time of retrieval process: %.6lf seconds\n", retrieval_time);
    printf("Number of data blocks accessed by brute-force: %d\n", brute_force_blocks);
    printf("Running time of brute-force method: %.6lf seconds\n", brute_force_time);
}