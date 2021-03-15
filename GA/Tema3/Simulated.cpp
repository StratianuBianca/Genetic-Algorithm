#include<iostream>
#include<cmath>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<chrono>
#include<random>
#include<fstream>
#define len 323
using namespace std;
using namespace std::chrono;
ifstream fin("tspp.atsp");
int vc[601], vn[601];
int graf[len][len];
std::mt19937 generator(time(0));
std::uniform_real_distribution<float> dis(0, 0.999);
std::mt19937 generator1(generator);
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
int ran(int vizitat[]) {
	int ok = 0;
	int x;
	while (ok == 0) {
		x = rand() % len ;
		if (vizitat[x] == 0) {
			ok = 1;
			vizitat[x] = 1;
		}
	}
	return x;
}
void rand_n(int vn[], int vc[]) {
	int random_index = randomInt(0, len-1 );
	int random_index2 = randomInt(0, len-1);
	for (int ii = 0; ii < len; ii++) {
		vn[ii] = vc[ii];
	}
	int aux = vn[random_index];
	vn[random_index] = random_index2;
	for (int i = 0; i < len; i++) {
		if (i != random_index && vn[i] == random_index2) {
			vn[i] = aux;
		}
	}
}
float valoare(int v[]) {
	float val = 0;
	for (int i = 0; i < len-1; i++) {
		val = val + graf[v[i]][v[i+1]];
	}
	val = val + graf[v[len - 1]][v[0]];
	return val;
}
int main() {
	int i, j;
	for (i = 0; i < len; i++) {
		for (j = 0; j < len; j++) {
			fin >> graf[i][j];
			cout << graf[i][j] << " ";
		}
		cout << endl;
	}
	int d = 0;
	int ii;
	auto start = high_resolution_clock::now();
	srand(time(0));
	int vizitat1[len+1], vizitat2[len+1];
	for (int i = 0; i < len; i++){
		vizitat1[i] = 0;
		vizitat2[i] = 0;
}
	while (d < 30) {
		double t = 1;
		double T = 1000;
		double Tn = T;
		float vcv, vnv;
		for (int i = 0; i < len; i++) {
			vizitat1[i] = 0;
			vizitat2[i] = 0;
		}
		for (int ii = 0; ii < len; ++ii) {
			vc[ii] = ran(vizitat1);//rand() % (len-1);
				vn[ii] = ran(vizitat2);//rand() % (len-1);
		}
		do{

			ii = 0;
			do {
				rand_n(vn, vc);
				vnv = valoare(vn);
				vcv = valoare(vc);
				if (vnv < vcv) {
					for (int jj = 0; jj < len; ++jj) {
						vc[jj] = vn[jj];
					}
				}
				else {
					float p = randomFloat(0, 1);
					if (p < exp(-abs(vnv - vcv) / T)) {
						for (int jj = 0; jj < len; jj++) {
							vc[jj] = vn[jj];

						}
					}

				}
				ii++;
			} while (ii < 300);
			T = T * 0.999;
			t++;
		} while (T > 10e-8);
		double best = valoare(vc);
		cout << best << endl;
		d++;
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << endl;
	cout << endl;
	cout << endl;
	cout << duration.count() / 1000000.0 << "timpul" << endl;
	cout << endl;
	return 0;
}