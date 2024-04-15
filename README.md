# SistemasOperacionais

## Atividade: Jantar dos Filósofos
1. Descrição: O problema do Jantar dos Filósofos, proposto por Dijkstra em 1965, é descrito da seguinte forma: cinco filósofos estão ao redor de uma mesa e cada um desses filósofos têm um prato para comer. Entre cada par de pratos, há um hashi - os filósofos só conseguem comer quando possuem dois hashis.O filósofo possui algumas ações: pensar, sentir fome e comer. Quando ele sente fome, tenta pegar o hashi à esquerda e à direita do seu prato - se bem sucedido, ele come, devolve os hashis à mesa e volta a pensar.
2. Solução: A solução do problema deve usar um arranjo (estado), que controle se um filósofo está pensando, sentindo fome ou comendo. O filósofo pode comer apenas se seus vizinhos diretos à esquerda e à direita não estiverem comendo - os vizinhos do filósofo f são definidos como FILO_ESQ e FILO_DIR. O programa deve ser feito através do modelo de monitores e semáforos, para que os filósofos sejam bloqueados, se os talheres necessários a ele estiverem ocupados.
No conceito de Sistemas Operacionais, os filósofos são descritos como threads e os hashis são recursos - “filósofos usarem hashi” é uma analogia à “threads usarem recursos”, para realizar tarefas.

## Trabalho
O repositório contém dois móduos kernal do Linux: VideoLog e KeyLogger. Ambos utilizam a comunicação via socket entre um cliente Linux e um servidor Phyton. Para rodar os programas, siga as instruções abaixo:

1. Pré-requisitos
   - Linux (kernel compatível com a compilação de módulos);
   - Python 3.x;
2. Configuração
   -  Clone este repositório em sua máquina local;
   -  Compile o cliente usando o comando `make`. Se você precisar apagar o último build, execute `make clean` antes de compilar novamente. Isso se aplica tanto ao cliente do VideoLog quanto ao cliente do Keylogger;
   -   Execute o servidor usando o script Python fornecido;
   -   Certifique-se de usar o mesmo endereço IP e porta nos campos de rede/socket dos códigos do cliente e servidor;
3. Utilização
   - Inicie o servidor executando o script Python do servidor;
   - Compile e execute o cliente do VideoLog ou do Keylogger no Linux;
   - O cliente se conectará ao servidor usando o endereço IP e porta configurados anteriormente. Certifique-se de ter uma conexão de rede ativa;

Integrantes:
  - Carlos Henrique Hannas de Carvalho, número USP: 11965988;
  - Henrique Carobolante Parro, número USP: 11917987;
  - Leonardo Hannas de Carvalho Santos, número USP: 11800480.
