#include <errno.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct publickey { uint64_t n, e; };
struct privatekey { uint64_t n, d; };
struct keypair {
	struct publickey pub;
	struct privatekey priv;
};

static bool isprime(uint64_t n) {
	// This checks more numbers than necessary but whatever
	for (uint64_t m = 2; m < n; ++m) if (n % m == 0) return false;
	return true;
}

// Properly computes (base ^ exponent) % modulo
static uint64_t modpow(uint64_t base, uint64_t exponent, uint64_t modulo) {
	if (modulo == 1) return 0;
	uint64_t result = 1;
	base %= modulo;
	while (exponent) {
		if (exponent & 1) result = (result * base) % modulo;
		exponent >>= 1;
		base = (base * base) % modulo;
	}
	return result;
}

static uint64_t gcd(uint64_t a, uint64_t b) {
	uint64_t d = 0;
	while (!(a & 1) && !(b & 1)) {
		a >>= 1;
		b >>= 1;
		++d;
	}
	while (!(a & 1)) a >>= 1;
	while (!(b & 1)) b >>= 1;
	while (a != b) {
		if (a > b) {
			a -= b;
			while (!(a & 1)) a >>= 1;
		} else {
			b -= a;
			while (!(b & 1)) b >>= 1;
		}
	}
	return a * (1 << d);
}

// Solves for y in 1 = (x * y) mod modulo
static uint64_t invmod(uint64_t x, uint64_t modulo) {
	uint64_t r0 = modulo, r1 = x;
	uint64_t u0 = 0, u1 = 1;
	while (r1) {
		const uint64_t q = r0 / r1, r2 = r0, u2 = u0;
		r0 = r1;
		r1 = r2 - q * r1;
		u0 = u1;
		u1 = u2 - q * u1;
	}
	return u0;
}

struct keypair keygen(uint64_t p, uint64_t q) {
	struct keypair ks = {{.n = p * q}, {.n = p * q}};
	const uint64_t phi = (p - 1) * (q - 1);
	// There's probably a well-established way of picking a good value
	// for e that maximizes private key strength but I do not know it
	// So just try out one less than the powers of two until one fits
	// and doesn't result in a private key of d=1 lol
	for (uint64_t i = 64; i; --i) {
		ks.pub.e = (1 << i) - 1;
		if (1 < ks.pub.e && ks.pub.e < phi && gcd(ks.pub.e, phi) == 1) {
			ks.priv.d = invmod(ks.pub.e, phi);
			if (ks.priv.d != 1) break;
		}
	}
	return ks;
}

uint64_t rsa_encrypt(uint64_t msg, struct publickey key) {
	return modpow(msg, key.e, key.n);
}

uint64_t rsa_decrypt(uint64_t msg, struct privatekey key) {
	return modpow(msg, key.d, key.n);
}

int main(int argc, const char *argv[static argc]) {
	if (argc < 2) {
		fputs("Mode and arguments required\n", stderr);
		goto badusage;
	}

	if (!strcmp(argv[1], "keygen")) {
		if (argc < 4) {
			fputs("Two prime numbers P and Q required\n", stderr);
			goto badusage;
		}
		bool badusage = false;
		char *endptr = nullptr;
		errno = 0;
		const uint64_t p = strtoull(argv[2], &endptr, 0);
		if (errno || endptr == argv[2] || !isprime(p)) {
			badusage = true;
			if (errno) perror("Bad P"); else fputs("Bad P\n", stderr);
		}
		errno = 0;
		const uint64_t q = strtoull(argv[3], &endptr, 0);
		if (errno || endptr == argv[3] || !isprime(p)) {
			badusage = true;
			if (errno) perror("Bad Q"); else fputs("Bad Q\n", stderr);
		}
		if (badusage) goto badusage;
		struct keypair ks = keygen(p, q);
		printf(
			"Public key: e = %" PRIu64 ", n = %" PRIu64 "\n"
			"Private key: d = %" PRIu64 ", n = %" PRIu64 "\n",
			ks.pub.e, ks.pub.n, ks.priv.d, ks.priv.n
		);
		return 0;
	} else if (!strcmp(argv[1], "encrypt")) {
		if (argc < 5) {
			fputs("Message and public key E and N required\n", stderr);
			goto badusage;
		}
		bool badusage = false;
		char *endptr = nullptr;
		errno = 0;
		const uint64_t msg = strtoull(argv[2], &endptr, 0);
		if (errno || endptr == argv[2]) {
			badusage = true;
			if (errno) perror("Bad msg"); else fputs("Bad msg\n", stderr);
		}
		errno = 0;
		struct publickey k = {};
		k.e = strtoull(argv[3], &endptr, 0);
		if (errno || endptr == argv[3]) {
			badusage = true;
			if (errno) perror("Bad E"); else fputs("Bad E\n", stderr);
		}
		errno = 0;
		k.n = strtoull(argv[4], &endptr, 0);
		if (errno || endptr == argv[4]) {
			badusage = true;
			if (errno) perror("Bad N"); else fputs("Bad N\n", stderr);
		}
		if (badusage) goto badusage;
		printf("%#016" PRIx64 "\n", rsa_encrypt(msg, k));
		return 0;
	} else if (!strcmp(argv[1], "decrypt")) {
		if (argc < 5) {
			fputs("Message and private key D and N required\n", stderr);
			goto badusage;
		}
		bool badusage = false;
		char *endptr = nullptr;
		errno = 0;
		const uint64_t msg = strtoull(argv[2], &endptr, 0);
		if (errno || endptr == argv[2]) {
			badusage = true;
			if (errno) perror("Bad msg"); else fputs("Bad msg\n", stderr);
		}
		errno = 0;
		struct privatekey k = {};
		k.d = strtoull(argv[3], &endptr, 0);
		if (errno || endptr == argv[3]) {
			badusage = true;
			if (errno) perror("Bad D"); else fputs("Bad D\n", stderr);
		}
		errno = 0;
		k.n = strtoull(argv[4], &endptr, 0);
		if (errno || endptr == argv[4]) {
			badusage = true;
			if (errno) perror("Bad N"); else fputs("Bad N\n", stderr);
		}
		if (badusage) goto badusage;
		printf("%" PRIu64 "\n", rsa_decrypt(msg, k));
		return 0;
	}
	fputs("Invalid mode", stderr);
badusage:
	fprintf(stderr,
		"Usage:\n"
		"\t%s keygen  P Q\n"
		"\t%s encrypt MSG E N\n"
		"\t%s decrypt MSG D N\n",
		argv[0], argv[0], argv[0]
	);
	return 2;
}
