# Projeto 01 - Escada Rolante Dupla

## 1. Qual a estratégia que você utilizou para evitar que duas pessoas acessem a escada rolante ao mesmo tempo em cada abordagem?

Foram utilizadas duas filas, uma para cada sentido (0: esquerda para direita; 1: direita para esquerda). Conforme o tempo de chegada e de saída de cada pessoa, bem como o sentido em que a escada está funcionando no momento, uma ou outra fila será ativada. O gerenciamento do tempo de cada pessoa é feito da mesma forma, independentemente do sentido. Segue abaixo o algoritmo referente à lógica empregada para o controle de fluxo.

Inicialmente, é determinado o sentido da primeira pessoa que acessa a escada, tomando o menor tempo de chegada dentre as duas filas.

```c
// Setting the first value;
    if (lenFQ == 0) first = secondQueue[0];
    else if (lenSQ == 0) first = firstQueue[0];
    else first = firstQueue[0].time < secondQueue[0].time ? firstQueue[0] : secondQueue[0];
```

A partir desta seleção, o controle de acesso à escada é feito com base no sentido da próxima pessoa, que será identificada pela variável `first`.

```c
// going from left to right
        if (first.direction == 0) {

            // follows the same direction if the time of arrival is less than the last exit time or it's immediately next to the previous one
            if ((aux1 < lenFQ && (firstQueue[aux1].time <= out) || (firstQueue[aux1].time > out && firstQueue[aux1].time < secondQueue[aux2].time)) || aux2 == lenSQ) {
                first = firstQueue[aux1];               
                ++aux1;   

            // if the time of arrival is greater than the exit time or the queue is completely used 
            } else if (firstQueue[aux1].time > out || aux1 == lenFQ) {
                // change the escalator's direction, considering a new time of arrival based on the waiting time
                first = secondQueue[aux2];
                if (out > first.time) first.time = out;
                ++aux2;

                // if the exit time is greater than the following ones
                int i = aux2;
                while (out > secondQueue[i].time && i < lenSQ) {
                    secondQueue[i].time = out;
                    ++i;
                }
            } 
        } else if (first.direction == 1) { /* same logic as shown above */ }
```



## 2. Como garantir que somente uma das direções está ativa de cada vez em cada uma das abordagens?

Para cada abordagem, considerando que foi utilizada a estratégia de filas, o controle de sentido é feito de forma semelhante. O uso de threads e processos, neste caso, apenas determina como os dados passados para a função são acessados por meio destes recursos.

## 3. Discorra sobre as diferenças entre as implementações utilizando threads e processos e diga qual foi mais eficiente na solução do problema, justificando sua resposta.

Como as threads compartilham memória entre si, foi definida uma variável global `out` para armazenar o tempo de saída a última pessoa a acessar a escada rolante. Além disso, foi criada uma `struct` para armazenar os argumentos a serem passados para a função na thread utilizada.

```c
typedef struct {
    Person fQueue[MAXN];
    int lFQ;
    Person sQueue[MAXN];
    int lSQ;
} ThreadArgs;

/**
 * Restante do código
*/

pthread_create(&thread_id, NULL, escalator, (void *) &threadArgs);
```

Sobre a implementação com processos, foi utilizado um espaço compartilhado de memória, de modo a tornar possível o acesso à `struct` pelo processo filho criado. 

```c
// Creating a shared memory space to pass as function args
    processArgs *pArg = (processArgs *)mmap(NULL, sizeof(processArgs), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
```