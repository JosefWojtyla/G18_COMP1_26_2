# linguagem e escopo

## Introdução

Será feito um compilador da linguagem Portugol, versão Studio UNIVALI, para a linguagem Python.
Ambas são linguagens tipadas, imperativas e tem funções.

## Tipos de dados

Escolhemos para que o compilador faça a tradução direta dos seguintes tipos primitivos para seu equivalentes nativos em Python:

- `inteiro` -> `int`
- `real` -> `float`
- `cadeia` -> `str`
- `logico` -> `bool`

## Estruturas Suportadas

- **Condicional:** suporte para `se` e `senao`, permitindo aninhamento de blocos.
- **Laços de repetição:** suporte para o laço `para` e `enquanto` das seguintes formas:
    - *Para:*
    Na linguagem Portugol, é feito da seguinte forma:
    ```bash
    para (`[inicialização de variáveis]; [condições de parada do laço]; [formas de incremento das variáveis]`){
        escreva(i, "\n")
    }
    ```
    O compilador suportará:
        - Variável declarada no inicializador do `para` é tratada como local ao laço
        em Portugol, mas no código Python gerado ela permanecerá acessível após
        o laço (limitação aceita — veja seção correspondente).
        - A **condição** de parada pode ser somente comparações simples: `< / >`, `<= / >=`, `==` e `!=`
        - No incremento, serão aceitos incrementos simples: `++`, `--`, `+= 2`, `*= 3`.

    - *Enquanto:*
    Usa parênteses `()` para a condição e chaves `{}` para abrir e fechar o bloco.
    ```bash
    inteiro cont = 1

    enquanto(cont <= 5){
        escreva(cont, "\n")
        cont = contador + 1
    }
    ``` 
        - A condição aceita da mesma forma que no laço `para`.

- **Funções:** suporte a funções definidas pelo usuário, aceitando parâmetros tipados, retorno de valores, escopo local de variáveis e chamadas recursivas.

- **Vetores:** suporte restrito a vetores *unidimensionais* com tamanho declarado na inicialização e acesso via índice.

## Elementos Léxicos Suportados

- **Operadores aritméticos:** `+`, `-`, `*`, `/`, `%`
- **Operadores relacionais:** `<`, `>`, `<=`, `>=`, `==`, `!=`
- **Operadores lógicos:** `e`, `ou`, `nao`
- **Operador de atribuição:** `=`
- **Operadores de incremento/decremento:** `++`, `--`, `+=`, `-=`, `*=`, `/=`
- **Comentários:** `//` para linha única e `/* */` para bloco — ambos serão **descartados** na geração do código Python (não traduzidos para `#`).


## Fora do escopo

- matrizes multidimensionais
- estruturas, registros, classes ou orientação a objetos.
- importação de bibliotecas externas ou módulos (`importar`)
- tratamento de exceções (`tente/pegue`)
- comandos de interrupção de fluxo (`break`, `continue` e `switch-case`).
- manipulação de ponteiros, referências e acesso ao sistema de arquivos.

## Decisões de Comportamentos e Semântica

- **Variáveis não inicializadas:** variáveis declaradas sem atribuição de valor imediata receberão automaticamente um valor padrão relacionado ao seu tipo na geração do código Python (`0` para `inteiro`, `0.0` para `real`, `""` para `cadeia` e `False` para `logico`) para evitar erros de execução.
- **Variáveis inicializadas no laço `Para`:** Variável declarada no inicializador do `para` permanece acessível após o laço no código Python gerado, diferindo do comportamento do Portugol.
- **Comportamento do `leia()`:** o compilador gerará automaticamente a conversão do tipo adequada no código Python - por exemplo, `int(input())` para variáveis do tipo `inteiro` - com base no tipo declarado inicialmente da variável.
- **Comportamento do `escreva()`:** os argumentos serão separados por vírgula. Os argumentos são passados diretamente, sem conversão de tipo necessário - `escreva()` -> `print()`, mapeia diretamente adicionando quebra de linha ao final por padrão.

## Limitações Aceitas

- Não serão feitas otimizações avançadas no código gerado, apenas limitando-se à remoção de código morto óbvio.
- Não haverá validação de limite para recursão; fica à cargo do interpretador padrão do Python.
- O gerenciamento de escopo será binário, suportando apenas variáveis globais e locais, sem namespaces complexos.
- Cada função deve possuir um nome estritamente único.
