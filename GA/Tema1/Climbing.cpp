#include<iostream>
#include<cmath>
#include<chrono>
#include<stdio.h>
#include<time.h>
#include<stdlib.h>

#define pi 3.14
using namespace std;
using namespace std::chrono;
int n, precizie = 5, L;
float a, b, interval;
double value;
char functie, tipul;
int vc[601], vn[601];

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
int length(float a, float b, int prec)
{
	return ceil(log2((interval)*pow(10, prec)));
}

double valoare(int v[], char func) {
	int ii, jj;
	double value = 0, var;
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
		s = bi/ (pow(2, l) - 1);
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

void inspect_neighbors(int n, int L, float a, float b, bool& local)
{
	double val = valoare(vc, functie);
	double min = INFINITY;
	for (int ii = 0; ii < L; ++ii)
	{
		vc[ii] = !vc[ii];
		val = valoare(vc, functie);
		if (val < min)
		{
			min = val;
			for (int i = 0; i < L; i++) {
				vn[i] = vc[i];
			}
		}


		vc[ii] = !vc[ii];
	}
	if (min < value) {
		value = min;
		for (int ii = 0; ii < L; ii++)
			vc[ii] = vn[ii];
	}
	else {
		local = true;
	}
}

void inspect_neighborsf(int n, int L, float a, float b, bool& local, double& value) {
	int ok = 1, jj;
	double val;
	for (jj = 0; jj < L && ok == 1; jj++) {
		vc[jj] = !vc[jj];
		val = valoare(vc, tipul);
		if (val < value) {
			for (int ii = 0; ii < L; ii++)
				vn[ii] = vc[ii];
			ok = 0;
			value = val;
		}
		vc[jj] = !vc[jj];
	}
	if (ok == 0) {
		for (int ii = 0; ii < L; ii++)
			vc[ii] = vn[ii];
	}
	else {
		local = true;
	}
}
void improve(int vc[], int vn[], char tipul, double value) {
	if (tipul == 'f') {
		int ok = 0;
		double minim = valoare(vn, functie);
		for (int ii = 0; ii < L &&ok==0; ii++) {
			vc[ii] = vn[ii];
		}
		for (int ii = 0; ii < L; ii++) {
			vc[ii] = 1 - vc[ii];
			double val = valoare(vc, functie);
			if (val < minim)
				ok = 1;
			else {
				vc[ii] = 1 - vc[ii];
			}
		}
	}
	else {
		double minim = valoare(vn, functie);
		for (int ii = 0; ii < L; ii++) {
			vc[ii] = vn[ii];
		}
		for (int ii = 0; ii < L; ii++) {
			vc[ii] = 1 - vc[ii];
			double val = valoare(vc, functie);
			if (val < minim) {
				minim = val;
			}
			vc[ii] = 1 - vc[ii];
		}
	}
}
int main() {
	functie = 's';
	tipul = 'b';
	n = 30;
	initializare(functie);
	double medie[30];
	int d = 0;
	L = n * length(a, b, precizie);
	int l = length(a, b, precizie);
	int t = 0, ii, jj;
	double best = INFINITY;
	//cout << best << " ";
	int max = 100;
	auto start = high_resolution_clock::now();
	int teste = 0;
	srand(time(0));
	do {
		best = INFINITY;
		t = 0;
		do {
			bool local = false;
			for (ii = 0; ii < L; ii++) {
				vn[ii] = rand() % 2;
				vc[ii] = rand() % 2;
			}
			value = valoare(vc, functie);
			double value2, minimum=100000;
			while (local != true) {
				if (tipul == 'b') {
					for (int jj = 0; jj < L; jj++) {
						vc[jj] = !vc[jj];
						value2 = valoare(vc, functie);
						if (value2 < minimum) {
							minimum = value2;
							for (int ii = 0; ii < L; ii++) {
								vn[ii] = vc[ii];
							}
						}
						vc[jj] = !vc[jj];
					}
					if (minimum < value) {
						value = minimum;
						for (int ii = 0; ii < L; ii++) {
							vc[ii] = vn[ii];
						}
					}
					else {
						local = true;
					}
				}
				else {
					if (tipul= 'f') {
						int ok = 1;
						for (int jj = 0; jj < L && ok == 1; jj++) {
							vc[jj] = !vc[jj];
							value2 = valoare(vc, functie);
							if (value2 < value) {
								for (int ii = 0; ii < L; ii++) {
									vn[ii] = vc[ii];
								}
								ok = 0;
								value = value2;
							}
							vc[jj] = !vc[jj];
						}
						if (ok == 0) {
							for (int ii = 0; ii < L; ii++) {
								vc[ii] = vn[ii];
							}
						}
						else {
							local = true;

						}
					}
				}
			}
			t++;
			if (value < best)
				best = value;
		} while (t < 100);
		medie[d] = best;
		d++;
		cout << best << " " << teste<<endl;
		teste++;
	} while (teste < 30);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << endl;
	cout << endl;
	cout << endl;
	cout << duration.count() / 1000000.0 <<"timpul"<< endl;
	cout << best;
	cout << endl;
	cout << endl;
	cout << endl;
	double me = 0;
	for (int i = 0; i < 30; i++)
		me = medie[i] + me;
	cout << "medie" << "---m cu b---" << double(me / 30);
	double dev = 0;
	for (int i = 0; i < 30; i++) {
		dev = (medie[i] - me) * (medie[i] - me) + dev;
	}
	dev = dev / 30;
	dev = sqrt(dev);
	cout << endl;
	cout << "deviatia" << dev;
	return 0;
}