#include "definicoes.hpp"
#include <algorithm>
#include <bits/types/clock_t.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX(X, Y) ((X > Y) ? X : Y)

// #define DBG

#define PESO_TLNAVIO                                                           \
  10000 // PENALIZACAO POR ULTRAPASSAR TEMPO LIMITE DO NAVIO NO PORTO.
#define PESO_TFBERCO                                                           \
  10000 // PENALIZACAO POE ULTRAPASSAR TEMPO DE FECHAMENTO DO BERCO.
#define PESO_BERCO_PROIBIDO                                                    \
  10000 // PENALIZACAO POE ULTRAPASSAR TEMPO DE FECHAMENTO DO BERCO.
#define MAX_INT 1234567910
#define ALFA 0.5

using namespace std;
const double TEMPO_LIMITE = 5;
double ALPHA, BETA, TEMP_INICIAL;
const double TEMP_FINAL = 0.001;
int SA_MAX;

int main(int argc, const char *argv[]) {
  srand(time(NULL));
  lerDados(argv[2]);
  // lerDados("i01.txt");
  ALPHA = atof(argv[1]);
  BETA = atof(argv[3]);
  TEMP_INICIAL = atof(argv[4]);
  SA_MAX = ((NUMBERCOS * NUMNAVIOS) / 2);
  // ALPHA = 0.2;
  // BETA = 0.975;
  // TEMP_INICIAL = 3900;

  // double alpha = atof(argv[1]);
  // lerDados("dezxseis.txt");
  Solucao sol;
  // heuConGul(sol);
  // heuristicaMazin(sol);
  // heuristicaMazin2(sol);
  // mazin2(sol, ALFA);
  // clock_t h;
  // calcFO(sol);
  // calcFOSemPenalizacao(sol);
  // double tempo_limite, tempo_melhor, tempo_total;

  // tempo_limite = 5;
  // beta, Smax/numero de iteracoes para atingir o equilibrio, temperatura
  // inicial temperatura de resfriamento, tempo limite de execução, solucao
  // inicial, tempo da melhor fo, tempo total.
  // simulated_annealing(0.975, ((NUMBERCOS * NUMNAVIOS) / 2), 3900, 0.01,
  //                      tempo_limite, sol, tempo_melhor, tempo_total);
  // simulated_annealing(((NUMBERCOS * NUMNAVIOS) / 2), 3900, 0.01, sol);

  // printf("SA - FO: %d\t Tempo melhor: %.5f\tTempo total: %.5f\n", sol.fo,
  // tempo_melhor, tempo_total);
  sol = grasp();
  printf("heuristica - FO: %d \n\n", sol.fo);

  // ordemAtendimento(sol);
  // escreverSol(sol, "Solucao.txt");

  return 0;
}

void lerDados(const char *arq) {
  memset(&MAT_ATENDIMENTO, 0, sizeof(MAT_ATENDIMENTO));
  FILE *f = fopen(arq, "r");

  fscanf(f, "%d %d\n", &NUMNAVIOS, &NUMBERCOS);

  for (int i = 0; i < NUMBERCOS; i++) {
    for (int j = 0; j < NUMNAVIOS; j++) {
      fscanf(f, "%d", &MAT_ATENDIMENTO[i][j]);
    }
  }

  for (int i = 0; i < NUMBERCOS; i++) {
    fscanf(f, "%d", &tempAbertura[i]);
    fscanf(f, "%d", &tempFechamento[i]);
  }

  for (int i = 0; i < NUMNAVIOS; i++) {
    fscanf(f, "%d", &tempChegada[i]);
  }

  for (int i = 0; i < NUMNAVIOS; i++) {
    fscanf(f, "%d", &tempSaida[i]);
  }
  fclose(f);
}

void testarDados(char *arq) {
  FILE *f;
  if (arq == "")
    f = stdout;
  else
    f = fopen(arq, "w");

  fprintf(f, "%d %d\n", NUMNAVIOS, NUMBERCOS);

  for (int i = 0; i < NUMBERCOS; i++) {
    for (int j = 0; j < NUMNAVIOS; j++) {
      fprintf(f, "%d ", MAT_ATENDIMENTO[i][j]);
    }
    fprintf(f, "\n");
  }

  for (int i = 0; i < NUMBERCOS; i++) {
    fprintf(f, "%d ", tempAbertura[i]);
    fprintf(f, "%d ", tempFechamento[i]);
    fprintf(f, "\n");
  }

  for (int i = 0; i < NUMNAVIOS; i++) {
    fprintf(f, "%d ", tempChegada[i]);
  }
  fprintf(f, "\n");

  for (int i = 0; i < NUMNAVIOS; i++) {
    fprintf(f, "%d ", tempSaida[i]);
  }
  fclose(f);
}

void clonaSolucao(Solucao &s, Solucao &s_clone) {
  memcpy(&s_clone, &s, sizeof(s));
}

void heuConGul(Solucao &s) {
  memset(&s.qtd_navio_no_berco, 0, sizeof(s.qtd_navio_no_berco));
  memset(&s.MAT, -1, sizeof(s.MAT));

  int j;

  for (int i = 0; i < NUMNAVIOS; i++) {
    do {
      j = rand() % NUMBERCOS;
    } while (MAT_ATENDIMENTO[j][i] == 0);

    s.MAT[j][s.qtd_navio_no_berco[j]] = i;
    s.qtd_navio_no_berco[j]++;
  }

  for (int i = 0; i < NUMBERCOS; i++) {
    ordenarPosicaoMenorTempoChegada(s.MAT[i], s.qtd_navio_no_berco[i]);
  }

  imprimirSolucaoContrutiva(s);
}

void heuristicaMazin(Solucao &s) {
  int tempoEsperaBercoAtual[MAX_BERCOS];
  memset(&s.qtd_navio_no_berco, 0, sizeof(s.qtd_navio_no_berco));
  memset(&s.MAT, -1, sizeof(s.MAT));
  memset(tempoEsperaBercoAtual, 0, sizeof(tempoEsperaBercoAtual));

  int temposEsperaNavioBercos[MAX_BERCOS];
  int indiceTemposEsperaNavioBercos[MAX_BERCOS];
  memset(indiceTemposEsperaNavioBercos, -1,
         sizeof(indiceTemposEsperaNavioBercos));
  memset(temposEsperaNavioBercos, -1, sizeof(temposEsperaNavioBercos));

  int ordemChegadaNavioCrescente[MAX_NAVIOS];
  for (int i = 0; i < NUMNAVIOS; i++) {
    ordemChegadaNavioCrescente[i] = i;
  }
  ordenarPosicaoMenorTempoChegada(ordemChegadaNavioCrescente, NUMNAVIOS);

  int menorTempoEspera = 0;
  int indiceBercoMenorTempo = 0;

  for (int i = 0; i < NUMNAVIOS; i++) {

    for (int j = 0; j < NUMBERCOS; j++) {
      temposEsperaNavioBercos[j] = MAX_INT;
      indiceTemposEsperaNavioBercos[j] = j;
    }

    // primeiro berço que atende o navio
    for (int j = 0; j < NUMBERCOS; j++) {
      if (MAT_ATENDIMENTO[j][ordemChegadaNavioCrescente[i]] != 0) {
        menorTempoEspera = tempoEsperaBercoAtual[j];
        indiceBercoMenorTempo = j;
        temposEsperaNavioBercos[j] = menorTempoEspera;
        indiceTemposEsperaNavioBercos[j] = j;
        break;
      }
    }

    for (int k = indiceBercoMenorTempo + 1; k < NUMBERCOS; k++) {
      if (tempoEsperaBercoAtual[k] <
              tempoEsperaBercoAtual[indiceBercoMenorTempo] &&
          MAT_ATENDIMENTO[k][ordemChegadaNavioCrescente[i]] != 0) {
        menorTempoEspera = tempoEsperaBercoAtual[k];
        indiceBercoMenorTempo = k;
        temposEsperaNavioBercos[k] = menorTempoEspera;
        indiceTemposEsperaNavioBercos[k] = k;
      } else if (MAT_ATENDIMENTO[k][ordemChegadaNavioCrescente[i]] != 0) {
        temposEsperaNavioBercos[k] = tempoEsperaBercoAtual[k];
        indiceTemposEsperaNavioBercos[k] = k;
      }
    }

    if (menorTempoEspera < tempChegada[ordemChegadaNavioCrescente[i]]) {
      tempoEsperaBercoAtual[indiceBercoMenorTempo] =
          menorTempoEspera + tempChegada[ordemChegadaNavioCrescente[i]] +
          MAT_ATENDIMENTO[indiceBercoMenorTempo][ordemChegadaNavioCrescente[i]];
    } else {
      tempoEsperaBercoAtual[indiceBercoMenorTempo] =
          menorTempoEspera +
          MAT_ATENDIMENTO[indiceBercoMenorTempo][ordemChegadaNavioCrescente[i]];
    }
    ordenarPosicaoMenorTempoEspera(temposEsperaNavioBercos,
                                   indiceTemposEsperaNavioBercos, NUMBERCOS);
    int qtdAmostra = ALFA * NUMBERCOS;
    int indiceBercoEscolhido = rand() % qtdAmostra;
    s.MAT[indiceBercoEscolhido][s.qtd_navio_no_berco[indiceBercoEscolhido]] =
        ordemChegadaNavioCrescente[i];
    s.qtd_navio_no_berco[indiceBercoEscolhido]++;
  }
  imprimirSolucaoContrutiva(s);
}

void heuristicaMazin2(Solucao &s) {
  int ordemChegadaNavioCrescente[MAX_NAVIOS];  // guarda o indice dos navios em
                                               // ordem de chegada (crescente)
  int ordemInicioAtendimentoBerco[MAX_BERCOS]; // guarde o indices dos tempos de
                                               // inicios de atendimento
  int tempoInicioAtendimento[MAX_BERCOS];      // guarda os tempos de inicios de
                                               // atendimento no berco

  int inicioAtendimentoNavio[MAX_NAVIOS];   // guarda o tempo de inicio de
                                            // atendimento (no berco escolhido)
  int ultimoNavioAtendidoBerco[MAX_BERCOS]; // guarda o ultimo navio atendido no
                                            // berco

  // incializa os vetores
  memset(inicioAtendimentoNavio, -1, sizeof(inicioAtendimentoNavio));
  memset(ultimoNavioAtendidoBerco, -1, sizeof(ultimoNavioAtendidoBerco));
  memset(&s.qtd_navio_no_berco, 0, sizeof(s.qtd_navio_no_berco));
  memset(&s.MAT, 0, sizeof(s.MAT));
  memset(ordemChegadaNavioCrescente, -1, sizeof(ordemChegadaNavioCrescente));
  memset(tempoInicioAtendimento, -1, sizeof(tempoInicioAtendimento));
  memset(ordemInicioAtendimentoBerco, -1, sizeof(ordemInicioAtendimentoBerco));

  // preenche indice e ordena vetor de ordem de chegada
  preencherIndicesOrdemCrescente(ordemChegadaNavioCrescente, NUMNAVIOS);
  ordenarPosicaoMenorTempoChegada(ordemChegadaNavioCrescente, NUMNAVIOS);

  for (int i = 0; i < NUMNAVIOS; i++) {
    // preenche indices para serem ordenados pelo valor inicio atendimento de
    // cada navio no berco
    preencherIndicesOrdemCrescente(ordemInicioAtendimentoBerco, NUMBERCOS);

    for (int j = 0; j < NUMBERCOS; j++) {
      if (MAT_ATENDIMENTO[j][ordemChegadaNavioCrescente[i]] != 0) {
        if (ultimoNavioAtendidoBerco[j] != -1) {
          int aux = inicioAtendimentoNavio[ultimoNavioAtendidoBerco[j]] +
                    MAT_ATENDIMENTO[j][ultimoNavioAtendidoBerco[j]];
          if (aux > tempChegada[ordemChegadaNavioCrescente[i]]) {
            tempoInicioAtendimento[j] = aux;
          } else {
            tempoInicioAtendimento[j] =
                tempChegada[ordemChegadaNavioCrescente[i]];
          }
        } else {
          tempoInicioAtendimento[j] =
              tempChegada[ordemChegadaNavioCrescente[i]];
        }
      } else {
        tempoInicioAtendimento[j] = 10000;
      }
    }
    ordenarPosicaoMenorTempoEspera(tempoInicioAtendimento,
                                   ordemInicioAtendimentoBerco, NUMBERCOS);

    int qtdAmostra = ALFA * NUMBERCOS;
    int indiceDoIndiceBercoEscolhido = rand() % qtdAmostra;

    ultimoNavioAtendidoBerco
        [ordemInicioAtendimentoBerco[indiceDoIndiceBercoEscolhido]] =
            ordemChegadaNavioCrescente[i];
    inicioAtendimentoNavio[ordemChegadaNavioCrescente[i]] =
        tempoInicioAtendimento
            [ordemInicioAtendimentoBerco[indiceDoIndiceBercoEscolhido]];

    s.MAT[ordemInicioAtendimentoBerco[indiceDoIndiceBercoEscolhido]]
         [s.qtd_navio_no_berco
              [ordemInicioAtendimentoBerco[indiceDoIndiceBercoEscolhido]]] =
        ordemChegadaNavioCrescente[i];
    s.qtd_navio_no_berco
        [ordemInicioAtendimentoBerco[indiceDoIndiceBercoEscolhido]]++;
  }

  imprimirSolucaoContrutiva(s);
}

Solucao grasp() {
  int fMin = MAX_INT;
  Solucao sBest;
  clock_t hF, hI = clock();
  double tempoExecucao = ((double)hI) / CLOCKS_PER_SEC;

  while (tempoExecucao < TEMPO_LIMITE) {
    Solucao s1;
    mazin2(s1, ALPHA);
    calcFO(s1);
    simulated_annealing(SA_MAX, TEMP_INICIAL, TEMP_FINAL, s1);
    if (s1.fo < fMin) {
      clonaSolucao(s1, sBest);
      fMin = s1.fo;
    }
    hF = clock();
    tempoExecucao = ((double)(hF - hI)) / CLOCKS_PER_SEC;
  }
  cout << tempoExecucao << endl;
  return sBest;
}

void mazin2(Solucao &s, double alpha) {
  // inicializa solucao
  memset(&s.qtd_navio_no_berco, 0, sizeof(s.qtd_navio_no_berco));
  memset(&s.MAT, -1, sizeof(s.MAT));

  int navios[MAX_NAVIOS];
  int tempoChegadaAux[MAX_NAVIOS];
  int bercoTemp[MAX_BERCOS];
  int inicioAtendimentoNavioBerco[MAX_BERCOS];
  int ultimoNavioBerco[MAX_BERCOS];
  int inicioAtendimentoNavio[MAX_NAVIOS];

  memset(ultimoNavioBerco, -1, sizeof(ultimoNavioBerco));
  memset(inicioAtendimentoNavio, -1, sizeof(inicioAtendimentoNavio));
  memset(navios, -1, sizeof(navios));
  memset(bercoTemp, -1, sizeof(bercoTemp));

  memset(tempoChegadaAux, 0, sizeof(tempoChegadaAux));

  for (int i = 0; i < NUMNAVIOS; i++) {
    tempoChegadaAux[i] = tempChegada[i];
  }

  preencherVetorComIndices(navios, NUMNAVIOS);

  // ordena navios pelo tempo de chegada.
  ordenarVetorPorOutro(navios, tempoChegadaAux, NUMNAVIOS);

  for (int navio = 0; navio < NUMNAVIOS; navio++) {
    preencherVetorComIndices(bercoTemp, NUMBERCOS);
    memset(inicioAtendimentoNavioBerco, -1,
           sizeof(inicioAtendimentoNavioBerco));

    for (int berco = 0; berco < NUMBERCOS; berco++) {
      if (!bercoAtendeNavio(berco, navios[navio])) {
        inicioAtendimentoNavioBerco[berco] = 10000;
      } else {
        if (ultimoNavioBerco[berco] ==
            -1) { // posso ter um cenário onde o navio chegou porém o berço Ja
                  // fechou, colocar 10000?
          inicioAtendimentoNavioBerco[berco] = tempoChegadaAux[navios[navio]];
        } else {
          int tempoServicoNavioAnteriorBerco =
              inicioAtendimentoNavio[ultimoNavioBerco[berco]] +
              MAT_ATENDIMENTO[berco][ultimoNavioBerco[berco]];

          if (tempoChegadaAux[navios[navio]] <=
              tempoServicoNavioAnteriorBerco) { // aqui teria esse igual?
            inicioAtendimentoNavioBerco[berco] = tempoServicoNavioAnteriorBerco;
          } else { // novamente o berco pode estar fechado, considerar 10000?
            inicioAtendimentoNavioBerco[berco] = tempoChegadaAux[navios[navio]];
          }
        }
      }
    }

    // ordenar bercos por tempo inicio atendimento ao navio
    ordenarVetorPorOutro(bercoTemp, inicioAtendimentoNavioBerco, NUMBERCOS);

    int qtdAmostra = alpha * NUMBERCOS;
    int indiceBercoEscolhido = (alpha == 0) ? 0 : rand() % qtdAmostra;

    ultimoNavioBerco[bercoTemp[indiceBercoEscolhido]] = navios[navio];
    inicioAtendimentoNavio[navios[navio]] =
        inicioAtendimentoNavioBerco[bercoTemp[indiceBercoEscolhido]];

    s.MAT[bercoTemp[indiceBercoEscolhido]]
         [s.qtd_navio_no_berco[bercoTemp[indiceBercoEscolhido]]] =
        navios[navio];
    s.qtd_navio_no_berco[bercoTemp[indiceBercoEscolhido]]++;
  }

  // imprimirSolucaoContrutiva(s);
}

void preencherVetorComIndices(int vetor[], int size) {
  for (int i = 0; i < size; i++) {
    vetor[i] = i;
  }
}

void imprimirSolucaoContrutiva(Solucao &s) {
  for (int i = 0; i < NUMBERCOS; i++) {
    cout << "berco " << i + 1 << ": ";
    for (int j = 0; j < NUMNAVIOS; j++) {
      if (s.MAT[i][j] == -1) {
        continue;
      }
      cout << s.MAT[i][j] << ", ";
    }
    cout << endl;
  }
  cout << endl;
}

void preencherIndicesOrdemCrescente(int vetor[MAX_NAVIOS], int qtd) {
  for (int i = 0; i < qtd; i++) {
    vetor[i] = i;
  }
}

// void simulated_annealing(const double alfa, const int sa_max,
//                          const double temp_ini, const double temp_con,
//                          const double tempo_max, Solucao &s,
//                          double &tempo_melhor, double &tempo_total) {
void simulated_annealing(const int sa_max, const double temp_ini,
                         const double temp_final, Solucao &s) {

  int op = 0;
  //   clock_t hI, hF;
  Solucao s_atual, s_vizinha;
  double temp, delta, x;
  //   printf("\n\n>>> EXECUTANDO O SA...\n\n");
  //   hI = clock();
  //   mazin2(s, ALFA);
  // calcFO(s);
  //   hF = clock();
  //   tempo_melhor = ((double)(hF - hI)) / CLOCKS_PER_SEC;
  // #ifdef DBG
  //   printf("FO: %d\tTempo: %.2f\n", s.fo, tempo_melhor);
  // #endif
  //   tempo_total = tempo_melhor;
  clonaSolucao(s, s_atual);

  // while (tempo_total < tempo_max) {
  temp = temp_ini;
  while (temp > temp_final) {
    for (int i = 0; i < sa_max; i++) {
      clonaSolucao(s_atual, s_vizinha);
      op = rand() % 3;
      if (op == 0)
        gerar_vizinha_1(s_vizinha);
      if (op == 1)
        gerar_vizinha_2(s_vizinha);
      if (op == 2)
        gerar_vizinha_3(s_vizinha);
      delta = s_vizinha.fo - s_atual.fo;
      if (delta < 0) {
        clonaSolucao(s_vizinha, s_atual);
        if (s_vizinha.fo < s.fo) {
          clonaSolucao(s_vizinha, s);
          // hF = clock();
          // tempo_melhor = ((double)(hF - hI)) / CLOCKS_PER_SEC;
          // #ifdef DBG
          // printf("FO: %d\tTempo: %.2f\n", s.fo, tempo_melhor);
          // #endif
        }
      } else {
        x = rand() % 1001;
        x = x / 1000.0;
        if (x < exp(-delta / temp))
          clonaSolucao(s_vizinha, s_atual);
      }
    }
    temp = temp * BETA;
    // hF = clock();
    // tempo_total = ((double)(hF - hI)) / CLOCKS_PER_SEC;
    // if (tempo_total >= tempo_max)
    // break;
  }
  // }
}

void gerar_vizinha_1(Solucao &s) // troca o navio de berco
{
  int pnavio, bnavio, berconovo, navio;
  // escoloha do navio aleatório.

  do
    bnavio = rand() % NUMBERCOS; // seleciono uma linha/berco aleatório.
  while (s.qtd_navio_no_berco[bnavio] == 0);

  pnavio = rand() % s.qtd_navio_no_berco[bnavio];

  navio = s.MAT[bnavio][pnavio];

  do
    berconovo = rand() % NUMBERCOS; // seleciono novo berco aleatório.
  while (MAT_ATENDIMENTO[berconovo][navio] == 0 || bnavio == berconovo);

  // retirando o navio do berco antigo.
  for (int j = pnavio; j < s.qtd_navio_no_berco[bnavio]; j++) {
    s.MAT[bnavio][j] = s.MAT[bnavio][j + 1];
  }
  s.qtd_navio_no_berco[bnavio]--; // diminui a quantidade de navios no berco
                                  // antigo.

  s.MAT[berconovo][s.qtd_navio_no_berco[berconovo]] =
      navio;                         // colocando navio no berco novo.
  s.qtd_navio_no_berco[berconovo]++; // incrementa a quantidade de navios no
                                     // berco novo.

  ordenarPosicaoMenorTempoChegada(s.MAT[berconovo],
                                  s.qtd_navio_no_berco[berconovo]);

  calcFO(s);
}

void gerar_vizinha_2(
    Solucao &s) // troca dois navios de bercos, um pro berco do outro
                // respectivamente desde que não esteja no mesmo berco.
{
  int pnavio1, pnavio2, berco1, berco2, navio1, navio2;
// seleciono dois navios aleatorios.
DENOVO:
  do {
    berco1 = rand() % NUMBERCOS;
    berco2 = rand() % NUMBERCOS;
  } while (berco1 == berco2 || s.qtd_navio_no_berco[berco1] == 0 ||
           s.qtd_navio_no_berco[berco2] == 0);

  pnavio1 = rand() % s.qtd_navio_no_berco[berco1];
  pnavio2 = rand() % s.qtd_navio_no_berco[berco2];

  navio1 = s.MAT[berco1][pnavio1];
  navio2 = s.MAT[berco2][pnavio2];

  if (MAT_ATENDIMENTO[berco1][navio2] != 0 &&
      MAT_ATENDIMENTO[berco2][navio1] != 0) {
    s.MAT[berco1][pnavio1] = navio2;
    s.MAT[berco2][pnavio2] = navio1;
    ordenarPosicaoMenorTempoChegada(s.MAT[berco1],
                                    s.qtd_navio_no_berco[berco1]);
    ordenarPosicaoMenorTempoChegada(s.MAT[berco2],
                                    s.qtd_navio_no_berco[berco2]);
  } else {
    goto DENOVO;
  }

  calcFO(s);
}

void gerar_vizinha_3(
    Solucao &s) // troco a ordem de atendimento, pode ser que quem chegou
                // primeiro seja o ultimo a ser atendido
{
  int pnavio1, pnavio2, berco, aux;

  berco = rand() % NUMBERCOS; // seleciono o berco aleatorio

  if (s.qtd_navio_no_berco[berco] > 1) {
    pnavio1 = rand() % s.qtd_navio_no_berco[berco]; // seleciono uma posicao de
                                                    // navio aleatório.

    do
      pnavio2 = rand() % s.qtd_navio_no_berco[berco]; // seleciono outra posicao
                                                      // de navio aleatório.
    while (pnavio1 == pnavio2);

    aux = s.MAT[berco][pnavio1];
    s.MAT[berco][pnavio1] = s.MAT[berco][pnavio2];
    s.MAT[berco][pnavio2] = aux;

    calcFO(s);
  }
}

void ordenarPosicaoMenorTempoChegada(int vetTempChegadaOrd[MAX_NAVIOS],
                                     int qtd) {
  int flag, aux;
  flag = 1;

  while (flag) {
    flag = 0;
    for (int j = 0; j < qtd - 1; j++) {

      if (tempChegada[vetTempChegadaOrd[j]] >
          tempChegada[vetTempChegadaOrd[j + 1]]) {
        flag = 1;
        aux = vetTempChegadaOrd[j];
        vetTempChegadaOrd[j] = vetTempChegadaOrd[j + 1];
        vetTempChegadaOrd[j + 1] = aux;
      }
    }
  }
}

void ordenarPosicaoMenorTempoEspera(int temposEsperaNavioBercos[MAX_BERCOS],
                                    int indicesBercos[MAX_BERCOS], int qtd) {
  int flag, aux;
  flag = 1;

  while (flag) {
    flag = 0;
    for (int j = 0; j < qtd - 1; j++) {

      if (temposEsperaNavioBercos[indicesBercos[j]] >
          temposEsperaNavioBercos[indicesBercos[j + 1]]) {
        flag = 1;
        aux = indicesBercos[j];
        indicesBercos[j] = indicesBercos[j + 1];
        indicesBercos[j + 1] = aux;
      }
    }
  }
}

void calcFO(Solucao &s) {
  s.fo = 0; // inicia com valor 0.
  ordemAtendimento(s);
  // calcula valor da FO.
  for (int i = 0; i < NUMBERCOS; i++) {
    for (int j = 0; j < s.qtd_navio_no_berco[i]; j++) {
      if (!bercoAtendeNavio(i, s.MAT[i][j])) {
        s.fo += PESO_BERCO_PROIBIDO;
      }
      s.fo += (matInicioAtendimento[i][j] - tempChegada[s.MAT[i][j]] +
               MAT_ATENDIMENTO[i][s.MAT[i][j]]) +
              (PESO_TFBERCO *
               MAX(0, vetTerminoAtendimento[i] - tempFechamento[i])) +
              (PESO_TLNAVIO *
               MAX(0, matTerminoAtendimento[i][j] - tempSaida[s.MAT[i][j]]));
      // penalizar se estourou o tempo berco, do navio.
    }
  }
}

void calcFOSemPenalizacao(Solucao &s) {
  s.fo = 0; // inicia com valor 0.
  ordemAtendimento(s);
  // calcula valor da FO.
  for (int i = 0; i < NUMBERCOS; i++) {
    for (int j = 0; j < s.qtd_navio_no_berco[i]; j++) {

      s.fo += (matInicioAtendimento[i][j] - tempChegada[s.MAT[i][j]] +
               MAT_ATENDIMENTO[i][s.MAT[i][j]]);
      cout << "FO atual sem penalizacao: " << s.fo << endl;
      cout << "Penalizacao por tempo de fechamento do berco: "
           << (PESO_TFBERCO *
               MAX(0, vetTerminoAtendimento[i] - tempFechamento[i]))
           << ", berco: " << i << endl;
      cout << "Penalizacao por tempo limite de atendimento do navio: "
           << (PESO_TLNAVIO *
               MAX(0, matTerminoAtendimento[i][j] - tempSaida[s.MAT[i][j]]))
           << ", navio: " << s.MAT[i][j] << endl;
      if (!bercoAtendeNavio(i, s.MAT[i][j])) {
        cout << "Houve penalizacao por navio em berco proibido" << endl;
      }
      // penalizar se estourou o tempo berco, do navio.
    }
  }
}

void ordemAtendimento(Solucao &s) {
  /* for (int i = 0; i < NUMBERCOS; i++)
  {
      vetTerminoAtendimento[i] = tempAbertura[i]; //começa com o tempo de
  abertura do berco.
  }*/

  memset(&matInicioAtendimento, 0, sizeof(matInicioAtendimento));
  memset(&matTerminoAtendimento, 0, sizeof(matTerminoAtendimento));
  for (int i = 0; i < NUMBERCOS; i++) {
    vetTerminoAtendimento[i] = tempAbertura[i];
    for (int j = 0; j < s.qtd_navio_no_berco[i]; j++) {
      if (vetTerminoAtendimento[i] >
          tempChegada[s.MAT[i][j]]) // se o navio chegou antes do termino de
                                    // atendimento ou antes do berço abrir
      {
        matInicioAtendimento[i][j] =
            vetTerminoAtendimento[i]; // inicio do atendimento do navio é após o
                                      // termino do anterior
        vetTerminoAtendimento[i] +=
            MAT_ATENDIMENTO[i][s.MAT[i][j]]; // termino do atendimento será
                                             // somado com abertura do berco
        matTerminoAtendimento[i][j] = vetTerminoAtendimento[i];
      } else // se o navio chegou com o berço disponivel.
      {
        matInicioAtendimento[i][j] =
            tempChegada[s.MAT[i][j]]; // inicio do atendimento do navio é após o
                                      // termino do anterior
        vetTerminoAtendimento[i] =
            tempChegada[s.MAT[i][j]] +
            MAT_ATENDIMENTO[i][s.MAT[i][j]]; // termino de atendimento
        matTerminoAtendimento[i][j] = vetTerminoAtendimento[i];
      }
    }
  }
}

int totalViolacoesBercos() {
  int total = 0;
  for (int i = 0; i < NUMBERCOS; i++) {
    total += MAX(0, vetTerminoAtendimento[i] - tempFechamento[i]);
  }
  return total;
}

int totalViolacoesNavios(Solucao &s) {
  int total = 0;

  for (int i = 0; i < NUMBERCOS; i++) {
    for (int j = 0; j < NUMNAVIOS; j++) {
      total += MAX(0, matTerminoAtendimento[i][j] - tempSaida[s.MAT[i][j]]);
    }
  }
  return total;
}

void escreverSol(Solucao &s, std::string arq) {
  FILE *f;
  if (arq == "")
    f = stdout;
  else
    f = fopen(arq.c_str(), "w");

  int countBercos = 0;
  int countNavios = 0;
  cout << "< ----------------------------- SOLUÇÃO "
          "---------------------------- >";
  cout << "\n";

  for (int i = 0; i < NUMBERCOS; i++) {
    if (s.qtd_navio_no_berco[i] != 0)
      countBercos++;
  }

  cout << "Número de berços utilizados.........................: "
       << countBercos;
  cout << "\n";

  for (int i = 0; i < NUMBERCOS; i++) {
    if (s.qtd_navio_no_berco[i] != 0)
      countNavios += s.qtd_navio_no_berco[i];
  }

  cout << "Número de navios atendidos..........................: "
       << countNavios;
  cout << "\n";

  cout << "Tempo total de operação.............................: " << s.fo;
  cout << "\n";

  cout << "Total de viol. nas jan. de tempo dos bercos.........: "
       << totalViolacoesBercos();
  cout << "\n";

  cout << "Total de viol. nas jan. de tempo dos navios.........: "
       << totalViolacoesNavios(s);
  cout << "\n";

  cout << "FO da solução.......................................: " << s.fo;
  cout << "\n";

  // qtd navios no berço j.
  for (int i = 0; i < NUMBERCOS; i++) {
    cout << "Quantidade de navios atendidos no berço " << i + 1 << ": "
         << s.qtd_navio_no_berco[i] << "\n";
  }
  cout << "\n";

  // inicio de atendimento do navio j no berco i.
  for (int i = 0; i < NUMBERCOS; i++) {
    for (int j = 0; j < s.qtd_navio_no_berco[i]; j++) {
      if (matInicioAtendimento[i][j] != 0)
        cout << "Incio de atendimento do navio " << s.MAT[i][j] << " no berço "
             << i + 1 << ": " << matInicioAtendimento[i][j] << "\n";
    }
    cout << "\n";
  }
  cout << "\n";

  // ############Arquivo SOLUCAO##########################

  fprintf(f, "%s",
          "< ----------------------------- SOLUÇÃO "
          "---------------------------- >\n");

  fprintf(f, "%s %d", "Número de berços utilizados.........................: ",
          countBercos);
  fprintf(f, "%s", "\n");

  fprintf(f, "%s %d", "Número de navios atendidos..........................: ",
          countNavios);
  fprintf(f, "%s", "\n");

  fprintf(f, "%s %d",
          "Tempo total de operação.............................: ", s.fo);
  fprintf(f, "%s", "\n");

  fprintf(f, "%s %d", "Total de viol. nas jan. de tempo dos bercos.........: ",
          totalViolacoesBercos());
  fprintf(f, "%s", "\n");

  fprintf(f, "%s %d", "Total de viol. nas jan. de tempo dos navios.........: ",
          totalViolacoesNavios(s));
  fprintf(f, "%s", "\n");

  fprintf(f, "%s %d",
          "FO da solução.......................................: ", s.fo);
  fprintf(f, "%s", "\n\n");

  // qtd navios no berço j.
  for (int i = 0; i < NUMBERCOS; i++) {
    fprintf(f, "%s %d %s %d %s", "Quantidade de navios atendidos no berço ",
            i + 1, ": ", s.qtd_navio_no_berco[i], "\n");
  }
  fprintf(f, "%s", "\n");

  // inicio de atendimento do navio j no berco i.
  for (int i = 0; i < NUMBERCOS; i++) {
    for (int j = 0; j < NUMNAVIOS; j++) {
      if (matInicioAtendimento[i][j] != 0)
        fprintf(f, "%s %d %s %d %s %d %s", "Incio de atendimento do navio ",
                s.MAT[i][j], " no berço ", i + 1, ": ",
                matInicioAtendimento[i][j], "\n");
    }
    fprintf(f, "%s", "\n");
  }
  fprintf(f, "%s", "\n");
  fclose(f);
}

bool bercoAtendeNavio(int berco, int navio) {
  return (MAT_ATENDIMENTO[berco][navio] != 0);
}

void trocarElementos(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

void ordenarVetorPorOutro(int arrayA[MAX_NAVIOS], int arrayB[MAX_NAVIOS],
                          int tamanho) {
  // Criar um vetor de índices para o vetor A
  int indices[MAX_NAVIOS];
  for (int i = 0; i < tamanho; i++) {
    indices[i] = i;
  }

  // Usar a função std::sort com um lambda para ordenar os índices com base nos
  // elementos de B
  std::sort(indices, indices + tamanho,
            [&arrayB](int i, int j) { return arrayB[i] < arrayB[j]; });

  // Criar um vetor temporário para armazenar o resultado ordenado
  int temp[MAX_NAVIOS];
  for (int i = 0; i < tamanho; i++) {
    temp[i] = arrayA[indices[i]];
  }

  // Copiar o vetor temporário de volta para o vetor A
  for (int i = 0; i < tamanho; i++) {
    arrayA[i] = temp[i];
  }
}
