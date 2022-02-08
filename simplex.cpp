#include <iostream>
// Numero de equações restritivas
#define REST_NUM 4
// Numero de variaveis basicas e não basicas
#define VAR_NUM 6

using namespace std;

// Printa a matriz e o valor das variaveis
void print_mat(double func_obj[], double func_rest[REST_NUM][VAR_NUM+1], int var_base[REST_NUM]) {
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
bool otimizavel(double func[], int *var) {
	*var = 0;
	for(int i = 1;i < VAR_NUM;i++) {
		if(func[i] < func[*var]) {
			*var = i;
		}
	}
	
	return (func[*var] < 0);
}

// Retorna qual função possui a menor restrição positiva
int maior_otimizacao(double func_rest[REST_NUM][VAR_NUM+1], int var) {
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
void simplificar_eq(double func_rest[REST_NUM][VAR_NUM+1], int pivot, int var) {
	double fator = func_rest[pivot][var];
	for(int i = 0;i <= VAR_NUM;i++) {
		func_rest[pivot][i] /= fator;
	}	
}

// Zera o coeficiente da variavel que entra na base das demais equações
void zerar_variaveis(double func_obj[], double func_rest[REST_NUM][VAR_NUM+1], int pivot, int var) {
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
void simplex(double func_obj[], double func_rest[REST_NUM][VAR_NUM+1], int var_base[REST_NUM]) {
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

int main() {
	// Variaveis que começam na base, no caso: x3, x4, x5, x6
	int var_base[REST_NUM] = {2, 3, 4, 5};
	// Coeficientes das variaveis na função objetivo, o ultimo valor corresponde ao lucro obtido
	double func_obj[VAR_NUM+1] = {-8.0, -5.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	// Coeficientes das variaveis na matriz na forma padrão, o ultimo valor de cada linha corresponde ao valor obtido
	double func_rest[REST_NUM][VAR_NUM+1] =	{{2.0, 1.0 , 1.0, 0.0, 0.0, 0.0, 1200.0},
											{3.0, 4.0, 0.0, 1.0, 0.0, 0.0, 2400.0},
											{1.0, 1.0, 0.0, 0.0, 1.0, 0.0, 800.0},
											{1.0, -1.0, 0.0, 0.0, 0.0, 1.0, 450.0}};
	simplex(func_obj, func_rest, var_base);
	return 0;
}