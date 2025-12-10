#include <string.h> //strlen
#include <ctype.h> //isascii

void	rev_polish_word(char *str)
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
