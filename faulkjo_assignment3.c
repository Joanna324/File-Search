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
        fd = open(fileName,O_RDWR | O_CREAT | O_APPEND,0750);
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
    // Stores metadata/details about the files
    DIR* currDir;
    struct dirent *entry;
    struct stat dirStat;

    
    //Adapted From Notes:"Exploration:Directories" 
    currDir = opendir(".");
    if (currDir == NULL) {
        perror("Cannot open directory");
        return EXIT_FAILURE;
    }

    LinkedListNode *head = NULL;
    int numberChoice = 0;


    // Declare variables for linked list, menu options, and file size tracking
    off_t largestSize = 0;
    off_t smallestSize = __LONG_MAX__;

    char createString[200];
 
     // directory path.
    char pathname[300];
    char inputFileName[300];
    
    
    //For my random seed generator
    srand(time(0));
    int randomNumber;

   

    
    while (1) {
        //Menu
        printf("1. Select file to process\n");
        printf("2. Exit the program\n");
        printf("Enter a choice 1 or 2:\n");

        // Get the choice and clear the buffer
        scanf("%d", &numberChoice);
        getchar();

        // Exit the program if the user selects option 2
        if (numberChoice == 2) {
            printf("Exiting program..bye:).\n");
            break;
        }if (numberChoice != 1) {
            printf("You entered an incorrect option. Try again.\n");
            continue; }

            while (1){
            //Next menu.
        int numberChoice2 = 0;
        printf("Which file you want to process?\n");
        printf("Enter 1 to pick the largest file\n");
        printf("Enter 2 to pick the smallest file\n");
        printf("Enter 3 to specify the name of a file\n");

        // Get user's choice for file processing
        scanf("%d", &numberChoice2);
        getchar();                                      //Clear it

        // Reset directory inside the loop
        rewinddir(currDir);
        largestSize = 0;
        smallestSize = __LONG_MAX__;
        //allocate file name for max file: 
        char largestFileString[300] = "";
        //allocate file name for max file: 
        char smallestFileString[300] = "";

         // Read through em -   
        //Adapted From Notes:"Exploration:Directories"  
        if (numberChoice2 == 1) {
            while ((entry = readdir(currDir)) != NULL) {
                if (stat(entry->d_name, &dirStat) == 0) {
                    if (S_ISREG(dirStat.st_mode)) {
                    if (strncmp(entry->d_name, "movies_", 7) == 0) {
                            if (strstr(entry->d_name, ".csv") != NULL) {
                                if (dirStat.st_size >largestSize) {
                            largestSize= dirStat.st_size;
                            strcpy(largestFileString, entry->d_name);
                                }
                         }
                    }
                    }
         }
            }
//check foor empty string.
                            if (strlen(largestFileString)== 0) {
                            printf("movie files not found.\n");
                                continue;
            }

     // largest file
       printf("Now processing the chosen file named %s\n", largestFileString);
            processMovieFile(largestFileString, &head);
            break; 
        // option2 
        } else if (numberChoice2 == 2) {
            while ((entry = readdir(currDir)) != NULL) {
                if (stat(entry->d_name,&dirStat) == 0) {
                 if (S_ISREG(dirStat.st_mode)) {
                if (strncmp(entry->d_name,"movies_", 7) == 0) {
                    if (strstr(entry->d_name,".csv") != NULL) {
                                if (dirStat.st_size < smallestSize) {
                                smallestSize=dirStat.st_size;
                                    strcpy(smallestFileString, entry->d_name);
                            }
                            }
                    }
                  }
        }
            }         if (strlen(smallestFileString) == 0) {
            printf("The file was not found..\n");
            continue; }

     // smallest file
                printf("Now processing the chosen file named %s\n", smallestFileString);
            processMovieFile(smallestFileString, &head);
     break; 
        ///option3
        } else if (numberChoice2 == 3) {
            printf("Enter the complete file name:\n");
            scanf("%s", inputFileName);
            getchar();    

            int found = 0;
        while ((entry = readdir(currDir)) != NULL) {
             if (stat(entry->d_name, &dirStat) == 0) {
                       if (strcmp(entry->d_name,inputFileName) == 0) {
                        if (S_ISREG(dirStat.st_mode)) {
                            found = 1;
                            printf("Now processing the chosen file named %s\n", inputFileName);
                            processMovieFile(inputFileName,&head);
                            break; }
                            
                }}
         }

            if (!found) {
                printf("The file %s was not found. Try again.\n", inputFileName);
                continue;                               // re-loop
            }
            break; 
        } else {
            printf("Invalid value. Please select 1, 2, or 3.\n");
            continue; }

        }

        // onto directory creation.
        char createString[200];
        DIR* testDir = NULL;

        randomNumber = rand() % 100000;
        sprintf(createString, "%d", randomNumber);
        strcpy(pathname, "faulkjo.movies.");
        strcat(pathname, createString);
        
        testDir = opendir(pathname);
        while (testDir) {
            closedir(testDir);
        randomNumber = rand() %100000;
            sprintf(createString, "%d", randomNumber);
            strcpy(pathname, "faulkjo.movies.");
        strcat(pathname,createString);
            testDir = opendir(pathname);
        }
        
        // Create the directory
        if (mkdir(pathname, 0777) == 0) {
            chmod(pathname, 0750);
             printf("Created directory with name %s\n", pathname);
            intoFile(head, pathname);
        } else {
            perror("Failed");
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
//https://man7.org/linux/man-pages/man3/rewinddir.3.html

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
