//
// Handles reading/writing blocks, storing records
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/storage.h"

#include <tgmath.h>

#include "../include/record.h"
#include "../include/block.h"
#define MAX_LINE_LENGTH 256

// Function to print a record (for debugging)
void print_record(const Record *rec) {
    printf("GAME_DATE_EST: %s\n", rec->GAME_DATE_EST);
    printf("TEAM_ID_home: %d\n", rec->TEAM_ID_home);
    printf("PTS_home: %d\n", rec->PTS_home);
    printf("FG_PCT_home: %.2f\n", rec->FG_PCT_home);
    printf("FT_PCT_home: %.2f\n", rec->FT_PCT_home);
    printf("FG3_PCT_home: %.2f\n", rec->FG3_PCT_home);
    printf("AST_home: %d\n", rec->AST_home);
    printf("REB_home: %d\n", rec->REB_home);
    printf("HOME_TEAM_WINS: %d\n", rec->HOME_TEAM_WINS);
}

// Function to read records from a file
int read_records(const char *filename, Record **records, int *num_records) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        return -1;
    }

    // Skip header line
    char line[MAX_LINE_LENGTH];
    if (!fgets(line, sizeof(line), file)) {
        fprintf(stderr, "Empty file.\n");
        fclose(file);
        return -1;
    }

    // Allocate initial memory for records
    int capacity = 100;
    *records = (Record *)malloc(sizeof(Record) * capacity);
    if (!*records) {
        perror("malloc");
        fclose(file);
        return -1;
    }

    // Read records from the file
    *num_records = 0;
    while (fgets(line, sizeof(line), file)) {
        if (*num_records >= capacity) {
            capacity *= 2;
            *records = (Record *)realloc(*records, sizeof(Record) * capacity);
            if (!*records) {
                perror("realloc");
                fclose(file);
                return -1;
            }
        }

        Record rec;
        char *token = strtok(line, "\t");
        if (!token) continue;
        strncpy(rec.GAME_DATE_EST, token, sizeof(rec.GAME_DATE_EST));
        rec.GAME_DATE_EST[10] = '\0';  // Ensure null-termination

        token = strtok(NULL, "\t");
        if (!token) continue;
        rec.TEAM_ID_home = atoi(token);

        token = strtok(NULL, "\t");
        if (!token) continue;
        rec.PTS_home = atoi(token);

        token = strtok(NULL, "\t");
        if (!token) continue;
        rec.FG_PCT_home = atof(token);

        token = strtok(NULL, "\t");
        if (!token) continue;
        rec.FT_PCT_home = atof(token);

        token = strtok(NULL, "\t");
        if (!token) continue;
        rec.FG3_PCT_home = atof(token);

        token = strtok(NULL, "\t");
        if (!token) continue;
        rec.AST_home = atoi(token);

        token = strtok(NULL, "\t");
        if (!token) continue;
        rec.REB_home = atoi(token);

        token = strtok(NULL, "\t\n");
        if (!token) continue;
        rec.HOME_TEAM_WINS = atoi(token);

        (*records)[*num_records] = rec;
        (*num_records)++;
    }

    fclose(file);
    return 0;
}

int write_blocks(const char *filename, Record *records, int num_records, int block_size, int *records_per_block, int *num_blocks) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("fopen");
        return -1;
    }

    // Calculate how many records fit into a block
    *records_per_block = block_size / sizeof(Record);
    *num_blocks = ceil((double)num_records / (*records_per_block));

    // Allocate memory for a block
    Block block;
    block.records = (Record *)malloc(sizeof(Record) * (*records_per_block));
    if (!block.records) {
        perror("malloc");
        fclose(file);
        return -1;
    }

    for (int i = 0; i < *num_blocks; i++) {
        block.record_count = 0;
        for (int j = 0; j < *records_per_block && (i * (*records_per_block) + j) < num_records; j++) {
            block.records[j] = records[i * (*records_per_block) + j];
            block.record_count++;
        }
        // Fill the rest with empty records if necessary
        for (int j = block.record_count; j < *records_per_block; j++) {
            memset(&block.records[j], 0, sizeof(Record));
        }

        fwrite(&block, sizeof(Block), 1, file);
    }

    free(block.records);
    fclose(file);
    return 0;
}

// Function to read a block from a binary file
void read_block(FILE *file, Block *block, int block_num, int records_per_block) {
    fseek(file, block_num * sizeof(Block), SEEK_SET);
    fread(block, sizeof(Block), 1, file);

    // Allocate memory for the records in the block
    block->records = (Record *)malloc(sizeof(Record) * records_per_block);
    if (block->records == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Read the records into the allocated memory
    fread(block->records, sizeof(Record), block->record_count, file);
}

// Function to print the details of a block
void print_block(const Block *block) {
    printf("Block contains %d records:\n", block->record_count);
    for (int i = 0; i < block->record_count; i++) {
        printf("Record %d:\n", i + 1);
        print_record(&block->records[i]);
        printf("\n");
    }
}

// Function to report storage statistics
void report_storage_stats(int record_size, int num_records, int records_per_block, int num_blocks) {
    printf("Storage Statistics:\n");
    printf("-------------------\n");
    printf("Size of a record: %d bytes\n", record_size);
    printf("Number of records: %d\n", num_records);
    printf("Number of records per block: %d\n", records_per_block);
    printf("Number of blocks: %d\n\n", num_blocks);
}