#include <stdio.h>
#include <stack>
#include <fstream>
#include <iostream>
#include <cstdlib>

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
/* Inserir elemento:

	 pos_t pos;
	 pos.i = 1;
	 pos.j = 3;
	 valid_positions.push(pos)
 */
// Retornar o numero de elementos:
//    valid_positions.size();
//
// Retornar o elemento no topo:
//  valid_positions.top();
//
// Remover o primeiro elemento do vetor:
//    valid_positions.pop();

// Função que le o labirinto de um arquivo texto, carrega em
// memória e retorna a posição inicial
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
	if (i > 0 && j > 0 && i < num_linhas + 1 && j < num_colunas + 1)
	{
		return true;
	}

	return false;
}

bool position_is_s(int i, int j, char **matriz)
{
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

		// confere a posição ao sul
		if (position_is_valid(pos.i + 1, pos.j, matriz))
		{
			pos_t auxSul;
			auxSul.i = pos.i + 1;
			auxSul.j = pos.j;
			valid_positions.push(auxSul);
		}

		// confere a posição ao leste
		if (position_is_valid(pos.i, pos.j + 1, matriz))
		{
			pos_t auxLeste;
			auxLeste.i = pos.i;
			auxLeste.j = pos.j + 1;
			valid_positions.push(auxLeste);
		}

		// confere a posição ao norte

		if (position_is_valid(pos.i - 1, pos.j, matriz))
		{
			pos_t auxNorte;
			auxNorte.i = pos.i - 1;
			auxNorte.j = pos.j;
			valid_positions.push(auxNorte);
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
	}

	if (matriz[pos.i][pos.j] == 's')
		return true;

	return false;

	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
	// Marcar a posição atual com o símbolo '.'
	// Limpa a tela
	// Imprime o labirinto

	/* Dado a posição atual, verifica quais sao as próximas posições válidas
		Checar se as posições abaixo são validas (i>0, i<num_rows, j>0, j <num_cols)
		e se são posições ainda não visitadas (ou seja, caracter 'x') e inserir
		cada uma delas no vetor valid_positions
			- pos.i, pos.j+1
			- pos.i, pos.j-1
			- pos.i+1, pos.j
			- pos.i-1, pos.j
		Caso alguma das posiçÕes validas seja igual a 's', retornar verdadeiro
	*/

	// Verifica se a pilha de posições nao esta vazia
	// Caso não esteja, pegar o primeiro valor de  valid_positions, remove-lo e chamar a funçao walk com esse valor
	// Caso contrario, retornar falso
}

int main(int argc, char *argv[])
{

	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze("../data/maze.txt");

	print_maze();

	// chamar a função de navegação
	bool exit_found = walk(initial_pos, maze);

	// Tratar o retorno (imprimir mensagem)

	return 0;
}
