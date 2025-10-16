#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define TAMANHO_FILA 5

// Estrutura para representar uma peça do Tetris
typedef struct
{
  char nome; // Tipo da peça ('I', 'O', 'T', 'L')
  int id;    // Identificador único da peça
} Peca;

// Estrutura para representar a fila de peças
typedef struct
{
  Peca pecas[TAMANHO_FILA];
  int frente;  // Índice do primeiro elemento
  int tras;    // Índice após o último elemento
  int tamanho; // Número atual de elementos na fila
} FilaPecas;

// Variável global para controlar o ID das peças
int proximoId = 0;

// Função para gerar uma peça aleatória
Peca gerarPeca()
{
  char tipos[] = {'I', 'O', 'T', 'L'};
  Peca novaPeca;

  // Gera um tipo aleatório
  novaPeca.nome = tipos[rand() % 4];
  // Atribui o próximo ID disponível
  novaPeca.id = proximoId++;

  return novaPeca;
}

// Função para inicializar a fila
void inicializarFila(FilaPecas *fila)
{
  fila->frente = 0;
  fila->tras = 0;
  fila->tamanho = 0;

  // Preenche a fila com 5 peças iniciais
  for (int i = 0; i < TAMANHO_FILA; i++)
  {
    fila->pecas[fila->tras] = gerarPeca();
    fila->tras = (fila->tras + 1) % TAMANHO_FILA;
    fila->tamanho++;
  }
}

// Função para verificar se a fila está vazia
int filaVazia(FilaPecas *fila)
{
  return fila->tamanho == 0;
}

// Função para verificar se a fila está cheia
int filaCheia(FilaPecas *fila)
{
  return fila->tamanho == TAMANHO_FILA;
}

// Função para remover uma peça da frente da fila (dequeue)
Peca jogarPeca(FilaPecas *fila)
{
  if (filaVazia(fila))
  {
    printf("Erro: A fila está vazia!\n");
    Peca pecaVazia = {' ', -1};
    return pecaVazia;
  }

  Peca pecaJogada = fila->pecas[fila->frente];
  fila->frente = (fila->frente + 1) % TAMANHO_FILA;
  fila->tamanho--;

  return pecaJogada;
}

// Função para inserir uma nova peça no final da fila (enqueue)
int inserirPeca(FilaPecas *fila)
{
  if (filaCheia(fila))
  {
    printf("Erro: A fila está cheia!\n");
    return 0;
  }

  fila->pecas[fila->tras] = gerarPeca();
  fila->tras = (fila->tras + 1) % TAMANHO_FILA;
  fila->tamanho++;

  return 1;
}

// Função para exibir o estado atual da fila
void exibirFila(FilaPecas *fila)
{
  printf("\nFila de peças\n");

  if (filaVazia(fila))
  {
    printf("A fila está vazia!\n");
    return;
  }

  int indice = fila->frente;
  for (int i = 0; i < fila->tamanho; i++)
  {
    printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
    indice = (indice + 1) % TAMANHO_FILA;
  }
  printf("\n");
}

// Função para exibir o menu de opções
void exibirMenu()
{
  printf("\nOpções de ação:\n");
  printf("1 - Jogar peça (dequeue)\n");
  printf("2 - Inserir nova peça (enqueue)\n");
  printf("0 - Sair\n");
  printf("Escolha uma opção: ");
}

int main()
{
  // Configuração simplificada que funciona melhor no Windows
  system("chcp 65001 > nul");
  setlocale(LC_ALL, "C.UTF-8");

  // Inicializa o gerador de números aleatórios
  srand(time(NULL));

  FilaPecas fila;
  int opcao;

  // Inicializa a fila com peças
  inicializarFila(&fila);

  printf("=== TETRIS STACK - FILA DE PEÇAS ===\n");

  do
  {
    // Exibe o estado atual da fila
    exibirFila(&fila);

    // Exibe o menu e lê a opção do usuário
    exibirMenu();
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 1:
    {
      // Jogar uma peça (remover da frente)
      Peca pecaJogada = jogarPeca(&fila);
      if (pecaJogada.id != -1)
      {
        printf("Peça jogada: [%c %d]\n", pecaJogada.nome, pecaJogada.id);
      }
      break;
    }
    case 2:
    {
      // Inserir nova peça (adicionar no final)
      if (inserirPeca(&fila))
      {
        printf("Nova peça inserida na fila!\n");
      }
      break;
    }
    case 0:
      printf("Saindo do programa...\n");
      break;
    default:
      printf("Opção inválida! Tente novamente.\n");
      break;
    }

    // Pausa para melhor visualização
    if (opcao != 0)
    {
      printf("\nPressione Enter para continuar...");
      getchar(); // Consome o \n deixado pelo scanf
      getchar(); // Aguarda o Enter do usuário
    }

  } while (opcao != 0);

  return 0;
}