#include <stdio.h>
#include <stdlib.h>

typedef struct item
{
    int valor;
    int peso;
} ITEM;

int **aloca_matriz(int linhas, int colunas) //alocacao da matriz com os itens
{
    int **m = (int **)malloc(linhas * sizeof(int *));
    if (!m)
        return NULL;
    for (int i = 0; i < linhas; i++)
    {
        m[i] = (int *)calloc(colunas, sizeof(int)); //calloc usado para evitar necessidade de zerar manualmente primeira linha e coluna
        if (!m[i])
        {
            for (int k = 0; k < i; k++)
                free(m[k]);
            free(m);
            return NULL;
        }
    }
    return m;
}

int main(void)
{
    int n, W;
    scanf("%d", &n);

    ITEM *itens = (ITEM *)malloc(n * sizeof(ITEM));
    if (itens == NULL)
        return 0;

    for (int i = 0; i < n; i++)
    {
        // leia: valor peso (consistent with other algorithms)
        scanf("%d %d", &itens[i].valor, &itens[i].peso);
    }
    
    scanf("%d", &W);

    // aloca uma matriz n+1 por w+1, que corresponde aos possíveis pesos e considerando inclusões e exclusoes de itens(abordagem bottom up)
    int **m = aloca_matriz(n + 1, W + 1); //elementos são todos alocados inicialmente como 0(calloc) para evitar colocar 0s desnecessários(primeira linha e primeira coluna)
    if (m == NULL)
    {
        free(itens);
        return 0;
    }

    for (int i = 1; i <= n; i++)
    {
        int p = itens[i - 1].peso; //peso e valor do item da linha
        int v = itens[i - 1].valor;
        for (int w = 0; w <= W; w++)
        {
            m[i][w] = m[i - 1][w];  //caso inicial em que o valor para certo peso w não inclui o item itens[i-1].valor
            if (p <= w)
            {
                int pega = m[i - 1][w - p] + v; //caso em que se usa o item itens[i-1].valor para determinado peso w 
                if (pega > m[i][w]) //confere qual dos dois casos é mais vantajoso e se necessario troca
                    m[i][w] = pega; 
            }
        }
    }

    int melhor = m[n][W]; //melhor valor é sempre o do final da tabela(canto inferior direito)
    printf("Valor: %d\n", melhor);

    //conferir quais itens foram pegos
    printf("Itens: ");
    int *selecionados = (int *)calloc(n, sizeof(int)); // Track selected items
    int w = W;
    for (int i = n; i >= 1; i--)
    {
        if (m[i][w] != m[i - 1][w])
        {
            //caso em que item i-1 foi pego
            selecionados[i - 1] = 1;
            w -= itens[i - 1].peso;
        }
    }
    
    // Print selected items in correct order as letters
    for (int i = 0; i < n; i++)
    {
        if (selecionados[i])
        {
            printf("%c ", (char)(i + 65)); // Convert to letters A, B, C...
        }
    }
    printf("\n");
    
    free(selecionados);


    //desalocação de memória
    for (int i = 0; i <= n; i++) 
        free(m[i]);
    free(m);
    free(itens);
    return 0;
}
