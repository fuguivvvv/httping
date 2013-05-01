/* Released under GPLv2 with exception for the OpenSSL library. See license.txt */
/* $Revision$ */

#ifdef __GNUC__
#define _GNU_SOURCE
#endif
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#include "error.h"

double get_ts(void)
{
	struct timeval ts;
       	struct timezone tz;

	if (gettimeofday(&ts, &tz) == -1)
		error_exit("gettimeofday failed");

	return (double)ts.tv_sec + ((double)ts.tv_usec)/1000000.0 + (double)(tz.tz_minuteswest * 60);
}

void split_string(const char *in, const char *split, char ***list, int *list_n)
{
	char *copy = strdup(in), *pc = copy;
	int split_len = strlen(split);

	for(;;)
	{
		char *term = strstr(pc, split);

		if (term)
			*term = 0x00;

		*list = (char **)realloc(*list, (*list_n + 1) * sizeof(char *));
		(*list)[*list_n] = strdup(pc);
		(*list_n)++;

		if (!term)
			break;

		pc = term + split_len;
	}

	free(copy);
}

void free_splitted_string(char **list, int n)
{
	int index=0;

	for(index=0; index<n; index++)
		free(list[index]);

	free(list);
}

void str_add(char **to, const char *what, ...)
{
	int len_to = *to ? strlen(*to) : 0;
	char *buffer = NULL;
	int len_what = 0;

        va_list ap;

	/* FIXME: should aprintf here: does solaris have aprintf? */
        va_start(ap, what);
        len_what = vasprintf(&buffer, what, ap);
        va_end(ap);

	*to = (char *)realloc(*to, len_to + len_what + 1);

	memcpy(&(*to)[len_to], buffer, len_what);

	(*to)[len_to + len_what] = 0x00;

	free(buffer);
}
