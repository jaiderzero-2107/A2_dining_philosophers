# Jantar dos Filósofos: Método do Garçom ou Árbitro

Este projeto é uma simulação clássica multithread do problema do Jantar dos Filósofos, implementada em C. Ele demonstra uma solução que é **livre tanto de deadlock quanto de inanição**, utilizando o padrão "Árbitro" (ou "Garçom") com mutexes e variáveis de condição. A simulação é construída com a biblioteca POSIX Threads (Pthreads) e foi projetada para ser compilada e executada em um ambiente Windows com uso do WSL (subsistema do Windows para Linux) para permitir comandos Linux.

## Solução Implementada

A ideia central é que os filósofos não pegam mais os hashis diretamente. Em vez disso, eles interagem com uma entidade central que gerencia o estado de todos os recursos.

1.  **Mutex Central (`waiter_lock`):** Um único mutex protege o estado de todos os hashis. Qualquer filósofo que queira alterar seu estado (de pensar para com fome, de comendo para pensando) deve primeiro adquirir este lock.

<img width="712" height="367" alt="Sem título17" src="https://github.com/user-attachments/assets/3d5b8959-2506-44a6-9dd1-f303e18b1233" />


2.  **Array de Estados:** Um array global monitora o estado de cada filósofo (`PENSANDO`, `FAMINTO`, `COMENDO`).
3.  **Variável de Condição (`can_eat_cond`):** Funciona como uma "sala de espera". Se um filósofo fica com fome mas não pode comer porque um de seus vizinhos já está comendo, ele entra nesta sala de espera usando `pthread_cond_wait`. O poder da `pthread_cond_wait` é que ela **atomicamente libera o mutex central e coloca a thread para dormir**. Isso é crucial, pois permite que outras threads interajam com o garçom enquanto a primeira espera.
<img width="735" height="245" alt="Sem título18" src="https://github.com/user-attachments/assets/9c9f38b1-726a-4afc-817b-38bfe560b8c1" />


4.  **Reavaliação:** Quando um filósofo termina de comer, ele libera seus hashis e sinaliza (`pthread_cond_broadcast`) para **acordar todas as threads** que estavam na sala de espera. As threads acordadas então reavaliam se agora podem comer.

<img width="627" height="389" alt="Sem título19" src="https://github.com/user-attachments/assets/9d0779be-683d-47d1-8e90-f044e65c4893" />

**Problemas Resolvidos:**
- **Deadlock:** É impossível, pois um filósofo nunca segura um recurso (um hashi) enquanto espera por outro, ele espera na "sala de condição" com as "mãos vazias".
- **Inanição (Starvation):** É evitada porque o sistema operacional e a biblioteca Pthreads gerenciam a fila de espera da variável de condição de forma justa. Uma thread que está esperando há muito tempo eventualmente será acordada e terá a chance de adquirir os recursos, com essa lógica garante que todos os filósofos famintos sejam reavaliados quando um recurso é liberado.

### Detalhes
- **Multithread:** Cada filósofo é representado por uma `pthread` separada.
- **Livre de Deadlock e Inanição:** Garante a correção e a justiça da alocação de recursos.
- **Configurável:** É fácil alterar o número de filósofos e refeições através de macros em `main.c`.

## Estrutura do Projeto

```
.
├── CMakeLists.txt      # O script de build.
├── main.c              # Configura o "Garçom", inicializa recursos e cria as threads dos filósofos.
├── philosopher.h       # Define as estruturas de dados e os protótipos das funções.
└── philosopher.c       # Contém o ciclo de vida do filósofo e a lógica de interação com o "Garçom".
```
- **Concorrência:** POSIX Threads (Pthreads), incluindo Mutexes e Variáveis de Condição
- **Sistema de Build:** CMake
- **Ambiente Recomendado:** IDE CLion com uma Toolchain WSL
