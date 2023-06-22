#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

char *stationParse(char *buff)
{

    char *time;
    char *firstCutoff;
    char *secondCutoff;

    firstCutoff = strstr(buff, "Jakobsbergs station");
    secondCutoff = strstr(buff, "ExpectedDateTime");

    time = strtok(secondCutoff, ",");
    time = strstr(time, "20");
    printf("%s", time);

    return (time);
}

char *fileRead()
{
    FILE *fp;
    char buff[10000];

    fp = fopen("jsondata3.json", "r");
    fgets(buff, 9999, fp);
    return (stationParse(buff));
}

int toTimes(char *charTime)
{
    int buff, time = 0;
    char *year = strtok(charTime, "-");
    char *month = strtok(NULL, "-");
    char *day = strtok(NULL, "T");
    char *hour = strtok(NULL, ":");
    char *min = strtok(NULL, ":");
    char *sec = strtok(NULL, "\"");
    sscanf(hour, "%d", &buff);
    time += buff * 3600;
    sscanf(min, "%d", &buff);
    time += buff * 60;
    sscanf(sec, "%d", &buff);
    time += buff;

    return (time);
}

int main()
{
    int dates;
    printf("Hello World!\n");
    dates = toTimes(fileRead());

    return (0);
}
