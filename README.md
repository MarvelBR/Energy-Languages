# Energy-Languages
Este guia detalha os passos para configurar, executar e analisar os benchmarks de linguagens de programação para medição de desempenho (tempo de execução) em um ambiente WSL (Windows Subsystem for Linux).

**Nota Importante:** A ferramenta de medição de energia (RAPL) neste projeto é otimizada para processadores Intel. Se você possui um processador AMD (igual a mim), a medição de consumo de energia (em Joules) não funcionará, mas a medição de tempo de execução (em milissegundos) será registrada com sucesso.

## Configuração Inicial no WSL
### Atualizar Pacotes do Sistema
1. Abra seu terminal WSL e execute:
    1. `sudo apt-get update`
    2. `sudo apt-get upgrade`

### Instalar Ferramentas Essenciais e Compiladores
1. Instale as ferramentas básicas de compilação e os compiladores para as linguagens que você deseja testar. Ferramentas básicas (inclui make, gcc, g++)
    1. `sudo apt-get install build-essential`
    
2. Exemplo de instalação de compiladores/interpretadores (instale apenas os que você precisa):
    1. Para Ada: `sudo apt-get install gnat`         
    2. Para C# e F#: `sudo apt-get install dotnet-sdk-8.0`
    3. Para Dart: `sudo apt-get install dart`         
    4. Para Erlang: `sudo apt-get install erlang`       
    5. Para Fortran: `sudo apt-get install gfortran`     
    6. Para Go: `sudo apt-get install golang-go`    
    7. Para Haskell: `sudo apt-get install ghc`          
    8. Para Java: `sudo apt-get install default-jdk`
    9. Para JavaScript e TypeScript: `sudo apt-get install nodejs npm`   
    10. Para TypeScript: `sudo npm install -g typescript`    
    11. Para Julia: `sudo apt-get install julia`        
    12. Para Lisp: `sudo apt-get install sbcl`      
    13. Para Lua: `sudo apt-get install lua5.3`       
    14. Para OCaml: `sudo apt-get install ocaml`       
    15. Para Pascal: `sudo apt-get install fpc`         
    16. Para PHP: `sudo apt-get install php-cli`      
    17. Para Racket: `sudo apt-get install racket`    
    18. Para Ruby: `sudo apt-get install ruby-full`    
    19. Para Smalltalk: `sudo apt-get install gnu-smalltalk`
    20. Para Rust: `curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh`
        1. Siga as instruções na tela e reinicie o terminal.

### Instalar Dependências Específicas do Projeto
Alguns benchmarks em C exigem bibliotecas adicionais. Instale-as:
1. `sudo apt-get install -y libapr1-dev libhts-dev`
2. `sudo apt-get install -y libpcre3-dev`
3. `sudo apt-get install libhts-dev`

### Instalar Dependências Python
O script `compile_all.py` requer a biblioteca `lazyme`:
1. `pip install lazyme`

## Preparação dos Benchmarks
### Gerar Arquivos de Entrada
O script `gen-input.sh` cria os arquivos de entrada necessários para alguns benchmarks.
1. `./gen-input.sh`

## Compilar e Medir os Benchmarks
### Compilar a Ferramenta `RAPL/main`
A ferramenta de medição de energia precisa ser compilada.
1. Navegue até a pasta RAPL
    1. `cd Energy-Languages/RAPL/`
    2. `make`
    3. Volte para a pasta Energy-Languages
        1. `cd ../`

### Executar a Medição
Antes de executar o script, rode `compile_all.py` com a regra `compile` para verificar se todos os benchmarks estão funcionando corretamente.
1. `compile_all.py compile`.

Agora você pode executar o script `compile_all.py` com a regra `measure`, o que vai nos gerar um `.csv`.
1. `compile_all.py measure`.

**Importante:** A medição de energia requer `sudo`. Você precisará digitar sua senha quando solicitado.

Para medir todos os benchmarks de uma linguagem específica (ex: C):
1. `cd Energy-Languages/C/`
    1. Recomendado: `compile_all.py compile`
2. `python3 compile_all.py measure`

Para medir todos os benchmarks de todas as linguagens (após compilar tudo):
1. `cd Energy-Languages/`
2. `python3 compile_all.py measure`

## Análise dos Resultados
Para entender melhor, a estrutura do .csv é a seguinte: `benchmark-name ; PKG (Joules) ; CPU (J) ; GPU (J) ; DRAM (J) ; Time (ms)`. 

Porém você verá que (caso esteja usando um processador que não seja da Intel), onde deveriam estar os valores `PKG (Joules) ; CPU (J) ; GPU (J) ; DRAM (J)` estão com 0 ou vazios, isso ocorre pois, o RAPL (Running Average Power Limit) que é a interface que mede o consumo de energia de componentes do processador, não funciona em processadores que não sejam da Intel!

Mas ainda nos sobra o tempo que demorou para as linguagens gerarem esses .csv, que é o que analisarei.


# Energy Efficiency in Programming Languages
#### Checking Energy Consumption in Programming Languages Using the _Computer Language Benchmark Game_ as a case study.

### What is this?

This repo contains the source code of 10 distinct benchmarks, implemented in 28 different languages (exactly as taken from the [Computer Language Benchmark Game](https://benchmarksgame-team.pages.debian.net/benchmarksgame/)).

It also contains tools which provide support, for each benchmark of each language, to 4 operations: *(1)* **compilation**, *(2)* **execution**, *(3)* **energy measuring** and *(4)* **memory peak detection**.

### How is it structured and hows does it work?

This framework follows a specific folder structure, which guarantees the correct workflow when the goal is to perform and operation for all benchmarks at once.
Moreover, it must be defined, for each benchmark, how to perform the 4 operations considered.

Next, we explain the folder structure and how to specify, for each language benchmark, the execution of each operation.

#### The Structure
The main folder contains 32 elements: 
1. 28 sub-folders (one for each of the considered languages); each folder contains a sub-folder for each considered benchmark.
2. A `Python` script `compile_all.py`, capable of building, running and measuring the energy and memory usage of every benchmark in all considered languages.
3. A `RAPL` sub-folder, containing the code of the energy measurement framework.
4. A `Bash` script `gen-input.sh`, used to generate the input files for 3 benchmarks: `k-nucleotide`, `reverse-complement`, and `regex-redux`.

Basically, the directories tree will look something like this:

```Java
| ...
| <Language-1>
	| <benchmark-1>
		| <source>
		| Makefile
		| [input]
	| ...
	| <benchmark-i>
		| <source>
		| Makefile
		| [input]
| ...
| <Language-i>
	| <benchmark-1>
	| ...
	| <benchmark-i>
| RAPL
| compile_all.py
| gen-input.sh

```

Taking the `C` language as an example, this is how the folder for the `binary-trees` and `k-nucleotide` benchmarks would look like:

```Java
| ...
| C
	| binary-trees
		| binarytrees.gcc-3.c
		| Makefile
	| k-nucleotide
		| knucleotide.c
		| knucleotide-input25000000.txt
		| Makefile
	| ...
| ...

```

#### The Operations

Each benchmark sub-folder, included in a language folder, contains a `Makefile`.
This is the file where is stated how to perform the 4 supported operations: *(1)* **compilation**, *(2)* **execution**, *(3)* **energy measuring** and *(4)* **memory peak detection**.

Basically, each `Makefile` **must** contains 4 rules, one for each operations:

| Rule | Description |
| -------- | -------- |
| `compile` | This rule specifies how the benchmark should be compiled in the considered language; Interpreted languages don't need it, so it can be left blank in such cases. |
| `run` | This rule specifies how the benchmark should be executed; It is used to test whether the benchmark runs with no errors, and the output is the expected. |
| `measure` | This rule shows how to use the framework included in the `RAPL` folder to measure the energy of executing the task specified in the `run` rule. |
| `mem` | Similar to `measure`, this rule executes the task specified in the `run` rule but with support for memory peak detection. |

To better understand it, here's the `Makefile` for the `binary-trees` benchmark in the `C` language:

```Makefile
compile:
	/usr/bin/gcc -pipe -Wall -O3 -fomit-frame-pointer -march=native -fopenmp -D_FILE_OFFSET_BITS=64 -I/usr/include/apr-1.0 binarytrees.gcc-3.c -o binarytrees.gcc-3.gcc_run -lapr-1 -lgomp -lm
	
measure:
	sudo ../../RAPL/main "./binarytrees.gcc-3.gcc_run 21" C binary-trees

run:
	./binarytrees.gcc-3.gcc_run 21

mem:
	/usr/bin/time -v ./binarytrees.gcc-3.gcc_run 21

```

### Running an example.

*First things first:* We must give sudo access to the energy registers for RAPL to access
```
sudo modprobe msr
```
and then generate the input files, like this
```Makefile
./gen-input.sh
```
This will generate the necessary input files, and are valid for every language.

We included a main Python script, `compile_all.py`, that you can either call from the main folder or from inside a language folder, and it can be executed as follows:

```PowerShell
python compile_all.py [rule]
```

You can provide a rule from the available 4 referenced before, and the script will perform it using **every** `Makefile` found in the same folder level and bellow.

The default rule is `compile`, which means that if you run it with no arguments provided (`python compile_all.py`) the script will try to compile all benchmarks.

The results of the energy measurements will be stored in files with the name `<language>.csv`, where `<language>` is the name of the running language. 
You will find such file inside of corresponding language folder.

Each <language>.csv will contain a line with the following: 

```benchmark-name ; PKG (Joules) ; CPU (J) ; GPU (J) ; DRAM (J) ; Time (ms)```

Do note that the availability of GPU/DRAM measurements depend on your machine's architecture. These are requirements from RAPL itself.

### Add your own example!
#### Wanna know your own code's energy behavior? We can help you!
#### Follow this steps:

##### 1. Create a folder with the name of you benchmark, such as `test-benchmark`, inside the language you implemented it.

##### 2. Follow the instructions presented in the [Operations](#the-operations) section, and fill the `Makefile`.

##### 3. Use the `compile_all.py` script to compile, run, and/or measure what you want! Or run it yourself using the [`make`](https://linux.die.net/man/1/make) command.

### Further Reading
Wanna know more? Check [this website](https://sites.google.com/view/energy-efficiency-languages)!

There you can find the results of a successful experimental setup using the contents of this repo, and the used machine and compilers specifications.

You can also find there the paper which include such results and our discussion on them:

>**"_Energy Efficiency across Programming Languages: How does Energy, Time and Memory Relate?_"**, 
>Rui Pereira, Marco Couto, Francisco Ribeiro, Rui Rua, Jácome Cunha, João Paulo Fernandes, and João Saraiva. 
>In *Proceedings of the 10th International Conference on Software Language Engineering (SLE '17)*

#### IMPORTANT NOTE:
The `Makefiles` have specified, for some cases, the path for the language's compiler/runner. 
It is most likely that you will not have them in the same path of your machine.
If you would like to properly test every benchmark of every language, please make sure you have all compilers/runners installed, and adapt the `Makefiles` accordingly.

### Contacts and References

[Green Software Lab](http://greenlab.di.uminho.pt)

Main contributors: [@Marco Couto](http://github.com/MarcoCouto) and [@Rui Pereira](http://haslab.uminho.pt/ruipereira)


[The Computer Language Benchmark Game](https://benchmarksgame-team.pages.debian.net/benchmarksgame/)

