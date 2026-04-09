#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

// Definição da estrutura do nó da árvore binária (agora chamada Sala)
typedef struct Sala {
    // Armzena nome ou valor do nó (até 49 caracteres + '\0')
    char nome[50];
    // Ponteiro para o filho à esquerda
    struct Sala *esquerda;
    // Ponteiro para o filho à direita
    struct Sala *direita;
} Sala;

/*
    Função: criarSala
    -------------------------------------------------------
    Aloca dinamicamente uma nova sala, define o nome
    e inicializa as portas (filhos) como NULL.
*/
Sala* criarSala(char* nome) {
    // Aloca dinamicamente memória para um novo nó e
    // guarda o endereço em 'novo'
    Sala* novaSala = (Sala*) malloc(sizeof(Sala)); // Alocação dinâmica de memória
    if (novaSala == NULL) {
        printf("Erro: Falha na alocação de memória!\n");
        exit(1);    // Encerra o programa se em caso de erro de memória
    }

    strcpy(novaSala->nome, nome);   // Copia a string do valor para o campo do nó
    novaSala->esquerda = NULL;        // Inicializa filho esquerdo como NULL
    novaSala->direita = NULL;         // Inicializa filho direito como NULL
    return novaSala;
}

/*
    Função: explorarSalas
    -------------------------------------------------------
    Permite ao jogador navegar pela árvore binária
    escolhendo os caminhos interativamente.
*/
void explorarSalas(Sala* atual) {
    char opcao;
    int explorando = 1; // Flag para controlar o loop de exploração

    while (atual != NULL && explorando) {
        printf("\n========================================\n");
        printf("Você está no(a): %s\n", atual->nome);

        // Verifica se é um nó folha (sala sem saídas)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho! Você chegou a um cômodo sem saídas.\n");
            break;
        }

        printf("Opções de navegação: (e) esquerda | (d) direita | (s) sair\n");
        printf("Sua escolha: ");
        scanf(" %c", &opcao);

        switch (opcao) {
        case 'e':
        case 'E':
            if (atual->esquerda != NULL) {
                atual = atual->esquerda; // Move para a sala à esquerda
            } else {
                printf("Caminho bloqueado! Não há sala à esquerda.\n");

            }
            break;
        case 'd':
        case 'D':
            if (atual->direita != NULL) {
                atual = atual->direita; // Move para a sala à direita
            } else {
                printf("Caminho bloqueado! Não há sala à direita.\n");
            }
            break;
        case 's':
        case 'S':
            printf("Você encerrou a investigação e saiu da mansão.\n");
            explorando = 0;
            break;
        default:
            printf("Opção inválida! Digite 'e', 'd' ou 's'.\n");
            break;
        }
    }
}

/*
    Função: liberar
    -------------------------------------------------------
    Libera recursivamente (em pós-ordem) toda a memória alocada.
*/
void liberar(struct Sala* raiz) {
    if (raiz != NULL) {
        liberar(raiz->esquerda);
        liberar(raiz->direita);
        free(raiz); // LIbera o nó atual
    }
}

int main() {

    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
    // - Use funções como criarSala(), conectarSalas() e explorarSalas().
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
    // - Finalize a exploração com uma opção de saída (s).
    // - Exiba o nome da sala a cada movimento.
    // - Use recursão ou laços para caminhar pela árvore.
    // - Nenhuma inserção dinâmica é necessária neste nível.
        // Criando a árvore binária do mapa da mansão
    Sala* hall = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* quarto = criarSala("Quarto");
    Sala* cozinha = criarSala("Cozinha");
    Sala* sotao = criarSala("Sótão");

    // Montagem estática do mapa da mansão (conectando os ponteiros)
    hall->esquerda = salaEstar;
    hall->direita = biblioteca;
    
    salaEstar->esquerda = quarto;
    salaEstar->direita = cozinha;
    
    biblioteca->direita = sotao; // Biblioteca só tem caminho para a direita

    printf("DETECTIVE QUEST - INÍCIO DA INVESTIGAÇÃO 🔍\n");
    explorarSalas(hall);

    // Libera a memória ao final da execução
    liberar(hall);

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

    return 0;
}
