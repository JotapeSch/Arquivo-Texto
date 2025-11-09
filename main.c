#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ARQUIVO1 "2-readme.code.txt"
#define ARQUIVO2 "3-readme.decifra.txt"
#define ARQUIVO3 "4-readme.nlines.txt"

void AbrirArquivo(FILE **arquivo, const char *modo_abertura, const char *nome_arquivo)
{
    *arquivo = fopen(nome_arquivo, modo_abertura);
    if (*arquivo == NULL)
    {
        *arquivo = fopen(nome_arquivo, "w");
        if (*arquivo == NULL)
        {
            printf("Erro ao abrir o arquivo!\n");
            exit(100);
        }
    }
}

// Objetivo: Decifra o arquivo
// parametros: nome do arquivo
// retorno: Nenhum
void DecifrarArquivo(const char *nome_arquivo)
{
    FILE *arquivo_entrada;
    FILE *arquivo_saida;
    int codigo_ascii;

    AbrirArquivo(&arquivo_entrada, "r", nome_arquivo);
    if (arquivo_entrada == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        return;
    }

    AbrirArquivo(&arquivo_saida, "w", ARQUIVO2);
    if (arquivo_saida == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        return;
    }
    char carac;
    while ((carac = fgetc(arquivo_entrada)) != EOF)
    {
        if (carac == '[')
        {
            if (fscanf(arquivo_entrada, "%d]", &codigo_ascii) == 1)
            {
                char caractere = (char)codigo_ascii;
                fprintf(arquivo_saida, "%c", caractere);
            }
            else
            {
                printf("ERRO: Colchete sem numero valido. Parando.\n");
                break;
            }
        }
    }
    fclose(arquivo_entrada);
    fclose(arquivo_saida);
    printf("----------------------------------\n");
    printf("Arquivo decifrado com sucesso!\n");
}

// Objetivo: Faz a varredura ate o final do arquivo e verifica os '\n', fazendo a contagem
// parametros: nome do arquivo
// retorno: quantidade de linha se der certo, senao NULL
char *ler_linha(FILE *arquivo)
{
    int tamanho_atual = 128;
    int indice = 0;
    char *linha = (char *)malloc(tamanho_atual);
    int c;

    if (linha == NULL)
    {
        printf("Erro de alocacao de memoria!\n");
        return NULL;
    }

    while ((c = fgetc(arquivo)) != EOF)
    {
        if (indice == tamanho_atual - 1)
        {
            tamanho_atual *= 2;

            char *temp = (char *)realloc(linha, tamanho_atual);

            if (temp == NULL)
            {
                printf("Erro de alocacao de memora!\n");
                free(linha);
                return NULL;
            }
            linha = temp;
        }

        linha[indice++] = (char)c;
        if (c == '\n')
        {
            break;
        }
    }
    if (indice > 0)
    {
        linha[indice] = '\0';
        char *final_linha = (char *)realloc(linha, indice + 1);
        if (final_linha != NULL)
        {
            linha = final_linha;
        }
        return linha;
    }
    free(linha);
    return NULL;
}

// Objetivo: grava linha que foi lida em *ler_linha e ja faz a concatenacao de forma direta
// parametros: nome do arquivo
// retorno: Nenhum
void GravaLinha(const char *nome_arquivo)
{
    FILE *arquivo_entrada;
    FILE *arquivo_saida;
    char *linha = NULL;
    int contador = 0;

    AbrirArquivo(&arquivo_entrada, "r", nome_arquivo);
    if (arquivo_entrada == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    AbrirArquivo(&arquivo_saida, "w", ARQUIVO3);
    if (arquivo_saida == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    while ((linha = ler_linha(arquivo_entrada)) != NULL)
    {
        fprintf(arquivo_saida, "[linha:%d]%s", contador + 1, linha);
        free(linha);
        contador++;
    }
    fclose(arquivo_entrada);
    fclose(arquivo_saida);
    printf("----------------------------------\n");
    printf("Numero de linha gravado em arquivo 3 com sucesso!\n");
}
int ContarLinhas(const char *nome_arquivo)
{ // Obj: Contar quantas linhas existem no arquivo
    FILE *arquivo;
    AbrirArquivo(&arquivo, "r", nome_arquivo);
    int contador = 0;
    char c;
    int tem_conteudo = 0;

    while ((c = fgetc(arquivo)) != EOF)
    { // fgets le ate contar o /n
        tem_conteudo = 1;
        if (c == '\n')
            contador++;
    }

    if (tem_conteudo && c != '\n') // msm se o arquivo não terminar com \n, ainda conta a ultima linha
        contador++;

    fclose(arquivo);
    printf("----------------------------------\n");
    printf("O arquivo 4-readme.nlines.txt possui %d linha(s).\n", contador);
    return contador;
} // retorno: numero de linhas

// Obj: Encontrar a maior linha  de um arquivo
void MaiorLinha(const char *nome_arquivo)
{
    FILE *arquivo;
    AbrirArquivo(&arquivo, "r", nome_arquivo);

    char *linha = NULL;
    char *conteudo_maior_linha = NULL;
    int maior = 0;
    int numero_linha = 0;
    int linha_maior = 0;

    while ((linha = ler_linha(arquivo)) != NULL)
    {
        numero_linha++;
        int tamanho = strlen(linha);

        if (tamanho > maior)
        {
            maior = tamanho;
            linha_maior = numero_linha;

            if (conteudo_maior_linha != NULL)
            {
                free(conteudo_maior_linha);
            }

            conteudo_maior_linha = (char *)malloc(maior + 1);
            if (conteudo_maior_linha == NULL)
            {
                printf("Erro de alocacao\n");
                fclose(arquivo);
                return;
            }

            strcpy(conteudo_maior_linha, linha);
        }

        free(linha); // libera a memoria alocada dinamicamente
    }

    fclose(arquivo);

    if (linha_maior == 0)
        printf("O arquivo esta vazio.\n");
    else
    {
        printf("----------------------------------\n");
        printf("A linha [%s] de numero [%d] e a maior com (%d) caracteres.\n", conteudo_maior_linha, linha_maior, maior);
    }
    if (conteudo_maior_linha != NULL)
    {
        free(conteudo_maior_linha);
    }
}

void mostrar_menu()
{
    printf("\n");
    printf("1-Decifra arquivo\n");               // JOTA
    printf("2-Grava numero linha em arquivo\n"); // JOTA
    printf("3-Quantas linhas o arquivo tem\n");  // RYAN
    printf("4-Qual a maior linha\n");            // RYAN
    printf("5-Pesquisar uma palavra\n");         // VINI
    printf("0-Encerrar\n");
}

int main()
{
    int opcao;
    do
    {
        mostrar_menu();
        printf("\n");
        printf("Qual opcao: \n");
        if (scanf("%d", &opcao) != 1)
        {
            printf("Digite uma opcao valida!\n");
        }
        switch (opcao)
        {
        case 1:
            DecifrarArquivo(ARQUIVO1);
            break;

        case 2:
            GravaLinha(ARQUIVO2);
            break;

        case 3:
            ContarLinhas(ARQUIVO2);
            break;
        case 4:
            MaiorLinha(ARQUIVO2);
            break;
        case 5: //VINI
        {
            char palavra[256];
            printf("\nQual palavra pesquisar: ");
            if (scanf("%255s", palavra) != 1)
            {
                break;
            }

            FILE *fp = fopen(ARQUIVO3, "r");
            if (!fp)
            {
                printf("\nErro ao abrir o arquivo %s!\n", ARQUIVO3);
                break;
            }

            char linha[4096];
            int total_ocorrencias = 0;

            printf("\nAs linhas em que a palavra %s ocorrem:\n", palavra);

            while (fgets(linha, sizeof(linha), fp))
            {
                int num_linha;
                char conteudo[4096];

                // Verifica o formato da linha e extrai o número e o conteúdo
                if (sscanf(linha, "[linha:%d]%[^\n]", &num_linha, conteudo) == 2)
                {
                    char *inicio = conteudo;

                    // Loop para encontrar TODAS as ocorrências na linha
                    while ((inicio = strstr(inicio, palavra)) != NULL)
                    {
                        // Verifica o caractere ANTES
                        char antes = (inicio == conteudo) ? ' ' : *(inicio - 1);

                        // Verifica o caractere DEPOIS
                        char depois = *(inicio + strlen(palavra));

                        // Condição de Palavra Isolada por Espaço (usando isspace para ser robusta)
                        if ((isspace((unsigned char)antes) || antes == '\0') &&
                            (isspace((unsigned char)depois) || depois == '\0'))
                        {
                            // >>> AQUI ESTÁ A MUDANÇA PRINCIPAL: IMPRIME DENTRO DO IF <<<
                            printf("[linha:%d]%s\n", num_linha, conteudo);
                            total_ocorrencias++;
                        }

                        // AVANÇO: Move para o próximo caractere após a ocorrência
                        inicio++;
                    }
                }
            }

            fclose(fp);

            printf("\ne tem %d ocorrencia%s.\n", total_ocorrencias, (total_ocorrencias == 1 ? "" : "s"));
            printf("----------------------------------\n");
        }
        break;

        case 0:

            break;

        default:
            printf("Opcao invalida\n");
            break;
        }

    } while (opcao != 0);

    return 0;
}
