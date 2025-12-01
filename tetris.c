#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição da struct Peca
typedef struct {
    char tipo;  // Tipo da peça (e.g., 'I', 'O', 'T', 'L')
    int id;     // ID exclusivo
} Peca;

// Constantes
#define TAM_FILA 5
#define TAM_PILHA 3

// Variáveis globais para fila circular
Peca fila[TAM_FILA];
int front = 0;
int rear = TAM_FILA - 1;
int id_global = 1;

// Variáveis globais para pilha
Peca pilha[TAM_PILHA];
int top = -1;

// Função para gerar uma nova peça automaticamente
Peca gerarPeca() {
    Peca nova;
    char tipos[4] = {'I', 'O', 'T', 'L'};
    nova.tipo = tipos[rand() % 4];
    nova.id = id_global++;
    return nova;
}

// Funções para a fila circular
int filaVazia() {
    return front == (rear + 1) % TAM_FILA;
}

int filaCheia() {
    return (rear + 1) % TAM_FILA == front;
}

void inicializarFila() {
    for (int i = 0; i < TAM_FILA; i++) {
        fila[i] = gerarPeca();
    }
}

void enqueue(Peca peca) {
    if (filaCheia()) {
        printf("Erro: Fila cheia!\n");
        return;
    }
    rear = (rear + 1) % TAM_FILA;
    fila[rear] = peca;
}

Peca dequeue() {
    if (filaVazia()) {
        printf("Erro: Fila vazia!\n");
        Peca erro = {'\0', -1};
        return erro;
    }
    Peca removida = fila[front];
    front = (front + 1) % TAM_FILA;
    return removida;
}

void mostrarFila() {
    printf("Fila (frente -> final):\n");
    if (filaVazia()) {
        printf("  (vazia)\n");
        return;
    }
    int count = 1;
    int i = front;
    do {
        printf("  Posição %d: Tipo='%c', ID=%d\n", count++, fila[i].tipo, fila[i].id);
        i = (i + 1) % TAM_FILA;
    } while (i != (rear + 1) % TAM_FILA);
}

// Funções para a pilha
int pilhaVazia() {
    return top == -1;
}

int pilhaCheia() {
    return top == TAM_PILHA - 1;
}

void inicializarPilha() {
    top = -1;
}

void push(Peca peca) {
    if (pilhaCheia()) {
        printf("Erro: Pilha cheia!\n");
        return;
    }
    pilha[++top] = peca;
}

Peca pop() {
    if (pilhaVazia()) {
        printf("Erro: Pilha vazia!\n");
        Peca erro = {'\0', -1};
        return erro;
    }
    return pilha[top--];
}

void mostrarPilha() {
    printf("Pilha de Reserva (topo -> base):\n");
    if (pilhaVazia()) {
        printf("  (vazia)\n");
        return;
    }
    for (int i = top; i >= 0; i--) {
        printf("  Posição %d: Tipo='%c', ID=%d\n", top - i + 1, pilha[i].tipo, pilha[i].id);
    }
}

// Função para trocar peça da frente da fila com o topo da pilha (opção 4)
void trocarFrenteComTopo() {
    if (filaVazia()) {
        printf("Erro: Fila vazia, não há peça para trocar!\n");
        return;
    }
    if (pilhaVazia()) {
        printf("Erro: Pilha vazia, não há peça para trocar!\n");
        return;
    }
    Peca temp = fila[front];
    fila[front] = pilha[top];
    pilha[top] = temp;
    printf("Troca realizada: Peça da frente da fila com o topo da pilha.\n");
}

// Função para trocar os 3 primeiros da fila com as 3 da pilha (opção 5)
void trocarTresPrimeirosComTresPilha() {
    if (top != TAM_PILHA - 1) {
        printf("Erro: Pilha deve ter exatamente 3 peças!\n");
        return;
    }
    // Verifica se a fila tem pelo menos 3 peças (sempre terá, mas por segurança)
    int count = 0;
    int i = front;
    while (count < 3) {
        if (i == (rear + 1) % TAM_FILA) {
            printf("Erro: Fila não tem 3 peças suficientes!\n");
            return;
        }
        i = (i + 1) % TAM_FILA;
        count++;
    }
    // Troca os 3 primeiros da fila com os 3 da pilha
    Peca temp[3];
    for (int j = 0; j < 3; j++) {
        temp[j] = fila[(front + j) % TAM_FILA];
        fila[(front + j) % TAM_FILA] = pilha[j];
        pilha[j] = temp[j];
    }
    printf("Troca realizada: 3 primeiros da fila com as 3 da pilha.\n");
}

int main() {
    srand(time(NULL));
    inicializarFila();
    inicializarPilha();
    int opcao;

    do {
        printf("\n--- Menu Tetris Stack ---\n");
        printf("1 - Jogar peça (remover da frente da fila e inserir nova)\n");
        printf("2 - Enviar peça da fila para reserva (pilha)\n");
        printf("3 - Usar peça da reserva (remover do topo da pilha)\n");
        printf("4 - Trocar peça da frente da fila com topo da pilha\n");
        printf("5 - Trocar 3 primeiros da fila com os 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca removida = dequeue();
                if (removida.id != -1) {
                    printf("Jogando peça: Tipo='%c', ID=%d\n", removida.tipo, removida.id);
                    enqueue(gerarPeca());
                    printf("Nova peça inserida automaticamente no final da fila.\n");
                }
                mostrarFila();
                mostrarPilha();
                break;
            }
            case 2: {
                if (filaVazia()) {
                    printf("Erro: Fila vazia, não há peça para enviar!\n");
                } else {
                    Peca enviada = dequeue();
                    push(enviada);
                    printf("Peça enviada para reserva: Tipo='%c', ID=%d\n", enviada.tipo, enviada.id);
                    enqueue(gerarPeca());
                    printf("Nova peça inserida automaticamente no final da fila.\n");
                }
                mostrarFila();
                mostrarPilha();
                break;
            }
            case 3: {
                Peca usada = pop();
                if (usada.id != -1) {
                    printf("Usando peça da reserva: Tipo='%c', ID=%d\n", usada.tipo, usada.id);
                }
                mostrarFila();
                mostrarPilha();
                break;
            }
            case 4:
                trocarFrenteComTopo();
                mostrarFila();
                mostrarPilha();
                break;
            case 5:
                trocarTresPrimeirosComTresPilha();
                mostrarFila();
                mostrarPilha();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
