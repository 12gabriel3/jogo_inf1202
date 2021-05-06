#include <env.h>


void update_env(ENV *col)
{
    if(col->anim)
        al_draw_bitmap(animate(col->anim),
                       col->pos_graphic.x,
                       col->pos_graphic.y,
                       0);
    else
        al_draw_bitmap(col->sprite->bitmap,
                       col->pos_graphic.x,
                       col->pos_graphic.y,
                       0);
}

void Atualiza_jogo(MONSTROS *Monstro,POSICAO *Change, char Nome_Mapa[], char Mapa1 [][COLUNA], OBJ *Teste1)
{
    FILE *Arq;
    int N_monstro = 0, N_troll = 0, N_zumbi = 0, i;
    (Teste1->N_Parede) = 0;
    (Teste1->N_Espinho) = 0;
    for(i=0; i<=N_Max; i++)
        {
            (Teste1->Parede_x[i]) = 0;
            (Teste1->Parede_x[i]) = 0;
        }
    for(i=0; i<=N_Max; i++)
        {
            (Teste1->Espinho_x[i]) = 0;
            (Teste1->Espinho_y[i]) = 0;
        }

    Busca(Nome_Mapa, Arq, Change, Monstro, Teste1,&N_troll, &N_zumbi,Mapa1);
}



int Salva_Jogo(char mapa[][COLUNA],char nome_arquivo_out[MAX_NOME], OBJ *obj)
{
    FILE *arq;
    int i,falha = 0;
    if((arq = fopen((nome_arquivo_out),"w+")))
        {
            for(i=0; i<LINHA; i++)
                fputs(mapa[i],arq);

            if(fprintf(arq,"\n\n--------------------------------------------------------------\n")< 0)
                falha = 1;
            if(fprintf(arq,"\nFase: %d\n",obj->Fase) < 0)
                falha = 1;
            if(fprintf(arq,"\nVidas: %d\n",obj->Vida) < 0)
                falha = 1;
            if(fprintf(arq,"\nPontos: %d\n",obj->Pontos) < 0)
                falha = 1;

        }
    else
        falha = 1;

    if(!falha)
        return 1;
    else
        return 0;
    fclose(arq);
}

void Busca(char nome_arquivo_inp[MAX_NOME], FILE *arq, POSICAO *pos, MONSTROS *monstro, OBJ *teste, int *N_Troll, int *N_Zumbi, char mapa[][COLUNA])
{
    int i,j,n_monstro = 0,N_Parede = 0,n_espinhos = 0;
    char texto_str[COLUNA];

    if(!(arq = fopen((nome_arquivo_inp),"r")))
        {
            printf("\n\n Erro na abertura do arquivo de leitura! \n\n");              // Escreve mensagem de erro caso não consiga executar a funcao
        }
    else
        {
            do
                {
                    while(fgets(texto_str, COLUNA, arq) != NULL)
                        {
                            strncpy(mapa[i],texto_str,COLUNA);
                            i++;
                        }
                }
            while(!feof(arq));
            fclose(arq);

            for(i=0; i<LINHA; i++)
                {
                    Coordenada(i,0, &(pos->coordenada_x), &(pos->coordenada_y), 'J', mapa[i]);

                    Coordenada(i,0, &(teste->Bau_x), &(teste->Bau_y), 'B', mapa[i]);

                    Coordenada(i,0, &(monstro->coordenada_x[n_monstro]), &(monstro->coordenada_y[n_monstro]), 'T', mapa[i]);
                    if(((monstro->coordenada_x[n_monstro])!= 0)||((monstro->coordenada_y[n_monstro]) != 0))                                  //verifica se a posição atual da matriz ainda é zero, caso contrario atualiza o numero de OGROS no mapa
                        {
                            (monstro->Typo[n_monstro]) = TROLL;
                            n_monstro += 1;
                            *N_Troll +=1;
                        }

                    Coordenada(i,0, &(monstro->coordenada_x[n_monstro]), &(monstro->coordenada_y[n_monstro]), 'Z', mapa[i]);
                    if(((monstro->coordenada_x[n_monstro])!= 0)||((monstro->coordenada_y[n_monstro]) != 0))                                  //verifica se a posição atual da matriz ainda é zero, caso contrario atualiza o numero de OGROS no mapa
                        {
                            (monstro->Typo[n_monstro]) = ZUMBI;
                            n_monstro += 1;
                            *N_Zumbi +=1;
                        }



                    for(j=0; j<COLUNA; j++)
                        {
                            Coordenada(i,j, &(teste->Parede_x[(teste->N_Parede)]), &(teste->Parede_y[(teste->N_Parede)]), '#', mapa[i]);
                            if(((teste->Parede_x[(teste->N_Parede)])!= 0)||((teste->Parede_y[(teste->N_Parede)]) != 0))                                  //verifica se a posição atual da matriz ainda é zero, caso contrario atualiza o numero de OGROS no mapa
                                {
                                    (teste->N_Parede) += 1;
                                }
                        }
                    for(j=0; j<COLUNA; j++)
                        {
                            Coordenada(i,j, &(teste->Espinho_x[(teste->N_Espinho)]), &(teste->Espinho_y[(teste->N_Espinho)]), 'X', mapa[i]);
                            if(((teste->Espinho_x[(teste->N_Espinho)])!= 0)||((teste->Espinho_y[(teste->N_Espinho)]) != 0))                                  //verifica se a posição atual da matriz ainda é zero, caso contrario atualiza o numero de OGROS no mapa
                                {
                                    (teste->N_Espinho) += 1;
                                }
                        }
                }
        }
}

void Coordenada(int linha,int coluna,int *cord_x,int *cord_y, char busca, char Mapa[])
{
    int corre_coluna;
    if((busca != '#') && (busca != 'X'))
        {
            for(corre_coluna = 0; corre_coluna < COLUNA; corre_coluna ++)
                {
                    if(Mapa[corre_coluna] == busca)
                        {

                            *cord_y = corre_coluna;
                            *cord_x = linha;
                        }
                }
        }
    if((busca == '#') || (busca == 'X'))
        {
            if(Mapa[coluna] == busca)
                {
                    *cord_y = coluna;
                    *cord_x = linha;
                }
        }
}

