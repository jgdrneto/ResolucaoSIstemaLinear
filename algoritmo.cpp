#include <vector>
#include <cmath>

#include "matriz.cpp"

//Constante com o valor de erro mínimo que pode ocorrer
#define ERRO 0.01
//Constante de número de máximos de vezes que se executa o algoritmo
#define TOL 100

//Classe static para uso dos algoritmos
class Algoritmo{
	private:

		/**
		* Descrição: Método que verifica se é preciso trocar a linha
		* @return int : número da linha a ser trocada pela 1º linha
		*/
		int precisaTrocarLinha(Matriz& matrizA){
			//Número da linha do maior valor da 1º coluna
			int v;

			//Iniciando procura
			double valor = matrizA.getMatriz()[0][0];
			v=0;
			
			for(int i=1;i<matrizA.getNumeroL();i++){
				if(matrizA.getMatriz()[0][i]>valor){
					valor = matrizA.getMatriz()[0][i];
					v = i;
				}
			}
			return v;
		}	

		/**
		* Descrição: Método que verifica se é o sistema é possível
		* @params Matriz& : Matriz que se deseja fazer a análise 
		* @return bool : TRUE para caso seja possivel o sistema e FAlSE caso não seja	
		*/
		bool sistemaPossivel(Matriz& m){
			//Verificando se o sistema é possível
			for(int j=0;j<m.getNumeroL();j++){
				if(m.getMatriz()[j][j]==0){
					return false;
				}
			}
			return true;	
		}

		/**
		* Descrição: Método que usa a fotaração LU com pivotação parcial
		* @params Matriz& : matriz que se deseja executar o algoritmo (equações)
		* @params Matriz& : matriz que se deseja executar o algoritmo (respostas)
		* @return std::vector<double> : Vetor com as soluções.
		*/
		std::vector<double>& encontrarSolucao(Matriz& matrizA, Matriz& matrizB){
			
			double soma = 0;
			double aux;

			//Vetor com as soluções
			std::vector<double>* resultados = new std::vector<double>(matrizB.getNumeroL(),1);

			//Encontrando cada solução
			for(int i = matrizA.getNumeroL()-1; i >= 0; i--){
				for(int j = 0; j < matrizA.getNumeroC(); j++){
					if(i == j){
						aux = matrizA.getMatriz()[i][j];
					}else{
						soma += matrizA.getMatriz()[i][j] * ((*resultados)[j]);
					}
				}

				soma = soma * -1;
				((*resultados)[i]) = matrizB.getMatriz()[i][0] + soma;
				((*resultados)[i]) = ((*resultados)[i])/aux;
				soma = 0.0;
			}

			return (*resultados);
		}

	public:
		/**
		* Descrição: Método que usa a fotaração LU com pivotação parcial
		* @params Matriz& : matriz que se deseja executar o algoritmo (equações)
		* @params Matriz& : matriz que se deseja executar o algoritmo (respostas)
		* @return std::vector<double> : vetor com a solução das equações lineares da matriz 
		*/
		
		std::vector<double> fatoracaoLUcomPivotacaoParcial(Matriz& matrizA, Matriz& matrizB){
			//Falta Implementar
	
			//Verifica se precisa trocar as linha
			int linha = precisaTrocarLinha(matrizA);
			
			//Troca a linha se o maior valor não estiver na 1º linha
			if(linha>0){				
				matrizA.trocarLinhas(linha);
				matrizB.trocarLinhas(linha);
			}

			//Criando a matriz de Gauss
			Matriz Gauss(matrizA.getNumeroL(),matrizA.getNumeroC());

			//Gerando L e U
			for(int i=0;i<Gauss.getNumeroC()-1;i++){

				//Começa pela identidade
				Gauss = Gauss.identidade();

				//Criando as matrizes da Gauss
				for(int r = i+1; r < Gauss.getNumeroL(); r++){
					Gauss.getMatriz()[r][i] =  matrizA.getMatriz()[r][i] * (-1.0) / matrizA.getMatriz()[i][i];
				}

				//Gerando L e U
				matrizA = Gauss*matrizA;
				matrizB = Gauss*matrizB;	

				if(!sistemaPossivel(matrizA)){
					cout << "Sistema é impossível ou a matriz apresenta linhas ou colunas linearmente dependentes" << endl;
					return std::vector<double>();
				}
				
			}
			//retoraa vetor com as soluções
			return encontrarSolucao(matrizA,matrizB);
		}
		
		/**
		* Descrição: Método que usa a fotaração de Cholesky
		* @params Matriz& : matriz que se deseja executar o algoritmo (equações)
		* @params Matriz& : matriz que se deseja executar o algoritmo (respostas)
		* @return std::vector<double> : vetor com a solução das equações lineares da matriz 
		*/
		
		std::vector<double> cholesky(Matriz& matrizA, Matriz& matrizB){
			//Falta Implementar

			std::vector<double> resultados(matrizB.getNumeroL());

			//Verificando se é simátrica
			if(matrizA.verificaSimetria() && matrizA.getNumeroL()>=2 && matrizA.getNumeroC()>=2){
				
				Matriz L = matrizA.identidade();

				//Encontrando sua identidade
				Matriz Gauss = matrizA.identidade();

				//Encontrando L
				for(int i = 0; i < matrizA.getNumeroL()-1;i++){
					Gauss = Gauss.identidade();

					for(int r = i+1; r < Gauss.getNumeroL(); r++){
						Gauss.getMatriz()[r][i] =  matrizA.getMatriz()[r][i] * (-1.0) / matrizA.getMatriz()[i][i];
					}

					L = Gauss*L;
					matrizA = Gauss*matrizA;
					matrizB = Gauss*matrizB;

					//Verifica se o sistema é possível
					if(!sistemaPossivel(matrizA)){
						return std::vector<double>();
					}
				}

				//Verifica se o sistema é possível
				if(matrizA.positivaDefinida()){
					return std::vector<double>();
				}

				//Encontra a matriz diagonal
				Matriz diagonal = matrizA.diagonalMat();
				//Faz a transposta de L
				L.transposta();
				//Inverte os valores da diagonal da matriz diagonal
				diagonal.inverteD();
				//Multiplicando L pela diagonal
				diagonal = L*diagonal;
				//Multiplicando a diagonal por matrizB
				matrizB = diagonal*matrizB; 

				//Colocando tudo em um vetor de double
				for(int i=0;i<matrizB.getNumeroL();i++){
					resultados[i] = matrizB.getMatriz()[i][0];
				}

				return resultados;
			}else{
				cout << "A matriz não é simétrica. Não é possível usar Cholesky!" << endl;
			}

			return std::vector<double>();
		}
		
		/**
		* Descrição: Método que usa a fotaração de Gauss-Siegel
		* @params Matriz& : matriz que se deseja executar o algoritmo (equações)
		* @params Matriz& : matriz que se deseja executar o algoritmo (respostas)
		* @return std::vector<double> : vetor com a solução das equações lineares da matriz 
		*/
		
		std::vector<double> gaussSiegel (Matriz& matrizA, Matriz& matrizB){
			
			std::vector<double> resultados(matrizB.getNumeroL());

			if(matrizA.converge()){

				//variável para guardar o número de interações
				int i=0;

				//Variável para soma
				double soma=0;

				//Matriz coluna com a resposta inicial
				Matriz matrizX0(matrizB.getNumeroL(), matrizB.getNumeroC());

				//Matriz com coluna a resposta real
				Matriz matrizX(matrizB.getNumeroL(), matrizB.getNumeroC());

				//Solução inicial
				for(int i=0;i<matrizX.getNumeroL();i++){
					matrizX0.getMatriz()[i][0] = 0;
					matrizX.getMatriz()[i][0] = 0;
				}

				//Verificar se converge
				while(i <=TOL){
					//Iteração de Gauss-Siegel
			        for (int i = 0; i<matrizA.getNumeroL();i++){
			            soma = 0;
			            for(int j = 0;j<matrizA.getNumeroC();j++){
			                if(i!=j){
			                	soma += matrizA.getMatriz()[i][j]*matrizX.getMatriz()[j][0];
			        		}
			        	}

			            matrizX.getMatriz()[i][0] = (matrizB.getMatriz()[i][0] - soma)/matrizA.getMatriz()[i][i];
			        
			        }

			        //Verificando se o resultado já é satisfatório
			        if(abs((matrizX-matrizX0).maiorValor()) <= ERRO){
			        	
			        	for(int i=0;i<matrizX.getNumeroL();i++){
			    			resultados[i] = matrizX.getMatriz()[i][0];
			   			}

			   			return resultados;

			        }
			        
			        //Atribuindo valor para a proxima interação	
			        for(int i=0;i<matrizX.getNumeroL();i++){
			        	for(int j=0;j<matrizX.getNumeroC();j++){
			        		matrizX0.getMatriz()[i][j]=matrizX.getMatriz()[i][j];
			        	}
			        }

					//Aumentando contador
			        i++;
			    }   

			    cout << "ERRO: Número de interações maximas atingido" <<endl;
			}else{
				cout << "A matriz não converge" << endl;
			}
			return std::vector<double>(); 
		}

		/**
		* Descrição: Imprime a resolução de um algoritmo na tela.
		*/
		void imprimirResposta(std::vector<double> solucao){
			//Contador
			int cont=0;

			//Imprimindo respostas
			for (double s : solucao){

				cout << "Para x_" << cont+1 << " : " << s << endl;

				cont++;
			}	
		}

		/**
		* Descrição: Método usado para testes da fatoração LU com pivotamento parcial
		* @params Matriz& : matriz que se deseja executar o algoritmo (equações)
		* @params Matriz& : matriz que se deseja executar o algoritmo (respostas)
		* @params vector<double> : Vetor com as soluções reais do problema
		*/
		void verificarSolucaoFatoracaoLUcomPivotacaoParcial(Matriz& matrizA,Matriz& matrizB, vector<double> solucoesReais){
			
			//Aplica o algoritmo e armazena resultados em um vetor
			std::vector<double> solucoes = this->fatoracaoLUcomPivotacaoParcial(matrizA, matrizB);

			//Imprime a correção na tela
			this->imprimirVeririficao(solucoesReais,solucoes);
		}

		/**
		* Descrição: Método usado para testes da fatoração de Cholesky
		* @params Matriz& : matriz que se deseja executar o algoritmo (equações)
		* @params Matriz& : matriz que se deseja executar o algoritmo (respostas)
		* @params vector<double> : Vetor com as soluções reais do problema
		*/
		void verificarSolucaoCholesky(Matriz& matrizA,Matriz& matrizB, vector<double> solucoesReais){
			
			//Aplica o algoritmo e armazena resultados em um vetor
			std::vector<double> solucoes = this->cholesky(matrizA, matrizB);

			//Imprime a correção na tela
			this->imprimirVeririficao(solucoesReais,solucoes);
		}

		/**
		* Descrição: Método usado para testes da fatoração de Gauss-Siegel
		* @params Matriz& : matriz que se deseja executar o algoritmo (equações)
		* @params Matriz& : matriz que se deseja executar o algoritmo (respostas)
		* @params vector<double> : Vetor com as soluções reais do problema
		*/
		void verificarSolucaoGaussSiegel(Matriz& matrizA,Matriz& matrizB, vector<double> solucoesReais){
			
			//Aplica o algoritmo e armazena resultados em um vetor
			std::vector<double> solucoes = this->gaussSiegel(matrizA, matrizB);

			//Imprime a correção na tela
			this->imprimirVeririficao(solucoesReais,solucoes);
		}

		/**
		* Descrição: Método que imprime na tela o resultado da correção da solução real com o valor encontrado
		*/
		void imprimirVeririficao(vector<double> solucoesReais,std::vector<double> solucoes){
			//imprime o resultado na tela
			int cont=0;
			for (double s : solucoes){

				cout << "Para x_" << cont+1 << " : ";

				if( s < (solucoesReais[cont] + ERRO) && s > (solucoesReais[cont] - ERRO)){
					cout << "Certo, número aproximado " << s << endl;
				}else{
					cout << "Errado, número aproximado " << s << endl;
				}

				//Aumentando contador
				cont++;
			}
		}	
};	
		