#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/types.h>
#include <unistd.h>
#include <alloca.h>
#include <stdint.h>


char * __global_variable_inited = "Hello";
char * __global_variable_uninited;


struct memlist {
    void * ptr;
    struct memlist * next;
};

void func1(int func_arg1, int func_arg2)
{
    int local_var;
    static int local_static_var;
    void * local_alloca;

    printf("%-32s %p\n", "function argument 1", &func_arg1);
    printf("%-32s %p\n", "function argument 2", &func_arg2);
    printf("%-32s %p\n", "local variable", &local_var);
    printf("%-32s %p\n", "local static variable", &local_static_var);

    local_alloca = alloca(8);
    printf("%-32s %p\n", "local alloca", &local_alloca);
}


int main(int argc, char *argv[])
{
    int i;
    const int ZERO = 0;
    void * local_malloc_1KB;
    void * local_malloc_128MB;
    char buffer[64];
    char c;
    int param1 = 0;
    int param2 = 0;

    struct memlist * head = NULL;
    struct memlist * tmp = NULL;

    printf("%-32s %p\n", "function address", func1);
    printf("%-32s %p\n", "inited global variable", &__global_variable_inited);
    printf("%-32s %p\n", "uninited global variable", &__global_variable_uninited);
    printf("%-32s %p\n", "command line argument", argv);
    printf("%-32s %p\n", "local constant variable", &ZERO);
    printf("%-32s %p\n", "global constant variable", __global_variable_inited);

    local_malloc_1KB = malloc(1024);
    printf("%-32s %p\n", "local malloc 1KB", local_malloc_1KB);
    local_malloc_128MB = malloc(128*1024*1024);
    printf("%-32s %p\n", "local malloc 128MB", local_malloc_128MB);

    func1(ZERO, 2);

    printf("================== supported actions <pid %ld> ================\n"
           "a 128             malloc memory of size 128\n"
           "A 128  10         malloc 10 pieces of memory of size 128\n"
           "f                 free all memory allocated by 'a' and 'A'\n"
           "r 0x1000          read a byte from address 0x1000\n"
           "w 0x1000          write 'x' to address 0x1000\n"
           "R 0x1000 0x2000   read bytes from address range [0x1000, 0x2000]\n"
           "W 0x1000 0x2000   write 'x' to address range [0x1000, 0x2000]\n"
           "================================================================\n\n", (long)getpid());

    while (1)
    {
        fgets(buffer, sizeof(buffer), stdin);
        switch(buffer[0])
        {
            case 'a':
                sscanf(buffer, " %c %d", &c, &param1);

                tmp = (struct memlist *)malloc(sizeof(struct memlist));
                if (!tmp) break;
                tmp->ptr = (void *) malloc(param1);
                if (!tmp->ptr) break;
                tmp->next = head;
                head = tmp;
                printf("malloced: %-8d   0x%p\n", param1, head->ptr);
                break;
            case 'A':
                sscanf(buffer, " %c %d %d", &c, &param1, &param2);
                for (i=0; i<param2; i++)
                {
                    tmp = (struct memlist *)malloc(sizeof(struct memlist));
                    if (!tmp) break;
                    tmp->ptr = (void *) malloc(param1);
                    if (!tmp->ptr) break;
                    tmp->next = head;
                    head = tmp;
                    printf("malloced: %-8d   0x%p\n", param1, head->ptr);
                }
                break;
            case 'f':
                tmp = head;
                while (head)
                {
                    tmp = head;
                    head = head->next;
                    free(tmp->ptr);
                    printf("free:                0x%p\n", tmp->ptr);
                    free(tmp);
                }
                break;
            case 'r':
                sscanf(buffer, " %c %x", &c, &param1);
                printf("reading data from 0x%08x\n", param1);
                c = *((char *)(uintptr_t)param1);
                printf("0x%08x: 0x%x\n", param1, (unsigned char)c);
                break;
            case 'w':
                sscanf(buffer, " %c %x", &c, &param1);
                printf("writing data to 0x%08x\n", param1);
                *((char *)(uintptr_t)param1) = 'x';
                c = *((char *)(uintptr_t)param1);
                printf("0x%08x: 0x%x\n", param1, (unsigned char)c);
                break;
            case 'R':
                sscanf(buffer, " %c %x %x", &c, &param1, &param2);
                printf("reading data from 0x%08x to 0x%08x\n", param1, param2);
                for (i=param1; i<=param2; i++)
                {
                    c = *((char *)(uintptr_t)i);
                    printf("0x%08x: 0x%x\n", i, (unsigned char)c);
                }
                break;
            case 'W':
                sscanf(buffer, " %c %x %x", &c, &param1, &param2);
                printf("writing data from 0x%08x to 0x%08x\n", param1, param2);
                for (i=param1; i<=param2; i++)
                {
                    *((char *)(uintptr_t)i) = 'x';
                    c = *((char *)(uintptr_t)i);
                    printf("0x%08x: 0x%x\n", i, (unsigned char)c);
                }
                break;
            default:
                return 0;
        }
    }

    return 0;
}


