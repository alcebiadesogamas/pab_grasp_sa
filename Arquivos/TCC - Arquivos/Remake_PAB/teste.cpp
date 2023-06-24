// C++ program to illustrate the
// iterators in vector
#include <iostream>
#include <vector>
#include <memory.h>
#include <algorithm>
#define MAX_NAVIOS 100
#define MAX_BERCOS 20

using namespace std;

void lerDados(std::string arq);
void ordenarPosicaoMenorTempoChegada(int vetTempChegadaOrd[MAX_NAVIOS], int qtd);
static int NUMNAVIOS;
static int NUMBERCOS;

static int MAT_ATENDIMENTO[MAX_BERCOS][MAX_NAVIOS];

static int tempAbertura[MAX_BERCOS];
static int tempFechamento[MAX_BERCOS];

static int tempChegada[MAX_NAVIOS];
static int tempSaida[MAX_NAVIOS];

int matInicioAtendimento[MAX_BERCOS][MAX_NAVIOS];
int matTerminoAtendimento[MAX_BERCOS][MAX_NAVIOS];

int vetTerminoAtendimento[MAX_BERCOS];
int main()
{
    int vetTempChegadaOrd[MAX_NAVIOS];
    for (int i = 0; i < MAX_NAVIOS; i++)
    {
        vetTempChegadaOrd[i] = i;
    }
    lerDados("i01.txt");
    ordenarPosicaoMenorTempoChegada(vetTempChegadaOrd, NUMNAVIOS);

    return 0;
}

void ordenarPosicaoMenorTempoChegada(int vetTempChegadaOrd[MAX_NAVIOS], int qtd)
{
    // BUBBLE SORT O(N^2)
    int flag, aux;
    flag = 1;

    while (flag)
    {
        flag = 0;
        for (int j = 0; j < qtd - 1; j++)
        {

            if (tempChegada[vetTempChegadaOrd[j]] > tempChegada[vetTempChegadaOrd[j + 1]])
            {
                flag = 1;
                aux = vetTempChegadaOrd[j];
                vetTempChegadaOrd[j] = vetTempChegadaOrd[j + 1];
                vetTempChegadaOrd[j + 1] = aux;
            }
        }
    }
}

void lerDados(string arq)
{
    memset(&MAT_ATENDIMENTO, 0, sizeof(MAT_ATENDIMENTO));
    FILE *f = fopen(arq.c_str(), "r");

    fscanf(f, "%d %d\n", &NUMNAVIOS, &NUMBERCOS);

    for (int i = 0; i < NUMBERCOS; i++)
    {
        for (int j = 0; j < NUMNAVIOS; j++)
        {
            fscanf(f, "%d", &MAT_ATENDIMENTO[i][j]);
        }
    }

    for (int i = 0; i < NUMBERCOS; i++)
    {
        fscanf(f, "%d", &tempAbertura[i]);
        fscanf(f, "%d", &tempFechamento[i]);
    }

    for (int i = 0; i < NUMNAVIOS; i++)
    {
        fscanf(f, "%d", &tempChegada[i]);
    }

    for (int i = 0; i < NUMNAVIOS; i++)
    {
        fscanf(f, "%d", &tempSaida[i]);
    }
    fclose(f);
}