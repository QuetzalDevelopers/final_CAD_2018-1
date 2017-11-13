#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int lecturaTeclado(int n, int *mat); 
void lecturaDeArchivoTXT(FILE * fichero, char * cad, int n);

int main(int argc, char const *argv[])
{
	char *cad;
	int tam,*mat,n;
	FILE* fichero; 

	n=lecturaTeclado(n,mat);	
	lecturaDeArchivoTXT(fichero, cad, n);

	return 0;

}
int lecturaTeclado(int n, int * mat){
	//funcion que permite la lectura de datos de teclado de n y de la matriz
	int i,j,val; 
	printf("Ingrese el valor de n\n");
	scanf("%d",&n);
	mat = (int *)malloc((n*n)* sizeof(int));
	printf("Ingrese la matriz de %d*%d\n",n,n);
	//validar si es transponible?
	// el scanf no permite el ingreso de una matriz menor al tamaño establecido
	for(i=0; i<(n*n); i++){
			scanf("%d",&val);
			mat[i]=val;
	}
	return n;
}

void lecturaDeArchivoTXT(FILE * fichero, char * cad, int n){
	int letra, contador,i,j,inicio,fin,modulo;
	fichero = fopen("ejemplo.txt","r");
	contador=0; 
	while(letra!=EOF)//para saber el tamaño de la cadena
    {
        letra= getc(fichero);  //obtienes un caracter y lo guardas, el valor es su equivalente en ascii, por eso es de tipo int
        contador++;
    }
    fclose(fichero);
    contador=contador-1;
    modulo= contador%n;//revisar si se pueden agrupar en grupos de n
   	printf("modulo=> %d mod %d = %d\n",contador,n, modulo);
   	if(modulo>0){
    		contador=contador+(n-modulo); 
    	}
    
    cad  = (char *)malloc(contador * sizeof(char));
    fichero = fopen("ejemplo.txt","r");
    for(i=0; i<contador; i++){
    	cad[i]=getc(fichero);
    	if(i>(contador-n)){
    		cad[i]='x';
    	}
    }
	fclose(fichero);
	inicio=0;
	fin=n;
	printf("\n");
    
    //para imprimir en periodos de n 
	for(i=0; i<(contador/n); i++){
		for (j =inicio; j < fin; ++j){
			printf("%c",cad[j]);	
		}
		printf("\n");
		inicio=inicio+n;
		fin=inicio+n; 
	}
		
}



