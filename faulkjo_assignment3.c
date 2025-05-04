#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for EXIT_SUCCESS and EXIT_FAILURE
#include <sys/types.h> //For Directories
#include <dirent.h>
#include <sys/stat.h>   // stat()
#include <time.h>

///Adapted From: GeeksforGeeks-Generating Randome Numbers generator.

// Func. for generating a random number in min/max.

int generateRandoms(int min, int max) {

    return rand() % (max - min + 1) + min;
    }



int main() {

    //Stores Metadata/ details about the files
    static struct stat largestFile; 
    static struct stat smallestFile; 

    //Adapted From Notes:"Exploration:Directories"  
    DIR* currDir;
    struct dirent *entry;
    struct stat dirStat;

    // Open the current directory
    currDir = opendir(".");
    if (currDir == NULL) {
        printf("Cannot open directory!\n");
        exit(EXIT_FAILURE);
    }

    // Read through the directory entries
    while ((entry = readdir(currDir)) != NULL) {

        // Get metadata info about the entries (in dirStat)
        if (stat(entry->d_name, &dirStat) == 0) {
            // Calculate the length of the file name
            size_t len = strlen(entry->d_name);

            // printf("Directory properly accessed\n");

        } else {
            printf("Cannot get metadata\n");
        }
    }


    // Main prompt - menu
    int numberChoice = 0;
    off_t largestSize = 0;
    off_t smallestSize = 0;
    

    //allocate file name for max file: 
    char largestFileString[200];
    //Allocate file name for max file: 
    char smallestFileString[200];
    
    // directory path.
    char pathname[300];

    //For my random seed generator
    srand(time(0)); 
    int randomNumber = generateRandoms(100, 1200);


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
            int numberChoice2 = 0; 

            //Next menu.
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
    
                    // make sure directory was closed
                    closedir(currDir);          
                    currDir = opendir("."); 

                    if (currDir == NULL) {
                        return EXIT_FAILURE;
                    }

                    // Read through em -   
                    //Adapted From Notes:"Exploration:Directories"  
                    while ((entry = readdir(currDir)) != NULL) {
                        // get the data info.
                        if (stat(entry->d_name, &dirStat) == 0) {
                            // find "movies_" file
                            if (strncmp(entry->d_name, "movies_", 7) == 0) {
                                // printf("File : %s\n", entry->d_name);
                                //is it  a file?
                                if (S_ISREG(dirStat.st_mode)) {
                                    // printf("%s info %ld\n", entry->d_name, dirStat.st_mtime);
                                    //check if file is a csv?
                                    if (strstr(entry->d_name, ".csv") != NULL) {
                                        // printf(" CSV file: %s\n", entry->d_name);
                                        //locate the largest one?
                                    if (dirStat.st_size >= largestFile.st_size ){
                                        largestFile.st_size = dirStat.st_size;
                                        strcpy(largestFileString, entry->d_name);
                                        printf("Now processing the chosen file named ...%s\n",largestFileString );
                                    

                                            // Generate a random number and create directory
                                            randomNumber = rand() % 100000;         // Generate new random number
                                            char createString[10];
                                            sprintf(createString, "%d", randomNumber);
                                            strcpy(pathname, "movies");
                                            strcat(pathname, createString);

                                            DIR* testDir = opendir(pathname);
                                            if (testDir) {
                                                // Directory exists, close it - generate a new one
                                                printf("Directory %s already exists. New directoty loading\n", pathname);
                                                closedir(testDir);
                                                randomNumber = rand() % 100000;
                                                sprintf(createString, "%d", randomNumber);
                                                strcpy(pathname, "movies");
                                                strcat(pathname, createString);
                                            }
                                            if (mkdir(pathname, 0777) == 0) {
                                                printf("Created directory with name %s\n", pathname);
                                            } else {
                                                printf("Issue?\n");
                                            }
                                        }
                                    }
                                }
                            }


                        } else {
                            printf("Cannot get metadata\n");
                        }
                    }

                    break;                     // exit 
                } else if (numberChoice2 == 2) {
            
                    // open the directory again to search - make sure it was closed
                    closedir(currDir);          
                    currDir = opendir("."); 

                    if (currDir == NULL) {
                        return EXIT_FAILURE;
                    }

                    // Read through em - Adapted From Notes:"Exploration:Directories" 
                    while ((entry = readdir(currDir)) != NULL) {
                        if (stat(entry->d_name, &dirStat) == 0) {
                            if (strncmp(entry->d_name, "movies_", 7) == 0) {
                                // printf("File found: %s\n", entry->d_name);
                                if (S_ISREG(dirStat.st_mode)) {
                                    // printf("%s info at %ld\n", entry->d_name, dirStat.st_mtime);
                                    if (strstr(entry->d_name, ".csv") != NULL) {
                                        // printf("CSV file: %s\n", entry->d_name);
                                    if (dirStat.st_size <= smallestFile.st_size){
                                        smallestFile.st_size = dirStat.st_size;
                                        strcpy(smallestFileString, entry->d_name);
                                        printf("Now processing the chosen file named ...%s\n",smallestFileString);
                                        
                                            // random number
                                            randomNumber = rand() % 100000; 
                                            char createString[10];
                                            sprintf(createString, "%d", randomNumber);
                                            strcpy(pathname, "movies_");
                                            strcat(pathname, createString);

                                            DIR* testDir = opendir(pathname);
                                            if (testDir) {
    
                                                // Directory exists, close it and generate a new one
                                                printf("Directory %s already exists. New directoty loading.\n", pathname);
                                                closedir(testDir);
                                                randomNumber = rand() % 100000; 
                                                sprintf(createString, "%d", randomNumber);
                                                strcpy(pathname, "movies");
                                                strcat(pathname, createString);
                                            }

                                            // Create the new directory
                                            if (mkdir(pathname, 0777) == 0) {
                                                printf("Created directory with name %s\n", pathname);
                                            } else {
                                                perror("Failed directory");
                                            }
                                        }
                                    }
                                }
                            }


                        } else {
                            perror("Cannot get metadata");
                        }
                    }

                    break;
                } else if (numberChoice2 == 3) {
                    char inputFileName [300];
                    int found = 0;

                    printf("Enter the complete file name:\n");
                    scanf("%s", inputFileName);
                    getchar();

                    closedir(currDir);          
                    currDir = opendir("."); 

                    if (currDir == NULL) {
                        perror("Error reopening DIRECTORY");
                        return EXIT_FAILURE;
                    }

                    // Read through em - Adapted from NOTES---DO not exit, until correct file is enterd
                    while ((entry = readdir(currDir)) != NULL) {
                        if (stat(entry->d_name, &dirStat) == 0) {
                            if (strcmp(entry->d_name, inputFileName) == 0) {
                                found = 1;
                                printf("Now processing the chosen file named: %s\n", inputFileName);

                                randomNumber = rand() % 100000;
                                char createString[10];
                                sprintf(createString, "%d", randomNumber);
                                strcpy(pathname, "movies_");
                                strcat(pathname, createString);

                                DIR* testDir = opendir(pathname);
                                if (testDir) {
                                    printf("Directory %s already exists. Generating new one...\n", pathname);
                                    closedir(testDir);
                                    randomNumber = rand() % 100000;
                                    sprintf(createString, "%d", randomNumber);
                                    strcpy(pathname, "movies");
                                    strcat(pathname, createString);
                                }

                                // Create the new directory
                                if (mkdir(pathname, 0777) == 0) {
                                    printf("Created directory with name %s \n", pathname);
                                } else {
                                    perror("Failed");
                                }

                                break; 
                            }
                        }
                    }

                    if (!found) {
                        printf("The File '%s' was not found. Try again.\n", inputFileName);
                        continue;                       // re-loop

                
                    break;  //break loop


                } else {
                    printf("Invalid value. Please select 1, 2, or 3.\n");
                }
            }
        } else {
            printf("You entered an incorrect option. Try again.\n");
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