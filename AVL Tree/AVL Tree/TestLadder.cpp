//#include "Game.h"
//#include "AVLTree.h"
//
//
//int main()
//{
//
//
//	AvlTree<int> tree;
//
//	tree.Insert(1);	tree.Insert(3);	tree.Insert(5);	tree.Insert(7);
//	tree.Insert(9);	tree.Insert(9);	tree.Insert(9);	tree.Insert(11);
//	tree.Insert(2);	tree.Insert(9);	tree.Insert(4);	tree.Insert(8);
//	cout << tree.ToString("Initial Data") << endl;
//
//	tree.Remove(7);
//	tree.Remove(9);
//	cout << tree.ToString("Removed 7,9") << endl;
//
//	tree.Insert(30); tree.Insert(50); tree.Insert(30);
//	tree.Insert(30); tree.Insert(15); tree.Insert(18);
//	cout << tree.ToString("Add 30,50,30,30,15,18") << endl;
//
//	cout << "Removed: " << tree.RemoveMin() << endl;
//	cout << tree.ToString("Remove Min") << endl;
//	cout << "Removed: " << tree.RemoveMin() << endl;
//	cout << tree.ToString("Remove Min") << endl;
//	cout << "Removed: " << tree.RemoveMin() << endl;
//	cout << tree.ToString("Remove Min") << endl;
//
//	tree.Insert(17);
//	cout << tree.ToString("Add 17");
//
//	const int RANDOMCT = 1;
//	Game g("dictionary.txt");
//	g.printToCmd = true;
//	g.printToFile = true;
//	g.Play("kiss", "woof");
//	g.Play("cock", "numb");
//	g.Play("jura", "such");
//	g.Play("stet", "whey");
//	g.Play("flute", "taper");
//	for (int i = 0; i < RANDOMCT; i++)
//		g.Play("stone");  //if two arguments are not provided, randomly pick both strings.  Use the length of provided argument as length of strings
//	return 0;
//
//
//
//
//	return 0;
//}
