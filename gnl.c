#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER 10
#define MAX_FD 1024
#define FAIL 1
#define SUCCESS 0


static char	*strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	len;
	size_t	s1_len;
	size_t	s2_len;
	size_t	i;
	size_t	j;

	if (!s1)
		return (strdup(s2));
	if (!s2)
		return (strdup(s1));
	s1_len = strlen(s1);
	s2_len = strlen(s2);
	len = s1_len + s2_len + 1;
	str = (char *)malloc(sizeof(char) * len);
	if (!str)
		return (NULL);
	i = 0;
	while (i < s1_len)
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < s2_len)
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	return (str);
}

static char	*substr(char *str, int start, int len)
{
	char	*new_str;
	int		str_len;
	int		i;

	if (!str)
		return (NULL);
	str_len = strlen(str);
	if (start >= str_len)
		return (strdup(""));
	if (len > str_len - start)
		len = str_len - start;
	new_str = malloc(len + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	while (str[start + i] && i < len)
	{
		new_str[i] = str[start + i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

static int	ft_add_to_rest(int fd, char **rest)
{
	char	*buff;
	char	*temp;
	int		read_b;

	buff = malloc(BUFFER + 1);
	if (!buff)
		return (FAIL);
	while (!strchr(*rest, '\n'))
	{
		read_b = read(fd, buff, BUFFER);
		if (read_b < 0)
			return (free(buff), FAIL);
		if (read_b == 0)
			break;
		buff[read_b] = '\0';
		temp = strjoin(*rest, buff);
		if (!temp)
			return (free(buff), FAIL);
		free(*rest);
		*rest = temp;
	}
	return (free(buff), SUCCESS);
}

static char	*ft_line(char **rest)
{
	char	*new_line_ptr;
	int		line_len;
	char	*line;
	char	*temp;

	new_line_ptr = strchr(*rest, '\n');
	if (new_line_ptr)
		line_len = new_line_ptr - *rest + 1;
	else
		line_len = strlen(*rest);
	line = substr(*rest, 0, line_len);
	if (!line)
		return (free(*rest), *rest = NULL, NULL);
	temp = strdup(*rest + line_len);
	if (!temp)
		return (free(*rest), *rest = NULL, free(line), NULL);
	free(*rest);
	*rest = temp;
	return (line);
}

char *gnl(int fd)
{
	static char	*rest;
	char		*line;

	if (fd < 0 || fd >= MAX_FD || BUFFER <= 0)
		return (NULL);
	if (!rest)
	{
		rest = strdup("");
		if (!rest)
			return (NULL);
	}
	if (ft_add_to_rest(fd, &rest) == FAIL)
		return (free(rest), rest = NULL, NULL);
	if (!rest || !rest[0])
		return (free(rest), rest = NULL, NULL);
	line = ft_line(&rest);
	if (!line && rest)
		free(rest), rest = NULL;
	return (line);
}