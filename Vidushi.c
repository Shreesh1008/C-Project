#include "akinator.h"

// Recursively writes each structure in the array to the file
static void write_records_recursive(FILE *fp, struct Question *arr, int total_items, int index) {
    // Base Case: Stop when all records are written
    if (index >= total_items) {
        return;
    }

    struct Question *ptr = (arr + index);
    fprintf(fp, "%d|%d|%d|%d|%s\n",
            ptr->number,
            ptr->final_train,
            ptr->if_yes,
            ptr->if_no,
            ptr->text);

    // Recursive Step: Write the next structure in the array
    write_records_recursive(fp, arr, total_items, index + 1);
}

// Writes all structures from our array into akinator.txt
void save_file(char file_name[], struct Question *list, int total_items) {
    char *file_ptr = file_name;
    FILE *fp = fopen(file_ptr, "w");

    if (fp == NULL) {
        printf("\n[Error] Unable to open '%s' for writing!\n", file_ptr);
        return;
    }

    // Write the total count on the first line
    fprintf(fp, "%d\n", total_items);

    // Recursively write every record from index 0
    write_records_recursive(fp, list, total_items, 0);

    fclose(fp);
}

// Recursively reads records from akinator.txt into the structure array
static int read_records_recursive(FILE *fp, struct Question *arr, int total_items, int index) {
    // Base Case: Stop when all expected records have been read
    if (index >= total_items) {
        return index;
    }

    struct Question *ptr = (arr + index);
    int read_count = fscanf(fp, "%d|%d|%d|%d|",
                            &ptr->number,
                            &ptr->final_train,
                            &ptr->if_yes,
                            &ptr->if_no);

    if (read_count != 4) {
        printf("\n[Warning] Stopped reading early at record %d.\n", index);
        return index;
    }

    // Read the full sentence including spaces
    if (fgets(ptr->text, 200, fp) != NULL) {
        int length = (int)strlen(ptr->text);
        if (length > 0 && *(ptr->text + length - 1) == '\n') {
            *(ptr->text + length - 1) = '\0';
        }
    }

    // Recursive Step: Read the next line into the next array slot
    return read_records_recursive(fp, arr, total_items, index + 1);
}

// Reads all structures from akinator.txt back into our array
int read_file(char file_name[], struct Question *list, int *total_items) {
    char *file_ptr = file_name;
    FILE *fp = fopen(file_ptr, "r");

    if (fp == NULL) {
        return 0; // File does not exist yet
    }

    // Read total count from first line
    if (fscanf(fp, "%d\n", total_items) != 1) {
        fclose(fp);
        return 0;
    }

    if (*total_items <= 0 || *total_items > 100) {
        *total_items = 0;
        fclose(fp);
        return 0;
    }

    // Recursively load each record into list[]
    *total_items = read_records_recursive(fp, list, *total_items, 0);

    fclose(fp);
    return 1; // Success
}