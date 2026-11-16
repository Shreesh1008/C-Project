#include "akinator.h"

// Prints a divider line and centered title using a char pointer
static void print_header(char *title) {
    int len = (int)strlen(title);
    printf("\n============================================================\n");
    if (len > 0) {
        printf("  %s\n", title);
    }
    printf("============================================================\n");
}

// Updates session counters using pointer arithmetic
// stats_ptr[0] = games played, stats_ptr[1] = resets performed
static void update_session(int *stats_ptr, int index) {
    *(stats_ptr + index) = *(stats_ptr + index) + 1;
}

// Recursive menu loop that repeats until the user selects Exit (6)
static void menu_loop(int *stats_ptr, char *file_ptr, struct Question *list, int *total_items) {
    int choice;
    char status_msg[200];

    // 2D character array (array of strings) storing menu options
    char menu_items[6][45] = {
        "1. Play Train Guessing Game",
        "2. View All Known Trains",
        "3. View Memory & Session Statistics",
        "4. How to Play",
        "5. Reset Data to Default Trains",
        "6. Exit Program"
    };

    printf("\n------------------------------------------------------------\n");
    printf("                         MAIN MENU                          \n");
    printf("------------------------------------------------------------\n");

    // Print each string using pointer notation
    for (int i = 0; i < 6; i++) {
        printf("%s\n", *(menu_items + i));
    }

    printf("------------------------------------------------------------\n");
    printf("Enter your choice (1-6): ");

    if (scanf("%d", &choice) != 1) {
        clear_keyboard();
        printf("\n[Error] Invalid input! Please enter a number from 1 to 6.\n");
        menu_loop(stats_ptr, file_ptr, list, total_items);
        return;
    }
    clear_keyboard();

    // Base Case: Exit program when user selects 6
    if (choice == 6) {
        strcpy(status_msg, "Saving all records before closing...");
        printf("\n%s (%s)\n", status_msg, file_ptr);
        save_file(file_ptr, list, *total_items);
        printf("Goodbye! Thanks for playing Train Akinator.\n");
        return;
    }

    if (choice == 1) {
        print_header("THINK OF ANY INDIAN RAILWAYS TRAIN IN YOUR MIND!");
        printf("Answer each question using 'y' for Yes or 'n' for No.\n");

        ask_question(0, list, total_items);
        update_session(stats_ptr, 0);
        save_file(file_ptr, list, *total_items);
    }
    else if (choice == 2) {
        show_all_trains(list, *total_items);
    }
    else if (choice == 3) {
        show_statistics(list, *total_items);
        printf("Games Played This Session  : %d\n", *(stats_ptr + 0));
        printf("Database Resets Done       : %d\n", *(stats_ptr + 1));
        printf("============================================================\n");
    }
    else if (choice == 4) {
        print_header("HOW TO PLAY TRAIN AKINATOR");
        printf("1. Think of an Indian Railways train before starting.\n");
        printf("2. Answer each Yes/No question with 'y' or 'n'.\n");
        printf("3. If my guess is wrong, teach me the train and a question!\n");
        printf("============================================================\n");
    }
    else if (choice == 5) {
        char confirm;
        printf("\nAre you sure you want to erase learned trains? (y/n): ");
        scanf(" %c", &confirm);
        clear_keyboard();

        if (confirm == 'y' || confirm == 'Y') {
            load_default_data(list, total_items);
            save_file(file_ptr, list, *total_items);
            update_session(stats_ptr, 1);
            strcpy(status_msg, "[Success] Reset complete! Default trains restored.");
            printf("\n%s\n", status_msg);
        } else {
            strcpy(status_msg, "[Info] Reset cancelled.");
            printf("\n%s\n", status_msg);
        }
    }
    else {
        printf("\n[Error] Wrong choice! Please select a number between 1 and 6.\n");
    }

    // Recursive Step: Show the menu again
    menu_loop(stats_ptr, file_ptr, list, total_items);
}

int main() {
    // Local array of structures and counter (no extern/global variables needed)
    struct Question list[100];
    int total_items = 0;

    // Array tracking [0] = games played, [1] = resets done
    int session_data[2] = {0, 0};
    char file_name[30];

    strcpy(file_name, "akinator.txt");
    print_header("INDIAN RAILWAYS TRAIN AKINATOR");

    // Step 1: Load saved questions and trains from akinator.txt
    int file_loaded = read_file(file_name, list, &total_items);

    if (file_loaded == 0) {
        printf("\n[Notice] '%s' was not found.\n", file_name);
        printf("[Notice] Creating a fresh starter list of trains...\n");
        load_default_data(list, &total_items);
        save_file(file_name, list, total_items);
    } else {
        printf("\n[Success] Loaded %d records from '%s'!\n", total_items, file_name);
    }

    // Step 2: Start recursive menu passing array and variable pointers
    menu_loop(session_data, file_name, list, &total_items);

    return 0;
}