#include "../../include/minishell.h"

static int find_env(char **envp, const char *name)
{
    int i = 0;
    size_t len = strlen(name);
    while (envp[i])
    {
        if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
            return i;
        i++;
    }
    return -1;
}

static int check_export_name(const char *name)
{
    if (!name || !name[0] || name[0] == '=' || name[0] == ' ')
        return 0;
    for (int i = 0; name[i] && name[i] != '='; i++)
    {
        if (!(name[i] == '_' || (name[i] >= 'A' && name[i] <= 'Z') ||
              (name[i] >= 'a' && name[i] <= 'z') ||
              (i > 0 && name[i] >= '0' && name[i] <= '9')))
            return 0;
    }
    return 1;
}

static void set_env(char ***envp, const char *arg)
{
    char *eq = ft_strchr(arg, '=');
    size_t name_len = 0;
    // if (!eq)
    //     return; //this condition if i want to add to env and there is no '=' sign
    if (eq)
        name_len = eq - arg;
    else
        name_len = ft_strlen(arg);
    char name[name_len + 1];
    strncpy(name, arg, name_len);//convert to ft_stncpy
    name[name_len] = '\0';
    int idx = find_env(*envp, name);
    if (idx != -1)
    {
        free((*envp)[idx]);
        (*envp)[idx] = ft_strdup(arg);
    }
    else
    {
        // Add new variable
        int count = 0;
        while ((*envp)[count])
            count++;
        char **new_envp = malloc(sizeof(char *) * (count + 2));
        for (int i = 0; i < count; i++)
            new_envp[i] = (*envp)[i];
        new_envp[count] = ft_strdup(arg);//protection
        new_envp[count + 1] = NULL;
        free(*envp);
        *envp = new_envp;
    }
}

int export_cmd(char **args, t_minishell *shell)
{
    int i = 1;
    int ret = 0;
    if (!args[1])
    {
        // Print sorted env
        for (int j = 0; shell->envp[j]; j++)
            printf("declare -x %s\n", shell->envp[j]);
        return 0;
    }
    while (args[i])
    {
        if (!check_export_name(args[i]))
        {
            fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", args[i]);
            ret = 1;
        }
        else
        {
            set_env(&shell->envp, args[i]);
        }
        i++;
    }
    return ret;
}