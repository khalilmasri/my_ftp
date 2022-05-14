
     #include <stdlib.h>
          #include <fcntl.h>
          #include <unistd.h>

int main()
{
    srand(0);
    int a;
    int fd = open("testFile", O_WRONLY | O_CREAT);
    write(fd, "a", 1);
    for (int i = 0; i < 10; i++)
    {
        for (int i = 0; i < 10000; i++)
        {
            a = (rand() % 26) + 'a';
                write(fd, &a, 1);
        }
        write(fd, "\n", 1);
    }
}