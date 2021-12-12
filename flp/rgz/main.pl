% task 1
read_list([], N):- N =< 0.
read_list([H | T], N):- read(H), N1 is N - 1, read_list(T, N1).

dup_list([H | T], [H, H | DT]):- dup_list(T, DT).
dup_list([], []).

goal1:- 
	read(N), 
	read_list(L, N), 
	dup_list(L, DL), 
	write(L),
	write(' -> '),
	writeln(DL).

% task2

string_len_sum([], 0).
string_len_sum([Head | Tail], Sum):- 
	string_len_sum(Tail, Lhs),
	string_length(Head, Rhs),
	Sum is Lhs + Rhs.

plus_or_not(BaseSpaces, 0, BaseSpaces, 0).
plus_or_not(BaseSpaces, RemainderSpaces, NewBaseSpaces, NewRemainderSpaces):-
	NewBaseSpaces is BaseSpaces + 1,
	NewRemainderSpaces is RemainderSpaces - 1.

write_n_spaces(0).
write_n_spaces(N):-
	write(' '),
	N1 is N - 1,
	write_n_spaces(N1).

write_to_file([Head], _, _):- writeln(Head).
write_to_file([Head | Tail], BaseSpaces, RemainderSpaces):-
	write(Head),
	plus_or_not(BaseSpaces, RemainderSpaces, NewBaseSpaces, NewRemainderSpaces),
	write_n_spaces(NewBaseSpaces),
	write_to_file(Tail, BaseSpaces, NewRemainderSpaces), !.

goal2:-
	read(In),
	read(Out),
	exists_file(In),
	open(In, read, Fin), set_input(Fin),
	open(Out, write, Fout), set_output(Fout),
	repeat,
		read_line_to_codes(Fin, CodesL),
		string_to_list(String, CodesL),
		split_string(String, ' ', ' ', StringsL),
		string_len_sum(StringsL, Sum),
		Spaces is 40 - Sum,
		length(StringsL, ElementsCount),
		BaseSpaces is Spaces // (ElementsCount - 1),
		RemainderSpaces is Spaces mod (ElementsCount - 1),
		write_to_file(StringsL, BaseSpaces, RemainderSpaces),
	at_end_of_stream,
	close(Fin),
	close(Fout).
