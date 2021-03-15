#include<iostream>
#include<fstream>
#include<string>

#define length 17
#define pop_size 500
#define sizes (length)

using namespace std;

ifstream fin("rbg323.atsp");
ofstream fout("t1.out");
float mutation_probab=0.001
float cross_probab = 0.2;
int pop[pop_size][sizes], newpop[pop_size][sizes], graf[length][length];
float popfit[pop_size], selection_probab[pop_size + 1];
int rez[30];
int TSP(int* m)
{
    int cost = 0;
    for (int i = 0; i < length - 1; i++)
    {
        cost = cost + graf[m[i]][m[i + 1]];
    }
    cost = cost + graf[m[length - 1]][m[0]];
    return cost;
}

void decodare(int* a, int* res)
{
    int visited[length] = { 0 };
    for (int i = 0; i < length; i++)
    {
        int position = 0;
        for (int j = 0; j < length; j++)
        {
            if (visited[j] == 0)
            {
                ++position;
                if (position == a[i])
                {
                    res[i] = j;
                    visited[j] = 1;
                    break;
                }
            }
        }
    }
}

int eval_pop()
{
    int res_eval[sizes], minim = 99999999;
    for (int i = 0; i < pop_size; ++i)
    {
        decodare(pop[i], res_eval);
        if (TSP(res_eval) < minim) {
            minim = TSP(res_eval);
        }
    }
    return minim;
}

void eval_popfitness()
{
    int res_fit[sizes];
    for (int i = 0; i < pop_size; i++)
    {
        decodare(pop[i], res_fit);
        popfit[i] = 1.0 / TSP(res_fit);
    }
}

void cross_over(int i1, int i2)
{
    int ind = rand() % sizes, a;
    for (int i = ind; i < sizes; ++i)
    {
        a = pop[i1][i];
        pop[i1][i] = pop[i2][i];
        pop[i2][i] = a;
    }
}

void AG()
{
    int best = 99999999;
    int runs = 0;
    while (++runs <= 30)
    {
        for (int i = 0; i < pop_size; ++i)
        {
            for (int j = 0; j < sizes; ++j)
            {
                pop[i][j] = rand() % (length - j) + 1;
            }
        }
        if (eval_pop() < best)
            best = eval_pop();
        int t = 0;
        while (++t <= 9000)
        {
            if (t == 3000) {
                cross_probab = 0.5;
            }
            if (t == 3500) {
                cross_probab = 0.2;
                mutation_probab = mutation_probab * 0.1;
            }
            float fit_total = 0;
            eval_popfitness();
            selection_probab[0] = 0;
            for (int i = 0; i < pop_size; ++i)
                fit_total = fit_total + popfit[i];
            for (int i = 0; i < pop_size; ++i)
                popfit[i] = popfit[i] / fit_total;
            for (int i = 0; i < pop_size; ++i)
                selection_probab[i + 1] = selection_probab[i] + popfit[i];
            int max1 =999999999 ,max2 = 999999999;
            int pop_aux[pop_size];
            int popindex[pop_size];
            for (int i = 0; i < pop_size; i++) {
                pop_aux[i] = popfit[i];
                popindex[i] = i;
            }
            for (int i = 0; i < pop_size-1 ; i++) {
                for (int j = 0; j < pop_size; j++) {
                    if (pop_aux[i] < pop_aux[j]) {
                        int aux, aux2;
                        aux = pop_aux[i];
                        pop_aux[i] = pop_aux[j];
                        pop_aux[i] = aux;
                        aux2 = popindex[i];
                        popindex[i] = popindex[j];
                        popindex[i] = aux2;
                    }
                }
            }
            int ki = 0;
                while (ki < 5) {
                for (int i = 0; i < pop_size; i++) {
                    newpop[ki][i] = pop[i][popindex[ki]];
                }
                ki++;
            }
            for (int i = 0; i < pop_size; ++i)
            {
                float r = 1.0 * rand() / RAND_MAX;
                for (int j = 0; j < pop_size; j++)
                {
                    if (selection_probab[j] < r && r <= selection_probab[j + 1])
                    {
                        for (int k = 0; k < sizes; k++)
                        {
                            newpop[i][k] = pop[j][k];
                        }
                    }

                }
            }
            for (int i = 0; i < pop_size; i++)
            {
                for (int j = 0; j < sizes; j++)
                {
                    pop[i][j] = newpop[i][j];
                }
            }

            int x = 0;
            int i1, i2;
            for (int i = 0; i < pop_size; i++)
            {
                if ((1.0 * rand() / RAND_MAX) < cross_probab)
                {
                    if (x == 0)
                    {
                        x = 1 - x;
                        i1 = i;
                    }
                    if (x == 1)
                    {
                        x = 1 - x;
                        i2 = i;
                        cross_over(i1, i2);
                    }
                }
            }
            for (int i = 0; i < pop_size; i++)
            {
                for (int j = 0; j < sizes; j++)
                {
                    if ((1.0 * rand() / RAND_MAX) < mutation_probab)
                    {
                        pop[i][j] = rand() % (length - j) + 1;
                    }
                }
            }
            if (eval_pop() < best) {
                best = eval_pop();
                cout << t << "    " << best << endl;
            }

        }
        rez[runs] = best
        fout << best << endl;
    }
    fout << best;
}

int main()
{
   int t= srand(time(0));
    int ok = 0;
        ok = 1;
        for (int i = 0; i < length; ++i)
        {
            for (int j = 0; j < length; ++j)
            {
                fin >> graf[i][j];
                cout << graf[i][j] << "  ";
            }
        }
       AG();
       int med = 0;
       for (int i = 0; i < 30; i++) {
           med = rez[i] + med;
       }
       fload d = 0;
       d = med / 30;
       fout << endl;
       fout << d;
       int d = srand(time(0));
       fout << "timp" << d - t;
    return 0;
}