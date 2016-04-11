#include"KDTree.h"

int main()
{
		
		KDTree<double> myKDtree(3);
		myKDtree.buildfromFile("sample_data.csv");
		myKDtree.serialize("myKDtree", ".csv");
		myKDtree.kNearestNeighbor("query_data.csv");
		myKDtree.clear();

		KDTree<double> treeFromFile(3);
		treeFromFile.deSerialize("myKDtree.csv");
		treeFromFile.kNearestNeighbor("query_data.csv","QueryList2",".csv");
		return 0;

}