#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // Para o uso da função sleep
#include <stdbool.h>

// Declara uma matriz 8x8 de ponteiros para strings
char *matrix[8][8];
// Declara variáveis para armazenar a posição inicial do robô
int stationLine, stationColumn;

// Estrutura para armazenar a posição do lixo
typedef struct {
    int line;
    int column;
} Position;

// Estrutura para armazenar a posição atual do robô
typedef struct {
    int line;
    int column;
} Robot;

Position trash[64]; // Supondo que possa haver até 64 posições de lixo em uma matriz 8x8

Robot robo; // Variável para a posição atual do robô

// Função para inicializar a matriz com valores padrão "***"
void createMatrix() {
    int i, j;
    // Percorre todas as linhas
    for (i = 0; i < 8; i++) {
        // Percorre todas as colunas
        for (j = 0; j < 8; j++) {
            // Define o valor padrão "***" em cada posição
            matrix[i][j] = "***";
        }
    }
}

// Função para imprimir a matriz no console
void printMatrix() {
    int i, j;
    // Percorre todas as linhas
    for (i = 0; i < 8; i++) {
        // Percorre todas as colunas
        for (j = 0; j < 8; j++) {
            // Imprime o valor da posição atual com formatação
            printf("|%s| ", matrix[i][j]);
        }
        // Pula para a próxima linha após imprimir todas as colunas
        printf("\n");
    }
}

// Função para definir a posição inicial do robô
void addStation() {
    // Exibe a matriz na tela
    printMatrix();
    
    // Solicita ao usuário a linha inicial do robô
    printf("Digite a linha inicial do robo: ");
    scanf("%d", &stationLine);
    
    // Verifica se a linha está fora do intervalo permitido e solicita novamente se necessário
    while (stationLine < 0 || stationLine > 7) {
        printf("Digite a linha inicial entre 0 e 7 do robo: ");
        scanf("%d", &stationLine);
    }
    
    // Solicita ao usuário a coluna inicial do robô
    printf("Digite a coluna inicial do robo: ");
    scanf("%d", &stationColumn);
    
    // Verifica se a coluna está fora do intervalo permitido e solicita novamente se necessário
    while (stationColumn < 0 || stationColumn > 7) {
        printf("Digite a coluna inicial entre 0 e 7 do robo: ");
        scanf("%d", &stationColumn);
    }
    
    // Define a posição inicial do robô na matriz com " E "
    matrix[stationLine][stationColumn] = " E ";
}

// Função para adicionar lixo na matriz
int addTrash() {
    int trashLine = 0, trashColumn = 0;  // Inicializa as variáveis para as coordenadas do lixo
    
    // Continua a solicitar entradas do usuário até que a linha do lixo seja -1
    while (trashLine != -1) {
        // Limpa a tela
        system("cls");
        // Imprime a matriz atualizada
        printMatrix();
        
        // Solicita ao usuário a linha do lixo
        printf("Digite a linha do lixo ou -1 para realizar a limpeza: ");
        scanf("%d", &trashLine);
        
        // Verifica se a linha do lixo está fora do intervalo permitido e solicita novamente se necessário
        while (trashLine < -1 || trashLine > 7) {
            printf("Digite um numero entre 0 e 7 para a linha do lixo ou -1 para realizar limpeza: ");
            scanf("%d", &trashLine);
        }
        
        // Se a linha do lixo for -1, chama a função de limpeza
        if (trashLine == -1) {
            moveRobot();
         	return 0;
        }
        
        // Solicita ao usuário a coluna do lixo
        printf("Digite a coluna do lixo: ");
        scanf("%d", &trashColumn);
        
        // Verifica se a coluna do lixo está fora do intervalo permitido e solicita novamente se necessário
        while (trashColumn < 0 || trashColumn > 7) {
            printf("Digite um numero entre 0 e 7 para a coluna do lixo: ");
            scanf("%d", &trashColumn);
        }
        
        // Verifica se a posição do lixo coincide com a posição inicial do robô
        if (matrix[trashLine][trashColumn] == matrix[stationLine][stationColumn]) {
            // Informa que não é possível adicionar lixo na posição do robô
            printf("Impossivel adicionar lixo na estacao!!\n");
        } else {
            // Define a posição do lixo na matriz com "@@@"
            matrix[trashLine][trashColumn] = "@@@";
        }
    }
}

// Função para verificar a direção do robô em relação à base
int checkDirectionBase() {
    // Verifica se a base está em uma das direções adjacentes ao robô
    bool verifyBaseR = robo.column < 7 && (strcmp(matrix[robo.line][robo.column + 1], " E ") == 0);
    bool verifyBaseL = robo.column > 0 && (strcmp(matrix[robo.line][robo.column - 1], " E ") == 0);
    bool verifyBaseD = robo.line < 7 && (strcmp(matrix[robo.line + 1][robo.column], " E ") == 0);
    bool verifyBaseT = robo.line > 0 && (strcmp(matrix[robo.line - 1][robo.column], " E ") == 0);
    
    // Verifica se há um espaço vazio em direção à base
    bool verifyEmptyR = (robo.column < stationColumn) && (strcmp(matrix[robo.line][robo.column + 1], "   ")) == 0;
    bool verifyEmptyL = (robo.column > stationColumn) && (strcmp(matrix[robo.line][robo.column - 1], "   ")) == 0;
    bool verifyEmptyD = (robo.line < stationLine) && (strcmp(matrix[robo.line + 1][robo.column], "   ")) == 0;
    bool verifyEmptyT = (robo.line > stationLine) && (strcmp(matrix[robo.line - 1][robo.column], "   ")) == 0;
 
    // Retorna a direção baseada nas verificações
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

// Função para mover o robô de volta à base
int voltaBase() {
    // Continua movendo o robô até a base
    while (1) {
        int direction = checkDirectionBase();
        
        // Move o robô baseado na direção retornada
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
        // Atualiza a posição do robô na matriz
        matrix[robo.line][robo.column] = "-_-";
        system("cls");
        printMatrix();
        return 0;
    }
}

// Função para verificar a direção do lixo em relação ao robô
int checkDirectionTrash() {
    // Verifica se há lixo em uma das direções adjacentes ao robô
    int verifyR = robo.column < 7 && strcmp(matrix[robo.line][robo.column + 1], "@@@") == 0;
    int verifyL = robo.column > 0 && strcmp(matrix[robo.line][robo.column - 1], "@@@") == 0;
    int verifyD = robo.line < 7 && strcmp(matrix[robo.line + 1][robo.column], "@@@") == 0;
    int verifyT = robo.line > 0 && strcmp(matrix[robo.line - 1][robo.column], "@@@") == 0;
    
    // Retorna a direção baseada nas verificações
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

// Função para mover o robô e limpar o lixo
int moveRobot() {
    // Inicializa a posição do robô na estação
    robo.line = stationLine;
    robo.column = stationColumn;

    while (1) {
        int directionTrash = checkDirectionTrash();
        int directionBase = checkDirectionBase();
        
        // Se o robô não está na estação, limpa a posição atual
        if (!(robo.line == stationLine && robo.column == stationColumn)) {
            matrix[robo.line][robo.column] = "   ";
        }
        
        // Move o robô baseado na direção do lixo
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
        
        // Atualiza a posição do robô na matriz
        matrix[robo.line][robo.column] = "-_-";
        system("cls");
        printMatrix();
        sleep(1); // Adiciona uma pausa para visualização do movimento
        
        if (!directionTrash && !directionBase) {
        	return 0;
		}
    }
}

// Função principal do programa
int main() {
    // Inicializa a matriz
    createMatrix();
    // Define a posição inicial do robô
    addStation();
    // Adiciona lixo na matriz
    addTrash();
    // Retorna 0 para indicar que o programa terminou corretamente
    return 0;
}

