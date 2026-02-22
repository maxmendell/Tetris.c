#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// Estrutura que representa uma peca do Tetris
typedef struct {
  char nome; // 'I', 'O', 'T', 'L'
  int id;    // Identificador exclusivo
} Peca;

// Estrutura da Fila Circular
typedef struct {
  Peca itens[MAX_FILA];
  int frente;
  int tras;
  int total;
} Fila;

// Estrutura da Pilha (Reserva)
typedef struct {
  Peca itens[MAX_PILHA];
  int topo;
} Pilha;

// Variavel global para controle de IDs exclusivos
int proximoId = 0;

// Funcao para gerar uma peca aleatoria
Peca gerarPeca() {
  Peca nova;
  char tipos[] = {'I', 'O', 'T', 'L'};
  nova.nome = tipos[rand() % 4];
  nova.id = proximoId++;
  return nova;
}

// --- Funcoes da Fila ---

void inicializarFila(Fila *f) {
  f->frente = 0;
  f->tras = -1;
  f->total = 0;
}

int filaVazia(Fila *f) { return f->total == 0; }

int filaCheia(Fila *f) { return f->total == MAX_FILA; }

void enfileirar(Fila *f) {
  if (filaCheia(f))
    return;
  f->tras = (f->tras + 1) % MAX_FILA;
  f->itens[f->tras] = gerarPeca();
  f->total++;
}

Peca desenfileirar(Fila *f) {
  Peca p = f->itens[f->frente];
  f->frente = (f->frente + 1) % MAX_FILA;
  f->total--;
  // Sempre que uma peca sai, uma nova entra no final
  enfileirar(f);
  return p;
}

// --- Funcoes da Pilha ---

void inicializarPilha(Pilha *p) { p->topo = -1; }

int pilhaVazia(Pilha *p) { return p->topo == -1; }

int pilhaCheia(Pilha *p) { return p->topo == MAX_PILHA - 1; }

void empilhar(Pilha *p, Peca peca) {
  if (pilhaCheia(p)) {
    printf("\n[AVISO] Pilha de reserva cheia!\n");
    return;
  }
  p->itens[++(p->topo)] = peca;
}

Peca desempilhar(Pilha *p) { return p->itens[(p->topo)--]; }

// --- Interface ---

void exibirEstado(Fila f, Pilha p) {
  printf("\n========================================\n");
  printf("Estado atual:\n\n");

  // Exibe Fila
  printf("Fila de pecas:\t");
  int i, cont = 0, idx = f.frente;
  while (cont < f.total) {
    printf("[%c %d] ", f.itens[idx].nome, f.itens[idx].id);
    idx = (idx + 1) % MAX_FILA;
    cont++;
  }

  // Exibe Pilha
  printf("\nPilha de reserva (Topo -> Base): ");
  if (pilhaVazia(&p)) {
    printf("(Vazia)");
  } else {
    for (i = p.topo; i >= 0; i--) {
      printf("[%c %d] ", p.itens[i].nome, p.itens[i].id);
    }
  }
  printf("\n========================================\n");
}

int main() {
  srand(time(NULL));

  Fila fila;
  Pilha pilha;
  inicializarFila(&fila);
  inicializarPilha(&pilha);

  // Inicializa a fila com 5 pecas
  for (int i = 0; i < MAX_FILA; i++) {
    enfileirar(&fila);
  }

  int opcao = -1;
  while (opcao != 0) {
    exibirEstado(fila, pilha);
    printf("\nOpcoes de Acao:\n");
    printf("1 - Jogar peca (Fila)\n");
    printf("2 - Reservar peca (Fila -> Pilha)\n");
    printf("3 - Usar peca reservada (Pilha)\n");
    printf("0 - Sair\n");
    printf("\nOpcao: ");

    if (scanf("%d", &opcao) != 1) {
      printf("\nEntrada invalida!\n");
      while (getchar() != '\n')
        ;
      continue;
    }

    switch (opcao) {
    case 1: {
      Peca p = desenfileirar(&fila);
      printf("\nVoce jogou a peca: [%c %d]\n", p.nome, p.id);
    } break;
    case 2:
      if (pilhaCheia(&pilha)) {
        printf("\n[ERRO] Nao ha espaco na reserva!\n");
      } else {
        Peca p = desenfileirar(&fila);
        empilhar(&pilha, p);
        printf("\nPeca [%c %d] movida para a reserva.\n", p.nome, p.id);
      }
      break;
    case 3:
      if (pilhaVazia(&pilha)) {
        printf("\n[ERRO] Pilha de reserva esta vazia!\n");
      } else {
        Peca p = desempilhar(&pilha);
        printf("\nVoce usou a peca reservada: [%c %d]\n", p.nome, p.id);
      }
      break;
    case 0:
      printf("\nEncerrando Tetris Stack...\n");
      break;
    default:
      printf("\nOpcao invalida!\n");
      break;
    }
  }

  return 0;
}
