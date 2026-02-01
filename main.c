/* Project 0 - Input Validation and Buffer Overruns
 * Author: TODO: add your name here
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
    strncpy(user_data.user_name[0], "ADMIN", strlen("ADMIN")+1);
    /* user at index 1 is the default user, with PIN  */
    user_data.user_pin[1] = 4369; // 1111 in hex
    user_data.user_isAdmin[1] = false;
    strncpy(user_data.user_name[1], "DEFAULT USER", strlen("DEFAULT USER")+1);
    /* we have 2 users so far */
    num_users = 2;

    /******* does the user want to run vulnerable code? *******/
    vulnerable_mode = get_user_preference();

    /* TODO:  Write this part */
    /******* loop so that we have a chance to do fun things *******/
        /* print out this information (info leak, but helps us learn) */
        for (i = 0; i < num_users; i++) {
            print_this_user_info(i, user_data.user_name[i],
                                 user_data.user_pin[i], user_data.user_isAdmin[i]);
        }
        printf("-------------\n");

        /******* Execute vulnerable code, or not, depending on user choice *******/
        /* if the user chose to live dangerously and run vulnerable functions */
            /* prompt user for which user they want to work with, using get_user_to_modify_vulnerable() */
            /* prompt user for new PIN (this can be a function you create, or just put the code directly here */
            /* change the pin using the function, change_pin_vulnerable */
        /* otherwise, if the user did not want to risk it, and chose to run the more secure functions */
            /* prompt user for which user they want to work with, using get_user_to_modify_more_secure() */
            /* prompt user for new PIN (this can be a function you create, or just put the code directly here */
            /* change the pin using the function, change_pin_more_secure */
    /* end of loop */

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

/* TODO:  WRITE THIS FUNCTION */
/* Purpose: Ask the user if they want to run the vulnerable version, or not.
 *          Print a menu.
 *          Read from the keyboard.  If the user enters a 1,
 *          return true (vulnerable option)
 *          otherwise, return false (not-vulnerable option).
 * Returns: true - if user chose to be vulnerable, false - otherwise */
bool get_user_preference() {
    char buffer[256] = "";          /* read from the keyboard */
    int selection = 0;              /* user's choice */
    /* print a menu for the 2 options */
    printf("Make a simple menu here.  Enter stuff: ");  // you should edit this line
    /* read input from keyboard using fgets() and sscanf() with %d */
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &selection);
    /* if they entered 1, return true */
    /* if they entered anything else, return false (default option is secure) */
    return false; // you will edit this line, too
}

/* TODO: WRITE THIS FUNCTION */
/* Purpose:  Read from the keyboard.
 *           No input validation is done in this function, so it is vulnerable.
 * Returns:  The (unvalidated) integer index that the user wants to modify. */
int get_user_to_modify_vulnerable(void) {
    char buffer[256] = "";          /* read from the keyboard */
    int  desired_index = 0;         /* index of user to modify */
    /* prompt the user to enter the desired index */
    /* read input from keyboard using fgets() and sscanf() with %d */
    /* quit program if desired */
    /* otherwise, return the result */
    return -1;  // you will edit this line, too
}

/* TODO:  WRITE THIS FUNCTION */
/* Purpose: When passed the user's index number (user_i),
 *          the entire pin array (u_pin[]), and
 *          the new pin (new_pin),
 *          reset that user's pin.
 *          Do not do any input validation in this intentionally vulnerable function.
 * Returns: nothing, but may have some vulnerabilities */
void change_pin_vulnerable(int user_i, unsigned short u_pin[], int new_pin) {
    /* TODO: modify the desired u_pin, which can be done on one line. */
    // does not return a value, so no return statement needed
}

/* TODO:  WRITE THIS FUNCTION */
/* Purpose:  Read from the keyboard.
 *           Verify that value entered is valid. Re-prompt until satisfied.
 * Returns:  the (validated) integer index that the user wants to modify. */
int get_user_to_modify_more_secure(int current_num_users) {
    /* loop, unless they type EXIT_VALUE */
    /* read input from keyboard using fgets() and sscanf() with %d */
    /* quit the program, if the user entered the EXIT_VALUE */
    /* perform input validation on the user's input */
    /* if valid, return the answer */
    /* otherwise, print an error message and loop to reprompt the user */
    return -1; // you will edit this line, too
}

/* TODO:  WRITE THIS FUNCTION */
/* Purpose: When passed the user's index number (user_i),
 *          the entire pin array (u_pin[]), and
 *          the new pin (new_pin),
 *          reset that user's pin.
 * Returns: true - if successfully changed, false - if unchanged
 * Note:  It is better to not pass the entire pin array --
 *        yikes the information available to this function is excessive!
 *        However, C syntax with pointers is a pain, and this makes it "feel"
 *        like a more familiar language such as Java. */
bool change_pin_more_secure(int user_i, unsigned short u_pin[], int new_pin) {
    /* validate index */
    /* validate pin */
    /* assign if valid */
    return false; // you will edit this line, too
}