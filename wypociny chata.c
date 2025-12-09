#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

// #define BUF 1000

// static int ft_isupper(char c)
// {
// 	return ((c >= 'A' && c <= 'Z'));
// 		// || c == 'Ą' || c == 'Ć' || c == 'Ę'
// 		// || c == 'Ł' || c == 'Ń' || c == 'Ó'
// 		// || c == 'Ś' || c == 'Ź' || c == 'Ż');
// }

// int	parse_buff(char *buff)
// {
// 	int i = 0;
// 	int hasla = 0;
// 	while (buff[i])
// 	{
// 		while (buff[i] == ' ')
// 			i++;
// 		if (ft_isupper(buff[i]) && buff[i + 1] && ft_isupper(buff[i + 1]))
// 			hasla++;
// 		while (buff[i] && buff[i] != ' ')
// 			i++;
// 	}
// 	return (hasla);
// }

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// ---------------- UTF-8 reverse ----------------
// Odwraca słowo UTF-8 (poprawnie dla polskich znaków)
char *utf8_reverse(const char *s) {
    int len = strlen(s);
    char *rev = malloc(len + 1);

    const char *p = s + len;
    char *w = rev;

    while (p > s)
	{
        // cofamy się do początku znaku UTF-8
        const char *start = p - 1;

        // szukamy pierwszego bajtu sekwencji
        while (start > s && ((*start & 0xC0) == 0x80))
            start--;

        int clen = p - start;
        memcpy(w, start, clen);
        w += clen;

        p = start;
    }

    *w = '\0';
    return rev;
}

// ---------------- Rozpoznawanie dużych słów ----------------
bool is_upper_utf8(const char *s) {
    // wystarczy sprawdzić ASCII + polskie wielkie litery
    while (*s) {
        unsigned char c = *s;

        // ASCII małe → fałsz
        if (c >= 'a' && c <= 'z')
            return false;

        // polskie małe zaczynają się od bajtów 0xC4–0xC5 itd.
        // ale łatwo je poznać po tym, że drugi bajt >= 0xB9
        if ((unsigned char)s[0] >= 0xC4 && (unsigned char)s[0] <= 0xC5) {
            if ((unsigned char)s[1] >= 0xB9)  // ó ą ę ć ł ś ź ż – wersje małe
                return false;
        }

        // skok po UTF-8
        if (c < 0x80) s += 1;
        else if ((c >> 5) == 0x6) s += 2;
        else if ((c >> 4) == 0xE) s += 3;
        else if ((c >> 3) == 0x1E) s += 4;
        else s++;
    }

    return true;
}

// ---------------- Wczytywanie taboo.txt ----------------
char ***load_taboo(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Nie mogę otworzyć pliku!\n");
        return NULL;
    }

    // 12 kolumn, 6 słów każda
    int COLS = 12;
    int ROWS = 6;

    char ***taboo = malloc(COLS * sizeof(char**));
    for (int i = 0; i < COLS; i++)
        taboo[i] = malloc(ROWS * sizeof(char*));

    char word[256];
    int col = 0, row = 0;

    while (fscanf(f, "%255s", word) == 1) {

        char *w = strdup(word);

        // jeśli to słowo DUŻE – jest odwrócone, trzeba odwrócić
        if (is_upper_utf8(w)) {
            char *fixed = utf8_reverse(w);
            free(w);
            w = fixed;
        }

        // zapisywanie do struktury
        taboo[col][row] = w;

        col++;
        if (col == COLS) {
            col = 0;
            row++;
            if (row == ROWS) break;
        }
    }

    fclose(f);
    return taboo;
}


// int main()
// {
// 	// char ***taboo;

// 	int fd = open("cz1.txt", O_RDONLY);
// 	if (fd < 0)
// 		return (printf("Error: open\n"), 1);

// 	char	buff[BUF] = "";
// 	int	read_b = read(fd, buff, BUF - 1);
// 	if (read_b < 0)
// 		return (printf("Error: read 1%i\n", read_b), 1);
// 	buff[read_b] = '\0';

// 	int hasla = parse_buff(buff);

// 	printf("hasla = %i\n%s\n", hasla, buff);
// 	if (close(fd))
// 		return (printf("Error: close\n"), 1);
// 	return (0);
// }

int main() {
    char ***taboo = load_taboo("cz1.txt");

    // Przykład:
    printf("taboo[0][0] = %s\n", taboo[0][0]);  // GIMNAZJUM
    printf("taboo[0][1] = %s\n", taboo[0][1]);  // Nauka
    printf("taboo[0][2] = %s\n", taboo[0][2]);  // Dzieci

    printf("taboo[4][0] = %s\n", taboo[4][0]);  // KARK
    printf("taboo[4][1] = %s\n", taboo[4][1]);  // Nagroda

    return 0;
}
