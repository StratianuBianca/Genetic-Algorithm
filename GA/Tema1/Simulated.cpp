#include<iostream>
#include<cmath>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<chrono>
#include<random>
#define pi 3.14
using namespace std;
using namespace std::chrono;
int n, precizie = 5;
float a, b, interval;
double value;
char functie, tipul;
int L;
int vc[601], vn[601];
std::mt19937 generator(time(0));
std::uniform_real_distribution<float> dis(0, 0.999);
std::mt19937 generator1(generator);
float convert_to_dec(int b[], short prec, short dim, short st, short dr)
{
	int p = 1;
	short dim_decimals = prec * log2(10) + 1;
	float p10 = 1.0, down_aprox = 0, decimals = 0;

	for (short ii = 1; ii <= prec; ++ii)
		p10 *= 10;

	for (short ii = st + 1; ii < dr - dim_decimals; ++ii)
	{
		down_aprox = down_aprox + ((float)(b[ii]) * p);
		p <<= 1;
	}

	p = 1;
	for (short ii = dr - dim_decimals; ii < dr; ++ii)
	{
		decimals += b[ii] * p;
		p <<= 1;
	}

	float val = down_aprox + decimals / p10;
	if (b[st] == 1)
		val = -val;

	return val;
}
void initializare(char tip) {
	if (tip == 'r' || tip == 'd') {
		a = -5.12;
		b = 5.12;
	}
	else {
		if (tip == 'm') {
			a = 0;
			b = pi;
		}
		else {
			if (tip == 's') {
				a = -500;
				b = 500;
			}
		}
	}
	interval = b - a;
}
int length(float a, float b, int prec)
{
	return ceil(log2((interval)*pow(10, prec)));
}
float decode_dimension(int start, int end, int l, float a, float b, int v[])
{
	double bi = 0;
	for (auto ii = start; ii != end; ++ii)
	{
		bi *= 2;
		bi += v[ii];
	}
	double s = bi / (pow(2, l) - 1);
	return (s * interval + a);
}
double valoare(int v[], char func) {
	int ii, jj;
		double var;
	double value = 0;
	double power, bi, s;
	int l = length(a, b, precizie);
	for (jj = 0; jj < n; ++jj) {
		power = pow(2, l) / 2;
		bi = 0;
		s = 0;
		var = 0;
		for (ii = jj * l; ii <= (((jj + 1) * l) - 1); ii++) {
			bi = bi + v[ii] * power;
			power /= 2;
		}
		s = bi / (pow(2, l) - 1);
		var = s * interval + a;
		//var = decode_dimension(jj * l, ((jj + 1) * l - 1), l, a, b, v);
		if (func == 'r') {
			value = value + var * var - 10 * cos(2 * pi * var);
		}
		if (func == 'd') {
			value = value + var * var;
		}
		if (func == 's') {
			value = value - var * sin(sqrt(abs(var)));
		}
		if (func == 'm') {
			value = value - sin(var) * pow(sin(((jj + 1) * var * var) / pi), 20);
		}
	}
	if (func == 'r') {
		value = value + 10 * n;

	}
	return value;
}
int randomInt(int low, int upp) {
	std::random_device generator;
	std::uniform_int_distribution<int> distribution(low, upp);
	return distribution(generator);


}
double randomFloat(double low, double upp) {
	std::random_device generator;
std:uniform_real_distribution<double> distribution(low, upp);
	double result;
	result = distribution(generator);
	return result;
}
void random_neighbor(int vn[], int l, int n, int vc[]) {
	int random_index = randomInt(0, l * (n + 1) - 1);
	//int random_index = randomInt(0, l * (n) - 1);
	for (int ii = 0; ii < n * l; ii++) {
		vn[ii] = vc[ii];
	}
	vn[random_index] = 1 - vn[random_index];
	/*int ok = 1;
	double  x = 0;
	x = valoare(vn, functie);
	if (x < a && x > b)
		ok = 0;
	while (ok == 0) {
		int random_index = randomInt(0, l * (n + 1) - 1);
		//int random_index = randomInt(0, l * (n) - 1);
		for (int ii = 0; ii < n * l; ii++) {
			vn[ii] = vc[ii];
		}
		vn[random_index] = 1 - vn[random_index];
		x = convert_to_dec(vn,precizie, n, a, b );
		if (x >= a && x <= b)
			ok = 1;
	}*/
}
int main() {
	initializare('m');
	n = 5;
	functie = 'm';
	L = n * length(a, b, precizie);
	int ii, l = length(a, b, precizie);
	int d = 0;
	double me[30];
	int f = 0;
	auto start = high_resolution_clock::now();
	srand(time(0));
	while (d < 30) {
		double t = 1;
		double T = 1000;
		double Tn = T;
		double vnv, vcv;
		srand(time(0));
		int ok = 0;
		//while (ok == 0) {
			for (int ii = 0; ii < L; ++ii) {
				vc[ii] = rand() % 2;
				vn[ii] = rand() % 2;
			}
		/*	double x = decode_dimension(0, L, l, a, b, vc);
			cout << x << endl;
			if (x >= a && x <= b) {
				ok = 1;
			}
		}*/
		do {
			ii = 0;
			do {
				random_neighbor(vn, l, n, vc);
				vnv = valoare(vn, functie);
				vcv = valoare(vc, functie);
				//cout << vnv << "  " << vcv << endl;
				if (vnv < vcv) {
					for (int jj = 0; jj < n * l; ++jj) {
						vc[jj] = vn[jj];
					}
				}
				else {
					float p = randomFloat(0, 1);
					if (p < exp(-abs(vnv - vcv) / T)) {
						for (int jj = 0; jj < n * l; jj++) {
							vc[jj] = vn[jj];

						}
					}

				}
				ii++;
			//	cout << vnv << endl;

			} while (ii < 150);
			T = T * 0.999;
			t++;
		} while (T > 10e-8);
		double best = valoare(vc, functie);
		cout << best << " " << "Testul " << d << endl;
		d++;
		me[f] = best;
		f++;
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << endl;
	cout << endl;
	cout << endl;
	cout << duration.count() / 1000000.0 << "timpul" << endl;

	cout << endl;
	double medie=0;
	for (int i = 0; i < 30; i++) {
		medie = me[i] + medie;
	}
	double final = medie / 30;
	cout << final << "fct s";
	return 0;
}