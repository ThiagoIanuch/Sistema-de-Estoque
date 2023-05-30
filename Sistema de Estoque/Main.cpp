#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <math.h>

// Definções de mensagens e outros
#define SAIR_PROGRAMA "\n\033[31mPrograma encerrado com sucesso!\x1B[0m\n"
#define OPCAO_INVALIDA "\033[31mOpção inválida. Escolha novamente!\x1B[0m\n\n"
#define NENHUM_CADASTRO "\033[31mNenhum produto foi cadastrado no sistema!\x1B[0m\n\n"
#define QUANTIA_MAXIMA "\033[31mNão é possível adicionar mais produtos, a quantia máxima já foi cadastrada!\x1B[0m\n\n"
#define PRODUTO_EXISTENTE "\033[31m\nEste produto já foi cadastrado no sistema!\x1B[0m\n\n"
#define PRODUTO_NAO_ENCONTRADO "\n\033[31mProduto não encontrado!\x1B[0m\n\n"
#define GRUPO_NAO_ENCONTRADO "\n\033[31mGrupo não encontrado!\x1B[0m\n\n"
#define FORNECEDOR_NAO_ENCONTRADO "\n\033[31mFornecedor não encontrado!\x1B[0m\n\n"
#define CANCELADO_SUCESSO "\n\033[31mOperação cancelada com sucesso!\x1B[0m\n\n"
#define CADASTRO_SUCESSO "\n\033[32mProduto cadastrado com sucesso!\x1B[0m\n\n"
#define DELETADO_SUCESSO "\n\033[32mProduto deletado com sucesso!\x1B[0m\n\n"
#define ALTERADO_SUCESSO "\n\033[32mAlteração realizada com sucesso!\x1B[0m\n\n"
#define PAUSAR "pause"
#define LIMPAR "cls"

// Estrutura do produto e variáveis globais
const int MAX_PRODUTO = 50;
int cont_produto;

typedef struct TProduto {
	long long int codigo;
	char grupo[50];
	char descricao[50];
	char unidade[3];
	char fornecedor[50];
	int quantidade;
	float pr_compra;
	float pr_venda;
	float lucro;
	int estoque_min;
}TProduto;

TProduto produto[MAX_PRODUTO];

FILE* arquivo;

// Função para ler os dados do arquivo
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

// Função para escrever os dados no arquivo
void escreverArquivo() {
	arquivo = fopen("estoque.dat", "w+b");

	if (arquivo != NULL) {
		fwrite(&cont_produto, sizeof(cont_produto), 1, arquivo);

		TProduto aux;

		// Ao gravar os dados no arquivo já deixa organizado pelo código
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
}

// Função para adicionar os produtos no sistema
void adicionarProduto() {
	system(LIMPAR);

	if (cont_produto < MAX_PRODUTO) {

		bool verificar_codigo = false;

		// Pedir o código e verificar se já foi cadastrado
		do {
			printf("Digite o código..............: ");
			scanf("%lli", &produto[cont_produto].codigo);

			for (int i = 0; i < cont_produto; i++) {
				if (produto[cont_produto].codigo == produto[i].codigo) {
					verificar_codigo = true;

					break;
				}
			}
		} while (produto[cont_produto].codigo <= 0);

		if (!verificar_codigo) {
			// Pedir os outros dados do produto
			printf("Digite o grupo...............: ");
			getchar();
			gets_s(produto[cont_produto].grupo);

			printf("Digite a descrição...........: ");
			gets_s(produto[cont_produto].descricao);

			printf("Digite a unidade.............: ");
			gets_s(produto[cont_produto].unidade);

			printf("Digite o fornecedor..........: ");
			gets_s(produto[cont_produto].fornecedor);

			do {
				printf("Digite a quantidade..........: ");
				scanf("%i", &produto[cont_produto].quantidade);
			} while (produto[cont_produto].quantidade < 0);

			do {
				printf("Digite o preço de compra.....: R$ ");
				scanf("%f", &produto[cont_produto].pr_compra);
			} while (produto[cont_produto].pr_compra < 0);

			do {
				printf("Digite o preço de venda......: R$ ");
				scanf("%f", &produto[cont_produto].pr_venda);
			} while (produto[cont_produto].pr_venda < 0);

			produto[cont_produto].lucro = produto[cont_produto].pr_venda - produto[cont_produto].pr_compra;

			do {
				printf("Digite a quantidade mínima...: ");
				scanf("%i", &produto[cont_produto].estoque_min);
			} while (produto[cont_produto].estoque_min < 0);

			// Realizar confirmação
			char confirmar;
			printf("\n");
			do {
				printf("Deseja cadastrar este produto no sistema? (S/N): ");
				scanf(" %c", &confirmar);
			} while (toupper(confirmar) != 'S' && toupper(confirmar) != 'N');

			// Cadastrar no sistema
			if (toupper(confirmar) == 'S') {
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

void alterarDados() {
	system(LIMPAR);
	
	if (cont_produto > 0) {
		// Pedir o código
		long long int codigo_busca;
		printf("Digite o código do produto que deseja alterar algum dado: ");
		scanf("%lli", &codigo_busca);

		// Verificar
		bool produto_encontrado = false;
		for (int i = 0; i < cont_produto; i++) {
			if (codigo_busca == produto[i].codigo) {
				produto_encontrado = true;

				system(LIMPAR);

				// Opção de escolha do dado a ser alterado
				int opcao;

				do {
					printf("O código do produto a ter os dados alterados é \033[32m%.13lli\x1B[0m!\n\n", produto[i].codigo, produto[i].descricao);

					// Exibir os dados do produto
					printf("Grupo...........: %s\n", produto[i].grupo);
					printf("Descrição.......: %s\n", produto[i].descricao);
					printf("Unidade.. ......: %s\n", produto[i].unidade);
					printf("Fornecedor......: %s\n", produto[i].fornecedor);
					printf("Quantidade......: %i\n", produto[i].quantidade);
					printf("Preço compra....: R$ %.2f\n", produto[i].pr_compra);
					printf("Preço venda.....: R$ %.2f\n", produto[i].pr_venda);
					printf("Lucro...........: R$ %.2f\n", produto[i].lucro);
					printf("Quantidade mín..: %i\n\n", produto[i].estoque_min);

					// Listar as opções
					printf("<01>. Alterar o grupo\n");
					printf("<02>. Alterar a descrição\n");
					printf("<03>. Alterar a unidade\n");
					printf("<04>. Alterar o fornecedor\n");
					printf("<05>. Alterar a quantidade\n");
					printf("<06>. Alterar o preço de compra\n");
					printf("<07>. Alterar o preço de venda\n");
					printf("<08>. Alterar o estoque mínimo.\n");
					printf("<09>. Cancelar\n\n");

					// Pedir a opção
					printf("Escolha uma das opções: ");
					scanf("%i", &opcao);

					switch (opcao) {
					case 1:
						printf("\nDigite o novo grupo: ");
						getchar();
						gets_s(produto[i].grupo);
						break;
					case 2:
						printf("\nDigite a nova descrição: ");
						getchar();
						gets_s(produto[i].descricao);
						break;
					case 3:
						printf("\nDigite a nova unidade: ");
						getchar();
						gets_s(produto[i].unidade);
						break;
					case 4:
						printf("\nDigite o novo fornecedor: ");
						getchar();
						gets_s(produto[i].fornecedor);
						break;
					case 5:
						do {
							printf("\nDigite a nova quantidade: ");
							scanf("%i", &produto[i].quantidade);
						} while (produto[i].quantidade < 0);
						break;
					case 6:
						do {
							printf("\nDigite o novo preço de compra: ");
							scanf("%f", &produto[i].pr_compra);
						} while (produto[i].pr_compra < 0);
						break;
					case 7:
						do {
							printf("\nDigite o novo preço de venda: ");
							scanf("%f", &produto[i].pr_venda);
						} while (produto[i].pr_venda < 0);
						break;
					case 8:
						do {
							printf("\nDigite a nova quantidade mínima: ");
							scanf("%i", &produto[i].estoque_min);
						} while (produto[i].estoque_min < 0);
						break;
					case 9:
						printf(CANCELADO_SUCESSO);
						break;
					default:
						system(LIMPAR);
						printf(OPCAO_INVALIDA);
					}
				} while (opcao < 1 || opcao > 9);

				if (opcao != 9) {
					escreverArquivo();
					printf(ALTERADO_SUCESSO);
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

// Função para excluir produtos do sistema
void excluirProduto() {
	system(LIMPAR);

	if (cont_produto > 0) {
		// Pedir o código
		long long int codigo_deletar;
		printf("Digite o código do produto a ser deletado: ");
		scanf("%lli", &codigo_deletar);

		// Realizar a verificação
		bool produto_encontrado = false;
		for (int i = 0; i < cont_produto; i++) {
			if (codigo_deletar == produto[i].codigo) {
				produto_encontrado = true;

				// Confirmar se deseja deletar
				printf("\nO código do produto a ser deletado é \033[32m%.13lli\x1B[0m e a descrição é \033[32m%s\x1B[0m!\n\n", produto[i].codigo, produto[i].descricao);
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

// Função para buscar o produto pelo codigo
void buscarProdutoCodigo() {
	system(LIMPAR);

	if (cont_produto > 0) {
		// Pedir o código a ser buscado
		long long int codigo_busca;
		printf("Digite o código do produto a ser buscado: ");
		scanf("%lli", &codigo_busca);

		// Realizar a busca
		bool produto_encontrado = false;
		for (int i = 0; i < cont_produto; i++) {
			if (codigo_busca == produto[i].codigo) {
				produto_encontrado = true;

				printf("\nCódigo..........: %.13lli\n", produto[i].codigo);
				printf("Grupo...........: %s\n", produto[i].grupo);
				printf("Descrição.......: %s\n", produto[i].descricao);
				printf("Unidade.. ......: %s\n", produto[i].unidade);
				printf("Fornecedor......: %s\n", produto[i].fornecedor);
				printf("Quantidade......: %i\n", produto[i].quantidade);
				printf("Preço compra....: R$ %.2f\n", produto[i].pr_compra);
				printf("Preço venda.....: R$ %.2f\n", produto[i].pr_venda);
				printf("Lucro...........: R$ %.2f\n", produto[i].lucro);
				printf("Quantidade mín..: %i\n\n", produto[i].estoque_min);

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

// Função para buscar o produto pela descrição
void buscarProdutoDescricao() {
	system(LIMPAR);

	if (cont_produto > 0) {

		// Pedir a descrição a ser buscada
		char descricao_busca[50];
		printf("Digite a descrição do produto a ser buscado: ");
		getchar();
		gets_s(descricao_busca);

		// Realizar a busca
		bool produto_encontrado = false;
		int cont_encontrados = 0;
		for (int i = 0; i < cont_produto; i++) {
			if (strstr(produto[i].descricao, descricao_busca)) {
				produto_encontrado = true;
				cont_encontrados++;

				printf("\nCódigo..........: %.13lli\n", produto[i].codigo);
				printf("Grupo...........: %s\n", produto[i].grupo);
				printf("Descrição.......: %s\n", produto[i].descricao);
				printf("Unidade.. ......: %s\n", produto[i].unidade);
				printf("Fornecedor......: %s\n", produto[i].fornecedor);
				printf("Quantidade......: %i\n", produto[i].quantidade);
				printf("Preço compra....: R$ %.2f\n", produto[i].pr_compra);
				printf("Preço venda.....: R$ %.2f\n", produto[i].pr_venda);
				printf("Lucro...........: R$ %.2f\n", produto[i].lucro);
				printf("Quantidade mín..: %i\n\n", produto[i].estoque_min);
			}
		}

		// Exibir o total encontrado
		if (produto_encontrado) {
			printf("Total de produtos encontrados: \033[32m%i\x1B[0m!\n\n", cont_encontrados);
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

// Função para listar os produtos do sistema
void listarProdutos() {
	system(LIMPAR);

	if (cont_produto > 0) {
		int produtos_por_pagina = 2;
		int paginas_total = ceil((float)cont_produto / produtos_por_pagina);
		int pagina_atual = 1;
		char continuar;

		for (int i = 0; i < cont_produto; i++) {
			// Informações do produto
			printf("=============================================================================================\n");
			printf("Código: %-53.13lli Grupo: %s\n", produto[i].codigo, produto[i].grupo);
			printf("Descrição: %-50s Unidade: %s\n", produto[i].descricao, produto[i].unidade);
			printf("Fornecedor: %s\n", produto[i].fornecedor);
			printf("Preço de compra: R$ %-10.2f Preço de venda: R$ %-11.2f Lucro: R$ %.2f\n", produto[i].pr_compra, produto[i].pr_venda, produto[i].lucro);
			printf("Quantidade em estoque: %-38i Quantidade mínima: %i\n", produto[i].quantidade, produto[i].estoque_min);
			printf("=============================================================================================\n\n");

			if ((i + 1) % produtos_por_pagina == 0 || (i + 1) == cont_produto) {
				// Exibir a contagem de páginas e produtos
				printf("Exibindo página \033[32m<%i>\x1B[0m de \033[32m<%i>\x1B[0m. Total de produtos: \033[32m<%i>\x1B[0m!\n\n", pagina_atual, paginas_total, cont_produto);

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
	}
	else {
		printf(NENHUM_CADASTRO);
	}

	system(PAUSAR);

	system(LIMPAR);
}

// Função para listar os preços
void listarPrecos() {
	system(LIMPAR);

	if (cont_produto > 0) {
		int opcao = 1;

		do {
			if (opcao < 1 || opcao > 3) {
				system(LIMPAR);
				printf(OPCAO_INVALIDA);
			}
			printf("<1>. Listar os preços em ordem crescente\n");
			printf("<2>. Listar os preços em ordem decrescente\n");
			printf("<3>. Retornar ao menu principal\n\n");

			printf("Escolha uma das opções: ");
			scanf("%i", &opcao);
		} while (opcao < 1 || opcao > 3);

		// Retornar ao menu se a opção for 3
		if (opcao == 3) {
			system(LIMPAR);
			return;
		}
			
		system(LIMPAR);

		int produtos_por_pagina = 15;
		int paginas_total = ceil((float)cont_produto / produtos_por_pagina);
		int pagina_atual = 1;
		char continuar;
		TProduto aux;

		printf("=============================================================================================\n");
		printf("Código               Descrição                                          Preço venda\n");
		printf("=============================================================================================\n");

		for (int i = 0; i < cont_produto; i++) {
			// Organizar pelo preço em ordem crescente
			if (opcao == 1) {
				for (int j = i + 1; j < cont_produto; j++) {
					if (produto[i].pr_venda > produto[j].pr_venda) {
						aux = produto[i];
						produto[i] = produto[j];
						produto[j] = aux;
					}
				}
			}
			// Organizar pelo preço em ordem decrescente
			else {
				for (int j = i + 1; j < cont_produto; j++) {
					if (produto[i].pr_venda < produto[j].pr_venda) {
						aux = produto[i];
						produto[i] = produto[j];
						produto[j] = aux;
					}
				}
			}

			// Exibir preços
			printf("%-20.13lli %-50s R$ %.2f\n", produto[i].codigo, produto[i].descricao, produto[i].pr_venda);

			// Exibir a contagem de páginas e produtos
			if ((i + 1) % produtos_por_pagina == 0 || (i + 1) == cont_produto) {
				printf("\nExibindo página \033[32m<%i>\x1B[0m de \033[32m<%i>\x1B[0m. Total de produtos: \033[32m<%i>\x1B[0m!\n\n", pagina_atual, paginas_total, cont_produto);

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
						printf("Código               Descrição                                          Preço venda\n");
						printf("=============================================================================================\n");
					}
					else {
						printf(CANCELADO_SUCESSO);
						break;
					}
				}
			}
		}

		// Reorganizar pelo código
		for (int i = 0; i < cont_produto; i++) {
			for (int j = i + 1; j < cont_produto; j++) {
				if (produto[i].codigo > produto[j].codigo) {
					aux = produto[i];
					produto[i] = produto[j];
					produto[j] = aux;
				}
			}
		}
	}
	else {
		printf(NENHUM_CADASTRO);
	}

	system(PAUSAR);

	system(LIMPAR);
}

// Função para realizar aumento ou desconto de um grupo de produtos
void movimentacaoProdutos() {
	system(LIMPAR);

	if (cont_produto > 0) {
		// Pedir o grupo
		char grupo_busca[50];
		printf("Digite o grupo do produto............: ");
		getchar();
		gets_s(grupo_busca);

		// Verificar se o grupo existe 
		bool grupo_encontrado = false;
		for (int i = 0; i < cont_produto; i++) {
			if (strcmp(grupo_busca, produto[i].grupo) == 0) {
				grupo_encontrado = true;

				break;
			}
		}

		if (grupo_encontrado) {
			// Perguntar o percentual
			float percentual;
			printf("\nDigite o percentual a ser alterado...: ");
			scanf("%f", &percentual);

			system(LIMPAR);

			int opcao;
			do {
				// Escolher se será realizado desconto ou aumento
				printf("O grupo a sofrer alteração no preço será \033[32m%s\x1B[0m e o percentual será de \033[32m%.2f%%\x1B[0m\n\n", grupo_busca, percentual);
				printf("<01>. Realizar desconto\n");
				printf("<02>. Realizar aumento\n");
				printf("<03>. Cancelar\n\n");

				printf("Escolha uma das opções: ");
				scanf("%i", &opcao);

				switch (opcao) {
				// Desconto
				case 1:
					for (int i = 0; i < cont_produto; i++) {
						if (strcmp(grupo_busca, produto[i].grupo) == 0) {
							produto[i].pr_venda -= (produto[i].pr_venda / 100) * percentual;
							produto[i].lucro = produto[i].pr_venda - produto[i].pr_compra;
						}
					}
					escreverArquivo();
					printf(ALTERADO_SUCESSO);
					break;
				// Aumento
				case 2:
					for (int i = 0; i < cont_produto; i++) {
						if (strcmp(grupo_busca, produto[i].grupo) == 0) {
							produto[i].pr_venda += (produto[i].pr_venda / 100) * percentual;
							produto[i].lucro = produto[i].pr_venda - produto[i].pr_compra;
						}
					}
					escreverArquivo();
					printf(ALTERADO_SUCESSO);
					break;
				// Cancelar
				case 3:
					printf(CANCELADO_SUCESSO);
					break;
				default:
					system(LIMPAR);
					printf(OPCAO_INVALIDA);
					break;
				}
			} while (opcao < 1 || opcao > 3);
		}
		else {
			printf(GRUPO_NAO_ENCONTRADO);
		}

	}
	else {
		printf(NENHUM_CADASTRO);
	}

	system(PAUSAR);

	system(LIMPAR);
}

// Função para listar os produtos de um fornecedor
void produtosFornecedores() {
	system(LIMPAR);
	
	if (cont_produto > 0) {
		// Pedir o fornecedor
		char fornecedor_busca[50];
		printf("Digite o fornecedor: ");
		getchar();
		gets_s(fornecedor_busca);

		// Verificar se existe
		bool fornecedor_encontrado = false;
		for (int i = 0; i < cont_produto; i++) {
			if (strcmp(fornecedor_busca, produto[i].fornecedor) == 0) {
				fornecedor_encontrado = true;
				break;
			}
		}

		// Exibir os produtos
		if (fornecedor_encontrado) {
			int cont_encontrados = 0;
			printf("\nCódigo               Descrição\n");
			printf("====================================================================================\n");;
			for (int i = 0; i < cont_produto; i++) {
				if (strcmp(fornecedor_busca, produto[i].fornecedor) == 0) {
					cont_encontrados++;
					printf("%-20.13lli %s\n", produto[i].codigo, produto[i].descricao);
				}
			}

			// Exibir o total encontrado
			printf("\nTotal de produtos encontrados: \033[32m%i\x1B[0m!\n\n", cont_encontrados);
		}
		else {
			printf(FORNECEDOR_NAO_ENCONTRADO);
		}
	}
	else {
		printf(NENHUM_CADASTRO);
	}

	system(PAUSAR);

	system(LIMPAR);
}

// Função principal
int main() {
	setlocale(LC_ALL, "portuguese");

	lerArquivo();

	int opcao;

	// Menu de opções
	do {
		printf("================ Sistema de Estoque ================\n");
		printf("<01>. Adicionar produto.\n");
		printf("<02>. Alterar dados de um produto.\n");
		printf("<03>. Excluir produto.\n");
		printf("<04>. Procurar por um produto pelo código.\n");
		printf("<05>. Procurar por um produto pela descrição.\n");
		printf("<06>. Listar todos os produtos.\n");
		printf("<07>. Listar preços\n");
		printf("<08>. Realizar aumento ou desconto de um grupo.\n");
		printf("<09>. Produtos fornecidos por fornecedores\n");
		printf("<10>. Sair do programa.\n");
		printf("====================================================\n");

		printf("\nEscolha uma opção: ");
		scanf("%i", &opcao);

		switch (opcao) {
		case 1:
			adicionarProduto();
			break;
		case 2:
			alterarDados();
			break;
		case 3:
			excluirProduto();
			break;
		case 4:
			buscarProdutoCodigo();
			break;
		case 5:
			buscarProdutoDescricao();
			break;
		case 6:
			listarProdutos();
			break;
		case 7:
			listarPrecos();
			break;
		case 8:
			movimentacaoProdutos();
			break;
		case 9:
			produtosFornecedores();
			break;
		case 10:
			printf(SAIR_PROGRAMA);
			exit(0);
			break;
		default:
			system(LIMPAR);
			printf(OPCAO_INVALIDA);
			break;
		}
	} while (opcao != 10);

	return 0;
} 