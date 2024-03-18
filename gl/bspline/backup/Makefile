LDLIBS=-lGLEW -lGL -lglut -lm
index_len: bspline_index_length.c
	gcc bspline_index_length.c -o index_len  $(LDLIBS)

bspline_len: bspline_param_length.c
	gcc bspline_param_length.c -o bspline_len $(LDLIBS)

percent_plot: bspline_percent_plot.c
	gcc bspline_percent_plot.c -o percent_plot $(LDLIBS)

algo:	recursive_bspline.c
	gcc recursive_bspline.c -o algo $(LDLIBS) 
input: create_input.c
	gcc create_input.c -o input $(LDLIBS)
