#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double** parse_fft_json(char *s) {
	double **ret = NULL;
	int m, n;

	if (strncmp(s, "{\"type\":\"fft\",\"data\":", 21) != 0)
		return NULL;

	s += 21;
	if (*s != '[')
		return NULL;
	m = 0;
	s++;
	while(*s == '[') {
		m++;
		ret = realloc(ret, m * sizeof(double*));
		ret[m-1] = NULL;
		s++;
		n = 1;
		do {
			ret[m - 1] = realloc(ret[m - 1], n * sizeof(double));
			ret[m - 1][n - 1] = atof(s);
			while(*s != ',' && *s != ']')
				s++;
			if (*s == ',')
				s++;
			n++;
		} while(*s != ']');
		while(*s == ']' || *s == ',')
			s++;
	}

	//fprintf(stderr, "%dx%d\n", m, n);

	return ret;
}
