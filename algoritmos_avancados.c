#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

// Definição da estrutura do nó da árvore binária (agora chamada Sala)
typedef struct Sala {
    // Armzena nome ou valor do nó (até 49 caracteres + '\0')
    char nome[50];
    char pista[50]; // Novo campo para guardar a pista
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
Sala* criarSala(char* nome, const char* pista) {
    // Aloca dinamicamente memória para um novo nó e
    // guarda o endereço em 'novo'
    Sala* novaSala = (Sala*) malloc(sizeof(Sala)); // Alocação dinâmica de memória
    if (novaSala == NULL) {
        printf("Erro: Falha na alocação de memória!\n");
        exit(1);    // Encerra o programa se em caso de erro de memória
    }

    strcpy(novaSala->nome, nome);   // Copia a string do valor para o campo do nó

    if (pista != NULL) {
        strcpy(novaSala->pista, pista);  
    } else {
        strcpy(novaSala->pista, ""); // Se não tiver pista, deixa a string vazia
    }

    novaSala->esquerda = NULL;        // Inicializa filho esquerdo como NULL
    novaSala->direita = NULL;         // Inicializa filho direito como NULL
    return novaSala;
}
// =======================================================
// FUNÇÕES DA ÁRVORE DE PISTAS (BST)
// =======================================================

// Estrutura do nó da BST
// Definição das estruturas (BST)
typedef struct NoBST {
    char valor[50];         // Valor armazenado no nó (string)
    struct NoBST* esquerda; // [PONTEIROS] Aponta para o filho esquerdo
    struct NoBST* direita;  // [PONTEIROS] Aponta para o filho direito
} NoBST;

// Cria um novo nó BST
// Aloca memória para uma nova pista na BST
NoBST* criarNoBST(const char* valor) {
    NoBST* novo = (NoBST*)malloc(sizeof(NoBST)); // [PONTEIROS] malloc retorna o endereço do nó criando, que será manipulado via ponteiros.
    strcpy(novo->valor, valor); 
    novo->esquerda = NULL; // [PONTEIROS] Um ponteiro nulo indica "não tem filho" (folha).
    novo->direita = NULL;  // [PONTEIROS] Idem acima.
    return novo;           // [PONTEIROS] Retorna o endereço do nó criado.
}

// Inserir elementos na BST (ordem alfabética)
NoBST* inserirPista(NoBST* raiz, const char* valor) {
    if (raiz == NULL)
        return criarNoBST(valor);
    if (strcmp(valor, raiz->valor) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, valor); // [PONTEIROS] Liga o campo "esquerda" para apontar para o novo subnó.
    else
        raiz->direita = inserirPista(raiz->direita, valor);   // [PONTEIROS] Liga o campo "direita" para apontar para o novo subnó.
    return raiz; // [PONTEIROS] Sempre devolve o ponteiro (endereço) da raiz da árvore atualizada.
}

// Busca na BST
// Percurso Em Ordem: exibe as pistas em ordem alfabética
bool buscarBST(NoBST* raiz, const char* chave) {
    if (raiz == NULL)
        return false;
    if (strcmp(chave, raiz->valor) == 0)
        return true;
    if (strcmp(chave, raiz->valor) < 0)
        return buscarBST(raiz->esquerda, chave); // [PONTEIROS] Segue o ponteiro "esquerda" para continuar a busca.
    else
        return buscarBST(raiz->direita, chave);  // [PONTEIROS] Segue o ponteiro "direita" para continuar a busca.
}

// Percurso em ordem (ordem alfabética)
// Visita a subárvore esquerda, o nó atual e depois a direita. Exibe valores em ordem alfabética (ou numérica).
void exibirPistas(NoBST* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);   // [PONTEIROS] Percorre a subárvore à esquerda
        printf("%s | ", raiz->valor);
        exibirPistas(raiz->direita);    // [PONTEIROS] Percorre a subárvore à direita
    }
}

// Libera a mémoria da BST
// Libera a memória da árvore de pistas em pós-ordem
void liberarBST(NoBST* raiz) {
    if (raiz != NULL) {
        liberarBST(raiz->direita);    // [PONTEIROS] Libera toda a subárvore à direita
        liberarBST(raiz->esquerda);   // [PONTEIROS] Libera toda a subárvore à esquerda
        free(raiz); // [PONTEIROS] Libera o nó atual
    }
}

/*
    Função: explorarSalas
    -------------------------------------------------------
    Permite ao jogador navegar pela árvore binária
    escolhendo os caminhos interativamente.
*/
void explorarSalasComPistas(Sala* atual, NoBST** raizPistas) {
    char opcao;
    int explorando = 1; // Flag para controlar o loop de exploração

    Sala* raizMapa = atual; // Guarda a referência à raiz do mapa para permitir voltar ao início

    while (atual != NULL && explorando) {
        printf("\n========================================\n");
        printf("Voce esta no(a): %s\n", atual->nome);

        // 1. AVISO GENÉRICO (Fica fora do switch)
        // O jogo te avisa que tem algo, mas não diz o que é e não coleta!
        if (strlen(atual->pista) > 0) {
            printf("Voce nota algo de interesse neste comodo... (Use 'v' para investigar)\n");
        }

        // Verifica se é um nó folha (sala sem saídas)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho! Voce chegou a um comodo sem saidas.\n");
        }

        printf("Opcoes de navegacao: (e) esquerda | (d) direita | (s) sair |\n");
        printf("Verificar (v) | Inventario (i) ");
        if (atual != raizMapa) {
            printf("| Voltar ao Hall (r)");
        }
        printf("\nSua escolha: ");
        scanf(" %c", &opcao);

        switch (opcao) {
        case 'e':
        case 'E':
            if (atual->esquerda != NULL) {
                atual = atual->esquerda; // Move para a sala à esquerda
            } else {
                printf("Caminho bloqueado! Nao ha sala a esquerda.\n");
            }
            break;
        case 'd':
        case 'D':
            if (atual->direita != NULL) {
                atual = atual->direita; // Move para a sala à direita
            } else {
                printf("Caminho bloqueado! Nao ha sala a direita.\n");
            }
            break;
        case 'r':
        case 'R':
            // Volta pro Hall
            if (atual != raizMapa) {
                printf("Voce deu meia volta e retornou ao Hall de Entrada!\n");
                atual = raizMapa; // Reinicia a posição pro ponto salvo lá em cima!
            } else {
                printf("Voce ja esta no Hall de Entrada!\n");
            }
            break;
        case 's':
        case 'S':
            printf("Voce encerrou a investigação e saiu da mansao.\n");
            explorando = 0;
            break;
        case 'v':
        case 'V':
            if (strlen(atual->pista) > 0) {
                printf("Pista revelada e coletada!\n");
                printf("Pista atual: %s\n", atual->pista);
                *raizPistas = inserirPista(*raizPistas, atual->pista); // Adiciona a pista à BST
                strcpy(atual->pista, ""); // Limpa a pista da sala para evitar duplicatas
            } else {
                printf("Voce vasculhou bem, mas nao ha nada (ou nada novo) aqui.\n");
            }
            break;
        case 'i':
        case 'I':
            printf("\nPistas coletadas (em ordem alfabetica):\n");
            if (*raizPistas == NULL) {
                printf(" Nenhuma pista foi coletada.\n");
            } else {
                exibirPistas(*raizPistas);
                printf("\n");
            }
            break;
        default:
            printf("Opcao invalida! Digite 'e', 'd' ou 's'.\n");
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

/*
    Função principal: main
    -------------------------------------------------------
    Monta a árvore binária do mapa da mansão manualmente
    e inicia a exploração.
*/
int main() {
    NoBST* inventarioPistas = NULL;
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

    Sala* hall = criarSala("Hall de Entrada", NULL);
    Sala* salaEstar = criarSala("Sala de Estar", "Pegadas de Lama");
    Sala* biblioteca = criarSala("Biblioteca", "Chave perdida");
    Sala* quarto = criarSala("Quarto", "Livro com paginas faltando");
    Sala* cozinha = criarSala("Cozinha", "Lencol manchado");
    Sala* sotao = criarSala("Sotao", "Gaveta perdida");

    // Montagem estática do mapa da mansão (conectando os ponteiros)
    hall->esquerda = salaEstar;
    hall->direita = biblioteca;
    
    salaEstar->esquerda = quarto;
    salaEstar->direita = cozinha;
    
    biblioteca->direita = sotao; // Biblioteca só tem caminho para a direita

    printf("DETECTIVE QUEST - INICIO DA INVESTIGACAO\n");
    explorarSalasComPistas(hall, &inventarioPistas);
    
    printf("\n========================================\n");
    printf("Pistas coletadas (em ordem alfabetica):\n");
    if (inventarioPistas == NULL) {
        printf(" Nenhuma pista foi coletada.\n");
    } else {
        // Usa o percurso "Em Ordem" da sua BST para exibir organizado
        exibirPistas(inventarioPistas);
    }
    printf("\n========================================\n");

    // Libera a memória ao final da execução
    liberar(hall);
    liberarBST(inventarioPistas);

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
