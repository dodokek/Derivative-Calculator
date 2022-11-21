xelatex -output-directory=data data/output.tex > tex_logs
cd data
del output.aux
del output.log
del output.tex
del graph.dot