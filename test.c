#include <stdio.h>
#include <string.h>

int stationParse(char *buff) {

    char *token;
    token = strtok(buff, " \n");
    while(token) {
        printf("putting token: \n\n");
        puts(token);
        token = strtok(NULL, " \n");
    }
    
    return(0);
}



void fileRead() {
    FILE *fp;
    char buff[2000];
    
    fp = fopen("jsondata.json", "r");
    fgets(buff, 1000, fp);
    //puts(buff);
    stationParse(buff);

}





int main() {

    printf("Hello World!\n");
    fileRead();
    return(0);
}

