#include <stdio.h>
#include <locale.h>
#include <windows.h>

// Defin��es de mensagens e outros
#define MAX_PRODUTO 50
#define OPCAO_INVALIDA "\033[31mOp��o inv�lida. Escolha novamente!\x1B[0m\n\n"
#define NENHUM_CADASTRO "\033[31mNenhum produto foi cadastrado no sistema!\x1B[0m\n\n"
#define QUANTIA_MAXIMA "\033[31mN�o � poss�vel adicionar mais produtos, a quantia m�xima j� foi adicionada!\x1B[0m\n\n"
#define CODIGO_EXISTENTE "\033[31m\nEste c�digo j� foi cadastrado no sistema!\x1B[0m\n\n"
#define PRODUTO_NAO_ENCONTRADO "\n\033[31mProduto n�o encontrado!\x1B[0m\n\n"
#define GRUPO_NAO_ENCONTRADO "\n\033[31mGrupo n�o encontrado!\x1B[0m\n\n"
#define FORNECEDOR_NAO_ENCONTRADO "\n\033[31mFornecedor n�o encontrado!\x1B[0m\n\n"
#define CANCELADO_SUCESSO "\n\033[31mCancelado com sucesso!\x1B[0m\n\n"
#define CADASTRO_SUCESSO "\n\033[32mProduto cadastrado com sucesso!\x1B[0m\n\n"
#define DELETADO_SUCESSO "\n\033[32mProduto deletado com sucesso!\x1B[0m\n\n"
#define ALTERADO_SUCESSO "\n\033[32mAlterado com sucesso!\x1B[0m\n\n"
#define FORMATACAO_ESCRITA "\n{\n\tcodigo: %lli\n\tgrupo: %s\n\tdescricao: %s\n\tunidade: %s\n\tfornecedor: %s\n\tquantidade: %i\n\tpr_compra: R$ %.2f\n\tpr_venda: R$ %.2f\n\tlucro: R$ %.2f\n\testoque_min: %i\n}"
#define FORMATACAO_LEITURA "\n{\n\tcodigo: %lli\n\tgrupo: %50[^\n]\n\tdescricao: %50[^\n]\n\tunidade: %3[^\n]\n\tfornecedor: %50[^\n]\n\tquantidade: %i\n\tpr_compra: R$ %f\n\tpr_venda: R$ %f\n\tlucro: R$ %f\n\testoque_min: %i\n}"
#define DADOS_ESCRITA produto[i].codigo, produto[i].grupo, produto[i].descricao, produto[i].unidade, produto[i].fornecedor, produto[i].quantidade, produto[i].pr_compra, produto[i].pr_venda, produto[i].lucro, produto[i].estoque_min
#define DADOS_LEITURA &produto[i].codigo, &produto[i].grupo, &produto[i].descricao, &produto[i].unidade, &produto[i].fornecedor, &produto[i].quantidade, &produto[i].pr_compra, &produto[i].pr_venda, &produto[i].lucro, &produto[i].estoque_min
#define PAUSAR "pause"
#define LIMPAR "cls"

// Estrutura do produto e vari�veis globais
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
} TProduto;
TProduto produto[MAX_PRODUTO];

int cont_produto;
FILE* arquivo;

// Fun��o para ler os dados do arquivo
void lerArquivo() {
	arquivo = fopen("estoque.dat", "r");

	if (arquivo != NULL) {
		fscanf(arquivo, "cont_produto: %i", &cont_produto);

		for (int i = 0; i < cont_produto; i++) {
			fscanf(arquivo, FORMATACAO_LEITURA, DADOS_LEITURA);
		}

		fclose(arquivo);
	}
	else {
		cont_produto = 0;
	}
}

// Fun��o para escrever os dados no arquivo
void escreverArquivo(bool deletar, long long int deletar_codigo) {
	arquivo = fopen("estoque.dat", "w");

	if (arquivo != NULL) {

		if (!deletar) {
			fprintf(arquivo, "cont_produto: %i", cont_produto);

			for (int i = 0; i < cont_produto; i++) {
				fprintf(arquivo, FORMATACAO_ESCRITA, DADOS_ESCRITA);
			}
		}
		else {
			fprintf(arquivo, "cont_produto: %i", cont_produto - 1);

			for (int i = 0; i < cont_produto; i++) {
				if (deletar_codigo != produto[i].codigo) {
					fprintf(arquivo, FORMATACAO_ESCRITA, DADOS_ESCRITA);
				}
			}
		}

		fclose(arquivo);
	}
}

// Fun��o para adicionar os produtos no sistema
void adicionarProduto() {
	system(LIMPAR);

	lerArquivo();

	// Verificar a quantia
	if (cont_produto < MAX_PRODUTO) {

		bool verificar_codigo = false;

		do {
			// Verificar o c�digo
			printf("Digite o c�digo: ");
			scanf("%lli", &produto[cont_produto].codigo);

			for (int i = 0; i < cont_produto; i++) {
				if (produto[cont_produto].codigo == produto[i].codigo) {
					verificar_codigo = true;

					printf(CODIGO_EXISTENTE);

					break;
				}
			}
			if(verificar_codigo == true) {
				break;
			}

			// Ler os dados do produto a ser adicionado
			printf("Digite o grupo: ");
			getchar();
			gets_s(produto[cont_produto].grupo);

			printf("Digite a descri��o: ");
			gets_s(produto[cont_produto].descricao);

			printf("Digite a unidade: ");
			gets_s(produto[cont_produto].unidade);

			printf("Digite o fornecedor: ");
			gets_s(produto[cont_produto].fornecedor);

			printf("Digite a quantidade: ");
			scanf("%i", &produto[cont_produto].quantidade);

			printf("Digite o pre�o de compra: R$ ");
			scanf("%f", &produto[cont_produto].pr_compra);

			printf("Digite o pre�o de venda: R$ ");
			scanf("%f", &produto[cont_produto].pr_venda);

			printf("Digite o lucro m�nimo: R$ ");
			scanf("%f", &produto[cont_produto].lucro);

			printf("Digite a quantia m�nima em estoque: ");
			scanf("%i", &produto[cont_produto].estoque_min);

			// Cadastrar o produto no sistema
			cont_produto++;

			escreverArquivo(false, 0);

			printf(CADASTRO_SUCESSO);

		} while (verificar_codigo);
	}
	else {
		printf(QUANTIA_MAXIMA);
	}

	system(PAUSAR);

	system(LIMPAR);
}

void alterarDados() {
	system(LIMPAR);
	
	lerArquivo();

	// Ler o c�digo
	if (cont_produto > 0) {
		long long int codigo_busca;
		printf("Digite o c�digo do produto que deseja alterar algum dado: ");
		scanf("%lli", &codigo_busca);

		bool produto_encontrado = false;

		// Verificar
		for (int i = 0; i < cont_produto; i++) {
			if (codigo_busca == produto[i].codigo) {
				produto_encontrado = true;

				system(LIMPAR);

				// Op��o de escolha do dado a ser alterado
				int opcao;
				do {
					printf("O c�digo do produto a ter os dados alterados � \033[32m%lli\x1B[0m e a sua descri��o � \033[32m%s\x1B[0m!\n\n", produto[i].codigo, produto[i].descricao);

					printf("<01>. Alterar o grupo\n");
					printf("<02>. Alterar a descri��o\n");
					printf("<03>. Alterar a unidade\n");
					printf("<04>. Alterar o fornecedor\n");
					printf("<05>. Alterar a quantidade\n");
					printf("<06>. Alterar o pre�o de compra\n");
					printf("<07>. Alterar o pre�o de venda\n");
					printf("<08>. Alterar o lucro m�nimo.\n");
					printf("<09>. Alterar o estoque m�nimo.\n");
					printf("<10>. Cancelar\n\n");

					printf("Escolha uma das op��es: ");
					scanf("%i", &opcao);

					switch (opcao) {
					case 1:
						printf("\nDigite o novo grupo: ");
						getchar();
						gets_s(produto[i].grupo);
						break;
					case 2:
						printf("\nDigite a nova descri��o: ");
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
						printf("\nDigite a nova quantidade: ");
						scanf("%i", &produto[i].quantidade);
						break;
					case 6:
						printf("\nDigite o novo pre�o de compra: ");
						scanf("%f", &produto[i].pr_compra);
						break;
					case 7:
						printf("\nDigite o novo pre�o de venda: ");
						scanf("%f", &produto[i].pr_venda);
						break;
					case 8:
						printf("\nDigite o novo lucro m�nimo: ");
						scanf("%f", &produto[i].lucro);
						break;
					case 9:
						printf("\nDigite o novo estoque m�nimo: ");
						scanf("%i", &produto[i].estoque_min);
						break;
					case 10:
						printf(CANCELADO_SUCESSO);
						break;
					default:
						system(LIMPAR);
						printf(OPCAO_INVALIDA);
					}
				} while (opcao < 1 || opcao > 10);

				if (opcao != 10) {
					escreverArquivo(false, 0);
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

// Fun��o para excluir produtos do sistema
void excluirProduto() {
	system(LIMPAR);

	lerArquivo();

	if (cont_produto > 0) {
		long long int deletar_codigo;
		bool produto_encontrado = false;

		// Verificar o c�digo
		printf("Digite o c�digo do produto a ser deletado: ");
		scanf("%lli", &deletar_codigo);

		for (int i = 0; i < cont_produto; i++) {
			if (deletar_codigo == produto[i].codigo) {
				produto_encontrado = true;

				escreverArquivo(true, produto[i].codigo);

				printf(DELETADO_SUCESSO);

				break;
			}
		}

		// Deletar o produto do sistema se o c�digo existir
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

	lerArquivo();	

	if (cont_produto > 0) {
		// Pedir o c�digo a ser buscado
		long long int codigo_busca;
		printf("Digite o c�digo do produto a ser buscado: ");
		scanf("%lli", &codigo_busca);

		// Realizar a busca
		bool produto_encontrado = false;
		for (int i = 0; i < cont_produto; i++) {
			if (codigo_busca == produto[i].codigo) {
				printf("\nC�digo: %lli\n", produto[i].codigo);
				printf("Grupo: %s\n", produto[i].grupo);
				printf("Descri��o: %s\n", produto[i].descricao);
				printf("Unidade: %s\n", produto[i].unidade);
				printf("Fornecedor: %s\n", produto[i].fornecedor);
				printf("Quantidade: %i\n", produto[i].quantidade);
				printf("Pre�o compra: R$ %.2f\n", produto[i].pr_compra);
				printf("Pre�o venda: R$ %.2f\n", produto[i].pr_venda);
				printf("Lucro m�nimo: R$ %.2f\n", produto[i].lucro);
				printf("Quantidade m�nima em estoque: %i\n\n", produto[i].estoque_min);

				produto_encontrado = true;
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

// Fun��o para buscar o produto pela descri��o
void buscarProdutoDescricao() {
	system(LIMPAR);

	lerArquivo();

	if (cont_produto > 0) {
		// Pedir a descri��o a ser buscada
		char descricao_busca[50];
		printf("Digite a descri��o do produto a ser buscado: ");
		getchar();
		gets_s(descricao_busca);

		// Realizar a busca
		bool produto_encontrado = false;
		for (int i = 0; i < cont_produto; i++) {
			if (strstr(produto[i].descricao, descricao_busca)) {
				printf("\nC�digo: %lli\n", produto[i].codigo);
				printf("Grupo: %s\n", produto[i].grupo);
				printf("Descri��o: %s\n", produto[i].descricao);
				printf("Unidade: %s\n", produto[i].unidade);
				printf("Fornecedor: %s\n", produto[i].fornecedor);
				printf("Quantidade: %i\n", produto[i].quantidade);
				printf("Pre�o compra: R$ %.2f\n", produto[i].pr_compra);
				printf("Pre�o venda: R$ %.2f\n", produto[i].pr_venda);
				printf("Lucro m�nimo: R$ %.2f\n", produto[i].lucro);
				printf("Quantidade m�nima em estoque: %i\n\n", produto[i].estoque_min);

				produto_encontrado = true;
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

// Fun��o para listar os produtos do sistema
void listarProdutos() {
	system(LIMPAR);

	lerArquivo();

	// Exibir dados
	if (cont_produto > 0) {
		for (int i = 0; i < cont_produto; i++) {
			printf("=============================================================================================\n");
			printf("C�digo: %-53.13lli Grupo: %s\n", produto[i].codigo, produto[i].grupo);
			printf("Descri��o: %-50s Unidade: %s\n", produto[i].descricao, produto[i].unidade);
			printf("Fornecedor: %s\n", produto[i].fornecedor);
			printf("Pre�o de compra: R$ %-10.2f Pre�o de venda: R$ %-11.2f Lucro m�nimo: R$ %.2f\n", produto[i].pr_compra, produto[i].pr_venda, produto[i].lucro);
			printf("Quantidade em estoque: %-38i Quantidade m�nima: %i\n", produto[i].quantidade, produto[i].estoque_min);
			printf("=============================================================================================\n\n");
		}
	}
	else {
		printf(NENHUM_CADASTRO);
	}

	system(PAUSAR);

	system(LIMPAR);
}

// Fun��o para listar os pre�os
void listarPrecos() {
	system(LIMPAR);

	lerArquivo();

	// Exibir dados
	if (cont_produto > 0) {
		printf("====================================================================================\n");
		printf("C�digo               Descri��o                                          Pre�o venda\n");
		printf("====================================================================================\n");

		for (int i = 0; i < cont_produto; i++) {
			printf("%-20.13lli %-50s R$ %.2f\n", produto[i].codigo, produto[i].descricao, produto[i].pr_venda);
		}
		printf("\n");
	}
	else {
		printf(NENHUM_CADASTRO);
	}

	system(PAUSAR);

	system(LIMPAR);
}

// Fun��o para realizar aumento ou desconto de um grupo de produtos
void movimentacaoProdutos() {
	system(LIMPAR);

	lerArquivo();

	if (cont_produto > 0) {
		// Pedir o grupo e verificar se existe
		char grupo_busca[50];

		printf("Digite o grupo do produto: ");
		getchar();
		gets_s(grupo_busca);

		bool grupo_encontrado = false;
		for (int i = 0; i < cont_produto; i++) {
			if (strcmp(grupo_busca, produto[i].grupo) == 0) {
				grupo_encontrado = true;
			}
		}

		if (grupo_encontrado) {
			// Perguntar o percentual
			float percentual;
			printf("\nDigite o percentual a ser alterado: ");
			scanf("%f", &percentual);

			system(LIMPAR);

			int opcao;
			do {
				// Escolher se ser� realizado desconto ou aumento
				printf("O grupo a sofrer altera��o no pre�o ser� \033[32m%s\x1B[0m e o percentual ser� de \033[32m%.2f%%\x1B[0m\n\n", grupo_busca, percentual);
				printf("<01>. Realizar desconto\n");
				printf("<02>. Realizar aumento\n");
				printf("<03>. Cancelar\n\n");

				printf("Escolha uma das op��es: ");
				scanf("%i", &opcao);

				switch (opcao) {
					// Desconto
					case 1:
						for (int i = 0; i < cont_produto; i++) {
							if (strcmp(grupo_busca, produto[i].grupo) == 0) {
								produto[i].pr_venda -= (produto[i].pr_venda / 100) * percentual;
							}
						}
						escreverArquivo(false, 0);
						printf(ALTERADO_SUCESSO);
						break;
					// Aumento
					case 2:
						for (int i = 0; i < cont_produto; i++) {
							if (strcmp(grupo_busca, produto[i].grupo) == 0) {
								produto[i].pr_venda += (produto[i].pr_venda / 100) * percentual;
							}
						}
						escreverArquivo(false, 0);
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

// Fun��o para listar os produtos de um fornecedor
void produtosFornecedores() {
	system(LIMPAR);
	
	lerArquivo();

	if (cont_produto > 0) {
		// Pedir o fornecedor e realizar busca
		char fornecedor_busca[50];
		printf("Digite o fornecedor: ");
		getchar();
		gets_s(fornecedor_busca);

		bool fornecedor_encontrado = false;
		for (int i = 0; i < cont_produto; i++) {
			if (strcmp(fornecedor_busca, produto[i].fornecedor) == 0) {
				fornecedor_encontrado = true;
			}
		}

		// Exibir os produtos
		if (fornecedor_encontrado) {
			printf("\nC�digo               Descri��o\n");
			printf("====================================================================================\n");;
			for (int i = 0; i < cont_produto; i++) {
				if (strcmp(fornecedor_busca, produto[i].fornecedor) == 0) {
					printf("%-20.13lli %s\n", produto[i].codigo, produto[i].descricao);
				}
			}
			printf("\n");
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

// Fun��o principal
int main() {
	setlocale(LC_ALL, "portuguese");

	int opcao;

	// Menu de op��es
	do {
		printf("================ Sistema de Estoque ================\n");
		printf("<01>. Adicionar produto.\n");
		printf("<02>. Alterar dados de um produto.\n");
		printf("<03>. Excluir produto.\n");
		printf("<04>. Procurar por um produto pelo c�digo.\n");
		printf("<05>. Procurar por um produto pela descri��o.\n");
		printf("<06>. Listar todos os produtos.\n");
		printf("<07>. Listar pre�os\n");
		printf("<08>. Realizar aumento ou desconto de um grupo.\n");
		printf("<09>. Produtos fornecidos por fornecedores\n");
		printf("<10>. Sair do programa.\n");
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