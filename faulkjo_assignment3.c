#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for EXIT_SUCCESS and EXIT_FAILURE
#include <sys/types.h> //For Directories
#include <dirent.h>
#include <sys/stat.h>   // stat()




int main() {

    //struct for file size -- mayb runnecesary? 
    //Storees -- the details about the files
    static struct stat largestFile; 



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
            // Calculate the length of the file name
            size_t len = strlen(entry->d_name);

            // Print a confirmation message
            printf("Directory properly accessed\n");
        } else {
            perror("Failed to get file metadata");
        }
    }


    // Main prompt - menu
    int numberChoice = 0;
    off_t largestSize = 0;

    //allocate file name for max file: 
    char largestFileString[200];
    
    //new directory
    // char newDirectory = "frank.movies";


  

    while (1) {
        // Menu
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

                    // Read through em - Adapted from NOTES
                    while ((entry = readdir(currDir)) != NULL) {
                        // get the data info.
                        if (stat(entry->d_name, &dirStat) == 0) {
                            // find "movies_" file
                            if (strncmp(entry->d_name, "movies_", 7) == 0) {
                                // make sure the file is found.
                                printf("File found: %s\n", entry->d_name);
                                //is it  a file?
                                if (S_ISREG(dirStat.st_mode)) {
                                    printf("%s modified at %ld\n", entry->d_name, dirStat.st_mtime);
                                    //Is it a csv?
                                    if (strstr(entry->d_name, ".csv") != NULL) {
                                        printf("This is a CSV file: %s\n", entry->d_name);
                                        //locate the largest one?
                                    if (dirStat.st_size >= largestFile.st_size ){
                                        largestFile.st_size = dirStat.st_size;
                                        strcpy(largestFileString, entry->d_name);
                                        printf("Now processing the chosen file named ...%s\n",largestFileString );


                                        //Create directory 
                                        //Creating Directory - retunr 0 on success- Adapted from notes
                                    // int mkdir(const char *pathname, mode_t mode); //0777 v 0755
                                        //if directory exist, generate a new directory?
                                    } 
                                        


                                    }
                                }
                                



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



//Reference 
// https://stackoverflow.com/questions/7430248/creating-a-new-directory-in-c
// https://stackoverflow.com/questions/40163270/what-is-s-isreg-and-what-does-it-do#:~:text=S_ISREG()%20is%20a%20macro,stat)%20is%20a%20regular%20file.
// https://stackoverflow.com/questions/12275831/why-is-the-st-size-field-in-struct-stat-signed
// https://man7.org/linux/man-pages/man2/mkdir.2.html
// https://man7.org/linux/man-pages/man7/inode.7.html
// https://stackoverflow.com/questions/586928/how-should-i-print-types-like-off-t-and-size-t