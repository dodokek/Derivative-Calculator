xelatex -output-directory=data data/output.tex > tex_logs
cd data
del output.aux
del output.log
del output.tex
@REM del graph.dot