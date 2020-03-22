#include <stdio.h>
#include <string.h>

int main()
{
    char S[] = "abc /sdsd.sdsd HTTP/sdsd";
    printf("%s\n", strtok(S, ". /"));

    printf("%s\n", strtok(NULL, ". /"));
    printf("%s\n", strtok(NULL, ". /"));
    printf("%s\n", strtok(NULL, ". /"));

    printf("Hello world\n");
    return 0;
}

