#include <stdlib.h>
#include <string.h>

#include "utils.h"

void add_cookie(char ***cookies, int *n_cookies, char *in)
{
	*cookies = (char **)realloc(*cookies, (*n_cookies + 1) * sizeof(char *));

	(*cookies)[*n_cookies] = strdup(in);

	(*n_cookies)++;
}

void combine_cookie_lists(char ***destc, int *n_dest, char **src, int n_src)
{
	int loop = 0;

	*destc = (char **)realloc(*destc, (*n_dest + n_src) * sizeof(char *));

	for(loop=0; loop<n_src; loop++)
		(*destc)[*n_dest + loop] = strdup(src[loop]);

	(*n_dest) += n_src;
}

void free_cookies(char **dynamic_cookies, int n_dynamic_cookies)
{
	int index = 0;

	for(index=0; index<n_dynamic_cookies; index++)
		free(dynamic_cookies[index]);

	free(dynamic_cookies);
}

void get_cookies(const char *headers, char ***dynamic_cookies, int *n_dynamic_cookies)
{
	int index = 0;
	char **header_lines = NULL;
	int n_header_lines = 0;

	split_string(headers, "\r\n", &header_lines, &n_header_lines);

	for(index=0; index<n_header_lines; index++)
	{
		char *result = NULL;
		int cparts_index = 0;
		char **cparts = NULL;
		int n_cparts = 0;

		if (strncmp(header_lines[index], "Set-Cookie:", 11) != 0)
			continue;

		split_string(&header_lines[index][12], ";", &cparts, &n_cparts);

		for(cparts_index=0; cparts_index<n_cparts; cparts_index++)
		{
			char *part = cparts[cparts_index];

			while(*part == ' ')
				part++;

			if (strncmp(part, "expires=", 8) == 0)
				continue;

			if (strncmp(part, "path=", 5) == 0)
				continue;

			if (strncmp(part, "domain=", 7) == 0)
				continue;

			if (strncmp(part, "HttpOnly", 8) == 0)
				continue;

			str_add(&result, "%s ", part);
		}

		free_splitted_string(cparts, n_cparts);

		add_cookie(dynamic_cookies, n_dynamic_cookies, result);

		free(result);
	}

	free_splitted_string(header_lines, n_header_lines);
}
