#include <iostream>
#include <fstream>
#include <string>

/*
* O numero de equações restritivas e o numero de variaveis devem ser passadas como argumento, nessa ordem.
* O nome do arquivo de entrada deve ser passado logo em seguida. Os dados devem estar escritos na forma tabular padrão
* e devem respeitar a ordem:
*
* Variaveis basicas
* Função objetiva
* Funções restrivas
*
* Como no exemplo:
* 3 4 5 (Variaveis basicas)
* 1 1 -4 0 0 0 0 (Função objetiva)
* 1 1 2 1 0 0 9 (Função restriva)
* 1 1 -1 0 1 0 2 (Função restriva)
* -1 1 1 0 0 1 4 (Função restriva)
*/

using namespace std;

// Numero de equacoes restritivas, Numero de variaveis
int REST_NUM, VAR_NUM;

// Printa a matriz e o valor das variaveis
void print_mat(double *func_obj, double **func_rest, int *var_base) {
	for(int i = 0;i <= VAR_NUM;i++) {
		cout << func_obj[i] << " ";
	}
	cout << endl;
	
	for(int i = 0;i < REST_NUM;i++) {
		for(int j = 0;j <= VAR_NUM;j++) {
			cout << func_rest[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	for(int i = 0;i < REST_NUM;i++) {
		cout << "x" << (var_base[i]+1) << " = " << func_rest[i][VAR_NUM] << " ";
	}
	cout << endl << "Max = " << func_obj[VAR_NUM] << endl << endl;	
}

// Verifica se a função objetivo é otimizavel, se sim, var guarda o indice da variavel
bool otimizavel(double *func, int *var) {
	*var = 0;
	for(int i = 1;i < VAR_NUM;i++) {
		if(func[i] < func[*var]) {
			*var = i;
		}
	}
	
	return (func[*var] < 0);
}

// Retorna qual função possui a menor restrição positiva
int maior_otimizacao(double **func_rest, int var) {
	int menor_index = 0;
	for(int i = 1;i < REST_NUM;i++) {
		if(((func_rest[menor_index][VAR_NUM]/func_rest[menor_index][var]) < 0) or
			((func_rest[i][VAR_NUM]/func_rest[i][var]) > 0 and
			(func_rest[i][VAR_NUM]/func_rest[i][var]) < 
			(func_rest[menor_index][VAR_NUM]/func_rest[menor_index][var]))) {
			menor_index = i;
		}
	}
	return menor_index;
}

// Divide a equação pivo pelo coeficiente da variavel que entra na base
void simplificar_eq(double **func_rest, int pivot, int var) {
	double fator = func_rest[pivot][var];
	for(int i = 0;i <= VAR_NUM;i++) {
		func_rest[pivot][i] /= fator;
	}	
}

// Zera o coeficiente da variavel que entra na base das demais equações
void zerar_variaveis(double *func_obj, double **func_rest, int pivot, int var) {
	double fator;
	for(int i = 0;i < REST_NUM;i++) {
		if(i != pivot) {
			fator = func_rest[i][var];
			for(int j = 0;j <= VAR_NUM;j++) {
				func_rest[i][j] += func_rest[pivot][j]*(-fator);
			}
		}
	}
	
	fator = func_obj[var];
	for(int i = 0;i <= VAR_NUM;i++) {
		func_obj[i] += func_rest[pivot][i]*(-fator);
	}
	
}

// Executa o algoritmo simplex
void simplex(double *func_obj, double **func_rest, int *var_base) {
	int var, pivot;
	print_mat(func_obj, func_rest, var_base);
	//fgetc(stdin); // Pausar a cada iteração
	while(otimizavel(func_obj, &var)) {
		pivot = maior_otimizacao(func_rest, var);
		var_base[pivot] = var; // "Var entra na base"
		simplificar_eq(func_rest, pivot, var);
		zerar_variaveis(func_obj, func_rest, pivot, var);
		
		print_mat(func_obj, func_rest, var_base);
		//fgetc(stdin); // Pausar a cada iteração
	}
}

int main(int argc, char *argv[]) {
	ifstream input;
	input.exceptions(ifstream::failbit | ifstream::badbit);
	// Tratamento dos argumentos e abertura do arquivo de entrada
	if(argc < 4) {
		cout <<  "Argumentos Insuficientes.\nUso:\n~ [Numero de Variaveis] [Numero de equacoes restritivas] [Nome do arquivo]\n";
		exit(EXIT_FAILURE);
	}
	try {
		VAR_NUM = stoi(string(argv[1]));
		REST_NUM = stoi(string(argv[2]));
		
		input.open(argv[3], ifstream::in);
	} catch(ifstream::failure e) {
		cerr << "Nao foi possivel abrir o arquivo" << endl;
		exit(EXIT_FAILURE);
	} catch(exception const & e) {
		cerr << "Valor invalido" << endl;
		exit(EXIT_FAILURE);
	}
	
	// Alocação dos arrays
	int *var_base = new int[REST_NUM];
	double *func_obj = new double[VAR_NUM+1];
	double **func_rest = new double*[REST_NUM];
	for(int i = 0;i < REST_NUM;i++) {
		func_rest[i] = new double[VAR_NUM+1];
	}
	
	// Leitura do arquivo de entrada
	try {
		for(int i = 0;i < REST_NUM;i++) {
			input >> var_base[i];
		}
		for(int i = 0;i <= VAR_NUM;i++) {
			input >> func_obj[i];
		}
		for(int i = 0;i < REST_NUM;i++) {
			for(int j = 0;j <= VAR_NUM;j++) {
				input >> func_rest[i][j];
			}
		}
	} catch(exception const & e) {
		cerr << "Erro de leitura. Verifique os valores de entrada." << endl;
		exit(EXIT_FAILURE);
	}
	
	// Execução simplex
	simplex(func_obj, func_rest, var_base);
	return 0;
}