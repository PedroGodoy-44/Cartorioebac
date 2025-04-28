#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

void registrar() {
    char arquivo[40];
    char cpf[40];
    char nome[40];
    char sobrenome[40];
    char cargo[40];

    printf("Digite o CPF a ser cadastrado: ");
    scanf("%s", cpf);

    sprintf(arquivo, "%s.txt", cpf); // Concatena o CPF com ".txt"

    FILE *file = fopen(arquivo, "w"); // Abre o arquivo para escrita
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    fprintf(file, "%s,", cpf);

    printf("Digite o nome a ser cadastrado: ");
    scanf("%s", nome);
    fprintf(file, "%s,", nome);

    printf("Digite o sobrenome a ser cadastrado: ");
    scanf("%s", sobrenome);
    fprintf(file, "%s,", sobrenome);

    printf("Digite o cargo a ser cadastrado: ");
    scanf("%s", cargo);
    fprintf(file, "%s", cargo);

    fclose(file); // Fecha o arquivo
}

void consultar() {
    char cpf[40];
    char arquivo[40];
    char buffer[255];

    printf("Digite o CPF para consultar: ");
    scanf("%s", cpf);

    sprintf(arquivo, "%s.txt", cpf); // Concatena o CPF com ".txt"

    FILE *file = fopen(arquivo, "r"); // Abre o arquivo para leitura
    if (file == NULL) {
        printf("Registro não encontrado!\n");
        return;
    }

    printf("Dados do registro:\n");
    while (fgets(buffer, 255, file)) {
        printf("%s", buffer);
    }

    fclose(file); // Fecha o arquivo
}

void deletar() {
    char cpf[40];
    char arquivo[40];

    printf("Digite o CPF do registro a ser deletado: ");
    scanf("%s", cpf);

    sprintf(arquivo, "%s.txt", cpf); // Concatena o CPF com ".txt"

    if (remove(arquivo) == 0) {
        printf("Registro deletado com sucesso!\n");
    } else {
        printf("Erro ao deletar o registro!\n");
    }
}

int main() {
    int opcao = 0;
    setlocale(LC_ALL, "pt_BR.UTF-8");

    while (1) {
        #ifdef _WIN32
        system("cls"); // Para Windows
        #else
        system("clear"); // Para sistemas UNIX-based (Linux/MacOS)
        #endif

        printf("### Cartório de nomes da Ebac ###\n\n");
        printf("Escolha a opção desejada do menu:\n\n");
        printf("\t1 Registrar nomes\n\n");
        printf("\t2 Consultar nomes\n\n");
        printf("\t3 Deletar nomes\n\n");
        printf("\t4 Sair\n\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        fflush(stdin); // Para limpar o buffer de entrada

        #ifdef _WIN32
        system("pause"); // Pausa no Windows
        #else
        system("read -p 'Pressione Enter para continuar...' var"); // Pausa no UNIX-based
        #endif

        switch (opcao) {
            case 1:
                registrar();
                break;
            case 2:
                consultar();
                break;
            case 3:
                deletar();
                break;
            case 4:
                exit(0);
            default:
                printf("ESSA OPÇÃO NÃO EXISTE!\n");
                break;
        }
    }

    return 0;
}
