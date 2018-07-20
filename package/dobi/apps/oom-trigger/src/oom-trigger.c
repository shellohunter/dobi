#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>


struct memlist {
    void * ptr;
    struct memlist * next;
};


int main(int argc, char *argv[])
{
    struct memlist * head = NULL;
    struct memlist * tmp = NULL;
    void *myblock = NULL;
    int count = 0;
    char action;
    int i;

    int x_hardalloc = 0;
    unsigned int x_allocnum = 0xFFFFFFFF;
    int x_allocunit = 1024*1024;

    while((i = getopt(argc, argv, "hn:u:")) != -1)
    {
        switch (i)
        {
            case 'h':
                x_hardalloc  = 1;
                break;
            case 'n':
                x_allocnum = atoi(optarg);
                break;
            case 'u':
                x_allocunit = atoi(optarg);
                break;
            case '?':
            default:
                exit(-1);
        }
    }

    do
    {
        tmp = (struct memlist *)malloc(sizeof(struct memlist));
        if (!tmp) break;

        tmp->ptr = (void *) malloc(x_allocunit);
        if (!tmp->ptr) break;
        tmp->next = head;
        head = tmp;

        if (x_hardalloc) memset(tmp->ptr, 1, x_allocunit);
        printf("Currently allocating %d MB\n", ++count);

        x_allocnum--;
    } while(x_allocnum>0);

    printf("%s\n",
        "w -> write\n"
        "f -> free\n"
        "q -> quit\n"
    );

    action = getchar();

    switch (action)
    {
        case 'w':
            tmp = head;
            count = 0;
            while (tmp)
            {
                printf("Currently writing block %d\n", count++);
                memset(tmp->ptr, 1, x_allocunit);
                tmp = tmp->next;
            }
            break;
        case 'f':
            tmp = head;
            while (tmp)
            {

                memset(tmp->ptr, 1, x_allocunit);
                tmp = tmp->next;
            }
            break;
        case 'q':
        default:
            break;
    }

    exit(0);
}
