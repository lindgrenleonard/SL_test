#include <stdio.h>
#include <string.h>

char *stationParse(char *buff)
{

    char *time;
    char *firstCutoff;
    char *secondCutoff;

    firstCutoff = strstr(buff, "Jakobsbergs station");
    secondCutoff = strstr(firstCutoff, "ExpectedDateTime");

    time = strtok(secondCutoff, ",");
    time = strstr(time, "20");
    // printf("%s", time);

    return (time);
}

char *fileRead()
{
    FILE *fp;
    char buff[10000];

    fp = fopen("jsondata2.json", "r");
    fgets(buff, 9999, fp);
    return (stationParse(buff));
}

int toEpochTime(char *charTime)
{
    int time;
    char *year = strtok(charTime, "-");
    char *month = strtok(NULL, "-");
    char *day = strtok(NULL, "T");
    char *hour = strtok(NULL, ":");
    char *min = strtok(NULL, ":");
    char *sec = strtok(NULL, "\"");
    int y, m, d, h, mi, s;
    sscanf(year, "%d", &y);
    sscanf(month, "%d", &m);
    sscanf(day, "%d", &d);
    sscanf(hour, "%d", &h);
    sscanf(min, "%d", &mi);
    sscanf(sec, "%d", &s);
    time = (((y - 1970) * 365.25) * 86400 + (m * 2629743) + (d * 86400) + (h * 3600) + (m * 60) + s);
    return time;
}

int main()
{

    printf("Hello World!\n\n");
    puts(fileRead());
    int unixTime = toEpochTime(fileRead());
    printf("%d", unixTime);
    return (0);
}
