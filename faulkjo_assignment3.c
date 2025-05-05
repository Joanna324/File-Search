#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for EXIT_SUCCESS and EXIT_FAILURE
#include <sys/types.h> // Directories
#include <dirent.h>
#include <sys/stat.h>   // stat()
#include <time.h>
#include <fcntl.h>  //opening 
#include <unistd.h> ///write/close 


//Code Adapted from my Assignment#2
typedef struct movie{
        char title[100];                  
        int year;
        char languages[1000];            //20 Char max for language, 5 limit
        double rating;   
    
    }Movie;
    
    // Code adapted from :Geeks4geeks: Linked List in C.

    typedef struct linkedListNode {
        Movie *film;
        struct linkedListNode *next;
    } LinkedListNode;
    
    
    //allocate memory, assign Movie/Film struct to 
    //Pass head Node, Movie struct
    void addMovieToLinkList(LinkedListNode **head, Movie *newFilm) {
        // get the size and allocate memory for the new node.// ptr->next = newNode;
        //MAKE SURE TO FREE MEMORY
        LinkedListNode *newFilmNode = malloc(sizeof(LinkedListNode));
        newFilmNode->film = newFilm;
        newFilmNode->next = *head;
        *head = newFilmNode; 
    
    }
    //returns nothing - function passes a file path, LL head double memory ptr.
    void processMovieFile(char *filePath, LinkedListNode **head){
        // char *currLine = NULL;
        // size_t len = 0;
    
        // Open the file for reading only
        FILE *movieFile = fopen(filePath, "r");
    
    
         //movie count for films.
         int filmCount = 0;
         //line by line
         char line[1000];
    
      
        // Skip the header
        fgets(line, sizeof(line), movieFile);
    
        // Code adapted from :Read CSV File Data Into An Array Of Structs- Youtube
        // Read and parse each line
        while (fgets(line, sizeof(line), movieFile) != NULL) {
            char *token;
    
            // Get title
            // Allocate memory for a new Film struct- dynamically
            Movie *newFilm = malloc(sizeof(Movie));
    
            //Code adapdted from 
            // Get title from the csv -  using --> to access the struct
    
            token = strtok(line, ",");
            
            strcpy(newFilm->title, token);
    
            // Get year - convert to number
            token = strtok(NULL, ",");
            newFilm->year = atoi(token);
    
            // Get languages
            token = strtok(NULL, ",");
            strcpy(newFilm->languages, token);
    
            // Get rating - convert to number
            token = strtok(NULL, ",");
            newFilm->rating = atof(token);
    
            filmCount += 1;
            
    
    
    ///Add the movie processed Movie struct to LL.
     addMovieToLinkList(head, newFilm);
        }
    
        fclose(movieFile);
    
     // Print - stored data
     for (int i = 0; i < filmCount; i++) {
        // printf("Title: %s | Year: %d | Languages: %s | Rating: %.1f\n",
        //     films[i].title, films[i].year, films[i].languages, films[i].rating);
    }
    
    printf("\nProcessed file %s and parsed data for %d movies\n", filePath, filmCount);
    }
    
// Adapted from GeeksforGeeks - Bubble Sort Algorithm
//This function is a bubble sort function that organizes the Link list, movie per year.
void bubbleSortPerYear(LinkedListNode *head){
        LinkedListNode *ptrOne;
        int switched;
        Movie tempM;
        
    //Check is the list - empty.
        if(head == NULL) return;
    
        do {switched = 0;
            ptrOne = head;
            while (ptrOne->next != NULL) {
                if (ptrOne->film->year > ptrOne->next->film->year) {
    //swap the movie structs
    tempM = *ptrOne->film;
                    *ptrOne->film = *ptrOne->next->film;
                    *ptrOne->next->film = tempM;
                    switched = 1;}
                    ptrOne = ptrOne->next;}
        } while (switched);
    }
    

//Adapted from Notes : Exploration:Files
void intoFile(LinkedListNode *head,const char *dirName) {
    LinkedListNode *currentFilm;
    bubbleSortPerYear(head);
    
    for (currentFilm = head;
        currentFilm != NULL; 
        currentFilm = currentFilm->next) {
        int fd;
        //  for loop- movie's year
        char fileName[300];
        snprintf(fileName,sizeof(fileName), "./%s/%d.txt", dirName, currentFilm->film->year);

        // Open & append
        fd = open(fileName,O_RDWR | O_CREAT | O_APPEND,0600);
        if (fd == -1) {
            printf("open() failed on \"%s\"\n", fileName);
            perror("Error");
            exit(1);
        }
        // Write
        int howMany = write(fd, currentFilm->film->title, strlen(currentFilm->film->title) + 1);
        write(fd, "\n", 1);
        // printf("Wrote %d bytes to the file: %s\n", howMany, fileName);

        close(fd);
    }
}

///Adapted From: GeeksforGeeks-Generating Randome Numbers generator.
// Func. for generating a random number in min/max.

int generateRandoms(int min, int max) {
    return rand() % (max - min + 1) + min;
    }

//Main

int main() {

    //Stores Metadata/ details about the files
    static struct stat largestFile; 
    static struct stat smallestFile; 

    //Adapted From Notes:"Exploration:Directories"  
    DIR* currDir;
    struct dirent *entry;
    struct stat dirStat;

    // Open the current directory
    currDir= opendir(".");
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


    //Parse-Process File
    LinkedListNode *head =NULL;
    int numberChoice = 0;
    int yearSelected;
    char languageSelected[1000];

    // Main prompt - menu
    off_t largestSize= 0;
    off_t smallestSize = 0;
    

    //allocate file name for max file: 
    char largestFileString[200];
    //Allocate file name for max file: 
    char smallestFileString[200];
    
    // directory path.
    char pathname[300];

    //For my random seed generator
    srand(time(0)); 
    int randomNumber =generateRandoms(0, 9999);


    while (1) {
        // Menu
        printf("1. Select file to process\n");
        printf("2. Exit the program\n");
        printf("Enter a choice 1 or 2:\n");

        // Get the choice and clear the buffer
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
                scanf("%d",&numberChoice2);
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
                                        largestFile.st_size= dirStat.st_size;
                                        strcpy(largestFileString, entry->d_name);

                                        printf("Now processing the chosen file named ...%s\n",largestFileString );
                                        //Parse date 
                                         
                                        // Process the movie file into structs and add them to a linked list
                                        processMovieFile(largestFileString, &head);

                                            // Generate a random number and create directory
                                            randomNumber = rand() %99999 +0;
                                            char createString[10];
                                            sprintf(createString, "%d", randomNumber);
                                            strcpy(pathname, "movies");
                                            strcat(pathname,createString);

                                            DIR* testDir = opendir(pathname);
                                            if (testDir) {
                                                // Directory exists, close it - generate a new one
                                                printf("Directory %s already exists. New directoty loading\n", pathname);
                                                closedir(testDir);
                                                randomNumber = rand() % 99999 +0;
                                                sprintf(createString, "%d", randomNumber);
                                                strcpy(pathname, "faulkjo.movies");
                                                strcat(pathname, createString);
                                            }
                                            if (mkdir(pathname, 0777) == 0) {
                                                printf("Created directory with name %s\n", pathname);
                                                intoFile(head,pathname);
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

                                        // Process the movie file into structs and add them to a linked list
                                        processMovieFile(smallestFileString, &head);
                                        
                                            // random number
                                            randomNumber = rand()% 99999 + 0; 
                                            char createString[10];
                                            sprintf(createString, "%d", randomNumber);
                                            strcpy(pathname, "movies_");
                                            strcat(pathname, createString);

                                            DIR* testDir= opendir(pathname);
                                            if (testDir) {
    
                                                // Directory exists, close it and generate a new one
                                                printf("Directory %s already exists. New directoty loading.\n", pathname);
                                                closedir(testDir);
                                                randomNumber = rand() % 100000; 
                                                sprintf(createString, "%d", randomNumber);
                                                strcpy(pathname, "faulkjo.movies");
                                                strcat(pathname, createString);
                                            }

                                            // Create the new directory
                                            if (mkdir(pathname, 0777) == 0) {
                                                printf("Created directory with name %s\n", pathname);
                                                intoFile(head, pathname);
                                                


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
                    currDir =opendir("."); 

                    if (currDir == NULL) {
                        perror("Error reopening DIRECTORY");
                        return EXIT_FAILURE;
                    }

                    // Read through em -*Adapted from Exploration Notes
                    while ((entry = readdir(currDir)) != NULL) {
                        if (stat(entry->d_name, &dirStat) ==0) {
                            if (strcmp(entry->d_name, inputFileName) == 0) {
                                found = 1;
                                printf("Now processing the chosen file named: %s\n", inputFileName);
                                // Process the movie file into structs and add them to a linked list
                                processMovieFile(inputFileName, &head);

                                randomNumber = rand() % 100000;
                                char createString[10];
                                sprintf(createString, "%d",randomNumber);
                                strcpy(pathname, "movies_");
                                strcat(pathname, createString);

                                DIR* testDir = opendir(pathname);
                                if (testDir) {
                                    printf("Directory %s already exists. Generating new one...\n", pathname);
                                    closedir(testDir);
                                    randomNumber = rand() % 100000;
                                    sprintf(createString, "%d", randomNumber);
                                    strcpy(pathname, "faulkjo.movies");
                                    strcat(pathname, createString);
                                }

                                // Create the new directory
                                if (mkdir(pathname,0777) == 0) {
                                    printf("Created directory with name %s \n", pathname);
                                    intoFile(head, pathname);
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

                
                    break;
                    }

                } else {
                    printf("Invalid value. Please select 1, 2, or 3.\n");
                }
            }
        } else {
            printf("You entered an incorrect option. Try again.\n");
        }
    }
   
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
//https://www.geeksforgeeks.org/bubble-sort-algorithm/

///Creaet a directory with name....your_onid.movies.random#'s
///random is a random number between 0 and 99999 (both numbers inclusive)
//The permissions of the directory must be set to rwxr-x--- the owner has read, write and execute permissions

//Parse data in the chosen file to find out the movies released in each year
//In the new directory, create one file for each year in which at least one movie was released 
//permisstins are , the owner can read and write to the file, while group can only read the file.
// YYYY.txt where YYYY is the 4 digit integer value for the year.
//Within the file for a year, write the titles of all the movies released in that year, one on each line.
// Inside each file, write the movie titles released that year
// One movie title per line
