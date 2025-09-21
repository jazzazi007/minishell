#include "minishell.h"

static int find_env(char **envp, const char *name)
{
    int i = 0;
    size_t len = strlen(name);
    while (envp[i])
    {
        if (ft_strncmp(envp[i], name, len) == 0)   // REMOVED 2nd condition
            return i;
        i++;
    }
    return -1;
}

static int check_export_name(const char *name)
{
    if (!name || !name[0] || name[0] == '=')
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
    strncpy(name, arg, name_len);//convert to ft_stncpy   FORBIDDEN FUNC
    name[name_len] = '\0';
    int idx = find_env(*envp, name);
    if (idx != -1)
    {
        free((*envp)[idx]);
        (*envp)[idx] = ft_strdup(arg);
    }
    else
    {                            //FIX EXPORT DUPLICATES KEYS (done in find_env)
        // Add new variable 
        int count = 0;
        while ((*envp)[count])
            count++; 
        char **new_envp = malloc(sizeof(char *) * (count + 2));
        for (int i = 0; i < count; i++)  // FORBIDDEN
            new_envp[i] = (*envp)[i];
        new_envp[count] = ft_strdup(arg);//protection
        new_envp[count + 1] = NULL;
        free(*envp);
        *envp = new_envp;
    }
}

int export_cmd(char **args, t_shell *shell)
{
    int i = 1;
    int ret = 0;
    if (!args[1])
    {
        // Print sorted env
        for (int j = 0; shell->envp[j]; j++)
        {
            char    *eq = ft_strchr(shell ->envp[j], '=');
            if (eq)
            {
                int     key_len = eq - shell -> envp[j];
                write(1, "declare -x ", 11);
                write(1, shell -> envp[j], key_len);
                printf("=\"%s\"\n", eq + 1);
            }
            else
                printf("declare -x %s\n", shell -> envp[j]);
        }
        return 0;
    }
    while (args[i])
    {
        if (!check_export_name(args[i]))
        {
            fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", args[i]); // FORBIDDEN FUNC
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
