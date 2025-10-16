#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

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

// Estrutura para representar a pilha de reserva
typedef struct
{
  Peca pecas[TAMANHO_PILHA];
  int topo; // Índice do topo da pilha (-1 para pilha vazia)
} PilhaReserva;

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

// Função para inicializar a pilha de reserva
void inicializarPilha(PilhaReserva *pilha)
{
  pilha->topo = -1; // Pilha vazia
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

// Função para verificar se a pilha está vazia
int pilhaVazia(PilhaReserva *pilha)
{
  return pilha->topo == -1;
}

// Função para verificar se a pilha está cheia
int pilhaCheia(PilhaReserva *pilha)
{
  return pilha->topo == TAMANHO_PILHA - 1;
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

// Função para empilhar uma peça na pilha de reserva (push)
int empilharPeca(PilhaReserva *pilha, Peca peca)
{
  if (pilhaCheia(pilha))
  {
    printf("Erro: A pilha de reserva está cheia!\n");
    return 0;
  }

  pilha->topo++;
  pilha->pecas[pilha->topo] = peca;
  return 1;
}

// Função para desempilhar uma peça da pilha de reserva (pop)
Peca desempilharPeca(PilhaReserva *pilha)
{
  if (pilhaVazia(pilha))
  {
    printf("Erro: A pilha de reserva está vazia!\n");
    Peca pecaVazia = {' ', -1};
    return pecaVazia;
  }

  Peca pecaUsada = pilha->pecas[pilha->topo];
  pilha->topo--;
  return pecaUsada;
}

// Função para reservar uma peça (move da fila para a pilha)
int reservarPeca(FilaPecas *fila, PilhaReserva *pilha)
{
  if (filaVazia(fila))
  {
    printf("Erro: Não há peças na fila para reservar!\n");
    return 0;
  }

  if (pilhaCheia(pilha))
  {
    printf("Erro: A pilha de reserva está cheia!\n");
    return 0;
  }

  // Remove peça da fila
  Peca pecaReservada = jogarPeca(fila);
  if (pecaReservada.id == -1)
  {
    return 0;
  }

  // Adiciona peça à pilha
  empilharPeca(pilha, pecaReservada);

  // Adiciona nova peça à fila para manter o tamanho
  inserirPeca(fila);

  printf("Ação: peça [%c %d] enviada para a pilha de reserva!\n", pecaReservada.nome, pecaReservada.id);
  return 1;
}

// Função para usar uma peça reservada
int usarPecaReservada(PilhaReserva *pilha)
{
  Peca pecaUsada = desempilharPeca(pilha);
  if (pecaUsada.id != -1)
  {
    printf("Ação: peça reservada [%c %d] foi usada!\n", pecaUsada.nome, pecaUsada.id);
    return 1;
  }
  return 0;
}

// Função para trocar a peça da frente da fila com o topo da pilha
int trocarPecaAtual(FilaPecas *fila, PilhaReserva *pilha)
{
  if (filaVazia(fila))
  {
    printf("Erro: A fila está vazia!\n");
    return 0;
  }

  if (pilhaVazia(pilha))
  {
    printf("Erro: A pilha de reserva está vazia!\n");
    return 0;
  }

  // Salva a peça da frente da fila
  Peca pecaFila = fila->pecas[fila->frente];

  // Salva a peça do topo da pilha
  Peca pecaPilha = pilha->pecas[pilha->topo];

  // Realiza a troca
  fila->pecas[fila->frente] = pecaPilha;
  pilha->pecas[pilha->topo] = pecaFila;

  printf("Ação: troca realizada entre a peça da frente da fila [%c %d] e o topo da pilha [%c %d]!\n",
         pecaFila.nome, pecaFila.id, pecaPilha.nome, pecaPilha.id);
  return 1;
}

// Função para trocar múltiplas peças (3 da fila com 3 da pilha)
int trocaMultipla(FilaPecas *fila, PilhaReserva *pilha)
{
  // Verifica se a fila tem pelo menos 3 peças
  if (fila->tamanho < 3)
  {
    printf("Erro: A fila deve ter pelo menos 3 peças para a troca múltipla!\n");
    return 0;
  }

  // Verifica se a pilha tem exatamente 3 peças
  if (pilha->topo != 2)
  {
    printf("Erro: A pilha deve ter exatamente 3 peças para a troca múltipla!\n");
    return 0;
  }

  // Arrays temporários para armazenar as peças
  Peca pecasFila[3];
  Peca pecasPilha[3];

  // Salva as 3 primeiras peças da fila
  int indiceFila = fila->frente;
  for (int i = 0; i < 3; i++)
  {
    pecasFila[i] = fila->pecas[indiceFila];
    indiceFila = (indiceFila + 1) % TAMANHO_FILA;
  }

  // Salva as 3 peças da pilha (do topo para a base)
  for (int i = 0; i < 3; i++)
  {
    pecasPilha[i] = pilha->pecas[2 - i]; // Inverte a ordem para manter a lógica LIFO
  }

  // Coloca as peças da pilha na fila (nas 3 primeiras posições)
  indiceFila = fila->frente;
  for (int i = 0; i < 3; i++)
  {
    fila->pecas[indiceFila] = pecasPilha[i];
    indiceFila = (indiceFila + 1) % TAMANHO_FILA;
  }

  // Coloca as peças da fila na pilha (invertendo a ordem para manter LIFO)
  for (int i = 0; i < 3; i++)
  {
    pilha->pecas[i] = pecasFila[2 - i];
  }

  printf("Ação: troca realizada entre os 3 primeiros da fila e os 3 da pilha!\n");
  return 1;
}

// Função para exibir o estado atual da fila
void exibirFila(FilaPecas *fila)
{
  printf("Fila de peças\t");

  if (filaVazia(fila))
  {
    printf("(vazia)");
  }
  else
  {
    int indice = fila->frente;
    for (int i = 0; i < fila->tamanho; i++)
    {
      printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
      indice = (indice + 1) % TAMANHO_FILA;
    }
  }
  printf("\n");
}

// Função para exibir o estado atual da pilha
void exibirPilha(PilhaReserva *pilha)
{
  printf("Pilha de reserva\t(Topo -> Base): ");

  if (pilhaVazia(pilha))
  {
    printf("(vazia)");
  }
  else
  {
    for (int i = pilha->topo; i >= 0; i--)
    {
      printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
    }
  }
  printf("\n");
}

// Função para exibir o estado completo do jogo
void exibirEstado(FilaPecas *fila, PilhaReserva *pilha)
{
  printf("\n=== Estado atual ===\n");
  exibirFila(fila);
  exibirPilha(pilha);
  printf("\n");
}

// Função para exibir o menu de opções
void exibirMenu()
{
  printf("Opções disponíveis:\n");
  printf("1 - Jogar peça da frente da fila\n");
  printf("2 - Enviar peça da fila para a pilha de reserva\n");
  printf("3 - Usar peça da pilha de reserva\n");
  printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
  printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
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
  PilhaReserva pilha;
  int opcao;

  // Inicializa as estruturas
  inicializarFila(&fila);
  inicializarPilha(&pilha);

  printf("=== TETRIS STACK - DESAFIO MESTRE ===\n");
  printf("Gerenciador avançado de peças com trocas entre fila e pilha\n");

  do
  {
    // Exibe o estado atual do jogo
    exibirEstado(&fila, &pilha);

    // Exibe o menu e lê a opção do usuário
    exibirMenu();
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 1:
    {
      // Jogar uma peça (remover da frente da fila)
      Peca pecaJogada = jogarPeca(&fila);
      if (pecaJogada.id != -1)
      {
        printf("Ação: peça [%c %d] foi jogada!\n", pecaJogada.nome, pecaJogada.id);
        // Adiciona nova peça à fila para manter o tamanho
        inserirPeca(&fila);
      }
      break;
    }
    case 2:
    {
      // Reservar uma peça (move da fila para a pilha)
      reservarPeca(&fila, &pilha);
      break;
    }
    case 3:
    {
      // Usar uma peça reservada (remove do topo da pilha)
      usarPecaReservada(&pilha);
      break;
    }
    case 4:
    {
      // Trocar peça da frente da fila com o topo da pilha
      trocarPecaAtual(&fila, &pilha);
      break;
    }
    case 5:
    {
      // Troca múltipla: 3 primeiras da fila com 3 da pilha
      trocaMultipla(&fila, &pilha);
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