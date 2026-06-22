% diff(F, X, DF)

diff(X, X, 1).

diff(C, X, 0) :- atomic(C), C \= X.

diff(-F, X, -DF) :- diff(F, X, DF).

diff(C * F, X, C * DF) :- diff(C, X, 0), diff(F, X, DF).

diff(F + G, X, DF + GF) :- diff(F, X, DF), diff(G, X, DG).

diff(X^N, X, N * X ^ M) :- atomic(X), number(N), M is N-1.

diff(U * V, X, DU * V + DV * U) :- diff(V, X, DV), diff(U, X, DU).

diff(sin(X), X, cos(X)) :- atomic(X).
diff(sin(U), X, cos(U) * DU)) :- \+ atomic(U), diff(U, X, DU).

diff(cos(X), X, -sin(X)) :- atomic(X).
diff(cos(U), X, -sin(X) * DU) :- \+ atomic(U), diff(U, X, DU).