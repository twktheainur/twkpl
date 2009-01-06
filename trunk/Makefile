GCC = g++ -Wall -W -O -g3
all: TestLecteurPhraseAvecArbre

######################################################################################################
# etape 4 - lecteur de phrase avec table des symboles et arbre
######################################################################################################
ObjTestLecteurPhraseAvecArbre = LecteurCaractere.o Symbole.o LecteurSymbole.o SymboleValue.o TableSymboles.o Arbre.o LecteurPhraseAvecArbre.o TestLecteurPhraseAvecArbre.o Type.o

Type.o: Type.h
	$(GCC) -c Type.cpp 
Arbre.o: Arbre.h Arbre.cc Symbole.h SymboleValue.h
	$(GCC) -c Arbre.cc
LecteurPhraseAvecArbre.o: LecteurPhraseAvecArbre.cc LecteurPhraseAvecArbre.h Symbole.h LecteurSymbole.h TableSymboles.h Arbre.h Type.h
	$(GCC) -c LecteurPhraseAvecArbre.cc

TestLecteurPhraseAvecArbre.o: TestLecteurPhraseAvecArbre.cc LecteurPhraseAvecArbre.h
	$(GCC) -c TestLecteurPhraseAvecArbre.cc
	
TestLecteurPhraseAvecArbre: $(ObjTestLecteurPhraseAvecArbre)
	$(GCC) -o TestLecteurPhraseAvecArbre $(ObjTestLecteurPhraseAvecArbre)
clean:
	rm *.o
	rm  TestLecteurPhraseAvecArbre
	
