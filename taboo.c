#include <unistd.h> //read, close
#include <fcntl.h> //open
#include <stdio.h> //printf
#include <string.h> //strlen, strcpy
#include <ctype.h> //isascii

static void	rev_polish_word(char *str)
{
	int	len = strlen(str);
	int	i = 0;
	int	j = len - 1;
	char	tmp;
	while (i < j)
	{
		tmp = str[i];
		str[i] = str[j];
		str[j] = tmp;
		i++;
		j--;
	}
	i = 0;
	while (i < len)
	{
		if (isascii(str[i]))
			i++;
		else
		{
			tmp = str[i];
			str[i] = str[i + 1];
			str[i + 1] = tmp;
			i += 2;
		}
	}
}

int	ismain(char *str)
{
	int  i = 0;
	while (str[i] && ft_isupper(str[i]))
		i++;
	if (str[i] == ' ')
		i++;
	

int	count_main(char *buff)
{
	int	i = 0;
	int	codes = 0;

	while (buff[i])
	{
		while (buff[i] == ' ')
			i++;
		if (buff[i])
			codes++;
		while (buff[i] && buff[i] != ' ')
			i++;
	}
	return (codes);
}

char	***parse_buff(char *buff)
{
	char	***taboo;

	taboo = malloc(sizeof(char **) * count_main(buff));
}

int	main()
{
	char ***taboo;

	int fd = open("cz1.txt", O_RDONLY);
	if (fd < 0)
		return (printf("Error: open\n"), 1);

	char	buff[BUF] = "";
	int	read_b = read(fd, buff, BUF - 1);
	if (read_b < 0)
		return (printf("Error: read 1st\n"), 1);
	buff[read_b] = '\0';

	taboo = parse_buff(buff);

	
	if (close(fd))
		return (printf("Error: close\n"), 1);
	return (0);
}