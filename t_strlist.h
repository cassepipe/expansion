#ifndef T_STRLIST_H
#define T_STRLIST_H

#include <stddef.h>

typedef struct s_strlist_node {
	char	*str;
	int		len;
	struct s_strlist_node *next;
	struct s_strlist_node *prev;
}	t_strlist_node;

typedef struct s_strlist {
	t_strlist_node	*head;
	size_t			size;
	char			**envp;
	int				ret;
} t_strlist;

t_strlist		init_strlist();
t_strlist		*new_strlist();
t_strlist_node	init_strlist_node();
t_strlist_node *new_strlist_node();
void			deinit_strlist(t_strlist *list);
void			free_strlist(t_strlist *list);
void			append_node_to_strlist(t_strlist *list, t_strlist_node *node);
int				append_str_to_strlist(t_strlist *list, char *str);
int				append_strarray_to_strlist(t_strlist *list, char **strarray);
void			remove_node_from_strlist(t_strlist *list, t_strlist_node *node);
void			remove_str_from_strlist(t_strlist *list, char *str);
int				update_strlist_envp(t_strlist *list);
t_strlist		*make_strlist_from_null_terminated_str_array(char **envp);
char			*find_strlist_node_varvalue(t_strlist *list, char *name);
int				get_strlist_total_len(t_strlist strlist);
void			print_strlist(t_strlist list);

#endif /* T_STRLIST_H */

