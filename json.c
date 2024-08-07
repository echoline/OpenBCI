#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double** parse_fft_json(char *s) {
	double **ret = NULL;
	int m, n;
	float max = 0;

	if (strncmp(s, "{\"type\":\"fft\",\"data\":", 21) == 0)
		s += 21;
	else if (strncmp(s, "{\"type\": \"fft\", \"data\": ", 24) == 0)
		s += 24;
	else
		return NULL;

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
			if (ret[m - 1][n - 1] > 200)
				ret[m - 1][n - 1] = 0;
			while(*s != ',' && *s != ']')
				s++;
			if (*s == ',')
				s++;
			n++;
		} while(*s != ']');
		while(*s == ']' || *s == ',' || *s == ' ')
			s++;
	}

	//fprintf(stderr, "%dx%d\n", m, n);

	return ret;
}
