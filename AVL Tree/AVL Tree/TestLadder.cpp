#include "Game.h"
#include "avlTreeTemplate.h"


int main()
{
	

	AvlTree<int> tree;

	tree.insert(1);	tree.insert(3);	tree.insert(5);	tree.insert(7);
	tree.insert(9);	tree.insert(9);	tree.insert(9);	tree.insert(11);
	tree.insert(2);	tree.insert(9);	tree.insert(4);	tree.insert(8);
	cout << tree.toString("Initial Data") << endl;

	tree.remove(7);
	tree.remove(9);
	cout << tree.toString("Removed 7,9") << endl;

	tree.insert(30); tree.insert(50); tree.insert(30);
	tree.insert(30); tree.insert(15); tree.insert(18);
	cout << tree.toString("Add 30,50,30,30,15,18") << endl;

	cout << "Removed: " << tree.removeMin() << endl;
	cout << tree.toString("Remove Min") << endl;
	cout << "Removed: " << tree.removeMin() << endl;
	cout << tree.toString("Remove Min") << endl;
	cout << "Removed: " << tree.removeMin() << endl;
	cout << tree.toString("Remove Min") << endl;

	tree.insert(17);
	cout << tree.toString("Add 17");

	const int RANDOMCT = 1;
	Game g("dictionary.txt");
	g.printToCMD = true;
	g.printToFile = true;
	g.play("kiss", "woof");
	g.play("cock", "numb");
	g.play("jura", "such");
	g.play("stet", "whey");
	g.play("flute", "taper");
	for (int i = 0; i < RANDOMCT; i++)
		g.play("stone");  //if two arguments are not provided, randomly pick both strings.  Use the length of provided argument as length of strings
		return 0;




	return 0;
}
