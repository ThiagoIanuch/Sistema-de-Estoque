#include <stdio.h>
#include <stdlib.h> // Permitir o uso de system() e exit()
#include <string.h> // Permitir o uso de strcmp() e strstr()
#include <math.h> // Permitir o uso de ceil()
#include <ctype.h> // Permitir o uso de tolower() e toupper()
#include <conio.h> // Permitir o uso de _getch()
#include <locale.h> // Permitir o uso de setlocale()

// Defini��es de mensagens e outros
#define SAIR_PROGRAMA "\n\033[31mPrograma encerrado com sucesso!\x1B[0m\n"
#define ERRO_BANCO "\n\033[31mOcorreu um erro ao acessar a base de dados. Contate um administrador!\x1B[0m\n"
#define OPCAO_INVALIDA "\033[31mOp��o inv�lida. Escolha novamente!\x1B[0m\n\n"
#define NENHUM_CADASTRO "\033[31mNenhum produto foi cadastrado no sistema!\x1B[0m\n\n"
#define QUANTIA_MAXIMA "\033[31mN�o � poss�vel adicionar mais produtos, a quantia m�xima j� foi cadastrada!\x1B[0m\n\n"
#define PRODUTO_EXISTENTE "\033[31m\nEste produto j� foi cadastrado no sistema!\x1B[0m\n\n"
#define PRODUTO_NAO_ENCONTRADO "\n\033[31mNenhum produto foi encontrado!\x1B[0m\n\n"
#define FORNECEDOR_NAO_ENCONTRADO "\n\033[31mFornecedor n�o encontrado!\x1B[0m\n\n"
#define CANCELADO_SUCESSO "\n\033[31mOpera��o cancelada com sucesso!\x1B[0m\n\n"
#define CADASTRO_SUCESSO "\n\033[32mProduto cadastrado com sucesso!\x1B[0m\n\n"
#define DELETADO_SUCESSO "\n\033[32mProduto deletado com sucesso!\x1B[0m\n\n"
#define ALTERADO_SUCESSO "\n\033[32mAltera��o realizada com sucesso!\x1B[0m\n\n"
#define PAUSAR "pause"
#define LIMPAR "cls"
#define ESC 27
#define SETAS 224
#define SETA_ESQUERDA 75
#define SETA_DIREITA 77

// Vari�veis globais e estrutura do produto
const int MAX_PRODUTO = 50;

typedef struct TProduto {
	long int codigo;
	int grupo;
	char descricao[41];
	char unidade[3];
	char fornecedor[41];
	float quantidade;
	float pr_compra;
	float pr_venda;
	int lucro;
	float estoque_min;
}TProduto;

TProduto produto[MAX_PRODUTO];
int cont_produto;
int percentual_venda = 50;
FILE* arquivo;

// Fun��o para ler os dados do arquivo
void lerArquivo() {
	arquivo = fopen("estoque.dat", "r+b");

	if (arquivo != NULL) {
		fread(&cont_produto, sizeof(cont_produto), 1, arquivo);

		for (int i = 0; i < cont_produto; i++) {
			fread(&produto[i], sizeof(TProduto), 1, arquivo);
		}

		fclose(arquivo);
	}
	else {
		cont_produto = 0;
	}
}

// Fun��o para escrever os dados no arquivo
void escreverArquivo() {
	arquivo = fopen("estoque.dat", "w+b");

	if (arquivo != NULL) {
		TProduto aux;

		fwrite(&cont_produto, sizeof(cont_produto), 1, arquivo);

		// Ao gravar os dados no arquivo j� deixa organizado pelo c�digo
		for (int i = 0; i < cont_produto; i++) {
			for (int j = i + 1; j < cont_produto; j++) {
				if (produto[i].codigo > produto[j].codigo) {
					aux = produto[i];
					produto[i] = produto[j];
					produto[j] = aux;
				}
			}
			fwrite(&produto[i], sizeof(TProduto), 1, arquivo);
		}

		fclose(arquivo);
	}
	else {
		printf(ERRO_BANCO);
	}
}

// Fun��o para apresentar o produto em formato de ficha
void fichaProduto(int indice) {
	printf("==============================================================================================\n");
	printf("C�digo: %-53li Grupo: %i\n", produto[indice].codigo, produto[indice].grupo);
	printf("Descri��o: %-50s Unidade: %s\n", produto[indice].descricao, produto[indice].unidade);
	printf("Fornecedor: %s\n", produto[indice].fornecedor);
	printf("Pre�o de compra: R$ %-10.2f Pre�o de venda: R$ %-11.2f ", produto[indice].pr_compra, produto[indice].pr_venda);

	if (produto[indice].lucro >= 50) {
		printf("Lucro M�nimo: \033[0;92m%i%%\033[0m\n", produto[indice].lucro);
	}
	else {
		printf("Lucro M�nimo: \033[0;91m%i%%\033[0m\n", produto[indice].lucro);
	}

	if (produto[indice].quantidade > produto[indice].estoque_min) {
		printf("Quantidade em estoque: %-38.2f ", produto[indice].quantidade);
	}
	else {
		printf("Quantidade em estoque: \033[0;91m%-38.2f\033[0m ", produto[indice].quantidade);
	}

	printf("Quantidade m�nima: %.2f\n", produto[indice].estoque_min);
	printf("==============================================================================================\n\n");
}

// Fun��o para realizar a pagina��o
void paginacao(int produtos_por_pagina, int* pagina_atual, int paginas_total, int* indice, int* sair) {

	int tecla;

	if ((*indice + 1) % produtos_por_pagina == 0 || (*indice + 1) == cont_produto) {
		// Exibir a contagem
		printf("Exibindo p�gina <%i> de <%i>. Total de produtos: <%i>!\n\n", *pagina_atual, paginas_total, cont_produto);

		// Menu de navega��o
		if (*pagina_atual == 1 && *pagina_atual < paginas_total) {
			printf("Avan�ar ->                      ESC retornar ao menu principal                                \n");
		}
		else if (*pagina_atual > 1 && *pagina_atual < paginas_total) {
			printf("Avan�ar ->                      ESC retornar ao menu principal                       <- Voltar\n");
		}
		else {
			printf("                                ESC retornar ao menu principal                       <- Voltar\n");
		}
		
		// Desativa o cursor
		printf("\33[?25l");

		// Pegar a tecla pressionada pelo usu�rio
		do {
			tecla = _getch();
			if (tecla == SETAS) {
				tecla = _getch();
			}

		} while (tecla != SETA_ESQUERDA && tecla != SETA_DIREITA && tecla != ESC);

		// Avan�ar p�gina
		if (tecla == SETA_ESQUERDA) {
			if (*pagina_atual > 1) {
				(*pagina_atual)--;

				if ((*indice + 1) % produtos_por_pagina == 0) {
					*indice -= produtos_por_pagina * 2;
				}
				else {
					*indice -= produtos_por_pagina + (cont_produto % produtos_por_pagina);
				}
			}
			else {
				*indice = -1;
			}
		}
		// Retornar p�gina
		else if (tecla == SETA_DIREITA) {
			if (*pagina_atual < paginas_total) {
				(*pagina_atual)++;
			}
			else {
				if (cont_produto % produtos_por_pagina == 0) {
					*indice -= produtos_por_pagina;
				}
				else {
					*indice -= cont_produto % produtos_por_pagina;
				}
			}
		}
		// Sair
		else {
			// Reativa o cursor
			printf("\33[?25h");

			*sair = 1;
		}

		system(LIMPAR);
	}
}

// Fun��o para adicionar os produtos no sistema
void adicionarProduto() {
	system(LIMPAR);

	if (cont_produto < MAX_PRODUTO) {

		TProduto aux;
		bool verificar_codigo = true;

		printf("========================================= Adicionar ==========================================\n\n");

		// Pedir o c�digo e verificar se j� foi cadastrado
		do {
			printf("Digite o c�digo................................: ");
			scanf("%li", &aux.codigo);

			for (int i = 0; i < cont_produto; i++) {
				if (aux.codigo == produto[i].codigo) {
					verificar_codigo = false;

					break;
				}
			}

			if (aux.codigo <= 0) {
				printf("\nO c�digo n�o pode ser menor ou igual a 0.\n\n");
			}
		} while (aux.codigo <= 0);

		if (verificar_codigo) {
			// Pedir os outros dados do 
			printf("\n==================== Grupos ====================\n");
			printf("[1]. Bebidas\n");
			printf("[2]. Doces\n");
			printf("[3]. Salgadinhos\n");
			printf("[4]. Conveni�ncias\n\n");
			
			do {
				printf("Digite o grupo.................................: ");
				scanf("%i", &aux.grupo);

				if (aux.grupo < 1 || aux.grupo > 4) {
					printf("\nO grupo n�o pode ser menor que 1 ou maior que 4.\n\n");
				}
			} while (aux.grupo < 1 || aux.grupo > 4);

			printf("\n");

			do {
				printf("Digite a descri��o.............................: ");
				getchar();
				gets_s(aux.descricao);

				if (strlen(aux.descricao) == 0) {
					printf("\nA descri��o n�o pode ser vazia.\n\n");
				}
			} while (strlen(aux.descricao) == 0);

			printf("\n");

			do {
				printf("Digite a unidade...............................: ");
				gets_s(aux.unidade);

				if (strlen(aux.unidade) == 0) {
					printf("\nA unidade n�o pode ser vazia.\n\n");
				}
			} while (strlen(aux.unidade) == 0);

			printf("\n");

			do {
				printf("Digite o fornecedor............................: ");
				gets_s(aux.fornecedor);

				if (strlen(aux.fornecedor) == 0) {
					printf("\nO fornecedor n�o pode ser vazio.\n\n");
				}
			} while (strlen(aux.fornecedor) == 0);

			printf("\n");

			do {
				printf("Digite a quantidade............................: ");
				scanf("%f", &aux.quantidade);

				if (aux.quantidade < 0) {
					printf("\nA quantidade n�o pode ser menor que 0.\n\n");
				}
			} while (aux.quantidade < 0);

			printf("\n");

			do {
				printf("Digite o pre�o de compra.......................: R$ ");
				scanf("%f", &aux.pr_compra);

				if (aux.pr_compra < 0) {
					printf("\nO pre�o de compra n�o pode ser menor que 0.\n\n");
				}
			} while (aux.pr_compra < 0);

			// Pre�o de venda e lucro autom�ticos
			aux.pr_venda = aux.pr_compra + (aux.pr_compra / 100) * percentual_venda;

			aux.lucro = ((aux.pr_venda - aux.pr_compra) * 100) / aux.pr_compra;

			printf("\n");

			do {
				printf("Digite a quantidade m�nima.....................: ");
				scanf("%f", &aux.estoque_min);

				if (aux.estoque_min <= 0) {
					printf("\nA quantidade m�nima n�o pode ser menor ou igual a 0.\n\n");
				}
			} while (aux.estoque_min <= 0);

			// Realizar confirma��o
			char confirmar;
			printf("\n");

			do {
				printf("Confirma que deseja adicionar no sistema? (S/N): ");
				scanf(" %c", &confirmar);
			} while (toupper(confirmar) != 'S' && toupper(confirmar) != 'N');

			// Cadastrar no sistema
			if (toupper(confirmar) == 'S') {
				produto[cont_produto] = aux;
				cont_produto++;
				escreverArquivo();
				printf(CADASTRO_SUCESSO);
			}
			else {
				printf(CANCELADO_SUCESSO);
			}
		}
		else {
			printf(PRODUTO_EXISTENTE);
		}
	}
	else {
		printf(QUANTIA_MAXIMA);
	}

	system(PAUSAR);

	system(LIMPAR);
}

// Fun��o para alterar dados de um produto
void alterarDados() {
	system(LIMPAR);

	if (cont_produto > 0) {
		printf("========================================== Alterar ===========================================\n\n");

		// Pedir o c�digo
		long int codigo_busca;
		do {
			printf("Digite o c�digo a ter os dados alterados: ");
			scanf("%li", &codigo_busca);
		} while (codigo_busca <= 0);

		// Procurar pelo c�digo
		bool produto_encontrado = false;
		for (int i = 0; i < cont_produto; i++) {
			if (codigo_busca == produto[i].codigo) {

				produto_encontrado = true;

				int opcao;
				TProduto aux = produto[i];

				system(LIMPAR);

				do {
					printf("========================================== Alterar ===========================================\n\n");

					// Exibir informa��o do produto
					fichaProduto(i);

					// Listar as op��es
					printf("==============================================================================================\n");
					printf("<1>. Alterar o grupo\n");
					printf("<2>. Alterar a descri��o\n");
					printf("<3>. Alterar a unidade\n");
					printf("<4>. Alterar o fornecedor\n");
					printf("<5>. Alterar a quantidade\n");
					printf("<6>. Alterar o pre�o de compra\n");
					printf("<7>. Alterar o estoque m�nimo.\n");
					printf("<0>. Retornar ao menu principal\n");
					printf("==============================================================================================\n\n");

					// Pedir a op��o
					printf("Escolha uma das op��es: ");
					scanf("%i", &opcao);

					system(LIMPAR);

					if (opcao >= 1 || opcao <= 6) {
						printf("========================================== Alterar ===========================================\n\n");
					}

					switch (opcao) {
					case 1:
						printf("============= Grupos =============\n");
						printf("[1]. Bebidas\n");
						printf("[2]. Doces\n");
						printf("[3]. Salgadinhos\n");
						printf("[4]. Conveni�ncias\n\n");
						do {
							printf("Digite o novo grupo...............: ");
							scanf("%i", &aux.grupo);

							if (aux.grupo < 1 || aux.grupo > 4) {
								printf("\nO grupo n�o pode ser menor que 1 ou maior que 4.\n\n");
							}
						} while (aux.grupo < 1 || aux.grupo > 4);
						break;
					case 2:
						do {
							printf("Digite a nova descri��o...........: ");
							getchar();
							gets_s(aux.descricao);

							if (strlen(aux.descricao) == 0) {
								printf("\nA descri��o n�o pode ser vazia.\n\n");
							}
						} while (strlen(aux.descricao) == 0);
						break;
					case 3:
						do {
							printf("Digite a nova unidade.............: ");
							getchar();
							gets_s(aux.unidade);

							if (strlen(aux.unidade) == 0) {
								printf("\nA unidade n�o pode ser vazia.\n\n");
							}
						} while (strlen(aux.unidade) == 0);
						break;
					case 4:
						do {
							printf("Digite o novo fornecedor..........: ");
							getchar();
							gets_s(aux.fornecedor);

							if (strlen(aux.fornecedor) == 0) {
								printf("\nO fornecedor n�o pode ser vazio.\n\n");
							}
						} while (strlen(aux.fornecedor) == 0);
						break;
					case 5:
						do {
							printf("Digite a nova quantidade..........: ");
							scanf("%f", &aux.quantidade);

							if (aux.quantidade < 0) {
								printf("\nA quantidade n�o pode ser menor que 0.\n\n");
							}
						} while (aux.quantidade < 0);
						break;
					case 6:
						do {
							printf("Digite o novo pre�o de compra.....: R$ ");
							scanf("%f", &aux.pr_compra);

							if (aux.pr_compra < 0) {
								printf("\nO pre�o de compra n�o pode ser menor que 0.\n\n");
							}
						} while (aux.pr_compra < 0);

						aux.pr_venda = aux.pr_compra + (aux.pr_compra / 100) * percentual_venda;

						aux.lucro = ((aux.pr_venda - aux.pr_compra) * 100) / aux.pr_compra;
						break;
						break;
					case 7:
						do {
							printf("Digite a nova quantidade m�nima...: ");
							scanf("%f", &aux.estoque_min);

							if (aux.estoque_min <= 0) {
								printf("\nA quantidade m�nima n�o pode ser menor ou igual a 0.\n\n");
							}
						} while (aux.estoque_min < 0);
						break;
					case 0:
						return;
						break;
					default:
						system(LIMPAR);
						printf(OPCAO_INVALIDA);
						break;
					}
				} while (opcao < 0 || opcao > 7);

				printf("\n");
				// Confirmar altera��o
				char confirmar;
				do {
					printf("Confirma que deseja alterar? (S/N): ");
					scanf(" %c", &confirmar);
				} while (toupper(confirmar) != 'S' && toupper(confirmar) != 'N');

				// Realizar altera��o
				if (toupper(confirmar) == 'S') {
					produto[i] = aux;
					escreverArquivo();
					printf(ALTERADO_SUCESSO);
				}
				else {
					printf(CANCELADO_SUCESSO);
				}

				break;
			}
		}

		if (!produto_encontrado) {
			printf(PRODUTO_NAO_ENCONTRADO);
		}
	}
	else {
		printf(NENHUM_CADASTRO);
	}

	system(PAUSAR);

	system(LIMPAR);
}

// Fun��o para excluir produtos do sistema
void excluirProduto() {
	system(LIMPAR);

	if (cont_produto > 0) {
		printf("========================================== Excluir ===========================================\n\n");

		// Pedir o c�digo a ser exclu�do
		long int codigo_excluir;
		do {
			printf("Digite o c�digo do produto a ser exclu�do......: ");
			scanf("%li", &codigo_excluir);

			if (codigo_excluir <= 0) {
				printf("\nO c�digo n�o pode ser menor ou igual a 0.\n\n");
			}
		} while (codigo_excluir <= 0);

		// Procurar pelo produto
		bool produto_encontrado = false;
		for (int i = 0; i < cont_produto; i++) {
			if (codigo_excluir == produto[i].codigo) {
				produto_encontrado = true;

				printf("\n");

				fichaProduto(i);

				// Confirmar se deseja excluir
				char confirmar;
				do {
					printf("Confirma que deseja excluir este produto? (S/N): ");
					scanf(" %c", &confirmar);
				} while (toupper(confirmar) != 'S' && toupper(confirmar) != 'N');

				// Excluir o produto
				if (toupper(confirmar) == 'S') {
					cont_produto--;

					for (int j = i; j < cont_produto; j++) {
						produto[j] = produto[j + 1];
					}

					escreverArquivo();

					printf(DELETADO_SUCESSO);
				}
				else {
					printf(CANCELADO_SUCESSO);
				}

				break;
			}
		}

		if (!produto_encontrado) {
			printf(PRODUTO_NAO_ENCONTRADO);
		}

	}
	else {
		printf(NENHUM_CADASTRO);
	}

	system(PAUSAR);

	system(LIMPAR);
}

// Fun��o para buscar o produto pelo codigo
void buscarProdutoCodigo() {
	system(LIMPAR);

	printf("=========================================== Buscar ===========================================\n\n");

	// Pedir o c�digo a ser buscado
	long codigo_busca;
	do {
		printf("Digite o c�digo do produto a ser buscado: ");
		scanf("%li", &codigo_busca);

		if (codigo_busca <= 0) {
			printf("\nO c�digo n�o pode ser menor ou igual a 0.\n\n");
		}
	} while (codigo_busca <= 0);

	// Realizar a busca
	bool produto_encontrado = false;
	for (int i = 0; i < cont_produto; i++) {
		if (codigo_busca == produto[i].codigo) {
			produto_encontrado = true;

			printf("\n");

			fichaProduto(i);

			break;
		}
	}

	if (!produto_encontrado) {
		printf(PRODUTO_NAO_ENCONTRADO);
	}

	system(PAUSAR);

	system(LIMPAR);
}

// Fun��o para buscar o produto pela descri��o
void buscarProdutoDescricao() {
	system(LIMPAR);

	printf("=========================================== Buscar ===========================================\n\n");

	// Pedir a descri��o a ser buscada
	char descricao_busca[50];
	printf("Digite a descri��o do produto a ser buscado: ");
	getchar();
	gets_s(descricao_busca);

	printf("\n");

	// Realizar a busca
	bool produto_encontrado = false;
	int cont_encontrados = 0;
	for (int i = 0; i < cont_produto; i++) {
		if (strstr(produto[i].descricao, descricao_busca)) {
			fichaProduto(i);

			produto_encontrado = true;
			cont_encontrados++;
		}
	}

	// Exibir o total encontrado
	if (produto_encontrado) {
		printf("Total de produtos encontrados: \033[32m%i\x1B[0m!\n\n", cont_encontrados);
	}
	else {
		printf(PRODUTO_NAO_ENCONTRADO);
	}

	system(PAUSAR);

	system(LIMPAR);
}

// Fun��o para exibir o menu de busca
void menuBusca() {
	system(LIMPAR);

	if (cont_produto > 0) {
		int opcao;

		// Menu de op��es
		do {
			printf("=========================================== Buscar ===========================================\n");
			printf("<1>. Buscar pelo c�digo.\n");
			printf("<2>. Buscar pela descri��o.\n");
			printf("<0>. Retornar ao menu principal.\n");
			printf("==============================================================================================\n\n");

			printf("Escolha uma op��o: ");
			scanf("%i", &opcao);

			switch (opcao) {
			case 1:
				buscarProdutoCodigo();
				break;
			case 2:
				buscarProdutoDescricao();
				break;
			case 0:
				system(LIMPAR);
				return;
				break;
			default:
				system(LIMPAR);
				printf(OPCAO_INVALIDA);
				break;
			}
		} while (opcao < 0 || opcao > 2);
	}
	else {
		printf(NENHUM_CADASTRO);

		system(PAUSAR);

		system(LIMPAR);
	}
}

// Fun��o para listar os produtos do sistema
void listarProdutos() {
	system(LIMPAR);

	int produtos_por_pagina = 2;
	int pagina_atual = 1;
	int paginas_total = ceil((float)cont_produto / produtos_por_pagina);
	int sair = 0;

	for (int i = 0; i < cont_produto; i++) {
		if (i % produtos_por_pagina == 0) {
			printf("====================================== Listar produtos =======================================\n\n");

		}

		// Listar os produtos
		fichaProduto(i);

		// Realizar contagem de p�ginas e produtos
		paginacao(produtos_por_pagina, &pagina_atual, paginas_total, &i, &sair);

		if (sair == 1) {
			return;
		}
	}

	system(PAUSAR);

	system(LIMPAR);
}

// Fun��o para listar os pre�os
void listarPrecos() {
	system(LIMPAR);

	TProduto aux;
	int produtos_por_pagina = 15;
	int pagina_atual = 1;
	int paginas_total = ceil((float)cont_produto / produtos_por_pagina);
	int sair = 0;

	// Organiza pelo pre�o
	for (int i = 0; i < cont_produto; i++) {
		for (int j = i + 1; j < cont_produto; j++) {
			if (produto[i].pr_venda > produto[j].pr_venda) {
				aux = produto[i];
				produto[i] = produto[j];
				produto[j] = aux;
			}
		}
	}

	for (int i = 0; i < cont_produto; i++) {
		if (i % produtos_por_pagina == 0) {
			printf("======================================= Listar pre�os ========================================\n\n");

			printf("==============================================================================================\n");
			printf("C�digo               Descri��o                                          Pre�o venda\n");
			printf("==============================================================================================\n");
		}

		// Exibir pre�os
		printf("%-20li %-50s R$ %.2f\n", produto[i].codigo, produto[i].descricao, produto[i].pr_venda);

		// Realizar contagem de p�ginas e produtos
		if ((i + 1) % produtos_por_pagina == 0 || (i + 1) == cont_produto) {
			printf("\n");
		}
		paginacao(produtos_por_pagina, &pagina_atual, paginas_total, &i, &sair);

		if (sair == 1) {
			// Reorganiza pelo c�digo
			for (int i = 0; i < cont_produto; i++) {
				for (int j = i + 1; j < cont_produto; j++) {
					if (produto[i].codigo > produto[j].codigo) {
						aux = produto[i];
						produto[i] = produto[j];
						produto[j] = aux;
					}
				}
			}

			return;
		}
	}

	system(PAUSAR);

	system(LIMPAR);
}

// Fun��o para exibir o menu de listar
void menuListar() {
	system(LIMPAR);

	if (cont_produto > 0) {
		int opcao;

		// Menu de op��es
		do {
			printf("========================================= Relat�rios =========================================\n");
			printf("<1>. Listar todos os produtos.\n");
			printf("<2>. Listar pre�os.\n");
			printf("<0>. Retornar ao menu principal.\n");
			printf("==============================================================================================\n\n");

			printf("Escolha uma op��o: ");
			scanf("%i", &opcao);

			switch (opcao) {
			case 1:
				listarProdutos();
				break;
			case 2:
				listarPrecos();
				break;
			case 0:
				system(LIMPAR);
				return;
				break;
			default:
				system(LIMPAR);
				printf(OPCAO_INVALIDA);
				break;
			}
		} while (opcao < 0 || opcao > 2);
	}
	else {
		printf(NENHUM_CADASTRO);

		system(PAUSAR);

		system(LIMPAR);
	}
}

// Fun��o para listar os produtos de um fornecedor
void listarProdutosFornecedor() {
	system(LIMPAR);

	printf("================================= Listar produtos fornecedor =================================\n\n");

	// Pedir o nome do fornecedor
	char fornecedor_busca[41];
	printf("Digite o nome do fornecedor: ");
	getchar();
	gets_s(fornecedor_busca);

	// Verificar se existe ao menos um produto com o nome deste fornecedor
	int total_produtos = 0;
	for (int i = 0; i < cont_produto; i++) {
		if (strcmp(fornecedor_busca, produto[i].fornecedor) == 0) {
			total_produtos++;
		}
	}

	// Listar os produtos
	if (total_produtos > 0) {
		system(LIMPAR);

		int cont = 0;
		int produtos_por_pagina = 15;
		int pagina_atual = 1;
		int paginas_total = ceil((float)total_produtos / produtos_por_pagina);
		char continuar;

		printf("================================= Listar produtos fornecedor =================================\n\n");

		printf("==============================================================================================\n");
		printf("C�digo               Descri��o                                 Fornecedor\n");
		printf("==============================================================================================\n");
		for (int i = 0; i < cont_produto; i++) {
			if (strcmp(fornecedor_busca, produto[i].fornecedor) == 0) {
				cont++;

				printf("%-20.13li %-41s %s\n", produto[i].codigo, produto[i].descricao, produto[i].fornecedor);

				// Exibir a contagem de p�ginas e produtos
				if (cont % produtos_por_pagina == 0 || cont == total_produtos) {
					printf("\nExibindo p�gina \033[32m<%i>\x1B[0m de \033[32m<%i>\x1B[0m. Total de produtos: \033[32m<%i>\x1B[0m!\n\n", pagina_atual, paginas_total, total_produtos);

					// Perguntar se deseja continuar
					if (pagina_atual < paginas_total) {
						do {
							printf("Deseja continuar? (S/N): ");
							scanf(" %c", &continuar);
						} while (toupper(continuar) != 'S' && toupper(continuar) != 'N');

						if (toupper(continuar) == 'S') {
							pagina_atual++;

							system(LIMPAR);

							printf("================================= Listar produtos fornecedor =================================\n\n");

							printf("==============================================================================================\n");
							printf("C�digo               Descri��o                                 Fornecedor\n");
							printf("==============================================================================================\n");
						}
						else {
							printf(CANCELADO_SUCESSO);
							break;
						}
					}
				}
			}
		}
	}
	else {
		printf(FORNECEDOR_NAO_ENCONTRADO);
	}

	system(PAUSAR);

	system(LIMPAR);
}

// Fun��o para realizar aumento ou desconto de um grupo de produtos
void movimentacaoProdutos() {
	system(LIMPAR);

	if (cont_produto > 0) {

		// Escolher se ser� realizado desconto ou aumento
		int opcao = 0;
		do {
			if (opcao < 0 || opcao > 2) {
				system(LIMPAR);
				printf(OPCAO_INVALIDA);
			}
			printf("======================================== Movimenta��o ========================================\n");
			printf("<1>. Realizar aumento.\n");
			printf("<2>. Realizar desconto.\n");
			printf("<0>. Retornar ao menu principal.\n");
			printf("==============================================================================================\n\n");

			printf("Escolha uma das op��es: ");
			scanf("%i", &opcao);
		} while (opcao < 0 || opcao > 2);

		if (opcao == 0) {
			system(LIMPAR);
			return;
		}

		// Escolher o grupo
		int grupo_busca;

		system(LIMPAR);

		printf("======================================== Movimenta��o ========================================\n\n");

		printf("============== Grupos ==============\n");
		printf("[1]. Bebidas\n");
		printf("[2]. Doces\n");
		printf("[3]. Salgadinhos\n");
		printf("[4]. Conveni�ncias\n\n");

		do {
			printf("Digite o grupo......................: ");
			scanf("%i", &grupo_busca);

			if (grupo_busca < 1 || grupo_busca > 4) {
				printf("\nO grupo n�o pode ser menor ou igual a 0.\n\n");
			}
		} while (grupo_busca < 1 || grupo_busca > 4);

		// Verificar se existe ao menos um produto cadastrado nesse grupo
		bool grupo_encontrado = false;
		for (int i = 0; i < cont_produto; i++) {
			if (grupo_busca == produto[i].grupo) {
				grupo_encontrado = true;

				break;
			}
		}

		// Pedir o porcentual a ser alterado
		if (grupo_encontrado) {
			float percentual;
			do {
				printf("\nDigite o percentual a ser alterado..: ");
				scanf("%f", &percentual);

				if (percentual <= 0) {
					printf("\nO percentual n�o pode ser menor ou igual a 0.\n");
				}
			} while (percentual <= 0);

			// Confirmar se deseja alterar
			printf("\n");
			char confirmar;
			do {
				printf("Confirma a altera��o no pre�o? (S/N): ");
				scanf(" %c", &confirmar);
			} while (toupper(confirmar) != 'S' && toupper(confirmar) != 'N');

			// Realizar a altera��o
			if (toupper(confirmar) == 'S') {
				// Aumento
				if (opcao == 1) {
					for (int i = 0; i < cont_produto; i++) {
						if (grupo_busca == produto[i].grupo) {
							produto[i].pr_venda += (produto[i].pr_venda / 100) * percentual;

							produto[i].lucro = ((produto[i].pr_venda - produto[i].pr_compra) * 100) / produto[i].pr_compra;
						}
					}
				}
				// Desconto
				else {
					for (int i = 0; i < cont_produto; i++) {
						if (grupo_busca == produto[i].grupo) {
							produto[i].pr_venda -= (produto[i].pr_venda / 100) * percentual;

							produto[i].lucro = ((produto[i].pr_venda - produto[i].pr_compra) * 100) / produto[i].pr_compra;
						}
					}
				}

				// Salvar na base dados
				escreverArquivo();
				printf(ALTERADO_SUCESSO);
			}
			else {
				printf(CANCELADO_SUCESSO);
			}
		}
		else {
			printf(PRODUTO_NAO_ENCONTRADO);
		}
	}
	else {
		printf(NENHUM_CADASTRO);

	}
	system(PAUSAR);

	system(LIMPAR);
}

// Fun��o principal
int main() {
	setlocale(LC_ALL, "portuguese");

	lerArquivo();

	int opcao;

	// Menu de op��es
	do {
		printf("===================================== Sistema de Estoque =====================================\n");
		printf("<1>. Adicionar.\n");
		printf("<2>. Alterar.\n");
		printf("<3>. Buscar.\n");
		printf("<4>. Excluir.\n");
		printf("<5>. Relat�rios.\n");
		printf("<6>. Relat�rio especial.\n");
		printf("<7>. Movimenta��o.\n");
		printf("<0>. Sair do programa.\n");
		printf("==============================================================================================\n");

		printf("\nEscolha uma op��o: ");
		scanf("%i", &opcao);

		switch (opcao) {
		case 1:
			adicionarProduto();
			break;
		case 2:
			alterarDados();
			break;
		case 3:
			menuBusca();
			break;
		case 4:
			excluirProduto();
			break;
		case 5:
			menuListar();
			break;
		case 6:
			listarProdutosFornecedor();
			break;
		case 7:
			movimentacaoProdutos();
			break;
		case 0:
			printf(SAIR_PROGRAMA);
			exit(0);
			break;
		default:
			system(LIMPAR);
			printf(OPCAO_INVALIDA);
			break;
		}
	} while (opcao != 0);

	return 0;
}
