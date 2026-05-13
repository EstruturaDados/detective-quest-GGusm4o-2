/*
  Programa: Detective Quest - Nível Novato
  -------------------------------------------------------------
  Este programa constrói o mapa de uma mansão usando uma 
  Árvore Binária e permite a exploração interativa pelo jogador.
*/

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAMANHO_TABELA 10

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

// =======================================================
// FUNCOES DA TABELA HASH (NIVEL MESTRE)
// =======================================================

typedef struct NoHash {
    char pista[50];
    char suspeito[50];
    struct NoHash* proximo; // Ponteiro para o próximo nó na lista encadeada (para tratamento de colisões)
} NoHash;

void inicializarHash(NoHash** tabela) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        tabela[i] = NULL;
    }
}

int funcao_hash(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += chave[i];
    }
    return soma % TAMANHO_TABELA;
}

void inserir_chaining(NoHash* tabela_chaining[], const char* pista, const char* suspeito) { 
    int indice = funcao_hash(pista); // Descobre onde armazenar
    // Cria um nó
    NoHash* novo = (NoHash*)malloc(sizeof(NoHash));

    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);

    // Insere no início da lista (head)
    novo->proximo = tabela_chaining[indice];
    tabela_chaining[indice] = novo;
}

NoHash* buscar_chaining(NoHash* tabela_chaining[], const char* pista) {
    int indice = funcao_hash(pista);
    NoHash* atual = tabela_chaining[indice];

    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
                return atual; // Encontrou
        }
        atual = atual->proximo;
    }
    return NULL; // Não está na lista
}

void liberarHash(NoHash* tabela_chaining[]) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        NoHash* atual = tabela_chaining[i];
        while (atual != NULL) {
            NoHash* temp = atual;
            atual = atual->proximo;
            free(temp); // Libera o nó
        }
        tabela_chaining[i] = NULL;
    }
}

// =======================================================
// FASE FINAL: JULGAMENTO (NIVEL MESTRE)
// =======================================================

void encontrarSuspeito(NoBST* raizPistas, NoHash* tabela_chaining[], int* votosMostarda, int* votosBranca, int* votosPlum) {
    if (raizPistas != NULL) {
        encontrarSuspeito(raizPistas->esquerda, tabela_chaining, votosMostarda, votosBranca, votosPlum);

        NoHash* conexao = buscar_chaining(tabela_chaining, raizPistas->valor);

        if (conexao != NULL) {
            if (strcmp(conexao->suspeito, "Mostarda") == 0) {
                (*votosMostarda)++;
            } else if (strcmp(conexao->suspeito, "Branca") == 0) {
                (*votosBranca)++;
            } else if (strcmp(conexao->suspeito, "Plum") == 0) {
                (*votosPlum)++;
            }
        }
        encontrarSuspeito(raizPistas->direita, tabela_chaining, votosMostarda, votosBranca, votosPlum);
    }
}

void verificarSuspeitoFinal(NoBST* inventarioPistas, NoHash* tabela_chaining[]) {
    printf("\n========================================\n");
    printf("SALA DE JULGAMENTO\n");
    printf("========================================\n");

    if (inventarioPistas == NULL) {
        printf("Nenhuma pista coletada. O julgamento não pode prosseguir.\n");
        printf("O culpado escapou por falta de provas!\n");
        return;
    }

    int votosMostarda = 0, votosBranca = 0, votosPlum = 0;
    encontrarSuspeito(inventarioPistas, tabela_chaining, &votosMostarda, &votosBranca, &votosPlum);

    printf("=== RELATORIO DO SISTEMA ===\n");
    if (votosMostarda == 0 && votosBranca == 0 && votosPlum == 0) {
        printf("Nenhuma de suas pistas aponta para os suspeitos conhecidos.\n");
    } else if (votosMostarda >= votosBranca && votosMostarda >= votosPlum) {
        printf("Suspeito mais provavel pelas pistas: Mostarda (%d evidencias)\n", votosMostarda);
    } else if (votosBranca >= votosMostarda && votosBranca >= votosPlum) {
        printf("Suspeito mais provavel pelas pistas: Branca (%d evidencias)\n", votosBranca);
    } else {
        printf("Suspeito mais provavel pelas pistas: Plum (%d evidencias)\n", votosPlum);
    }

    char acusado[50];
    printf("\nDetetive, quem voce acusa formalmente? (Mostarda[1], Branca[2] ou Plum[3]): ");
    scanf(" %49s", acusado);

    int provasContraAcusado = 0;
    if (strcmp(acusado, "Mostarda") == 0 || strcmp(acusado, "mostarda") == 0 || strcmp(acusado, "1") == 0) {
        provasContraAcusado = votosMostarda;
    } else if (strcmp(acusado, "Branca") == 0 || strcmp(acusado, "branca") == 0 || strcmp(acusado, "2") == 0) {
        provasContraAcusado = votosBranca;
    } else if (strcmp(acusado, "Plum") == 0 || strcmp(acusado, "plum") == 0 || strcmp(acusado, "3") == 0) {
        provasContraAcusado = votosPlum;
    }

    int maxProvas = votosMostarda;
    if (votosBranca > maxProvas) maxProvas = votosBranca;
    if (votosPlum > maxProvas) maxProvas = votosPlum;

    if (provasContraAcusado >= 2) {
        printf("\nPARABENS! O detetive acertou em cheio!\n");
        printf("As %d pistas provaram a culpa de %s. Caso encerrado!\n", provasContraAcusado, acusado);
    } else {
        if (maxProvas >= 2) {
            printf("\nFALHOU NO TRIBUNAL! Voce acusou %s, mas apresentou apenas prova(s) invalidas.\n", acusado);
        } else {
            printf("\nFALHOU NO TRIBUNAL! Voce acusou %s, mas apresentou apenas %d prova(s).\n", acusado, provasContraAcusado);
            printf("O juiz rejeitou o caso por falta de evidencias contundentes (minimo de 2).\n");
        }
        printf("Com o seu erro na acusacao, o verdadeiro culpado aproveitou a confusao e FUGIU da mansao!\n");
        printf("GAME OVER para o detetive...\n");
    }
}

/*
    Função: explorarSalas
    -------------------------------------------------------
    Permite ao jogador navegar pela árvore binária
    escolhendo os caminhos interativamente.
*/

void explorarSalasComPistas(Sala* atual, NoBST** raizPistas, NoHash* tabela_chaining[]) {
    char opcao;
    int explorando = 1; // Flag para controlar o loop de exploração

    Sala* raizMapa = atual; // Guarda a referência à raiz do mapa para permitir voltar ao início

    while (atual != NULL && explorando) {
        printf("\n========================================\n");
        printf("Voce esta no(a): %s\n", atual->nome);
        printf("Principais Suspeitos: Mostarda, Branca, Plum\n");

        // 1. AVISO GENÉRICO (Fica fora do switch)
        // O jogo te avisa que tem algo, mas não diz o que é e não coleta!
        if (strlen(atual->pista) > 0) {
            printf("Voce nota algo de interesse neste comodo... (Use 'v' para investigar)\n");
        }

        // Verifica se é um nó folha (sala sem saídas)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho! Voce chegou a um comodo sem saidas.\n");
        }

        printf("========================================\n");
        printf("Necessario no minimo 2 pistas para uma acusacao valida no julgamento final.\n");
        printf("Opcoes de navegacao: (e) esquerda | (d) direita | (s) iniciar investigacao |\n");
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

                int vMostarda = 0, vBranca = 0, vPlum = 0;
                encontrarSuspeito(*raizPistas, tabela_chaining, &vMostarda, &vBranca, &vPlum);
                printf("\n--- DEDUCAO ATUAL ---\n");
                if (vMostarda == 0 && vBranca == 0 && vPlum == 0) {
                    printf("Nenhuma de suas pistas aponta para os suspeitos conhecidos.\n");
                } else if (vMostarda >= vBranca && vMostarda >= vPlum) {
                    printf("Suspeito mais provavel pelas pistas: Mostarda (%d evidencias)\n", vMostarda);
                } else if (vBranca >= vMostarda && vBranca >= vPlum) {
                    printf("Suspeito mais provavel pelas pistas: Branca (%d evidencias)\n", vBranca);
                } else {
                    printf("Suspeito mais provavel pelas pistas: Plum (%d evidencias)\n", vPlum);
                }
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
    // 1. Inicializa o inventário de pistas vazio (Raiz da BST)
    NoBST* inventarioPistas = NULL;

    NoHash* tabela_chaining[TAMANHO_TABELA]; // Tabela hash para conectar pistas a suspeitos
    inicializarHash(tabela_chaining);

    inserir_chaining(tabela_chaining, "Pegadas de Lama", "Mostarda");
    inserir_chaining(tabela_chaining, "Chave perdida", "Branca");
    inserir_chaining(tabela_chaining, "Livro com paginas faltando", "Mostarda");
    inserir_chaining(tabela_chaining, "Lencol manchado", "Plum");
    inserir_chaining(tabela_chaining, "Gaveta perdida", "Branca");

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

    printf("========================================\n");
    printf("DETECTIVE QUEST - INICIO DA INVESTIGACAO\n");
    printf("========================================\n");

    explorarSalasComPistas(hall, &inventarioPistas, tabela_chaining);
    
    printf("\n========================================\n");
    printf("Pistas coletadas (em ordem alfabetica):\n");
    if (inventarioPistas == NULL) {
        printf(" Nenhuma pista foi coletada.\n");
    } else {
        // Usa o percurso "Em Ordem" da sua BST para exibir organizado
        exibirPistas(inventarioPistas);
    }
    printf("\n========================================\n");

    verificarSuspeitoFinal(inventarioPistas, tabela_chaining);

    // Libera a memória ao final da execução
    liberar(hall);
    liberarBST(inventarioPistas);
    liberarHash(tabela_chaining);

    return 0;
}
