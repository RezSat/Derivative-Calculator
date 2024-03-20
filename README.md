# Derivative-Calculator
A library which calculates derivatives of any function, moreover, it is capable of writing step by step calculation in LaTex format

## Before launch
This project uses some utils to visualize the results:
- MiKTeX. Program, which allows to create .pdf files straight from the command line. To have my project working correctly on your PC you will need to download it, if you don't have any of its interpretations: https://miktex.org/download
- GraphVis. This program is not necessary, but if you download it, beautiful drawings of the differentiator tree will appear in folder - data. You can get it here https://graphviz.org/download/.
- Also there are some .bat scripts applied in order to save my time. There is a tiny chance that something might go wrong on you device.

## Installations
To use my code, you need to clone it and then run a Makefile, despite it has .o files in the lib folder, to avoid errors I strongly recommend to do it.
~~~
git clone https://github.com/dodokek/Derivative-Calculator.git
~~~
Optional part, need to be executed in the top folder of the project.
~~~
make
~~~
## Execution
Right now you can write any expression without additional brackets thanks to recursive descend algorithm. In order to compute a derivative, you need to write function in file "input.txt".  
~~~
5*sin(ln(x+5))+2
~~~
This format represents ln(2x), you might have notice that all unary operations such as sin, cos, ln etc. need to have fictive zero at the beginning. All operations have to be written like ("expression")operation("another expression"), only like this, don't try to write anyhow different.

## This is my finals words, goodbye
This project is far from being complete, have fun with its prototype.
