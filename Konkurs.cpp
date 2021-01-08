#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <random>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <math.h>

using namespace std;

mt19937 generator(time(0));
uniform_real_distribution<double> distribution(0, 1);


float* ini_parameters()
{
    float  ini_parameters[3];
    fstream ini;
    string line;
    ini.open("ini.txt", ios::in);
    if (ini.good() == 0)
    {
        cout << "Blad pliku." << endl;
        return 0;
    }

    int i = 0;
    do
    {
        ini >> setprecision(3) >> ini_parameters[i];
        i++;
    } while (getline(ini, line) && i <= 2);

    ini.close();

    return ini_parameters;
}

int new_table(bool* table, int size, float frac)
{
    int n_pos = 0;
    for (int i = 0; i < size; i++)
    {
        float rand1 = distribution(generator);
        if (rand1 < frac)
        {
            table[i] = 1;
            n_pos++;
        }
        else
            table[i] = 0;
    }

    return n_pos;
}

void def_neighbours(int** neighbours, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < 2; j++)
            if (i == 0)
            {
                neighbours[i][0] = size - 1;
                neighbours[i][1] = 1;
            }
            else if (i == size - 1)
            {
                neighbours[i][0] = size - 2;
                neighbours[i][1] = 0;
            }
            else
            {
                neighbours[i][0] = i - 1;
                neighbours[i][1] = i + 1;
            }
    }
}


int main()
{
    float* parameters = ini_parameters();
    int N = parameters[0], L = parameters[2];
    float dx = parameters[1];

    if (parameters == 0) return 0;

    bool* table = new bool[N];

    int** neighbours = new int* [N];
    for (int i = 0; i < N; i++)
        neighbours[i] = new int[2];
    
    def_neighbours(neighbours, N);

    fstream save;
    stringstream stream1, stream2, stream3;
    stream1 << fixed << setprecision(1) << N;
    string N_s = stream1.str();
    stream2 << fixed << setprecision(2) << dx;
    string dx_s = stream2.str();
    stream3 << fixed << setprecision(1) << L;
    string L_s = stream3.str();

    save.open("N" + N_s + "dx" + dx_s + "L" + L_s + ".txt", ios::out);

    for (float x = 0; x <= (1.0 + 0.5*dx); x = x + dx)
    {
        cout << endl << x << endl;
        double t_avg = 0;
        double prob = 0;
        double positive_end = 0;

        for (int j = 0; j < L; j++)
        {
            cout << j << ' ';
            int N_pos = new_table(table, N, x);

            while (N_pos != N && N_pos != 0)
            {
                for (int i = 0; i < N; i++)
                {
                    int rand = static_cast<int>(distribution(generator) * N);
                    int rand2 = static_cast<int>(distribution(generator) * 2);

                    if (table[neighbours[rand][rand2]] == 0 && table[rand] == 1)
                    {
                        N_pos++;
                    }
                    else if (table[neighbours[rand][rand2]] == 1 && table[rand] == 0)
                    {
                        N_pos--;
                    }

                    table[neighbours[rand][rand2]] = table[rand];

                }
                t_avg++;

            }
            if (N_pos == N)
                positive_end++;

        }

        t_avg = t_avg / L;
        prob = positive_end / L;        
        save << x << '\t' << prob << '\t' << t_avg << endl;
    }

    delete[] table; 

    for (int i = 0; i < N; i++)
        delete[] neighbours[i];

    delete[] neighbours;

    save.close();

    return 0;
}