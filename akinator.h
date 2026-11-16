#include <stdio.h>
#include <string.h>

// A simple structure for each item in our list
struct Question {
    int number;             // Serial number (0, 1, 2...)
    char text[200];         // Either a Question OR a Train Name
    int final_train;        // 1 = Final Train Name, 0 = Question to ask
    int if_yes;             // Serial number to go to if user says Yes
    int if_no;              // Serial number to go to if user says No
};

// Module 2 
void clear_keyboard();
int add_item(struct Question *list, int *total_items, char text[], int final_train);
void load_default_data(struct Question *list, int *total_items);
void show_all_trains(struct Question *list, int total_items);
void show_statistics(struct Question *list, int total_items);

// Module 3 
void save_file(char file_name[], struct Question *list, int total_items);
int read_file(char file_name[], struct Question *list, int *total_items);

// Module 4
void ask_question(int current_number, struct Question *list, int *total_items);
void append_train(int wrong_train_number, struct Question *list, int *total_items);