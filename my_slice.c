#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

static void do_grep(regex_t *pat, FILE *f);

int
main(int argc, char *argv[])
{
    regex_t pat;
    int err;
    int i;

    if (argc < 2) {
        fputs("no pattern\n", stderr);
        exit(1);
    }
    err = regcomp(&pat, argv[1], REG_EXTENDED | REG_NOSUB | REG_NEWLINE);
    if (err != 0) {
        char buf[1024];

        regerror(err, &pat, buf, sizeof buf);
        puts(buf);
        exit(1);
    }
    if (argc == 2) {
        do_grep(&pat, stdin);
    }
    else {
        for (i = 2; i < argc; i++) {
            FILE *f;
            
            f = fopen(argv[i], "r");
            if (!f) {
                perror(argv[i]);
                exit(1);
            }
            do_grep(&pat, f);
            fclose(f);
        }
    }
    regfree(&pat);
    exit(0);
}

static void
do_grep(regex_t *pat, FILE *src)
{
    char buf[4096];
	
    while (fgets(buf, sizeof buf, src)) {
         regmatch_t str_match[1];
	 if (regexec(pat, buf, 1, str_match, 0) == 0) {
	     char *str = buf + str_match[0].rm_so;
	     regoff_t len = str_match[0].rm_eo - str_match[0].rm_so;

	     printf("%d\t%d\n\n", str_match[0].rm_eo, str_match[0].rm_so);
		// print for debug
	     fwrite(str, len, 1, stdout);
	     fputc('\n', stdout);
         }
    }
}
