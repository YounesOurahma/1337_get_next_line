/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourahma <yourahma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 17:42:02 by yourahma          #+#    #+#             */
/*   Updated: 2025/11/25 11:47:02 by yourahma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_strchr(const char *s, int c)
{
	char	ch;

	ch = (char) c;
	while (*s != '\0')
	{
		if (*s == ch)
			return ((char *)s);
		s++;
	}
	if (ch == '\0')
		return ((char *)s);
	return (NULL);
}

static char	*set_temp(char *temp, char *buff)
{
	char	*result;
	size_t	temp_len;
	size_t	buff_len;

	if (temp)
	{
		temp_len = ft_strlen(temp);
		buff_len = ft_strlen(buff);
		result = malloc(temp_len + buff_len + 1);
		if (!result)
		{
			free(temp);
			return (NULL);
		}
		ft_memcpy(result, temp, temp_len);
		ft_memcpy(result + temp_len, buff, buff_len);
		result[temp_len + buff_len] = '\0';
		free(temp);
		return (result);
	}
	else
		return (ft_strdup(buff));
}

static void	*ft_function(char **ptr)
{
	char	*temp;
	char	*new_line_position;

	new_line_position = ft_strchr(*ptr, '\n');
	if (!new_line_position)
	{
		free(*ptr);
		*ptr = NULL;
		return (NULL);
	}
	temp = ft_strdup(new_line_position + 1);
	free(*ptr);
	*ptr = temp;
	return (temp);
}

static char	*ft_print_o_restore(char **tp, char **bf, ssize_t read_bytes)
{
	char	*line;

	line = NULL;
	if (*tp && **tp && read_bytes == 0)
	{
		line = ft_strdup(*tp);
		free(*tp);
		*tp = NULL;
		free(*bf);
		*bf = NULL;
		return (line);
	}
	if (*tp && ft_strchr(*tp, '\n'))
	{
		line = ft_substr(*tp, 0, ft_strlen(*tp)
				- ft_strlen(ft_strchr(*tp, '\n')) + 1);
		ft_function(tp);
		return (line);
	}
	free(*tp);
	*tp = NULL;
	free(*bf);
	*bf = NULL;
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*temp;
	char		*buff;
	ssize_t		read_bytes;

	buff = NULL;
	read_bytes = 1;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	while (read_bytes != 0)
	{
		if (temp && ft_strchr(temp, '\n'))
			return (ft_print_o_restore(&temp, &buff, read_bytes));
		buff = malloc(BUFFER_SIZE + 1);
		if (buff == NULL)
			return (NULL);
		read_bytes = read(fd, buff, BUFFER_SIZE);
		if (read_bytes <= 0)
			break ;
		buff[read_bytes] = '\0';
		temp = set_temp(temp, buff);
		free(buff);
		buff = NULL;
	}
	return (ft_print_o_restore(&temp, &buff, read_bytes));
}
