#include <termios.h>
#include "header.h"

// reads from users.txt
char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50])
{
    // terminal settings structs
    struct termios oflags, nflags;

    system("clear"); // claer screen
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a); // read username, visible input

    // disabling echo (saves current terminal settings)
    tcgetattr(fileno(stdin), &oflags);
    // modifies settings to hide password input
    nflags = oflags;          // copy original settings
    nflags.c_lflag &= ~ECHO;  // disable echo, turn off character display
    nflags.c_lflag |= ECHONL; // enable newline echo, show ENTER
    // apply the new settings
    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    // read password (now hidden)
    scanf("%s", pass);

    // restore original terminal settings
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};

const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    // fscanf needs to write data into the variables
    // In C, array names (like char name[50]) are automatically treated as pointers to their first element.
    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            char *buff = userChecker.password;
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}

void registerMenu(char a[50], char pass[50], char passCheck[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Registration:");

    // Get username
    printf("\nEnter Your Name: ");
    scanf("%s", a);

    // Check if user already exists
    FILE *fp = fopen("./data/users.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening user file!\n");

        exit(1);
    }

    struct User userChecker;
    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, a) == 0)
        {
            printf("\nUser '%s' already exists!\n Please use another name.\n", a);
            fclose(fp);
            return; // Exit the function if user exists
        }
    }
    fclose(fp);

    // disabling echo (saves current terminal settings)
    tcgetattr(fileno(stdin), &oflags);
    // modifies settings to hide password input
    nflags = oflags;          // copy original settings
    nflags.c_lflag &= ~ECHO;  // disable echo, turn off character display
    nflags.c_lflag |= ECHONL; // enable newline echo, show ENTER
    // apply the new settings
    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }

    Password:
    printf("\nEnter a password: ");
    // read password (now hidden)
    scanf("%s", pass);

    printf("\t\t\t\tRetype password:");
    scanf("%s", passCheck);

    if (strcmp(pass, passCheck) == 0)
    {
        printf("\n\n\t\t\t\tPasswords Match!\n");
    }
    else
    {
        printf("\n\n\t\t\t\tPasswords don't match. Give it another try!\n");
        goto Password;
    }

    // restore original terminal settings
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }


    printf("\nUser '%s' registered successfully!\n", a);
    printf("Press enter to continue...\n");
    getchar(); // Wait for user to press enter
    getchar(); // Need two because of scanf buffer
}