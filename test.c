#include <stdio.h>
#include <string.h>

char* stationParse(char *buff) {

    char *time;
    char *firstCutoff;
    char *secondCutoff;

    firstCutoff = strstr(buff, "Jakobsbergs station");
    secondCutoff = strstr(firstCutoff, "ExpectedDateTime");

    time = strtok(secondCutoff, ",");
    time = strstr(time, "20");
    //printf("%s", time);
    
    return(time);
}



char* fileRead() {
    FILE *fp;
    char buff[10000];
    
    fp = fopen("jsondata2.json", "r");
    fgets(buff, 9999, fp);
    return(stationParse(buff));
}





int main() {

    printf("Hello World!\n");
    puts(fileRead());
    return(0);
}

