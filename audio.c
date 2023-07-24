#include "udp.h"
#include "json.h"
#include "openal.h"
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
	char buf[65536];
	double **mat;
	int r;
	int m, n;
	if ((r = init_openal()) != 0)
		return r;
	int listener = init_udp_listen(12345);

	r = read(listener, buf, 65536);
	mat = parse_fft_json(buf);

	delete_openal();
}

