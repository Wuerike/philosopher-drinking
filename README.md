# philosopher-drinking

O Bar dos Filósofos é um problema de programação concorrente que visa abordar questões de gerenciamento de recursos compartilhados entre processos e/ou threads, foi proposto por Chandy e Misra em 1984 como uma generalização de um problema anterior conhecido como Jantar dos Filósofos.

O problema diz que N filósofos estão numa bebedeira, eles podem estar dispostos em formas aleatórias, definidas num grafo em que cada vértice representa um filósofo enquanto as arestas expressão uma relação de vizinhança entre eles. Cada filosofo compartilha uma garrafa com cada um de seus vizinhos, durante as sessões de bebedeira os filósofos escolherão quantas garrafas querem beber e de quais vizinhos pegarão essas garrafas, um filósofo só poderá beber se adquirir todas as garrafas necessárias.

Nas sessões de bebedeiras os filósofos passam por 3 estados diferentes, na ordem, TRANQUILO, COM SEDE e BEBENDO. Quando TRANQUILO, um filósofo apenas aguarda por um tempo aleatório de 0 a 2 segundos até que fique COM SEDE. Neste momento, o filósofo em questão escolherá um número aleatório de garrafas que deve variar de 2 ao número máximo de vizinhos que ele possui. Este filósofo deve então solicitar as garrafas aos seus vizinhos com quem às compartilha, assim que possuir todas ele entra no estado de BEBENDO que dura por 1 segundo e então volta a TRANQUILO. Após beber o filósofo então pode disponibilizar as garrafas que está segurando para que os outros filósofos também possam beber.

A execução correta deste problema ocorre quando todos os filósofos são capazes de passarem por todos os estados um mesmo número de vezes de forma que o tempo médio no estado COM SEDE seja equilibrado.
