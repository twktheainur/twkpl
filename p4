declarer
 a:Reel;
 b:Reel;
 c:Entier;
debut
ecrire("a(float):");
lire(a);
ecrire("b(float):");
lire(b);
ecrire("c(int):");
lire(c);
 si(a==b)
  ecrire("a=b");
 sinon
  ecrire("zut! a!=b");
 finsi;
 ecrire("On affecte maintenant un entier a un reel! et ca throw ExTypeMismatch");
 c=b;
 
fin