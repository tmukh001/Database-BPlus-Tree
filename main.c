//
// Main file to drive the project
//

#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "../include/record.h"
#include "../include/block.h"
#include "../include/storage.h"
#include "../include/bplustree.h"

// Definitions
#define DATA_FILE "../data/data.db"  // Binary file storing the records in blocks
#define INDEX_FILE "../output/index.db"  // Binary file storing the B+ tree index


int main() {
    // Task 1: Storage Component
    Record *records = NULL;
    int num_records = 0;
    if (read_records("../data/games.txt", &records, &num_records) != 0) {
        fprintf(stderr, "Failed to read records.\n");
        return EXIT_FAILURE;
    }

    // Test: Print the first record
    // if (num_records > 0) {
    //     printf("Size of 1 Record struct: %zu bytes\n", sizeof(Record));
    //     print_record(&records[0]);
    // }

    int records_per_block = BLOCK_SIZE / sizeof(Record);
    int num_blocks = ceil((double)num_records / records_per_block);

    // Write the records back to a binary file
    if (write_blocks(DATA_FILE, records, num_records, BLOCK_SIZE, &records_per_block, &num_blocks) != 0) {
        fprintf(stderr, "Failed to write blocks.\n");
        free(records);
        return EXIT_FAILURE;
    }

    // Test: Read the first block and print it
    // Block *block = (Block *)malloc(sizeof(Block));
    // FILE *file = fopen(DATA_FILE, "rb");
    // if (file == NULL) {
    //     perror("fopen");
    //     free(block->records);
    //     free(block);
    //     free(records);
    //     return EXIT_FAILURE;
    // }
    // fclose(file);
    // read_block(file, block, 0, records_per_block);
    // print_block(block);

    // Report storage statistics
    report_storage_stats(sizeof(Record), num_records, records_per_block, num_blocks);


    // free(block);
    free(records);

    // TODO: Task 2: Design and implement the indexing component


    // TODO: Task 3: Search Component

    return EXIT_SUCCESS;
}
