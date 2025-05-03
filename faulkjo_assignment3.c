#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for EXIT_SUCCESS and EXIT_FAILURE
#include <sys/types.h> //For Directories
#include <dirent.h>
#include <sys/stat.h>   // stat()




int main() {
    DIR* currDir;
    struct dirent *entry;
    struct stat dirStat;

    // Open the current directory
    currDir = opendir(".");
    if (currDir == NULL) {
        perror("Error opening DIRECTORY");
        return EXIT_FAILURE;
    }

    // Read through the directory entries
    while ((entry = readdir(currDir)) != NULL) {
        // Get metadata info about the entries (in dirStat)
        if (stat(entry->d_name, &dirStat) == 0) {
            // Print a confirmation message
            printf("Directory properly accessed\n");
        } else {
            perror("Failed to get file metadata");
        }
    }


    // Main prompt - menu
    int numberChoice = 0;

    while (1) {
        // Display the menu
        printf("1. Select file to process\n");
        printf("2. Exit the program\n");
        printf("Enter a choice 1 or 2:\n");

        // Get the user choice and clear the buffer
        scanf("%d", &numberChoice);
        getchar();

        // Exit the program
        if (numberChoice == 2) {
            printf("Exiting program..bye:).\n");
            exit(0);
        } else if (numberChoice == 1) {
            int numberChoice2 = 0; // make == to 0 

            while (1) {
                printf("Which file you want to process?\n");
                printf("Enter 1 to pick the largest file\n");
                printf("Enter 2 to pick the smallest file\n");
                printf("Enter 3 to specify the name of a file\n");
                printf("Enter a choice from 1 to 3:\n");

                // take input
                scanf("%d", &numberChoice2);
                getchar();                      //Clear it

                if (numberChoice2 == 1) {
                    printf("Searching for the largest movie file...\n");

                    // open the directory again to search - make sure it was closed
                    closedir(currDir);          
                    currDir = opendir("."); 

                    if (currDir == NULL) {
                        perror("Error reopening DIRECTORY");
                        return EXIT_FAILURE;
                    }

                    // Read through em
                    while ((entry = readdir(currDir)) != NULL) {
                        // get the data info.
                        if (stat(entry->d_name, &dirStat) == 0) {
                            // find "movies_" file
                            if (strncmp(entry->d_name, "movies_", 7) == 0) {
                                // make sure the file is found.
                                printf("File found: %s\n", entry->d_name);
                            }
                        } else {
                            perror("Failed to get file metadata");
                        }
                    }

                    break; // exit while loop
                } else if (numberChoice2 == 2) {
                    printf("Searching for the smallest movie file...\n");
                    // TO BE IMPLEMENTED
                } else if (numberChoice2 == 3) {
                   // TO BE IMPLEMENTED
                } else {
                    printf("Invalid choice. Please select 1, 2, or 3.\n");
                }
            }
        } else {
            printf("You entered an incorrect choice. Try again.\n");
        }
    }
    // Close the directory
    closedir(currDir);
    
    return EXIT_SUCCESS;
}
