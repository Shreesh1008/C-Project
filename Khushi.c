#include "akinator.h"

// Clears extra Enter keys left in input buffer after scanf
void clear_keyboard() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// Adds a new question or train to our array using struct and int pointers
int add_item(struct Question *list, int *total_items, char text[], int final_train) {
    char *text_ptr = text;

    if (*total_items >= 100) {
        printf("\n[Error] Memory list is full! Cannot add more items.\n");
        return -1;
    }

    int pos = *total_items;
    struct Question *item_ptr = (list + pos);

    item_ptr->number = pos;
    strcpy(item_ptr->text, text_ptr);
    item_ptr->final_train = final_train;
    item_ptr->if_yes = -1;
    item_ptr->if_no = -1;

    (*total_items)++;
    return pos;
}

// Creates a default set of questions and trains
void load_default_data(struct Question *list, int *total_items) {
    *total_items = 0;

    int q0 = add_item(list, total_items, "Is it a semi-high-speed train without general unreserved coaches?", 0);
    int q1 = add_item(list, total_items, "Does it have automatic sliding doors and rotating seats?", 0);
    int q2 = add_item(list, total_items, "Is it a fully air-conditioned long-distance train?", 0);

    int t1 = add_item(list, total_items, "Vande Bharat Express", 1);
    int t2 = add_item(list, total_items, "Shatabdi Express", 1);
    int t3 = add_item(list, total_items, "Rajdhani Express", 1);
    int t4 = add_item(list, total_items, "Garib Rath Express", 1);

    // Link each question to its Yes and No destinations via pointers
    (list + q0)->if_yes = q1;
    (list + q0)->if_no = q2;

    (list + q1)->if_yes = t1;
    (list + q1)->if_no = t2;

    (list + q2)->if_yes = t3;
    (list + q2)->if_no = t4;
}

// Recursive helper to print all final train names using pointer arithmetic
static int print_trains_recursive(struct Question *arr, int total_items, int index, int count) {
    // Base Case: Reached end of active records
    if (index >= total_items) {
        return count;
    }

    struct Question *ptr = (arr + index);
    if (ptr->final_train == 1) {
        count++;
        printf("%2d. %s (Serial #%d, Length: %d chars)\n",
               count, ptr->text, ptr->number, (int)strlen(ptr->text));
    }

    // Recursive Step: Move to the next record
    return print_trains_recursive(arr, total_items, index + 1, count);
}

// Prints all known trains in memory
void show_all_trains(struct Question *list, int total_items) {
    char header[50];
    strcpy(header, "LIST OF KNOWN TRAINS");

    printf("\n============================================================\n");
    printf("                    %s                    \n", header);
    printf("============================================================\n");

    int count = print_trains_recursive(list, total_items, 0, 0);

    if (count == 0) {
        printf("No trains found in memory.\n");
    } else {
        printf("------------------------------------------------------------\n");
        printf("Total Trains Displayed: %d\n", count);
    }
    printf("============================================================\n");
}

// Recursive helper to count questions (target = 0) or trains (target = 1)
static int count_by_type_recursive(struct Question *arr, int total_items, int index, int count, int target) {
    // Base Case: End of list reached
    if (index >= total_items) {
        return count;
    }

    if ((arr + index)->final_train == target) {
        count++;
    }

    // Recursive Step: Check next element
    return count_by_type_recursive(arr, total_items, index + 1, count, target);
}

// Counts and displays total questions vs total trains stored
void show_statistics(struct Question *list, int total_items) {
    char title[50];
    strcpy(title, "MEMORY & ");
    strcat(title, "STATISTICS");

    int question_count = count_by_type_recursive(list, total_items, 0, 0, 0);
    int train_count = count_by_type_recursive(list, total_items, 0, 0, 1);

    printf("\n============================================================\n");
    printf("                     %s                      \n", title);
    printf("============================================================\n");
    printf("Total Questions Stored     : %d\n", question_count);
    printf("Total Trains Known         : %d\n", train_count);
    printf("Total Records Used         : %d / 100\n", total_items);
    printf("Free Slots Remaining       : %d\n", 100 - total_items);
}
