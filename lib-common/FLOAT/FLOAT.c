#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	// nemu_assert(0);
	//return 0;
	long long result = (long long )a * (long long)b;
	result = result >>16;
	return (FLOAT)result;	
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	/* Dividing two 64-bit integers needs the support of another library
	 * `libgcc', other than newlib. It is a dirty work to port `libgcc'
	 * to NEMU. In fact, it is unnecessary to perform a "64/64" division
	 * here. A "64/32" division is enough.
	 *
	 * To perform a "64/32" division, you can use the x86 instruction
	 * `div' or `idiv' by inline assembly. We provide a template for you
	 * to prevent you from uncessary details.
	 *
	 *     asm volatile ("??? %2" : "=a"(???), "=d"(???) : "r"(???), "a"(???), "d"(???));
	 *
	 * If you want to use the template above, you should fill the "???"
	 * correctly. For more information, please read the i386 manual for
	 * division instructions, and search the Internet about "inline assembly".
	 * It is OK not to use the template above, but you should figure
	 * out another way to perform the division.
	 */

	// nemu_assert(0);
	// return 0;
	int flag = 1;
	if (a<0) {
		flag = -flag;
		a = -a;
	}
	if (b<0) {
		flag = -flag;
		b = -b;
	}
	int result = a / b;
	a = a % b;
	int i;
	for(i = 0; i < 16; i++) {
		a = a<<1;
		result = result<<1;
		if (a >= b) {
			a = a-b;
			result ++;
		} 
	}
	return flag * result;
}

FLOAT f2F(float a) {
	/* You should figure out how to convert `a' into FLOAT without
	 * introducing x87 floating point instructions. Else you can
	 * not run this code in NEMU before implementing x87 floating
	 * point instructions, which is contrary to our expectation.
	 *
	 * Hint: The bit representation of `a' is already on the
	 * stack. How do you retrieve it to another variable without
	 * performing arithmetic operations on it directly?
	 */

	// nemu_assert(0);
	// return 0;
	int help = *(int *)&a;
	int flag = help >> 31;
	int exp = (help >> 23) & 0xff;
	FLOAT result = help & 0x7fffff;
	if (exp != 0) {
		result += 1 << 23;
	}
	exp -= 150;
	if (exp < -16) result >>= -16 - exp;
	if (exp > -16) result <<= exp + 16;
	return flag == 0 ? result : -result;
}

FLOAT Fabs(FLOAT a) {
	// nemu_assert(0);
	// return 0;
	return a<0? -a:a;
}

/* Functions below are already implemented */

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

