#include <stdio.h>
#include <stack>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

// Matriz de char representnado o labirinto
char **maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_linhas;
int num_colunas;

// Representação de uma posição
struct pos_t
{
    int i;
    int j;
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;

pos_t load_maze(const char *file_name)
{
    pos_t initial_pos;

    FILE *file = fopen(file_name, "r");

    if (!file)
    {
        printf("Erro ao abrir o arquivo.");
    }

    char linha[10];
    char coluna[10];

    fscanf(file, "%s %s", linha, coluna);
    fgetc(file); // Consumir o caractere de nova linha

    num_linhas = std::stoi(linha);
    num_colunas = std::stoi(coluna);

    std::cout << "num linhas = " << num_linhas << '\n';
    std::cout << "num colunas = " << num_colunas << '\n';

    maze = new char *[num_linhas];

    for (int i = 0; i < num_linhas; ++i)
    {
        maze[i] = new char[num_colunas + 1]; // +1 para o caractere nulo
        fgets(maze[i], num_colunas + 1, file);
        fgetc(file); // Consumir o caractere de nova linha
    }

    for (int i = 0; i < num_linhas; ++i)
    {
        for (int j = 0; j < num_colunas; ++j)
        {
            if (maze[i][j] == 'e')
            {
                initial_pos.i = i;
                initial_pos.j = j;
            }
        }
    }

    fclose(file);

    return initial_pos;
}

// Função que imprime o labirinto
void print_maze()
{
    for (int i = 0; i < num_linhas; ++i)
    {
        for (int j = 0; j < num_colunas; ++j)
        {
            printf("%c", maze[i][j]);
        }
        printf("\n");
    }
}

// Função que retorna se a posição existe no labirinto
bool position_exists(int i, int j)
{
    if (i > -1 && j > -1 && i < num_linhas && j < num_colunas)
    {
        return true;
    }

    return false;
}

bool position_is_s(int i, int j, char **matriz)
{
    if (!position_exists(i, j))
        return false;
    if (matriz[i][j] == 's')
    {
        return true;
    }
    return false;
}

bool position_is_valid(int i, int j, char **matriz)
{
    if (position_exists(i, j))
    {
        if (position_is_s(i, j, matriz))
            return true;
        return matriz[i][j] == 'x';
    }
    return false;
}

// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos, char **matriz)
{

    while (matriz[pos.i][pos.j] != 's')
    {
        // confere a posição a oeste
        if (position_is_valid(pos.i, pos.j - 1, matriz))
        {
            pos_t auxOeste;
            auxOeste.i = pos.i;
            auxOeste.j = pos.j - 1;
            valid_positions.push(auxOeste);
        }
        if (position_is_s(pos.i, pos.j - 1, matriz))
        {
            matriz[pos.i][pos.j] = '.';
            pos = valid_positions.top();
            system("clear");
            print_maze();
            usleep(10000);

            matriz[pos.i][pos.j] = '.';
            break;
        }

        // confere a posição ao sul
        if (position_is_valid(pos.i + 1, pos.j, matriz))
        {
            pos_t auxSul;
            auxSul.i = pos.i + 1;
            auxSul.j = pos.j;
            valid_positions.push(auxSul);
        }
        if (position_is_s(pos.i + 1, pos.j, matriz))
        {
            matriz[pos.i][pos.j] = '.';
            pos = valid_positions.top();
            system("clear");
            print_maze();
            usleep(10000);
            matriz[pos.i][pos.j] = '.';
            break;
        }

        // confere a posição ao leste
        if (position_is_valid(pos.i, pos.j + 1, matriz))
        {
            pos_t auxLeste;
            auxLeste.i = pos.i;
            auxLeste.j = pos.j + 1;
            valid_positions.push(auxLeste);
        }
        if (position_is_s(pos.i, pos.j + 1, matriz))
        {
            matriz[pos.i][pos.j] = '.';
            pos = valid_positions.top();
            system("clear");
            print_maze();
            usleep(10000);
            matriz[pos.i][pos.j] = '.';
            break;
        }

        // confere a posição ao norte

        if (position_is_valid(pos.i - 1, pos.j, matriz))
        {
            pos_t auxNorte;
            auxNorte.i = pos.i - 1;
            auxNorte.j = pos.j;
            valid_positions.push(auxNorte);
        }
        if (position_is_s(pos.i - 1, pos.j, matriz))
        {
            matriz[pos.i][pos.j] = '.';
            pos = valid_positions.top();
            system("clear");
            print_maze();
            usleep(10000);
            matriz[pos.i][pos.j] = '.';
            break;
        }

        // move o tabuleiro
        if (!valid_positions.empty())
        {
            matriz[pos.i][pos.j] = '.';
            pos = valid_positions.top();
            matriz[pos.i][pos.j] = 'o';
            valid_positions.pop();
        }
        system("clear");
        print_maze();
        usleep(10000);
    }

    system("clear");
    print_maze();
    usleep(10000);
    if (matriz[pos.i][pos.j] == '.')
    {
        std::cout << "Labirinto finalizado com sucesso!" << '\n';
        return true;
    }

    return false;

}

int main(int argc, char *argv[])
{

    // carregar o labirinto com o nome do arquivo recebido como argumento
    pos_t initial_pos = load_maze(argv[1]);

    print_maze();

    

    // chamar a função de navegação
    bool exit_found = walk(initial_pos, maze);

    // Tratar o retorno (imprimir mensagem)
    usleep(2000000);

    return 0;
}