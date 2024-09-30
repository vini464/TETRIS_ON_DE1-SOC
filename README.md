# Tetris na Placa DE1-SOC

*_Podemos Colocar uma Logo, como uma foto da tela inicial aqui_*

![VSCode](https://img.shields.io/badge/Editor-VSCode-blue?logo=visual-studio-code&logoColor=white)
![Neovim](https://img.shields.io/badge/Editor-Neovim-green?logo=neovim&logoColor=white)
![C](https://img.shields.io/badge/Linguagem-C-blue?logo=c&logoColor=white)
![Linux](https://img.shields.io/badge/OS-Linux-yellow?logo=linux&logoColor=white)

Nesse projeto foi desenvolvido um jogo *_TetrisLike_* para a plataforma DE1-SoC da Altera/Intel, com foco na integração hardware/software, onde foi preciso fazer a comunicação do acelerometro ADXL345 com o processador Dual-core ARM Cortex-A9. Com isso foi desenvolvido uma biblioteca para utilizar o acelerometro além de utilizarmos as bibliotecas já disponiveis da IntelFPGAUP para configurações de botoões, displays de 7 segmentos e exibição de video via VGA.

## <abbr title="Too Long Didn't Read">TL;DR</abbr>:

Caso você só esteja interessado em rodar o jogo, não precisa ler o texto todo:

- Este projeto se trata do desenvolvimento de um jogo baseado no famoso Tetris, para a placa DE1-SoC. Caso você tenha uma DE1-SoC em mãos, e com acesso à internet, basta colar o seguinte código no terminal:
```
git clone https://github.com/vini464/TETRIS_ON_DE1-SOC.git && cd TETRIS_ON_DE1-SOC
make game
```
> **Obs: Você precisa de privilégio de administrador para rodar o jogo.**

## Sumario

*_Adicionar links para cada topico e subtopicos aqui_*

## Como executar
Caso você só esteja interessado em rodar o jogo, não precisa ler o texto todo:

Para rodarmos o código, precisamos cumprir alguns requisitos que são:

 1. Uma Placa DE1-SoC.
 2. Linux instalados.
 3. As bibliotecas do IntelFPGAUP instaladas, mais especificamente a de Key, 7 segmentos e VGA.
 4. Um monitor com entrada VGA.
 5. Um cabo de Ethernet
 6. Um computador com terminal Linux para acessar a placa via protocolo SSH.

Com todos os requisitos cumpridos basta no computador acessar a placa via protocolo SSH com ambos conectados na mesma rede e transferir o arquivo desse repositoria via protocolo SCP. Caso possua conexão com a internet na placa, ao invés de usar SCP, basta digitar o seguinte código no terminal:
```
git clone https://github.com/vini464/TETRIS_ON_DE1-SOC.git && cd TETRIS_ON_DE1-SOC
make game
```
> **Obs: Você precisa de privilégio de administrador para rodar o jogo.**


## Desenvolvimento?
*_Podemos Colocar os diagramas gerais aqui_*

### Acelerometro




## Testes

## Conclusão
