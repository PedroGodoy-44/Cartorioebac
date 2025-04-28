#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Constantes
#define MAX_LENGTH 40
#define MAX_BUFFER 255
#define ARQUIVO_REGISTROS "registros.txt"
#define USERNAME "admin"
#define PASSWORD "admin"
#define MAX_TENTATIVAS 3

// Estrutura para armazenar dados do usuário
typedef struct {
    char cpf[MAX_LENGTH];
    char nome[MAX_LENGTH];
    char sobrenome[MAX_LENGTH];
    char cargo[MAX_LENGTH];
} Registro;

// Protótipos de funções
void limparBuffer();
void pausarTela();
bool fazerLogin();
void exibirMenu();
int validarCPF(const char *cpf);
void registrarRegistro();
void consultarRegistro();
void deletarRegistro();
void listarTodosRegistros();
void salvarRegistroArquivoMestre(const Registro *reg);
void removerRegistroArquivoMestre(const char *cpf);

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    
    // Sistema de login
    if (!fazerLogin()) {
        printf("Acesso negado. Saindo do sistema...\n");
        return 1;
    }
    
    int opcao = 0;
    
    while (1) {
        exibirMenu();
        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            printf("Entrada inválida!\n");
            pausarTela();
            continue;
        }
        limparBuffer();

        switch (opcao) {
            case 1:
                registrarRegistro();
                break;
            case 2:
                consultarRegistro();
                break;
            case 3:
                deletarRegistro();
                break;
            case 4:
                listarTodosRegistros();
                break;
            case 5:
                printf("Saindo do sistema...\n");
                exit(0);
            default:
                printf("Opção inválida!\n");
                break;
        }
        
        pausarTela();
    }

    return 0;
}

bool fazerLogin() {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    int tentativas = 0;
    
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
    
    printf("=== SISTEMA DE GERENCIAMENTO DE REGISTROS ===\n");
    printf("=== LOGIN ===\n\n");
    
    while (tentativas < MAX_TENTATIVAS) {
        printf("Usuário: ");
        if (scanf("%39s", username) != 1) {
            limparBuffer();
            continue;
        }
        limparBuffer();
        
        printf("Senha: ");
        
        // Desativar eco para esconder senha
        #ifdef _WIN32
        system("pause>nul");
        #else
        system("stty -echo");
        #endif
        
        if (scanf("%39s", password) != 1) {
            limparBuffer();
            #ifdef _WIN32
            system("pause>nul");
            #else
            system("stty echo");
            #endif
            continue;
        }
        limparBuffer();
        
        #ifdef _WIN32
        system("pause>nul");
        #else
        system("stty echo");
        #endif
        printf("\n");
        
        if (strcmp(username, USERNAME) == 0 && strcmp(password, PASSWORD) == 0) {
            printf("Login bem-sucedido!\n");
            pausarTela();
            return true;
        } else {
            tentativas++;
            printf("Usuário ou senha incorretos. Tentativas restantes: %d\n", MAX_TENTATIVAS - tentativas);
        }
    }
    
    return false;
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausarTela() {
    #ifdef _WIN32
    system("pause");
    #else
    printf("Pressione Enter para continuar...");
    limparBuffer();
    #endif
}

void exibirMenu() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif

    printf("### Cartório de Nomes da EBAC ###\n\n");
    printf("Usuário: ADMIN\n\n");
    printf("Menu Principal:\n\n");
    printf("1. Registrar novo nome\n");
    printf("2. Consultar registro\n");
    printf("3. Deletar registro\n");
    printf("4. Listar todos os registros\n");
    printf("5. Sair\n\n");
    printf("Escolha uma opção: ");
}

int validarCPF(const char *cpf) {
    if (strlen(cpf) != 11) {
        return 0;
    }
    
    for (int i = 0; i < 11; i++) {
        if (!isdigit(cpf[i])) {
            return 0;
        }
    }
    
    return 1;
}

void registrarRegistro() {
    Registro reg;
    char arquivo[MAX_LENGTH + 5];
    
    printf("\n--- Novo Registro ---\n");
    
    do {
        printf("CPF (apenas números): ");
        if (scanf("%11s", reg.cpf) != 1) {
            limparBuffer();
            continue;
        }
        limparBuffer();
        
        if (!validarCPF(reg.cpf)) {
            printf("CPF inválido! Deve conter 11 dígitos numéricos.\n");
        }
    } while (!validarCPF(reg.cpf));
    
    sprintf(arquivo, "%s.txt", reg.cpf);
    FILE *file = fopen(arquivo, "r");
    if (file != NULL) {
        fclose(file);
        printf("Erro: CPF já cadastrado!\n");
        return;
    }
    
    printf("Nome: ");
    scanf("%39s", reg.nome);
    limparBuffer();
    
    printf("Sobrenome: ");
    scanf("%39s", reg.sobrenome);
    limparBuffer();
    
    printf("Cargo: ");
    scanf("%39s", reg.cargo);
    limparBuffer();
    
    file = fopen(arquivo, "w");
    if (file == NULL) {
        printf("Erro ao criar arquivo de registro!\n");
        return;
    }
    
    fprintf(file, "%s,%s,%s,%s", reg.cpf, reg.nome, reg.sobrenome, reg.cargo);
    fclose(file);
    
    salvarRegistroArquivoMestre(&reg);
    printf("\nRegistro cadastrado com sucesso!\n");
}

void consultarRegistro() {
    char cpf[MAX_LENGTH];
    char arquivo[MAX_LENGTH + 5];
    char buffer[MAX_BUFFER];
    
    printf("\n--- Consultar Registro ---\n");
    printf("CPF para consulta: ");
    scanf("%11s", cpf);
    limparBuffer();
    
    if (!validarCPF(cpf)) {
        printf("CPF inválido!\n");
        return;
    }
    
    sprintf(arquivo, "%s.txt", cpf);
    FILE *file = fopen(arquivo, "r");
    
    if (file == NULL) {
        printf("Registro não encontrado!\n");
        return;
    }
    
    printf("\nDados do registro:\n");
    printf("-----------------\n");
    
    while (fgets(buffer, MAX_BUFFER, file)) {
        char *token = strtok(buffer, ",");
        printf("CPF: %s\n", token);
        
        token = strtok(NULL, ",");
        printf("Nome: %s\n", token);
        
        token = strtok(NULL, ",");
        printf("Sobrenome: %s\n", token);
        
        token = strtok(NULL, ",");
        printf("Cargo: %s\n", token);
    }
    
    fclose(file);
}

void deletarRegistro() {
    char cpf[MAX_LENGTH];
    char arquivo[MAX_LENGTH + 5];
    
    printf("\n--- Deletar Registro ---\n");
    printf("CPF do registro a ser deletado: ");
    scanf("%11s", cpf);
    limparBuffer();
    
    if (!validarCPF(cpf)) {
        printf("CPF inválido!\n");
        return;
    }
    
    sprintf(arquivo, "%s.txt", cpf);
    
    if (remove(arquivo) == 0) {
        printf("Registro deletado com sucesso!\n");
        removerRegistroArquivoMestre(cpf);
    } else {
        printf("Erro ao deletar registro. Verifique se o CPF está correto.\n");
    }
}

void listarTodosRegistros() {
    printf("\n--- Todos os Registros ---\n");
    
    FILE *file = fopen(ARQUIVO_REGISTROS, "r");
    if (file == NULL) {
        printf("Nenhum registro encontrado.\n");
        return;
    }
    
    char buffer[MAX_BUFFER];
    int count = 0;
    
    while (fgets(buffer, MAX_BUFFER, file)) {
        buffer[strcspn(buffer, "\n")] = 0;
        
        char *cpf = strtok(buffer, ",");
        char *nome = strtok(NULL, ",");
        char *sobrenome = strtok(NULL, ",");
        char *cargo = strtok(NULL, ",");
        
        printf("\nRegistro #%d\n", ++count);
        printf("CPF: %s\n", cpf);
        printf("Nome: %s %s\n", nome, sobrenome);
        printf("Cargo: %s\n", cargo);
    }
    
    if (count == 0) {
        printf("Nenhum registro encontrado.\n");
    } else {
        printf("\nTotal de registros: %d\n", count);
    }
    
    fclose(file);
}

void salvarRegistroArquivoMestre(const Registro *reg) {
    FILE *file = fopen(ARQUIVO_REGISTROS, "a");
    if (file == NULL) {
        printf("Erro ao acessar arquivo mestre de registros!\n");
        return;
    }
    
    fprintf(file, "%s,%s,%s,%s\n", reg->cpf, reg->nome, reg->sobrenome, reg->cargo);
    fclose(file);
}

void removerRegistroArquivoMestre(const char *cpf) {
    FILE *file = fopen(ARQUIVO_REGISTROS, "r");
    if (file == NULL) return;
    
    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        fclose(file);
        return;
    }
    
    char buffer[MAX_BUFFER];
    int encontrado = 0;
    
    while (fgets(buffer, MAX_BUFFER, file)) {
        char tempCpf[MAX_LENGTH];
        strncpy(tempCpf, buffer, 11);
        tempCpf[11] = '\0';
        
        if (strcmp(tempCpf, cpf) != 0) {
            fputs(buffer, temp);
        } else {
            encontrado = 1;
        }
    }
    
    fclose(file);
    fclose(temp);
    
    if (encontrado) {
        remove(ARQUIVO_REGISTROS);
        rename("temp.txt", ARQUIVO_REGISTROS);
    } else {
        remove("temp.txt");
    }
}