#ifndef DEFINICOES_HPP
#define DEFINICOES_HPP

#include <string.h>

#define MAX_NAVIOS 100
#define MAX_BERCOS 20

typedef struct tSolucao
{
    int fo;
    int MAT[MAX_BERCOS][MAX_NAVIOS];
    int qtd_navio_no_berco[MAX_BERCOS];
} Solucao;

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

void lerDados(std::string arq);

void testarDados(char *arq);

void clonaSolucao(Solucao &s, Solucao &s_clone);

void calcFO(Solucao &s);

void heuConGul(Solucao &s);

void escreverSol(Solucao &s, std::string arq);

void ordenarPosicaoMenorTempoChegada(int vetTempChegadaOrd[MAX_NAVIOS], int qtd);

int totalViolacoesNavios(Solucao &s);

int totalViolacoesBercos();

void simulated_annealing(const double alfa, const int sa_max, const double temp_ini, const double temp_con, const double tempo_max, Solucao &s, double &tempo_melhor, double &tempo_total);

void gerar_vizinha_1(Solucao &s);

void gerar_vizinha_2(Solucao &s);

void gerar_vizinha_3(Solucao &s);

void ordemAtendimento(Solucao &s);

void heuristicaMazin(Solucao &s);

void heuristicaMazin2(Solucao &s);

void calcFOSemPenalizacao(Solucao &s);

void imprimirSolucaoContrutiva(Solucao &s);

void preencherIndicesOrdemCrescente(int vetor[MAX_NAVIOS], int qtd);

void ordenarPosicaoMenorTempoEspera(int temposEsperaNavioBercos[MAX_BERCOS], int indicesBercos[MAX_BERCOS], int qtd);
#endif