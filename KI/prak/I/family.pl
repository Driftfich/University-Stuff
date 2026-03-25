
% person(Name, Geschlecht, Vater, Mutter)
m(X) :- person(X, m, _, _).
w(X) :- person(X, w, _, _).

% elternteil(Elternteil, Kind) <= Elternteil Mutter oder Vater und nicht nil
elternteil(E, K) :- (person(K, _, E, _); person(K, _, _, E)), E \= nil.

geschwister(X, Y) :- person(X, _, V, M), person(Y, _, V, M), X \= Y, V \= nil, M \= nil.

bruder(X, Y) :- geschwister(X,Y), m(X).
schwester(X, Y) :- geschwister(X,Y), w(X).

enkel(E, G) :- elternteil(P, E), elternteil(G, P).

onkel(O, N) :- elternteil(P, N), bruder(O, P).
tante(T, N) :- elternteil(P, N), schwester(T, P).

vorfahr(V, K) :- elternteil(V, K); (elternteil(V, Z), vorfahr(Z, K)).
