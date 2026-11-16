#include "akinator.h"

// Recursive function to read a full line of text into a char array via pointer
static void read_text_recursive(char *str, int index) {
    int ch = getchar();

    // Base Case: Stop when Enter key is pressed or array limit is reached
    if (ch == '\n' || ch == EOF || index >= 199) {
        *(str + index) = '\0';
        return;
    }

    *(str + index) = (char)ch;

    // Recursive Step: Read next character into next array slot
    read_text_recursive(str, index + 1);
}

// Recursive function that keeps calling itself until a train is guessed
void ask_question(int current_number, struct Question *list, int *total_items) {
    char reply;

    if (current_number < 0 || current_number >= *total_items) {
        printf("\n[Error] Invalid record number (%d).\n", current_number);
        return;
    }

    // Pointer to the current structure in our array
    struct Question *current_ptr = (list + current_number);

    // Stopping Condition (Base Case): This item is a final train guess!
    if (current_ptr->final_train == 1) {
        printf("\n************************************************************\n");
        printf("  I GUESS YOUR TRAIN IS: %s\n", current_ptr->text);
        printf("************************************************************\n");

        printf("Did I guess it right? (y/n): ");
        scanf(" %c", &reply);
        clear_keyboard();

        if (reply == 'y' || reply == 'Y') {
            printf("\nAwesome! I knew I could guess %s!\n", current_ptr->text);
        } else {
            printf("\nOh no, I missed it! Please teach me this train.\n");
            append_train(current_number, list, total_items);
        }
        return; // Stop recursion here
    }

    // Recursive Step: Ask the question and jump to the next number
    printf("\nQ: %s (y/n): ", current_ptr->text);
    scanf(" %c", &reply);
    clear_keyboard();

    if (reply == 'y' || reply == 'Y') {
        int next_num = current_ptr->if_yes;
        ask_question(next_num, list, total_items); // Recursive function call for YES
    }
    else if (reply == 'n' || reply == 'N') {
        int next_num = current_ptr->if_no;
        ask_question(next_num, list, total_items); // Recursive function call for NO
    }
    else {
        printf("[Invalid] Please press only 'y' or 'n'.\n");
        ask_question(current_number, list, total_items); // Repeat the same question
    }
}

// Appends a new train and a new question when the guess is wrong
void append_train(int wrong_train_number, struct Question *list, int *total_items) {
    char new_train[200];
    char new_question[200];
    char old_train[200];
    char answer_for_new;

    // Pointer to the wrong train record in the array
    struct Question *wrong_ptr = (list + wrong_train_number);

    // Keep a copy of the wrong train name
    strcpy(old_train, wrong_ptr->text);

    // Step 1: Ask for the actual train name using recursive text reader
    while (1) {
        printf("\nStep 1: What is the name of your train? ");
        read_text_recursive(new_train, 0);

        if (strlen(new_train) == 0) {
            printf("[Error] Train name cannot be empty. Try again.\n");
        } else if (strcmp(new_train, old_train) == 0) {
            printf("[Error] You entered the same train! Enter a different one.\n");
        } else {
            break;
        }
    }

    // Step 2: Ask for a distinguishing question
    while (1) {
        printf("\nStep 2: Enter a Yes/No question that separates %s from %s:\n> ",
               new_train, old_train);
        read_text_recursive(new_question, 0);

        if (strlen(new_question) > 0) {
            break;
        }
        printf("[Error] Question cannot be empty. Try again.\n");
    }

    // Step 3: Ask what the answer is for the new train
    while (1) {
        printf("\nStep 3: For %s, is the answer to your question Yes or No? (y/n): ", new_train);
        scanf(" %c", &answer_for_new);
        clear_keyboard();

        if (answer_for_new == 'y' || answer_for_new == 'Y' ||
            answer_for_new == 'n' || answer_for_new == 'N') {
            break;
        }
        printf("[Invalid] Please enter 'y' or 'n'.\n");
    }

    // Step 4: Add both trains to the end of our list
    int new_train_pos = add_item(list, total_items, new_train, 1);
    int old_train_pos = add_item(list, total_items, old_train, 1);

    if (new_train_pos == -1 || old_train_pos == -1) {
        printf("[Error] Memory full! Cannot save new train.\n");
        return;
    }

    // Step 5: Replace the old wrong train slot with the new question via pointer
    strcpy(wrong_ptr->text, new_question);
    wrong_ptr->final_train = 0;

    // Step 6: Connect the new question to the two trains
    if (answer_for_new == 'y' || answer_for_new == 'Y') {
        wrong_ptr->if_yes = new_train_pos;
        wrong_ptr->if_no = old_train_pos;
    } else {
        wrong_ptr->if_yes = old_train_pos;
        wrong_ptr->if_no = new_train_pos;
    }

    printf("\n[Learned] Thank you! I have saved %s in my memory.\n", new_train);
}