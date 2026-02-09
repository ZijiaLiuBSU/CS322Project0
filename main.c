/* Project 0 - Input Validation and Buffer Overruns
 * Author: TODO: Zijia Liu
 * Purpose: create a program that does not validate input and leads to a buffer
 *          overflow, and allows memory to directly accessed.  Also, write
 *          more secure versions of the functions that do not have these
 *          vulnerabilities.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h> /* contains constant USHRT_MAX */
#include <stdlib.h> /* immediately exit the program with exit(0) */

#define MAX_USERS 10    /* maximum number of users allowed */
#define MAX_NAME_LEN 39 /* maximum number of letters in username allowed */
#define EXIT_VALUE 99   /* what to type to quit the loop */

void print_this_user_info(unsigned short userindex, char username[], unsigned short userpin, bool userIsAdmin);
bool get_user_preference();
int get_user_to_modify_vulnerable();
void change_pin_vulnerable(int user_i, unsigned short u_pin[], int new_pin);
int get_user_to_modify_more_secure(int current_num_users);
bool change_pin_more_secure(int u_index, unsigned short u_pin[], int new_pin);

int main(void) {
    struct {
        unsigned short user_isAdmin[MAX_USERS];   /* an array, true if this user is an admin */
        unsigned short user_pin[MAX_USERS];       /* an array, user's PIN, in decimal form */
        char user_name[MAX_USERS][MAX_NAME_LEN]; /* an array of character strings */
    } user_data;                    /* declare one variable to hold all user information */

    int i = 0;                      /* loop counter */
    int num_users = 0;              /* how many users do we have? */
    int user_index = 0;             /* which user to work with */
    int new_pin = 0;                /* new value for pin */
    char buffer[256] = "";          /* read from the keyboard */
    bool vulnerable_mode = false;   /* user preference to run vulnerable functions, or not */
    bool success = false;           /* did the pin change succeed */

    /******* set up default user accounts *******/
    /* zero out all memory in the user_data variable -- for each array */
    memset(user_data.user_isAdmin, 0, sizeof(user_data.user_isAdmin));
    memset(user_data.user_pin, 0, sizeof(user_data.user_pin));
    memset(user_data.user_name, '-', sizeof(user_data.user_name));   // this makes it easier to see in memory.

    /* user at index 0 is the administrator, with PIN 16962 */
    user_data.user_pin[0] = 16962; // 4242 in hex
    user_data.user_isAdmin[0] = true;
    strncpy(user_data.user_name[0], "ADMIN", strlen("ADMIN") + 1);

    /* user at index 1 is the default user, with PIN 4369 */
    user_data.user_pin[1] = 4369; // 1111 in hex
    user_data.user_isAdmin[1] = false;
    strncpy(user_data.user_name[1], "DEFAULT USER", strlen("DEFAULT USER") + 1);

    /* we have 2 users so far */
    num_users = 2;

    /******* does the user want to run vulnerable code? *******/
    vulnerable_mode = get_user_preference();

    /******* loop so that we have a chance to do fun things *******/
    while (true) {

        /* print out this information (info leak, but helps us learn) */
        for (i = 0; i < num_users; i++) {
            print_this_user_info(i, user_data.user_name[i],
                                 user_data.user_pin[i], user_data.user_isAdmin[i]);
        }
        printf("-------------\n");

        /******* Execute vulnerable code, or not, depending on user choice *******/

        /* if the user chose to live dangerously and run vulnerable functions */
        if (vulnerable_mode) {
            /* prompt user for which user they want to work with, using get_user_to_modify_vulnerable() */
            user_index = get_user_to_modify_vulnerable();

            /* prompt user for new PIN (intentionally no validation here) */
            printf("Enter new PIN (decimal): ");
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                exit(0);
            }
            /* intentionally not checking sscanf result */
            sscanf(buffer, "%d", &new_pin);

            /* change the pin using the function, change_pin_vulnerable */
            change_pin_vulnerable(user_index, user_data.user_pin, new_pin);
        }
        /* otherwise, run the more secure functions */
        else {
            /* prompt user for which user they want to work with */
            user_index = get_user_to_modify_more_secure(num_users);

            /* prompt user for new PIN (re-prompt until success) */
            success = false;
            while (!success) {
                printf("Enter new PIN (0-%u) (decimal): ", USHRT_MAX);

                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    exit(0);
                }

                if (sscanf(buffer, "%d", &new_pin) != 1) {
                    printf("Invalid input. Please enter an integer.\n");
                    continue;
                }

                success = change_pin_more_secure(user_index, user_data.user_pin, new_pin);

                if (!success) {
                    printf("PIN change failed. Try again.\n");
                }
            }
        }

        printf("\n");
    }

    /* exit program */
    return 0;
}

/* Purpose: print all information
 *          -- revealing PINS is bad! but helps us understand
 * Returns: nothing */
void print_this_user_info(unsigned short userindex, char username[],
                          unsigned short userpin, bool userIsAdmin) {
    /* print one user at a time */
    printf("---User Index#%d---\nName: %s  \nPIN: %u  \tisAdmin: %i\n",
           userindex, username, userpin, userIsAdmin);
}

/* Purpose: Ask the user if they want to run the vulnerable version, or not.
 *          Print a menu.
 *          Read from the keyboard.  If the user enters a 1,
 *          return true (vulnerable option)
 *          otherwise, return false (not-vulnerable option).
 * Returns: true - if user chose to be vulnerable, false - otherwise */
bool get_user_preference() {
    char buffer[256] = "";          /* read from the keyboard */
    int selection = 0;              /* user's choice */

    /* (a) print a simple menu */
    printf("Choose mode:\n");
    printf("  1) Run vulnerable code\n");
    printf("  2) Run secure code (default)\n");
    printf("Enter choice: ");

    /* (b) read input from keyboard using fgets() and sscanf() with %d */
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        sscanf(buffer, "%d", &selection);
    }

    /* (c) if the user enters 1, return true; (d) otherwise return false */
    if (selection == 1) {
        return true;
    }
    return false;
}

/* Purpose:  Read from the keyboard.
 *           No input validation is done in this function, so it is vulnerable.
 * Returns:  The (unvalidated) integer index that the user wants to modify. */
int get_user_to_modify_vulnerable(void) {
    char buffer[256] = "";          /* read from the keyboard */
    int desired_index = 0;          /* index of user to modify */

    printf("Enter user index to modify (or %d to quit): ", EXIT_VALUE);

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        exit(0);
    }

    /* intentionally not checking return value */
    sscanf(buffer, "%d", &desired_index);

    if (desired_index == EXIT_VALUE) {
        exit(0);
    }

    return desired_index;
}

/* Purpose: When passed the user's index number (user_i),
 *          the entire pin array (u_pin[]), and
 *          the new pin (new_pin),
 *          reset that user's pin.
 *          Do not do any input validation in this intentionally vulnerable function.
 * Returns: nothing, but may have some vulnerabilities */
void change_pin_vulnerable(int user_i, unsigned short u_pin[], int new_pin) {
    /* intentionally no validation: buffer over/underflow possible */
    u_pin[user_i] = (unsigned short)new_pin;
}

/* Purpose:  Read from the keyboard.
 *           Verify that value entered is valid. Re-prompt until satisfied.
 * Returns:  the (validated) integer index that the user wants to modify. */
int get_user_to_modify_more_secure(int current_num_users) {
    char buffer[256] = "";
    int desired_index = 0;

    while (true) {
        printf("Enter user index (0-%d) to modify (or %d to quit): ",
               current_num_users - 1, EXIT_VALUE);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            exit(0);
        }

        if (sscanf(buffer, "%d", &desired_index) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }

        if (desired_index == EXIT_VALUE) {
            exit(0);
        }

        if (desired_index < 0 || desired_index >= current_num_users) {
            printf("Invalid index. Try again.\n");
            continue;
        }

        return desired_index;
    }
}

/* Purpose: When passed the user's index number (user_i),
 *          the entire pin array (u_pin[]), and
 *          the new pin (new_pin),
 *          reset that user's pin.
 * Returns: true - if successfully changed, false - if unchanged */
bool change_pin_more_secure(int user_i, unsigned short u_pin[], int new_pin) {
    /* validate index (defensive check against full array size) */
    if (user_i < 0 || user_i >= MAX_USERS) {
        return false;
    }

    /* validate pin range */
    if (new_pin < 0 || new_pin > USHRT_MAX) {
        return false;
    }

    u_pin[user_i] = (unsigned short)new_pin;
    return true;
}
