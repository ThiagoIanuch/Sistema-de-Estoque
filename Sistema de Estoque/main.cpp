#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <locale.h>

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

// Vari�veis globais e estrutura do produto
const int MAX_PRODUTO = 50;

typedef struct TProduto {
	long long int codigo;
	int grupo;
	char descricao[41];
	char unidade[3];
	char fornecedor[41];
	int quantidade;
	float pr_compra;
	float pr_venda;
	float lucro;
	int estoque_min;
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

// Fun��o para adicionar os produtos no sistema
void adicionarProduto() {
	system(LIMPAR);

	if (cont_produto < MAX_PRODUTO) {

		TProduto aux;
		bool verificar_codigo = false;

		// Pedir o c�digo e verificar se j� foi cadastrado
		do {
			printf("Digite o c�digo..............: ");
			scanf("%lli", &aux.codigo);

			for (int i = 0; i < cont_produto; i++) {
				if (aux.codigo == produto[i].codigo) {
					verificar_codigo = true;

					break;
				}
			}
		} while (aux.codigo <= 0);

		if (!verificar_codigo) {
			// Pedir os outros dados do produto
			printf("\n----------- Grupos -----------\n");
			printf("1. Bebidas....................\n");
			printf("2. Doces......................\n");
			printf("3. Salgadinhos................\n");
			printf("4. Conveni�ncias..............\n");
			printf("------------------------------\n\n");

			do {
				printf("Digite o grupo...............: ");
				scanf("%i", &aux.grupo);
			} while (aux.grupo < 1 || aux.grupo > 4);

			printf("\n");

			printf("Digite a descri��o...........: ");
			getchar();
			gets_s(aux.descricao);

			printf("\n");

			printf("Digite a unidade.............: ");
			gets_s(aux.unidade);

			printf("\n");

			printf("Digite o fornecedor..........: ");
			gets_s(aux.fornecedor);

			printf("\n");

			do {
				printf("Digite a quantidade..........: ");
				scanf("%i", &aux.quantidade);
			} while (aux.quantidade < 0);

			printf("\n");

			do {
				printf("Digite o pre�o de compra.....: R$ ");
				scanf("%f", &aux.pr_compra);
			} while (aux.pr_compra < 0);

			// Pre�o de venda e lucro autom�ticos
			aux.pr_venda = aux.pr_compra + (aux.pr_compra / 100) * percentual_venda;

			aux.lucro = aux.pr_venda - aux.pr_compra;

			printf("\n");

			do {
				printf("Digite a quantidade m�nima...: ");
				scanf("%i", &aux.estoque_min);
			} while (aux.estoque_min < 0);

			// Realizar confirma��o
			char confirmar;
			printf("\n");

			do {
				printf("Confirma que deseja cadastrar? (S/N): ");
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
		// Pedir o c�digo
		long long int codigo_busca;
		printf("Digite o c�digo do produto que deseja alterar algum dado: ");
		scanf("%lli", &codigo_busca);

		// Procurar pelo c�digo
		bool produto_encontrado = false;
		for (int i = 0; i < cont_produto; i++) {
			if (codigo_busca == produto[i].codigo) {

				int opcao;
				produto_encontrado = true;
				TProduto aux = produto[i];

				system(LIMPAR);

				do {
					// Listar as op��es
					printf("====================================================\n");
					printf("<1>. Alterar o grupo\n");
					printf("<2>. Alterar a descri��o\n");
					printf("<3>. Alterar a unidade\n");
					printf("<4>. Alterar o fornecedor\n");
					printf("<5>. Alterar a quantidade\n");
					printf("<6>. Alterar o pre�o de compra\n");
					printf("<7>. Alterar o estoque m�nimo.\n");
					printf("<0>. Retornar ao menu principal\n");
					printf("====================================================\n\n");

					// Exibir os dados do produto
					printf("C�digo..........: %.13lli\n", aux.codigo);
					printf("Grupo...........: %i\n", aux.grupo);
					printf("Descri��o.......: %s\n", aux.descricao);
					printf("Unidade.. ......: %s\n", aux.unidade);
					printf("Fornecedor......: %s\n", aux.fornecedor);
					printf("Quantidade......: %i\n", aux.quantidade);
					printf("Pre�o compra....: R$ %.2f\n", aux.pr_compra);
					printf("Pre�o venda.....: R$ %.2f\n", aux.pr_venda);
					printf("Lucro...........: R$ %.2f\n", aux.lucro);
					printf("Quantidade m�n..: %i\n\n", aux.estoque_min);

					// Pedir a op��o
					printf("Escolha uma das op��es: ");
					scanf("%i", &opcao);

					switch (opcao) {
					case 1:
						printf("\n----------- Grupos -----------\n");
						printf("1. Bebidas....................\n");
						printf("2. Doces......................\n");
						printf("3. Salgadinhos................\n");
						printf("4. Conveni�ncias..............\n");
						printf("------------------------------\n\n");
						do {
							printf("Digite o novo grupo: ");
							scanf("%i", &aux.grupo);
						} while (aux.grupo < 1 || aux.grupo > 4);
						break;
					case 2:
						printf("\nDigite a nova descri��o: ");
						getchar();
						gets_s(aux.descricao);
						break;
					case 3:
						printf("\nDigite a nova unidade: ");
						getchar();
						gets_s(aux.unidade);
						break;
					case 4:
						printf("\nDigite o novo fornecedor: ");
						getchar();
						gets_s(aux.fornecedor);
						break;
					case 5:
						do {
							printf("\nDigite a nova quantidade: ");
							scanf("%i", &aux.quantidade);
						} while (aux.quantidade < 0);
						break;
					case 6:
						do {
							printf("\nDigite o novo pre�o de compra: R$ ");
							scanf("%f", &aux.pr_compra);
						} while (aux.pr_compra < 0);

						aux.pr_venda = aux.pr_compra + (aux.pr_compra / 100) * percentual_venda;
						aux.lucro = aux.pr_venda - aux.pr_compra;
						break;
						break;
					case 7:
						do {
							printf("\nDigite a nova quantidade m�nima: ");
							scanf("%i", &aux.estoque_min);
						} while (aux.estoque_min < 0);
						break;
					case 0:
						system(LIMPAR);
						return;
						break;
					default:
						printf(OPCAO_INVALIDA);
					}
				} while (opcao < 0 || opcao > 7);

				// Confirmar altera��o
				char confirmar;
				do {
					printf("\nConfirma que os dados a serem alterados est�o corretos? (S/N): ");
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
		// Pedir o c�digo
		long long int codigo_deletar;
		printf("Digite o c�digo do produto a ser deletado: ");
		scanf("%lli", &codigo_deletar);

		// Procurar pelo produto
		bool produto_encontrado = false;
		for (int i = 0; i < cont_produto; i++) {
			if (codigo_deletar == produto[i].codigo) {
				produto_encontrado = true;

				// Confirmar se deseja deletar
				printf("\nO c�digo do produto a ser deletado � \033[32m%.13lli\x1B[0m e a descri��o � \033[32m%s\x1B[0m!\n\n", produto[i].codigo, produto[i].descricao);
				char confirmar;
				do {
					printf("Deseja deletar este produto? (S/N): ");
					scanf(" %c", &confirmar);
				} while (toupper(confirmar) != 'S' && toupper(confirmar) != 'N');


				// Deletar o produto
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

	// Pedir o c�digo a ser buscado
	long long int codigo_busca;
	printf("Digite o c�digo do produto a ser buscado: ");
	scanf("%lli", &codigo_busca);

	// Realizar a busca
	bool produto_encontrado = false;
	for (int i = 0; i < cont_produto; i++) {
		if (codigo_busca == produto[i].codigo) {
			printf("\nC�digo..........: %.13lli\n", produto[i].codigo);
			printf("Grupo...........: %i\n", produto[i].grupo);
			printf("Descri��o.......: %s\n", produto[i].descricao);
			printf("Unidade.. ......: %s\n", produto[i].unidade);
			printf("Fornecedor......: %s\n", produto[i].fornecedor);
			printf("Quantidade......: %i\n", produto[i].quantidade);
			printf("Pre�o compra....: R$ %.2f\n", produto[i].pr_compra);
			printf("Pre�o venda.....: R$ %.2f\n", produto[i].pr_venda);
			printf("Lucro...........: R$ %.2f\n", produto[i].lucro);
			printf("Quantidade m�n..: %i\n\n", produto[i].estoque_min);

			produto_encontrado = true;
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

	// Pedir a descri��o a ser buscada
	char descricao_busca[50];
	printf("Digite a descri��o do produto a ser buscado: ");
	getchar();
	gets_s(descricao_busca);

	// Realizar a busca
	bool produto_encontrado = false;
	int cont_encontrados = 0;
	for (int i = 0; i < cont_produto; i++) {
		if (strstr(produto[i].descricao, descricao_busca)) {
			printf("\nC�digo..........: %.13lli\n", produto[i].codigo);
			printf("Grupo...........: %i\n", produto[i].grupo);
			printf("Descri��o.......: %s\n", produto[i].descricao);
			printf("Unidade.. ......: %s\n", produto[i].unidade);
			printf("Fornecedor......: %s\n", produto[i].fornecedor);
			printf("Quantidade......: %i\n", produto[i].quantidade);
			printf("Pre�o compra....: R$ %.2f\n", produto[i].pr_compra);
			printf("Pre�o venda.....: R$ %.2f\n", produto[i].pr_venda);
			printf("Lucro...........: R$ %.2f\n", produto[i].lucro);
			printf("Quantidade m�n..: %i\n\n", produto[i].estoque_min);

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
			printf("====================================================\n");
			printf("<1>. Buscar pelo c�digo.\n");
			printf("<2>. Buscar pela descri��o.\n");
			printf("<0>. Retornar ao menu principal.\n");
			printf("====================================================\n");

			printf("\nEscolha uma op��o: ");
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
	int paginas_total = ceil((float)cont_produto / produtos_por_pagina);
	int pagina_atual = 1;
	char continuar;

	for (int i = 0; i < cont_produto; i++) {
		// Informa��es do produto
		printf("=============================================================================================\n");
		printf("C�digo: %-53.13lli Grupo: %i\n", produto[i].codigo, produto[i].grupo);
		printf("Descri��o: %-50s Unidade: %s\n", produto[i].descricao, produto[i].unidade);
		printf("Fornecedor: %s\n", produto[i].fornecedor);
		printf("Pre�o de compra: R$ %-10.2f Pre�o de venda: R$ %-11.2f Lucro: R$ %.2f\n", produto[i].pr_compra, produto[i].pr_venda, produto[i].lucro);
		printf("Quantidade em estoque: %-38i Quantidade m�nima: %i\n", produto[i].quantidade, produto[i].estoque_min);
		printf("=============================================================================================\n\n");

		if ((i + 1) % produtos_por_pagina == 0 || (i + 1) == cont_produto) {
			// Exibir a contagem de p�ginas e produtos
			printf("Exibindo p�gina \033[32m<%i>\x1B[0m de \033[32m<%i>\x1B[0m. Total de produtos: \033[32m<%i>\x1B[0m!\n\n", pagina_atual, paginas_total, cont_produto);

			// Perguntar se deseja continuar
			if (pagina_atual < paginas_total) {
				do {
					printf("Deseja continuar? (S/N): ");
					scanf(" %c", &continuar);
				} while (toupper(continuar) != 'S' && toupper(continuar) != 'N');

				if (toupper(continuar) == 'S') {
					pagina_atual++;
					system(LIMPAR);
				}
				else {
					printf(CANCELADO_SUCESSO);
					break;
				}
			}
		}
	}

	system(PAUSAR);

	system(LIMPAR);
}

// Fun��o para listar os pre�os
void listarPrecos() {
	system(LIMPAR);

	int produtos_por_pagina = 15;
	int paginas_total = ceil((float)cont_produto / produtos_por_pagina);
	int pagina_atual = 1;
	char continuar;

	printf("=============================================================================================\n");
	printf("C�digo               Descri��o                                          Pre�o venda\n");
	printf("=============================================================================================\n");

	for (int i = 0; i < cont_produto; i++) {
		// Exibir pre�os
		printf("%-20.13lli %-50s R$ %.2f\n", produto[i].codigo, produto[i].descricao, produto[i].pr_venda);

		// Exibir a contagem de p�ginas e produtos
		if ((i + 1) % produtos_por_pagina == 0 || (i + 1) == cont_produto) {
			printf("\nExibindo p�gina \033[32m<%i>\x1B[0m de \033[32m<%i>\x1B[0m. Total de produtos: \033[32m<%i>\x1B[0m!\n\n", pagina_atual, paginas_total, cont_produto);

			// Perguntar se deseja continuar
			if (pagina_atual < paginas_total) {
				do {
					printf("Deseja continuar? (S/N): ");
					scanf(" %c", &continuar);
				} while (toupper(continuar) != 'S' && toupper(continuar) != 'N');

				if (toupper(continuar) == 'S') {
					pagina_atual++;

					system(LIMPAR);

					printf("=============================================================================================\n");
					printf("C�digo               Descri��o                                          Pre�o venda\n");
					printf("=============================================================================================\n");
				}
				else {
					printf(CANCELADO_SUCESSO);
					break;
				}
			}
		}
	}

	system(PAUSAR);

	system(LIMPAR);
}

// Fun��o para listar os produtos de um fornecedor
void listarProdutosFornecedor() {
	system(LIMPAR);

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
		int paginas_total = ceil((float)total_produtos / produtos_por_pagina);
		int pagina_atual = 1;
		char continuar;

		printf("====================================================================================\n");;
		printf("C�digo               Descri��o                                 Fornecedor\n");
		printf("====================================================================================\n");;
		for (int i = 0; i < cont_produto; i++) {
			if (strcmp(fornecedor_busca, produto[i].fornecedor) == 0) {
				cont++;

				printf("%-20.13lli %-41s %s\n", produto[i].codigo, produto[i].descricao, produto[i].fornecedor);

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

							printf("====================================================================================\n");;
							printf("C�digo               Descri��o                                 Fornecedor\n");
							printf("====================================================================================\n");;
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

// Fun��o para exibir o menu de listar
void menuListar() {
	system(LIMPAR);

	if (cont_produto > 0) {
		int opcao;

		// Menu de op��es
		do {
			printf("====================================================\n");
			printf("<1>. Listar pre�os.\n");
			printf("<2>. Listar produtos de um fornecedor.\n");
			printf("<3>. Listar todos os produtos.\n");
			printf("<0>. Retornar ao menu principal.\n");
			printf("====================================================\n");

			printf("\nEscolha uma op��o: ");
			scanf("%i", &opcao);

			switch (opcao) {
			case 1:
				listarPrecos();
				break;
			case 2:
				listarProdutosFornecedor();
				break;
			case 3:
				listarProdutos();
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
		} while (opcao < 0 || opcao > 3);
	}
	else {
		printf(NENHUM_CADASTRO);

		system(PAUSAR);

		system(LIMPAR);
	}
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
			printf("====================================================\n");
			printf("<1>. Realizar aumento.\n");
			printf("<2>. Realizar desconto.\n");
			printf("<0>. Retornar ao menu principal.\n");
			printf("====================================================\n\n");

			printf("Escolha uma das op��es: ");
			scanf("%i", &opcao);
		} while (opcao < 0 || opcao > 2);

		if (opcao == 0) {
			system(LIMPAR);
			return;
		}

		// Escolher o grupo
		int grupo_escolha;

		system(LIMPAR);

		printf("----------- Grupos -----------\n");
		printf("1. Bebidas....................\n");
		printf("2. Doces......................\n");
		printf("3. Salgadinhos................\n");
		printf("4. Conveni�ncias..............\n");
		printf("------------------------------\n\n");

		do {
			printf("Digite o grupo...............: ");
			scanf("%i", &grupo_escolha);
		} while (grupo_escolha < 1 || grupo_escolha > 4);

		// Verificar se existe ao menos um produto cadastrado nesse grupo
		bool grupo_encontrado = false;
		for (int i = 0; i < cont_produto; i++) {
			if (grupo_escolha == produto[i].grupo) {
				grupo_encontrado = true;

				break;
			}
		}

		// Pedir o porcentual a ser alterado
		if (grupo_encontrado) {
			float percentual;
			do {
				printf("\nDigite o percentual a ser alterado...: ");
				scanf("%f", &percentual);
			} while (percentual <= 0);

			// Confirmar se deseja alterar
			printf("\n");
			char confirmar;
			do {
				printf("Deseja realizar a altera��o no pre�o? (S/N): ");
				scanf(" %c", &confirmar);
			} while (toupper(confirmar) != 'S' && toupper(confirmar) != 'N');

			// Realizar a altera��o
			if (toupper(confirmar) == 'S') {
				// Aumento
				if (opcao == 1) {
					for (int i = 0; i < cont_produto; i++) {
						if (grupo_escolha == produto[i].grupo) {
							produto[i].pr_venda += (produto[i].pr_venda / 100) * percentual;

							produto[i].lucro = produto[i].pr_venda - produto[i].pr_compra;
						}
					}
				}
				// Desconto
				else {
					for (int i = 0; i < cont_produto; i++) {
						if (grupo_escolha == produto[i].grupo) {
							produto[i].pr_venda -= (produto[i].pr_venda / 100) * percentual;

							produto[i].lucro = produto[i].pr_venda - produto[i].pr_compra;
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
		printf("================ Sistema de Estoque ================\n");
		printf("<1>. Adicionar produto.\n");
		printf("<2>. Alterar dados de um produto.\n");
		printf("<3>. Buscar produto.\n");
		printf("<4>. Excluir produto.\n");
		printf("<5>. Movimenta��o.\n");
		printf("<6>. Relat�rios.\n");
		printf("<0>. Sair do programa.\n");
		printf("====================================================\n");

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
			movimentacaoProdutos();
			break;
		case 6:
			menuListar();
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