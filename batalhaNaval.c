/*
    -------  Projeto 1 -----

    Cauã Grigolatto Domingos
    Gabriel VEntura Pires
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

#define LADO_MATRIZ 8
#define META_ATACANTE 3

void eliminarParteBarco(int matriz[][LADO_MATRIZ], char decisao[2]);
int ehPosicaoValida(int linha, int coluna);
int atingiuBarco(int matriz[][LADO_MATRIZ], char decisao[2]);
int ehAdjacenteLinha(int linha, int coluna, int matriz[][LADO_MATRIZ]);
int ehAdjacenteColuna(int linha, int coluna, int matriz[][LADO_MATRIZ]);
int getNumeroColuna(char decisao[2]);
int getNumeroLinha(char decisao[2]);
void mostrarMatriz(int matriz[][LADO_MATRIZ]);
void inicializarMatriz(int matriz[][LADO_MATRIZ]);
void skipMessage(char *message);
void separar();
void limparTela();

int main()
{
    char defensor[30];
    char atacante[30];

    printf("Informe nome do defensor: ");
    scanf("%[^\n]", defensor);
    getchar();
    
    printf("Informe nome do atacante: ");
    scanf("%[^\n]", atacante);
    getchar();

    separar();    
    printf("-----------BATALHA NAVAL-----------\n");
    separar();
    
    int gameBoard[LADO_MATRIZ][LADO_MATRIZ];
    inicializarMatriz(gameBoard);
    mostrarMatriz(gameBoard);
    
    separar();

    char decisao[2];
    int coluna;
    int linha;

    int ehEspacoLivre;
    int ehPrimeiraJogada = TRUE;
    int ehSegundaJogada = FALSE;
    int estaAdjacenteColuna, estaAdjacenteLinha;

    int partesBarco = 0;

    while (partesBarco < 3)
    {
        printf("Vez de %s\n", defensor);
        printf("Informe a posição do barco (ex: A5, B1): ");
        scanf("%s", decisao);
        getchar();

        coluna = getNumeroColuna(decisao);
        linha = getNumeroLinha(decisao);

        if (ehPosicaoValida(coluna, linha)) { 
            
            ehEspacoLivre = gameBoard[linha][coluna] == 0; 

            if (ehEspacoLivre) {
                
                if (ehPrimeiraJogada) { 
                    gameBoard[linha][coluna] = 1; 
                    ehPrimeiraJogada = FALSE;      
                    ehSegundaJogada = TRUE;    
                    partesBarco++;
                }
            
                else {

                    if (ehSegundaJogada) {         
                        estaAdjacenteLinha = ehAdjacenteLinha(linha, coluna, gameBoard);
                        estaAdjacenteColuna = ehAdjacenteColuna(linha, coluna, gameBoard);
                        
                        if (estaAdjacenteColuna || estaAdjacenteLinha) {

                            gameBoard[linha][coluna] = 1;
                            ehSegundaJogada = FALSE;
                            partesBarco++;
                        }
                        else {
                            separar();
                            printf("Posição não-adjacente!\n");
                        }
                        
                    }
                    else {

                        if (estaAdjacenteColuna) {

                            if (ehAdjacenteColuna(linha, coluna, gameBoard)) {
                                separar();
                                printf("SEU BARCO ESTÁ NO EIXO X\n");
                                gameBoard[linha][coluna] = 1;
                                partesBarco++;
                            } 
                            else {
                                separar();
                                printf("Posição não-adjacente!\n");
                            }

                        }
                        else if (estaAdjacenteLinha) {
                            
                            if (ehAdjacenteLinha(linha, coluna, gameBoard)) {
                                separar();
                                printf("SEU BARCO ESTÁ NO EIXO Y\n");
                                gameBoard[linha][coluna] = 1;
                                partesBarco++;
                            }
                            else {
                                separar();
                                printf("Posição não-adjacente!\n");
                            }
                        }
                    }
                }                
            }
            else {
                separar();
                printf("Você já inseriu o barco nessa posição!\n");
            }
        }
        else {
            separar();
            printf("Posição inválida!\n");
        }
        
        separar();
        mostrarMatriz(gameBoard);
        separar();
    }
    
    skipMessage("Pressione ENTER para continuar");
    limparTela();

    //Atacante:
    int tirosRestantes = 5;
    int pontosAtacante = 0;

    int copiaMatriz[LADO_MATRIZ][LADO_MATRIZ];
    inicializarMatriz(copiaMatriz);
    mostrarMatriz(copiaMatriz);

    separar();

    int bateuMetaDePontos = FALSE;
    int esgotouTiros = FALSE;

    while ((!bateuMetaDePontos) && (!esgotouTiros))
    {
        printf("Vez de %s (%d tiros)\n", atacante, tirosRestantes);
        printf("Posição para atirar: ");
        scanf("%s", decisao);
        getchar();

        linha = getNumeroLinha(decisao);
        coluna = getNumeroColuna(decisao);

        separar();

        if (ehPosicaoValida(linha, coluna)) {

            ehEspacoLivre = copiaMatriz[linha][coluna] == 0;

            if (ehEspacoLivre) {

                if (atingiuBarco(gameBoard, decisao)) {    
                    
                    eliminarParteBarco(copiaMatriz, decisao);
                    copiaMatriz[linha][coluna] = 1;
                    pontosAtacante++;
                    tirosRestantes++;
                    printf("Você acertou e ganhou mais uma tentativa!\n");
                }
                else {
                
                    tirosRestantes--;
                    printf("Você errou! %d restantes\n", tirosRestantes);
                }
            }
            else {
                printf("Você já atirou nessa posição!\n");
            }
        }
        else {
            printf("Posição inválida!\n");
        }

        separar();
        mostrarMatriz(copiaMatriz);
        separar();

        bateuMetaDePontos = pontosAtacante == META_ATACANTE;
        esgotouTiros = tirosRestantes == 0;        
    }

    limparTela();
    printf("Fim de jogo!\n");
    separar();
    skipMessage("Pressione ENTER para ver resultados");
    limparTela();

    if (bateuMetaDePontos) {
        printf("%s destruiu todo o barco!\n", atacante);
    }
    else {
        printf("%s defendeu o barco!\n", defensor);
    }

    separar();
    mostrarMatriz(gameBoard);

    return 0;
}

int ehAdjacenteLinha(int linha, int coluna, int matriz[][LADO_MATRIZ]) {
    int ehAdjacente = FALSE;

    int l = 0;
    int c = 0;

    while ((l < LADO_MATRIZ) && (! ehAdjacente)) {
        while ((c < LADO_MATRIZ) && (! ehAdjacente)) {
            if (matriz[l][c] == 1) {
                if (coluna == c) {
                    if ((linha == l + 1) || (linha == l - 1)) {
                        ehAdjacente = TRUE;
                    }
                }
            }
            c++;
        }
        l++;
        c = 0;
    }
    
    return ehAdjacente;
}

int ehAdjacenteColuna(int linha, int coluna, int matriz[][LADO_MATRIZ]) {
    int ehAdjacente = FALSE;
    
    int l = 0;
    int c = 0;

    while ((l < LADO_MATRIZ) && (! ehAdjacente)) {
        while ((c < LADO_MATRIZ) && (! ehAdjacente)) {

            if (matriz[l][c] == 1) {       
                if (linha == l) {
                    if ((coluna == c + 1) || (coluna == c - 1)) {
                        ehAdjacente = TRUE;
                    }
                }
            }
            c++;
        }
        l++;
        c = 0;
    }
    
    return ehAdjacente;
}

void eliminarParteBarco(int matriz[][LADO_MATRIZ], char decisao[2]) {
    int linha = getNumeroLinha(decisao);
    int coluna = getNumeroColuna(decisao);

    matriz[linha][coluna] = 0;
}

int ehPosicaoValida(int linha, int coluna) {
    int ehValida = TRUE;
    
    if ((linha == -1) || (coluna == -1)) {
        ehValida = FALSE;
    }

    return ehValida;
}

int atingiuBarco(int matriz[][LADO_MATRIZ], char decisao[2]) {
    int linha = getNumeroLinha(decisao);
    int coluna = getNumeroColuna(decisao);

    return matriz[linha][coluna] == 1;
} 

int getNumeroColuna(char decisao[2]){
    char opcoesColuna []= {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    char coluna = toupper(decisao[0]);
    
    int posicao = -1;
    int i = 0;

    while ((i < LADO_MATRIZ) && (posicao == -1))
    {
        if (opcoesColuna[i] == coluna) {
            posicao = i;
        }

        i++;
    }

    return posicao;
}

int getNumeroLinha(char decisao[2]){
    char opcoesLinha []= {'1', '2', '3', '4', '5', '6', '7', '8'};
    char linha = decisao[1];
    
    int posicao = -1;
    int i = 0;

    while ((i < LADO_MATRIZ) && (posicao == -1))
    {
        if (opcoesLinha[i] == linha) {
            posicao = i;
        }

        i++;
    }

    return posicao;
}

void mostrarMatriz(int matriz[][LADO_MATRIZ]) {
    char alfabeto = 'A';

    printf("    ");

    for (int i = 0; i < LADO_MATRIZ; i++)
    {
        printf("%c   ", alfabeto);
        alfabeto++;
    }

    printf("\n");
    
    for (int i = 0; i < LADO_MATRIZ; i++) {   
        printf("%d | ", (i + 1));
        
        for (int j = 0; j < LADO_MATRIZ; j++) {
            printf("%d | ", matriz[i][j]);
        }

        printf("\n");
    }
}

void inicializarMatriz(int matriz[][LADO_MATRIZ]) {
    for (int i = 0; i < LADO_MATRIZ; i++) {
        for (int j = 0; j < LADO_MATRIZ; j++) {
            matriz[i][j] = 0;
        }
    }
}

void skipMessage(char *message) {
    printf("%s\n", message);
    getchar();
}

void separar() {
    printf("...........................................\n\n");
}

void limparTela() {
    for (int i = 0; i < 100; i++)
    {
        printf("\n");
    }
}
