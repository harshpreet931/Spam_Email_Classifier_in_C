#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "spam_classifier.h"

#define MAX_EMAILS 10000
#define TRAIN_RATIO 0.8

int main() {
    char emails[MAX_EMAILS][MAX_EMAIL_SIZE];
    int labels[MAX_EMAILS];
    int email_count = 0;

    FILE* file = fopen("./spm_temp.txt", "r+");
    if(file == NULL) 
    {
        printf("Error opening file\n");
        return 1;
    }
    printf("File opened successfully\n");

    char line[MAX_EMAIL_SIZE + 10];
    while(fgets(line, sizeof(line), file) && email_count < MAX_EMAILS)
    {
        char *label_str = strtok(line, ",");
        char *content = strtok(NULL, "\n");

        if(label_str && content)
        {
            labels[email_count] = atoi(label_str);
            strncpy(emails[email_count], content, MAX_EMAIL_SIZE - 1);
            emails[email_count][MAX_EMAIL_SIZE - 1] = '\0';
            email_count++;
        }
        
        if(email_count % 1000 == 0)
        {
            printf("Processed %d emails\n", email_count);
        }
    }
    fclose(file);

    printf("Total emails processed: %d\n", email_count);

    if(email_count == 0)
    {
        printf("No emails were read from the file. Check the file content.\n");
        return 1;
    }

    int train_count = (int)(email_count * TRAIN_RATIO);
    int test_count = email_count - train_count;
    
    printf("Training on %d emails, testing on %d emails\n", train_count, test_count);

    train(emails, labels, train_count);
    test(emails + train_count, labels + train_count, test_count);

    return 0;
}