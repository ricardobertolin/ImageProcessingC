int main() {

    char arq[] = "01.ppm";  
    FILE *fp;  
    
    char id[3];
    int qtdCol, qtdRow, maximo;
    unsigned char r, g, b;
    unsigned char rf, gf, bf;
    int i, j, k;
    int posicao;

    structPos pos;
    structStack *pilha;
    structPixel **img;

    fp = fopen(arq, "rb"); 
    
    if (fp == NULL) {  
        printf("Erro na abertura do arquivo <%s>", arq);
        exit(EXIT_FAILURE); 
    }

    fscanf (fp, "%s", id);        // identificador;
    fscanf (fp, "%d", &qtdCol);  // largura da imagem = total de qtdCol;
    fscanf (fp, "%d", &qtdRow);   // altura da imagem = total de qtdRow;
    fscanf (fp, "%d", &maximo);   // máximo de intensidade ;
    fscanf (fp, "%c", &r);        // consumir um quebra de linha no arquivo;

    printf("Tipo do arquivo <%s>: %s\n", arq, id);
    printf("Maximo: %d\n", maximo);
    printf("Imagem tem %d x %d pixels\n", qtdRow, qtdCol);

	// criar a pilha que permite push e popStack de coordenadas;
    pilha = criaStack();
	
    img = (structPixel **)calloc (qtdRow, sizeof(structPixel *));
    for(i = 0; i < qtdRow; i++)
    {
        img[i]= (structPixel *) calloc (qtdCol, sizeof(structPixel));
    }
    // lendo os pixels da imagem, para cada pixel ler 3 valores, Red Green Blue (a cor em RGB);
    posicao = ftell(fp); // pegar a posicao fisica no arquivo anter de ler dados da imagem;
    // le o pixel 0,0 que é o pixel com a cor do fundo;
    fscanf (fp, "%c%c%c", &rf, &gf, &bf);
    fseek(fp, posicao, SEEK_SET); // voltando ao início da imagem;

    

//esse FOR aqui verifica todas as linhas e colunas atrás de pixels que tem cores diferente do fundo;
for (i=0; i<qtdRow; i++) {
        for (j=0; j<qtdCol; j++) {
            //esse fscanf está pegando os valores de r g b retirados do arquivo ppm que está aberto em fp;
            fscanf (fp, "%c%c%c", &r, &g, &b);
            if ((r!=rf) || (g!=gf)  || (b!=bf)  ){  // aqui se houve mudança de cor em relação ao fundo, então
                // para o pixel diferente, ele terá seu p setado para 1, para indicar que ele é diferente do fundo;
                img[i][j].p = 1;
            } 
        }
    } 
    //FOR para não sobrar pixels na imagem
    fclose(fp);
    
    int cont; //cria um contador, serve pra guardar quantos objetos tem na imagem;

    //Esses dois FOR vão verificar de novo a imagem, mas dessa vez buscando por pixels não visitados, com intenção de encher a pilha e ir verificando os vizinhos;
    for(i=0; i<qtdRow; i++)
    {
        for(j=0; j<qtdCol; j++)
        {
            //Buscar por um pixel que seja diferente e não seja visitado;
            if(img[i][j].p == 1 && img[i][j].v == 0)
            {
                //Encontrando um pixel diferente, chama a function EdgeScan, passando como parâmetro a struct de pixels, a struct da pilha, a posição I e J, e o tamanho da imagem em linhas e colunas;
                EdgeScan(img,pilha,i,j,qtdRow,qtdCol);
                //enquanto a pilha não for vazia,
                while(pilha->qtd>0)
                {
                    //retira a posição da pilha e
                    popStack(pilha,&pos);
                    //verifica os vizinhos, que por sua vez podem continuar adicionando na pilha;
                    EdgeScan(img, pilha, pos.x, pos.y, qtdRow, qtdCol);
                }
                //terminando de verificar quantos pixels são diferentes do fundo, a pilha vai estar vazia
                //sendo assim, os pixels visitados não serão visitados novamente e incrementa o contador de objetos em 1;
                cont++;
            }
        }
    }
    //imprime a quantidade total de objetos que o algoritmo encontrou na imagem e libera a memória ocupada pela struct de pixels e struct de pilha;
    printf("\n");
    printf("A imagem tem %d objetos no total.", cont);
    free(pilha);
    free(img);

    return 0;