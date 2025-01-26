/***********************************************************************
* @file  writer.c
* @version 0
* @brief  Implementation of writer file
*
* @author Iyona Lynn Noronha, iyonalynn.noronha@Colorado.edu
*
* @institution University of Colorado Boulder (UCB)
* @course   ECEN 5713 - Advanced Embedded Software Development
* @instructor Dan Walkes
*
* Revision history:
*   0 Initial release.
*
* @resources
* 1. Command Line Arguments: https://www.geeksforgeeks.org/command-line-arguments-in-c-cpp/
* 2. Check directory is there or not: https://stackoverflow.com/questions/12510874/how-can-i-check-if-a-directory-exists
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {

    // Error logging start
    openlog("writer", LOG_PID, LOG_USER);
	
    if (argc != 3) {
		syslog(LOG_ERR, "Error: Two arguments required");
        printf("Error: Two arguments required\n");
        printf("Usage: ./writer <writefile> <writestr>\n");
        return 1;
    }

    const char *writefile = argv[1];
    const char *writestr = argv[2];
	
	// Check if directory exists
    struct stat sb;

    if (!(stat(writefile, &sb) == 0 && S_ISDIR(sb.st_mode))) {
        syslog(LOG_ERR, "File doesn't exist");
	printf("Error: The file path doesn't exist\n");
	return 1;
    }
	
    // Write the string to the file, overwriting if it already exists
    FILE *fp = fopen(writefile, "w");
    if (fp == NULL) {
        syslog(LOG_ERR, "Error: Writing to '%s' failed, %s", writefile, strerror(errno));
        printf("Error: Writing to '%s' failed\n", writefile);
        return 1;
    }
    if (fputs(writestr, fp) == EOF) {
        syslog(LOG_ERR, "Error: Writing to '%s' failed, %s", writefile, strerror(errno));
        printf("Error: Writing to '%s' failed\n", writefile);
        fclose(fp); // Close the file before returning
        return 1;
    }

    // Log success message
    syslog(LOG_DEBUG, "Writing '%s' to '%s'", writestr, writefile);
    printf("Writing '%s' to '%s'", writestr, writefile);

    fclose(fp); // Close the file
    
    // Close syslog
    closelog();

    return 0;
}
