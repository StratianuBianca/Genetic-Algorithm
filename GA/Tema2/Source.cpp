#include <iostream>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <stdio.h>
#include <string.h>
#include <random>
#include<fstream>
//ofstream fout("ra30.out");
using namespace std;
using namespace std::chrono;
#define POP_SIZE 50
#define PI 3.14
#define MAX 100
ofstream fout("r30cu01.out");
bool pop[100][1000];
bool pop2[100][1000];
float best;
short n, prec = 3;
int max_dec;
int c;
std::mt19937 generator(time(0));
std::mt19937 generator1(generator);
std::uniform_int_distribution<int> dis(0, 1);
float convert_to_dec(bool b[], short prec, short dim, short st, short dr)
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

short nr_bits_1var(float a, float b, short prec)
{
    short nb = 0;

    if (a < 0)
        nb = prec * log2(10) + log2(max(abs(a), abs(b)));
    else
        nb = prec * log2(10.0) + log2((b - a));

    if (((int)(b) & (int)(b - 1)) == 0 && b - (int)b < 0.2)
        nb += 1 + 1 + 1; 
    else
        nb += 2;

    return nb;
}
void init(float& a, float& b, const char& f)
{
    if (f == 'R' || f == 'J')
        a = -5.12, b = 5.12;

    else if (f == 'S')
        a = -500, b = 500;
    else
        a = 0, b = PI;
}
float R(bool b[], short dim, short prec)
{
    float s = 10 * n, x;
    for (short ii = 1; ii < n * dim; ii += dim)
    {
        x = convert_to_dec(b, prec, dim, ii - 1, ii + dim - 1);
        s += x * x - 10 * cos(2 * PI * x);
    }
    return s;
}
float J(bool b[], short dim, short prec)
{
    float s = 0, x;
    for (short ii = 1; ii < n * dim; ii += dim)
    {
        x = convert_to_dec(b, prec, dim, ii - 1, ii + dim - 1);
        s += x * x;
    }
    return s;
}
float S(bool b[], short dim, short prec)
{
    float s = 0, x;
    for (short ii = 1; ii < n * dim; ii += dim)
    {
        x = convert_to_dec(b, prec, dim, ii - 1, ii + dim - 1);
        s = s - x * sin(sqrt(abs(x)));
    }
    return s;
}
float M(bool b[], short dim, short prec)
{
    float s = 0, x, put = 1;
    short jj = 0, kk;
    for (short ii = 1; ii < n * dim; ii += dim)
    {
        jj++;
        x = convert_to_dec(b, prec, dim, ii - 1, ii + dim - 1);
        put = sin((float)(jj)*x * x / PI);

        for (kk = 2; kk <= 20; ++kk)
            put *= sin((float)(jj)*x * x / PI);

        s += sin(x) * put;
    }
    s = -s;
    return s;
}
double eval[100], p[100], q[102], prob[100];
short select[101];
double pm = 0.1, pcx = 0.2;

bool compare(short a, short b)
{
    if (prob[a] <= prob[b])
        return 1;

    return 0;
}

double f(bool b[], short dim, short prec, const char& opt)
{
    if (opt == 'R')
        // return pow(1 / R(b, dim, prec), 4);

        return 1 / (R(b, dim, prec) + 0.0000001);

    else if (opt == 'J')
        return 1 / J(b, dim, prec);

    else if (opt == 'M')
        return -M(b, dim, prec);

    return c - S(b, dim, prec);

}
void random(bool pop[], int dim, int dimdec) {
    int jj, ii;
    for (jj = 0; jj < dim * n; ++jj) {
        pop[jj] = dis(generator1);
    }
}
int main() {
    srand(time(0));
    auto start = high_resolution_clock::now();
    double med[40];
    int ll = 1;
    float a, b;
    int ii, dim, dimdec, jj;
    n = 30;
    c = n * 500;
    init(a, b, 'R');
    for (ii = 1; ii <= prec; ++ii)
        max_dec = max_dec * 10 + 9;
    dim = nr_bits_1var(a, b, 3);
    dimdec = prec * log2(10) + 1;
    for (ii = 1; ii < 5; ii++)
        generator();
    std::uniform_real_distribution<double> dis2(0, 1);
    std::uniform_int_distribution<short> dis3(1, n * dim);


    best = (1 << 20);
    for (int k = 0; k < 30; ++k) {
        best = (1 << 20);
        for (ii = 0; ii < 100; ++ii)
            memset(pop[ii], 0, sizeof(pop[ii]));
        for (ii = 0; ii < 100; ++ii) {
            random(pop[ii], dim, dimdec);
            eval[ii] = f(pop[ii], dim, prec, 'R');
        }
        for (int rr = 0; rr < 10000; ++rr) {

            int nrs = 0;
            double t = 0;
            for (ii = 0; ii < 100; ++ii)
                t = t + eval[ii];
            for (ii = 0; ii < 100; ++ii)
                p[ii] = eval[ii] / t;
            q[0] = 0;
            for (ii = 0; ii < 100; ++ii)
                q[ii + 1] = q[ii] + p[ii];
            for (ii = 0; ii < 100; ++ii) {
                double r = dis2(generator1);
                for (jj = 0; jj < 100; ++jj)
                    if (q[jj] < r && r <= q[jj + 1]) {
                        select[nrs++] = jj;
                        break;
                    }
            }
            for (int l = 0; l < 100; l++) {
                for (int k = 0; k < n * dim; k++) {
                    pop2[l][k] = pop[select[l]][k];
                }
            }
            // nrs = nrs2;
            for (ii = 0; ii < nrs; ++ii)
                for (jj = 0; jj < n * dim; ++jj)
                    if (dis2(generator1) < pm)
                        pop2[ii][jj] = !pop2[ii][jj];
            for (ii = 0; ii < nrs; ++ii) {
                select[ii] = ii;
                prob[ii] = dis2(generator1);
            }

            for (int i = 0; i < nrs; i++) {
                for (int j = i + 1; j < nrs; j++) {
                    if (prob[j] < prob[i]) {
                        int aux;
                        aux = prob[j];
                        prob[j] = prob[i];
                        prob[i] = aux;
                        aux = select[j];
                        select[j] = select[i];
                        select[i] = aux;
                    }

                }
            }
            for (ii = 0; ii < nrs; ii = ii + 2)
                if (prob[ii + 1] < pcx) {
                    int cutp = dis3(generator1);
                    for (jj = cutp; jj < n * dim; ++jj)
                        swap(pop2[select[ii]][jj], pop2[select[ii + 1]][jj]);
                }
                else
                    break;

            for (ii = 0; ii < 100; ++ii)
                eval[ii] = f(pop2[ii], dim, prec, 'R');
            for (int l = 0; l < 100; l++)
                for (int k = 0; k < n * dim; k++)
                    pop[l][k] = pop2[l][k];
            for (jj = 0; jj < 100; ++jj) {
                if (R(pop[jj], dim, prec) < best)
                    best = R(pop[jj], dim, prec);
                // cout << best << endl;
            }
            // cout << rr << " " << best << endl;

        }
        med[ll] = best;
        cout << med[ll] << endl;
        //  cout << endl << "Timpul: " << duration.count() / 1000000.0 << endl;
        ll++;
    }
    // cout << endl << "Timpul: " << duration.count() / 1000000.0 << endl;
    double suma = 0;
    for (ii = 1; ii <= 30; ii++) {
        fout << "iteratia " << ii << "valoarea:" << med[ii] << endl;
        suma = suma + med[ii];
    }
    fout << endl;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    fout << endl << "Timpul: " << duration.count() / 1000000.0 << endl;
    fout << endl;
    double medie = suma / 30;
    fout << "media :" << medie << endl;
    //cout << best << endl;
    return 0;
}