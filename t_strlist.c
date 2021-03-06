#include "minishell.h"

t_strlist	init_strlist()
{
	t_strlist new_strlist;

	new_strlist.head = NULL;
	new_strlist.size = 0;
	new_strlist.envp = NULL;
	return (new_strlist);
}

t_strlist *new_strlist()
{
	t_strlist *new_list;

	new_list = malloc(sizeof(t_strlist));
	if (!new_list)
		return (NULL);
	*new_list = init_strlist();
	return (new_list);
}

t_strlist_node	init_strlist_node()
{
	t_strlist_node node;

	node.str = NULL;
	node.len = 0;
	node.prev = NULL;
	node.next = NULL;
	return (node);
}

t_strlist_node *new_strlist_node()
{
	t_strlist_node *new_node;

	new_node = malloc(sizeof(t_strlist_node));
	if (!new_node)
		return (NULL);
	*new_node = init_strlist_node();
	return (new_node);
}

void	deinit_strlist(t_strlist *list)
{
	t_strlist_node *next_node;

	if (list)
	{
		while (list->size--)
		{
			next_node = list->head->next;
			free(list->head->str);
			free(list->head);
			list->head = next_node;
		}
		list->head = NULL;
		free(list->envp);
	}
}

void	free_strlist(t_strlist *list)
{
	deinit_strlist(list);
	free(list);
	list = NULL;
}

void	append_node_to_strlist(t_strlist *list, t_strlist_node *node)
{
	if (list && node)
	{
		if (list->head)
		{
			node->prev = list->head->prev;
			node->next = list->head;
			list->head->prev->next = node;
			list->head->prev = node;
		}
		else
		{
			node->prev = node;
			node->next = node;
			list->head = node;
		}
		list->size++;
	}
}

int	append_str_to_strlist(t_strlist *list, char *str)
{
	t_strlist_node	*new_node;

	new_node = new_strlist_node();
	if (new_node == NULL)
		return (-1);
	new_node->str = ft_strdup(str);
	new_node->len = ft_strlen(str);
	if (new_node->str == NULL)
	{
		free(new_node);
		return (-1);
	}
	append_node_to_strlist(list, new_node);
	return (0);
}

int	append_strarray_to_strlist(t_strlist *list, char **strarray)
{
	int i;

	i = 0;
	while (strarray[i])
	{
		if (append_str_to_strlist(list, strarray[i]))
			return (-1);
		i++;
	}
	return (0);
}

void	remove_node_from_strlist(t_strlist *list, t_strlist_node *node)
{
	t_strlist_node *curr;
	size_t			size;

	if (list && node)
	{
		if (list->head && list->size > 0)
		{
			size = list->size;
			curr = list->head;
			while (curr != node && size--)
				curr = curr->next;
			if (size > 0)
			{
				if (curr == list->head)
					list->head = list->head->next;
				curr->prev->next = curr->next;
				curr->next->prev = curr->prev;
				free(curr->str);
				free(curr);
				list->size--;
				if (list->size == 0)
					list->head = NULL;
			}
		}
	}
}

void	remove_str_from_strlist(t_strlist *list, char *str)
{
	t_strlist_node *curr;
	size_t			size;

	if (list && str)
	{
		if (list->head)
		{
			curr = list->head;
			size = list->size;
			while (size-- && ft_strcmp(curr->str, str))
				curr =  list->head->next;
			if (size >  0)
			{
				curr->prev->next = curr->next;
				curr->next->prev = curr->prev;
				free(curr);
				list->size--;
			}
		}
	}
}

int			update_strlist_envp(t_strlist *list)
{
	t_strlist tmp_list;
	char	**new_envp;

	tmp_list = *list;
	new_envp = malloc((list->size + 1) * sizeof(char **));
	if (!new_envp)
		return (-1);
	free(list->envp);
	list->envp = new_envp;
	while (tmp_list.size--)
	{
		*new_envp = tmp_list.head->str;
		tmp_list.head = tmp_list.head->next;
		new_envp++;
	}
	*new_envp = NULL;
	return (0);
}

t_strlist	*make_strlist_from_null_terminated_str_array(char **envp)
{
	t_strlist *list;

	list = new_strlist();
	if (!list)
		return (NULL);
	if (envp)
	{
		while  (*envp)
		{
			if (append_str_to_strlist(list, *envp) == -1)
			{
				free_strlist(list);
				return (NULL);
			}
			envp++;
		}
		update_strlist_envp(list);
	}
	list->ret = 0;
	return (list);
}

char	*find_strlist_node_varvalue(t_strlist *list, char *name)
{
	size_t			i;
	t_strlist_node	*tmp;
	char			*env_name;
	size_t			y;

	i = 0;
	if (!list || !name || !list->envp || !list->head)
		return (NULL);
	tmp = list->head;
	while (i < list->size)
	{
		y = 0;
		env_name = ft_strdup(tmp->str);
		while (env_name && env_name[y] && tmp->str[y] != '=')
			y++;
		if (env_name && env_name[y] == '=')
			env_name[y] = 0;
		if (env_name && ft_strcmp(env_name, name) == 0)
		{
			free(env_name);
			return (&tmp->str[y + 1]);
		}
		free(env_name);
		i++;
		tmp = tmp->next;
	}
	return (NULL);
}

int	get_strlist_total_len(t_strlist strlist)
{
	int	total_len;

	total_len = 0;
	if (strlist.head)
	{
		while (strlist.size--)
		{
			total_len += strlist.head->len;
			strlist.head = strlist.head->next;
		}
	}
	return (total_len);
}

void	print_strlist(t_strlist list)
{
	while (list.size--)
	{
		printf("%s\n", list.head->str);
		list.head = list.head->next;
	}
}
