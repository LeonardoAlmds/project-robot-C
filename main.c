#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // Para o uso da fun��o sleep
#include <stdbool.h>

// Declara uma matriz 8x8 de ponteiros para strings
char *matrix[8][8];
// Declara vari�veis para armazenar a posi��o inicial do rob�
int stationLine, stationColumn;

// Estrutura para armazenar a posi��o do lixo
typedef struct {
    int line;
    int column;
} Position;

// Estrutura para armazenar a posi��o atual do rob�
typedef struct {
    int line;
    int column;
} Robot;

Position trash[64]; // Supondo que possa haver at� 64 posi��es de lixo em uma matriz 8x8

Robot robo; // Vari�vel para a posi��o atual do rob�

// Fun��o para inicializar a matriz com valores padr�o "***"
void createMatrix() {
    int i, j;
    // Percorre todas as linhas
    for (i = 0; i < 8; i++) {
        // Percorre todas as colunas
        for (j = 0; j < 8; j++) {
            // Define o valor padr�o "***" em cada posi��o
            matrix[i][j] = "***";
        }
    }
}

// Fun��o para imprimir a matriz no console
void printMatrix() {
    int i, j;
    // Percorre todas as linhas
    for (i = 0; i < 8; i++) {
        // Percorre todas as colunas
        for (j = 0; j < 8; j++) {
            // Imprime o valor da posi��o atual com formata��o
            printf("|%s| ", matrix[i][j]);
        }
        // Pula para a pr�xima linha ap�s imprimir todas as colunas
        printf("\n");
    }
}

// Fun��o para definir a posi��o inicial do rob�
void addStation() {
    // Exibe a matriz na tela
    printMatrix();
    
    // Solicita ao usu�rio a linha inicial do rob�
    printf("Digite a linha inicial do robo: ");
    scanf("%d", &stationLine);
    
    // Verifica se a linha est� fora do intervalo permitido e solicita novamente se necess�rio
    while (stationLine < 0 || stationLine > 7) {
        printf("Digite a linha inicial entre 0 e 7 do robo: ");
        scanf("%d", &stationLine);
    }
    
    // Solicita ao usu�rio a coluna inicial do rob�
    printf("Digite a coluna inicial do robo: ");
    scanf("%d", &stationColumn);
    
    // Verifica se a coluna est� fora do intervalo permitido e solicita novamente se necess�rio
    while (stationColumn < 0 || stationColumn > 7) {
        printf("Digite a coluna inicial entre 0 e 7 do robo: ");
        scanf("%d", &stationColumn);
    }
    
    // Define a posi��o inicial do rob� na matriz com " E "
    matrix[stationLine][stationColumn] = " E ";
}

// Fun��o para adicionar lixo na matriz
int addTrash() {
    int trashLine = 0, trashColumn = 0;  // Inicializa as vari�veis para as coordenadas do lixo
    
    // Continua a solicitar entradas do usu�rio at� que a linha do lixo seja -1
    while (trashLine != -1) {
        // Limpa a tela
        system("cls");
        // Imprime a matriz atualizada
        printMatrix();
        
        // Solicita ao usu�rio a linha do lixo
        printf("Digite a linha do lixo ou -1 para realizar a limpeza: ");
        scanf("%d", &trashLine);
        
        // Verifica se a linha do lixo est� fora do intervalo permitido e solicita novamente se necess�rio
        while (trashLine < -1 || trashLine > 7) {
            printf("Digite um numero entre 0 e 7 para a linha do lixo ou -1 para realizar limpeza: ");
            scanf("%d", &trashLine);
        }
        
        // Se a linha do lixo for -1, chama a fun��o de limpeza
        if (trashLine == -1) {
            moveRobot();
         	return 0;
        }
        
        // Solicita ao usu�rio a coluna do lixo
        printf("Digite a coluna do lixo: ");
        scanf("%d", &trashColumn);
        
        // Verifica se a coluna do lixo est� fora do intervalo permitido e solicita novamente se necess�rio
        while (trashColumn < 0 || trashColumn > 7) {
            printf("Digite um numero entre 0 e 7 para a coluna do lixo: ");
            scanf("%d", &trashColumn);
        }
        
        // Verifica se a posi��o do lixo coincide com a posi��o inicial do rob�
        if (matrix[trashLine][trashColumn] == matrix[stationLine][stationColumn]) {
            // Informa que n�o � poss�vel adicionar lixo na posi��o do rob�
            printf("Impossivel adicionar lixo na estacao!!\n");
        } else {
            // Define a posi��o do lixo na matriz com "@@@"
            matrix[trashLine][trashColumn] = "@@@";
        }
    }
}

// Fun��o para verificar a dire��o do rob� em rela��o � base
int checkDirectionBase() {
    // Verifica se a base est� em uma das dire��es adjacentes ao rob�
    bool verifyBaseR = robo.column < 7 && (strcmp(matrix[robo.line][robo.column + 1], " E ") == 0);
    bool verifyBaseL = robo.column > 0 && (strcmp(matrix[robo.line][robo.column - 1], " E ") == 0);
    bool verifyBaseD = robo.line < 7 && (strcmp(matrix[robo.line + 1][robo.column], " E ") == 0);
    bool verifyBaseT = robo.line > 0 && (strcmp(matrix[robo.line - 1][robo.column], " E ") == 0);
    
    // Verifica se h� um espa�o vazio em dire��o � base
    bool verifyEmptyR = (robo.column < stationColumn) && (strcmp(matrix[robo.line][robo.column + 1], "   ")) == 0;
    bool verifyEmptyL = (robo.column > stationColumn) && (strcmp(matrix[robo.line][robo.column - 1], "   ")) == 0;
    bool verifyEmptyD = (robo.line < stationLine) && (strcmp(matrix[robo.line + 1][robo.column], "   ")) == 0;
    bool verifyEmptyT = (robo.line > stationLine) && (strcmp(matrix[robo.line - 1][robo.column], "   ")) == 0;
 
    // Retorna a dire��o baseada nas verifica��es
    if (verifyBaseR || verifyEmptyR) {
        return 1;
    } else if (verifyBaseT || verifyEmptyT) {
        return 2;
    } else if (verifyBaseL || verifyEmptyL ) {
        return 3;
    } else if (verifyBaseD || verifyEmptyD) {
        return 4;
    }
    return 0;
}

// Fun��o para mover o rob� de volta � base
int voltaBase() {
    // Continua movendo o rob� at� a base
    while (1) {
        int direction = checkDirectionBase();
        
        // Move o rob� baseado na dire��o retornada
        switch (direction) {
            case 1: // Direita
                matrix[robo.line][robo.column] = "   ";
                robo.column += 1;
                break;
            case 2: // Esquerda
                matrix[robo.line][robo.column] = "   ";
                robo.line -= 1;
                break;
            case 3: // Baixo
                matrix[robo.line][robo.column] = "   ";
                robo.column -= 1;
                break;
            case 4: // Cima
              	matrix[robo.line][robo.column] = "   ";
                robo.line += 1;
                break;
        }
        // Atualiza a posi��o do rob� na matriz
        matrix[robo.line][robo.column] = "-_-";
        system("cls");
        printMatrix();
        return 0;
    }
}

// Fun��o para verificar a dire��o do lixo em rela��o ao rob�
int checkDirectionTrash() {
    // Verifica se h� lixo em uma das dire��es adjacentes ao rob�
    int verifyR = robo.column < 7 && strcmp(matrix[robo.line][robo.column + 1], "@@@") == 0;
    int verifyL = robo.column > 0 && strcmp(matrix[robo.line][robo.column - 1], "@@@") == 0;
    int verifyD = robo.line < 7 && strcmp(matrix[robo.line + 1][robo.column], "@@@") == 0;
    int verifyT = robo.line > 0 && strcmp(matrix[robo.line - 1][robo.column], "@@@") == 0;
    
    // Retorna a dire��o baseada nas verifica��es
    if (verifyR) {
        return 1;
    } else if (verifyT) {
        return 2;
    } else if (verifyL) {
        return 3;
    } else if (verifyD) {
        return 4;
    } 
    return 0;
}

// Fun��o para mover o rob� e limpar o lixo
int moveRobot() {
    // Inicializa a posi��o do rob� na esta��o
    robo.line = stationLine;
    robo.column = stationColumn;

    while (1) {
        int directionTrash = checkDirectionTrash();
        int directionBase = checkDirectionBase();
        
        // Se o rob� n�o est� na esta��o, limpa a posi��o atual
        if (!(robo.line == stationLine && robo.column == stationColumn)) {
            matrix[robo.line][robo.column] = "   ";
        }
        
        // Move o rob� baseado na dire��o do lixo
        switch (directionTrash) {
            case 1: // Direita
                robo.column += 1;
                matrix[stationLine][stationColumn] = " E ";
                break;
            case 2: // Cima        
                robo.line -= 1;
                matrix[stationLine][stationColumn] = " E ";
                break;
            case 3: // Esquerda
                robo.column -= 1;
                matrix[stationLine][stationColumn] = " E ";
                break;
            case 4: // Baixo
                robo.line += 1;
                matrix[stationLine][stationColumn] = " E ";
                break;
            default:
                voltaBase();
        }
        
        // Atualiza a posi��o do rob� na matriz
        matrix[robo.line][robo.column] = "-_-";
        system("cls");
        printMatrix();
        sleep(1); // Adiciona uma pausa para visualiza��o do movimento
        
        if (!directionTrash && !directionBase) {
        	return 0;
		}
    }
}

// Fun��o principal do programa
int main() {
    // Inicializa a matriz
    createMatrix();
    // Define a posi��o inicial do rob�
    addStation();
    // Adiciona lixo na matriz
    addTrash();
    // Retorna 0 para indicar que o programa terminou corretamente
    return 0;
}

