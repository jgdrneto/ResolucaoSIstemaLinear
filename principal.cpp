//Incluindo biblioteca necessárias
#include <locale.h>
#include <string.h>

#include "algoritmo.cpp"

/**
* Descrição: Limpa o console
*/
void limpaTela(){
	if(system("clear")){
		system("cls");
	}
}

/**
* Descrição: Aplica o algorimo escolhido pelo usuário
* @params Matriz& : Endereço do objeto da classe Matriz que se deseja usar no algoritmo(equações)
* @params Matriz& : Endereço do objeto da classe Matriz que se deseja usar no algoritmo(respostas)
# @params Algortimo& : Endereço do objeto da classe Algoritmo que aplicará os algoritmo na matriz.
* @params int : número do algoritmo que se deseja executar
*/
void aplicarAlgorimo(int escolha, Algoritmo& algoritmo , Matriz& matrizA, Matriz& matrizB){

	switch(escolha){
		case 1:
			algoritmo.imprimirResposta(algoritmo.fatoracaoLUcomPivotacaoParcial(matrizA, matrizB));
		break;
		case 2:
			algoritmo.imprimirResposta(algoritmo.cholesky(matrizA, matrizB));
		break;
		case 3:
			algoritmo.imprimirResposta(algoritmo.gaussSiegel(matrizA, matrizB));
		break;	
	}
}

/**
* Descrição: Escreve o menu de escolha dos algoritmos no terminal
* @return : Inteiro representando a escolha do usuário
*/
int menu(){
	
	//Inicializando variáveis
	int escolha = 0;	
	//Menu para escolha do método que se deseja utilizar
	do {
		cout << "Qual método deseja usar: " << endl;
		cout << "1 - Método de fatoração LU, com pivotação parcial" <<endl;
		cout << "2 - Método de fatoração de Cholesky" << endl;
		cout << "3 - método de fatoração de Gauss-Siegel" << endl;
		cout << "-> ";
		cin >> escolha;
	}while(escolha <1 || escolha >3);

	//Limpa a tela do console
	limpaTela();

	//Retorna o valor da escolha
	return escolha;
	
}

/**
* Descrição: Verifica se a versão usada será a de testes
* @params int : Quantidade de argumentos no comando de compilação
* @params char* : Vetor com os argumentos do comando de compilação
* @return : Verdadeiro se existe o programa com o nome TESTE e falso caso contrário
*/
bool teste(int argc,char* argv[]){
	for(int i=0;i<argc;i++){
		if(strcmp(argv[i], "TESTE")==0){
			return true;
		}
	}
	return false;
}	

void preencherValores(Matriz& matrizA, Matriz& matrizB, std::vector<double>& solucoesReais){
	//Adicionando valores estáticos a matriz
	matrizA.getMatriz()[0][0] = 1;
	matrizA.getMatriz()[0][1] = 0.2;
	matrizA.getMatriz()[0][2] = 0.2;
	matrizA.getMatriz()[1][0] = 0.75;
	matrizA.getMatriz()[1][1] = 1;
	matrizA.getMatriz()[1][2] = 0.25;
	matrizA.getMatriz()[2][0] = 0.5;
	matrizA.getMatriz()[2][1] = 0.5;
	matrizA.getMatriz()[2][2] = 1;
			
	matrizB.getMatriz()[0][0] = 1;
	matrizB.getMatriz()[1][0] = 1.5;
	matrizB.getMatriz()[2][0] = 0;

	solucoesReais[0]=1;
	solucoesReais[1]=1;
	solucoesReais[2]=-1;
	
}

//Método principal do programa
int main(int argc, char*argv[]){
	
	//Inicializando variáveis	
	int n = -1;
	Algoritmo algoritmo;	
	//Permitindo caracteres especiais
	setlocale(LC_ALL, "Portuguese");
		
	//Verifica se foi solicitado o teste ou não
	if(!teste(argc,argv)){	
		//Estrutura de repetição para decidir o tamanho da matriz.
		do{
			cout << "Digite o n para a matriz nxn que se deseja usar: ";
			cin >> n;
		}while(n == -1);

		//Cria a matriz
		Matriz matrizA(n,n);
		Matriz matrizB(n,1);
		
		cout << "Valores associados aos coeficientes das equações:\n" << endl;

		//Adiciona os valores númericos na matriz
		matrizA.adicionarValores();

		cout << "Valores da soluções:\n" << endl;

		matrizB.adicionarValores();

	 	//Aplicando os algoritmos
		aplicarAlgorimo(menu(),algoritmo, matrizA,matrizB);
	}else{
		
		//Área reservada a testes

		//Matriz para testes
		Matriz matrizA(3,3); //Matriz
		Matriz matrizB(3,1); //Respostas	
		std::vector<double> solucoesReais(3,0);

		//Adicionando valores estáticos a matriz
		preencherValores(matrizA, matrizB,solucoesReais);

		cout << "Matriz A:" << endl;
		matrizA.imprimir();
		cout << "Matriz B:" << endl;
		matrizB.imprimir();

		//Verificando soluções por LU

		cout << "Usando Método da fatoração LU: " << endl;

		algoritmo.verificarSolucaoFatoracaoLUcomPivotacaoParcial(matrizA, matrizB,solucoesReais);

		//Verificando soluções por Cholesky

		//Adicionando valores estáticos a matriz
		preencherValores(matrizA, matrizB,solucoesReais);

		cout << "Usando Método de Cholesky: " << endl;

		algoritmo.verificarSolucaoCholesky(matrizA, matrizB,solucoesReais);		

		//Verificando soluções por Gauss-Siegel

		//Adicionando valores estáticos a matriz
		preencherValores(matrizA, matrizB,solucoesReais);

		cout << "Usando Método de Gauss-Siegel: " << endl;

		algoritmo.verificarSolucaoGaussSiegel(matrizA, matrizB,solucoesReais);	
		
	}
}