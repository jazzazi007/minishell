#include "../include/minishell.h"

int cd(int ac, char **av, char **envp)
{
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];
    char *home;

    home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "HOME environment variable not set\n");
        return 1;
    }
    printf("HOME: %s\n", home);
    if (ac < 2) {
        fprintf(stderr, "Usage: %s <directory>\n", av[0]);
        return 1;
    }

    if (ac == 2)
    {
        if (chdir(home) != 0)
        {
            perror("chdir get");
            return 1;
        }
        else
        {
            if (getcwd(path, sizeof(path)) != NULL)
            {
                printf("Current working directory: %s\n", path);
                if (chdir("minishell") != 0)
                {
                    perror("chdir minishell");
                    return 1;
                }
            }
            else
            {
                perror("getcwd");
                return 1;
            }
            getcwd(path, sizeof(path));
            printf("Current working directory: %s\n", path);
            printf("Changed to minishell directory\n");
            return 0;
        }
    }


    if (chdir(av[2]) != 0) {
        perror("chdir");
        closedir(dir);
        return 1;
    }

    if (getcwd(path, sizeof(path)) == NULL)
    {
        perror("getcwd");
        closedir(dir);
        return 1;
    }

    dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
    return 0;
}