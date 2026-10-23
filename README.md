# mEditor - Editor de Texto via Terminal

Este é um editor de texto simples operado via terminal, desenvolvido em C para a disciplina de Algoritmos e Técnicas de Programação II. 

## Estruturas de Dados Utilizadas
- **Lista Duplamente Encadeada:** Representa o arquivo em memória, onde cada nó corresponde a uma linha do texto.
- **Pilhas (Stacks):** Duas pilhas gerenciam o histórico de ações para viabilizar as funcionalidades de `undo` e `redo` (com limite de 10 operações armazenadas).
- **Recursão:** Utilizada para a impressão do conteúdo do arquivo na tela de forma contínua.

## Como Compilar e Executar
No terminal Linux, compile os múltiplos arquivos utilizando o `gcc`:
```bash
gcc principal.c dupla_encad.c pilha.c funcoes.c -o programa
```
Para executar:
```bash
./programa
```

## Comandos Disponíveis
* **`open <arquivo>`**: Abre um arquivo existente. Linhas com mais de 254 caracteres serão quebradas.
* **`close`**: Fecha o arquivo atual e limpa a memória (não salva automaticamente).
* **`print`**: Imprime recursivamente o arquivo atual na tela (computa linhas vazias originais).
* **`save`**: Salva as alterações feitas no arquivo original, utilizando um arquivo temporário de segurança. 
* **`quit`**: Encerra o programa e libera toda a memória alocada.
* **`i <posição> <texto>`**: Insere uma nova linha de texto na posição especificada.
* **`d <posição>`**: Deleta a linha na posição indicada.
* **`r <posição> <texto>`**: Substitui (sobrescreve) o conteúdo da linha existente na posição especificada.
* **`a <posição> <texto>`**: Concatena (adiciona) o texto ao final da linha especificada, respeitando o limite de caracteres.
* **`split <posição> <índice>`**: Corta a linha no índice indicado, enviando o conteúdo posterior para uma nova linha abaixo.
* **`join <posição>`**: Puxa o conteúdo da próxima linha e concatena ao final da linha atual.
* **`undo`**: Desfaz a última modificação no texto (até 10 ações).
* **`redo`**: Refaz a última ação desfeita (disponível apenas logo após um comando undo).

## Estrutura dos Arquivos
* `principal.c`: Loop principal e interface de captura de comandos.
* `funcoes.c` / `funcoes.h`: Lógica central das operações do editor e integração das estruturas.
* `dupla_encad.c` / `dupla_encad.h`: Implementação da lista duplamente encadeada.
* `pilha.c` / `pilha.h`: Implementação da estrutura de pilha para o histórico.

## ⚠Observações
* O programa ignora linhas compostas unicamente por `\n` no final do arquivo para economizar memória, mas mantém o controle interno durante o processamento.
* O projeto original possui um arquivo `Arquivo.txt` sugerido para testes preliminares.
* Há um bug com mensagens de erro duplicadas
