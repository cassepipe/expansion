#include "minishell.h"

//main parsing function
void	print_strarray(char **com)
{
	if (com)
	{
		while (*com)
		{
			printf("%s\n", *com);
			com++;
		}
	}
}

char **expand_simple_command(char *simple_command, t_strlist *env)
{
	char **com;
	char *expanded = NULL;

	com = quote_preserving_split(simple_command);
	if (!com)
		return (NULL);
	/*printf( "--Result of quote preserving split --\n");*/
	/*print_strarray(com);*/

	com = expand_args(com, env);
	if (!com)
		return (NULL);
	/*printf( "--Now with variable expansion --\n" );*/
	/*print_strarray(com);*/

	expanded = ft_join_null_terminated_str_array(com);
	free_strarray(com);
	if (!expanded)
		return (NULL);
	/*printf( "--Expanded string--\n%s\n", expanded);*/

	com = split_on_unquoted_whitespace(expanded, " \t");
	free(expanded);
	if (!com)
		return (NULL);
	/*printf( "--Split on unquoted whitespace--\n" );*/
	/*print_strarray(com);*/

	com = handle_quotes(com);
	if (!com)
		return (NULL);
	/*printf("--After quote removal--\n");*/
	/*print_strarray(com);*/

	return (com);
}

//Expansion covers quote handling and variable replacement
t_strlist	*expand_pipeline(char *user_input, t_strlist *env)
{
	char	**pipeline;
	char	**simple_command;
	int 	i;
	t_strlist *arg_list;

	arg_list = new_strlist();
	pipeline = split_on_unquoted_redir(user_input, ">|<");
	/*printf( "--Result of split on unquoted |, < and > --\n" );*/
	/*print_strarray(pipeline);*/
	i = 0;
	while (pipeline[i])
	{
		simple_command = expand_simple_command(pipeline[i], env);
		if (!simple_command || append_strarray_to_strlist(arg_list, simple_command))
		{
			free_strarray(pipeline);
			free_strlist(arg_list);
			return (NULL);
		}
		free_strarray(simple_command);
		i++;
	}
	free_strarray(pipeline);
	return (arg_list);
}

int main(int ac, char **av, char **envp)
{
	char        *user_input;
	t_strlist	*env;
	t_strlist	*list;

	env = make_strlist_from_null_terminated_str_array(envp);
	while (true)
	{
		user_input = readline(PROMPT);
		if (!user_input)
		{
			ft_dprintf_str(STDERR_FILENO, "exit\n");
			free_strlist(env);
			return (0);
		}
		list = expand_pipeline(user_input, env);
		free(user_input);
		if (!list)
			break;
		print_strlist(*list);
		free_strlist(list);
	}
	free_strlist(env);
	return (0);
}
