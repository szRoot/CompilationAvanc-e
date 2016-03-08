#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define C mot & 7
#define B (mot >> 3) & 7
#define A (mot >> 6) & 7
#define SP (mot >> 25) & 7
#define VAL mot & 0x1ffffff


unsigned int get_file_size(char *fichier){
	  FILE *fp;
	  unsigned int size = 0;
	  if ((fp = fopen(fichier, "rb" )) == NULL) {
	    fprintf(stderr, "erreur lors d'ouverture du fichier : %s.\n", fichier);
	    return(size);
	  }
	  if (fseek(fp, (long)(0), SEEK_END) != 0) {
	    fclose(fp); return(size);
	  }
	  size = (unsigned int) (ftell(fp));
	  fclose(fp);
	  return(size);
}


int main (int argc, char ** argv) {
    
	static unsigned int reg[8], pc, * initTab;
	int a, i = 0;
	FILE * file ;

	//ouverture du fichier 
	file = fopen(argv[1], "rb");
	if (file == NULL){
		fprintf(stderr, "erreur lors d'ouverture du fichier :%s .\n",argv[1]);  exit(-1);
 	}

	//initialisation du tableau initTab et le remplir a partir du fichier
  	initTab = calloc( (unsigned int) get_file_size(argv[1])/4,4) ;

	while(EOF != (a = fgetc(file))) {
    		a=a << 24|fgetc(file)<< 16 | fgetc(file) << 8 |fgetc(file) ;
    		initTab[i] =  a;
    		i++;
  	}
  	fclose(file);

    while(1) {
        unsigned int mot = initTab[pc++];
        switch(mot >> 28) {
       		case 0: if (reg[C]) reg[A] = reg[B]; break;

		case 1: if (reg[B]==0){reg[A] = ((unsigned int*)initTab)[reg[C]]; break ;} 
 		        reg[A] = ((unsigned int*) reg[B])[reg[C]];break; 

		case 2: if (reg[A]==0){ ((unsigned int*)initTab)[reg[B]] = reg[C]; break ;} 
		        ((unsigned int*) reg[A])[reg[B]] = reg[C]; break; 

         	case 3: reg[A] = reg[B] + reg[C]; break; 
    
	        case 4: reg[A] = reg[B] * reg[C]; break; 
    
		case 5: reg[A] = reg[B] / reg[C]; break; 
    
		case 6: reg[A] = ~(reg[B] & reg[C]); break;  

		case 7: printf("\n \n =====Fin du Programme===== \n \n"); return 0;  
                       
        	case 8: {
			unsigned int *r = (unsigned int*)calloc((reg[C]+1),4);
			r[0]=reg[C];
			reg[B]=(unsigned int)(r+1);  break;
		        }

		case 9: free( (unsigned int*)reg[C]-1 ); break;  
   
		case 10: putchar(reg[C]); break;        
     
		case 11: reg[C] = getchar(); break; 
       
        	case 12:{                                    
            		  if (reg[B]) {
               		    free(initTab );
                	    int size = ((unsigned int*)reg[B])[-1];
                	    initTab = (unsigned int*)malloc(size*4);
                	    memcpy(initTab, (unsigned int*)reg[B], size * 4);
            		  }
		          pc = reg[C]; break;
			}
            
	       case 13: reg[SP] = VAL; 
        }
    }

}
