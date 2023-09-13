#include <stdio.h>
#include <stack>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <vector>



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

void walk(pos_t pos, char **matriz);

void move(char **matriz, int old_i, int old_j, int i, int j);

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::vector<pos_t> valid_positions;
bool exit_found = false;

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
		exit_found = true;
		return true;
	}
	return false;
}

bool position_has_already_been_added(int i, int j){
	for(int index = 0; index < valid_positions.size(); index++){
		if(valid_positions[index].i == i && valid_positions[index].j)
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

void move(char **matriz, int old_i, int old_j, int i, int j)
{
	matriz[old_i][old_j] = '.';
	if (position_is_s(i, j, matriz))
	{
		matriz[i][j] = 'o';
		std::cout << "Labirinto finalizado com sucesso!" << '\n';
		std::cout << "foi esse 1";
		exit_found = true;
		return;
	}
	matriz[i][j] = 'o';
	pos_t pos;
	pos.i = i;
	pos.j = j;
	system("clear");
	print_maze();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	walk(pos, matriz);
}
// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
void walk(pos_t pos, char **matriz)
{

	while (matriz[pos.i][pos.j] != 's')
	{
		// confere a posição a oeste
		if (position_is_valid(pos.i, pos.j - 1, matriz))
		{
			pos_t auxOeste;
			auxOeste.i = pos.i;
			auxOeste.j = pos.j - 1;
			move(matriz, pos.i, pos.j, auxOeste.i, auxOeste.j);
			if(!position_has_already_been_added(auxOeste.i, auxOeste.j))
				valid_positions.push_back(auxOeste);
		}
		if (position_is_s(pos.i, pos.j - 1, matriz))
		{
			matriz[pos.i][pos.j] = '.';
			pos = valid_positions.back();
			matriz[pos.i][pos.j] = '.';
			std::cout << "Labirinto finalizado com sucesso!" << '\n';
			std::cout << "foi esse 2";
			exit_found = true;
			return;
		}

		// confere a posição ao sul
		if (position_is_valid(pos.i + 1, pos.j, matriz))
		{
			pos_t auxSul;
			auxSul.i = pos.i + 1;
			auxSul.j = pos.j;
			if(!position_has_already_been_added(auxSul.i, auxSul.j))
				valid_positions.push_back(auxSul);
		}
		if (position_is_s(pos.i + 1, pos.j, matriz))
		{
			matriz[pos.i][pos.j] = '.';
			pos = valid_positions.back();
			matriz[pos.i][pos.j] = '.';
			std::cout << "Labirinto finalizado com sucesso!" << '\n';
			std::cout << "foi esse 3";
			exit_found = true;
			return;
		}

		// confere a posição ao leste
		if (position_is_valid(pos.i, pos.j + 1, matriz))
		{
			pos_t auxLeste;
			auxLeste.i = pos.i;
			auxLeste.j = pos.j + 1;
			if(!position_has_already_been_added(auxLeste.i, auxLeste.j))
				valid_positions.push_back(auxLeste);
		}
		if (position_is_s(pos.i, pos.j + 1, matriz))
		{
			matriz[pos.i][pos.j] = '.';
			pos = valid_positions.back();
			matriz[pos.i][pos.j] = '.';
			std::cout << "Labirinto finalizado com sucesso!" << '\n';
			std::cout << "foi esse 4";
			exit_found = true;
			return;
		}

		// confere a posição ao norte

		if (position_is_valid(pos.i - 1, pos.j, matriz))
		{
			pos_t auxNorte;
			auxNorte.i = pos.i - 1;
			auxNorte.j = pos.j;
			if(!position_has_already_been_added(auxNorte.i, auxNorte.j))
				valid_positions.push_back(auxNorte);
		}
		if (position_is_s(pos.i - 1, pos.j, matriz))
		{
			matriz[pos.i][pos.j] = '.';
			pos = valid_positions.back();
			matriz[pos.i][pos.j] = '.';
			std::cout << "Labirinto finalizado com sucesso!" << '\n';
			std::cout << "foi esse 5";
			exit_found = true;
			return;
		}

		// move o tabuleiro
		if (!valid_positions.empty())
		{
			if (valid_positions.size() == 1)
			{
				pos_t pos_top = valid_positions.back();
				valid_positions.pop_back();
				move(matriz, pos.i, pos.j, pos_top.i, pos_top.j);
				
			}
			else if (valid_positions.size() == 2)
			{
				pos_t pos_top = valid_positions.back();
				valid_positions.pop_back();
				pos_t pos_next = valid_positions.back();
				valid_positions.pop_back();

				std::thread thread1(move, matriz, pos.i, pos.j, pos_top.i, pos_top.j);
				thread1.detach();
				std::thread thread2(move, matriz, pos.i, pos.j, pos_next.i, pos_next.j);
				thread2.detach();

			}
			else if (valid_positions.size() == 3)
			{
				pos_t pos_top = valid_positions.back();
				valid_positions.pop_back();
				pos_t pos_next = valid_positions.back();
				valid_positions.pop_back();
				pos_t pos_third = valid_positions.back();
				std::thread thread1(move, matriz, pos.i, pos.j, pos_top.i, pos_top.j);
				thread1.detach();
				std::thread thread2(move, matriz, pos.i, pos.j, pos_next.i, pos_next.j);
				thread2.detach();
				std::thread thread3(move, matriz, pos.i, pos.j, pos_third.i, pos_third.j);
				thread3.detach();
				
			}
		}
	}

	return;
}



int main(int argc, char *argv[])
{

	// carregar o labirinto com o nome do arquivo recebido como argumento
	//pos_t initial_pos = load_maze(argv[1]);
	pos_t initial_pos = load_maze("../data/maze2.txt");
	

	while (!exit_found)
	{
		walk(initial_pos, maze);
	}


	std::cout << "chegou";
	// Tratar o retorno (imprimir mensagem)

	return 0;
}
