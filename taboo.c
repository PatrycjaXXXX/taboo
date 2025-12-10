#include <unistd.h> //read, close
#include <fcntl.h> //open
#include <stdio.h> //printf
#include <string.h> //strlen
#include <ctype.h>
#include <stdlib.h>

#define FAIL 1
#define SUCCESS 0

#define CARDS 1000
#define WORDS_PER_CARD 6
#define MAX_WORD_LEN 50
#define MAX_WORDS_PER_LINE 4

// void	rev_polish_word(char *str);
char *gnl(int fd);

int	spaces_split(char ***taboo, int *line_count, char *line)
{
	int	i;
	int	j;
	int	spaces_count;
	int	row_count;

	if (!line || !line_count || !taboo)
		return (FAIL);
	
	row_count = 0;

	// if (line[0] == '\n')
		// start_reversing and move to the next line następne carty;
	i = 0;
	while (line[i])
	{
		while (line[i] && line[i] == ' ')
			i++;
		spaces_count = 0;
		j = 0;
		while (line[i] && line[i] != ' ' && j < MAX_WORD_LEN)
		{
			taboo[row_count + MAX_WORDS_PER_LINE * (*line_count) / 7][(*line_count % 7) - 1][j] = line[i];
			printf("%c", line[i]);
			j++;
			i++;
		}
		if (line[i] == ' ' && ((line[i + 1] && line [i + 1]) == ' ' || !line[i + 1]))
		{
			taboo[row_count + MAX_WORDS_PER_LINE * (*line_count) / 7][(*line_count % 7) - 1][j] = '\0';
			row_count++;
		}
		if (row_count > MAX_WORDS_PER_LINE - 1)
			return (SUCCESS);
	}
	return (SUCCESS);
}

int	main()
{
	char	taboo[CARDS][WORDS_PER_CARD][MAX_WORD_LEN];
	char	*line;
	int		line_count;

	int fd = open("cz1.txt", O_RDONLY);
	if (fd < 0)
		return (printf("Error: open\n"), 1);
	
	line_count = 1;
	line = gnl(fd);
	spaces_split(taboo, &line_count, line);

	if (close(fd))
		return (printf("Error: close\n"), 1);
	return (0);
}