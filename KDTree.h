/******************************************************************************/
/*!
\file   KDTree.h
\author Unni Nair
\par    email: u.nair@digipen.edu
\par    KDTree
\date   03/04/2016

*/
/******************************************************************************/


/******************************************************************************/
/*!
\class KDTree
\brief
KDTree is responsible to construct tree based on the given data set, save the current tree state, read back a tree, find the nearest neighbor etc.

Operations include:

- construct a tree from a file.
- insert a node in the tree.
- destroy the created tree
- save the constructed tree.
- load back the constructed tree.
- Query closest neighbor

*/
/******************************************************************************/

#pragma once
#include "utilities.h"
#include "FileIO.h"
#include <limits>
#include <math.h>
#include <iostream>

template <typename T>
class KDTree
{
	//! this is the structure of the node for KDTree.
	typedef struct Node
	{
		Node(const std::vector<T>& newData);
		~Node();
		Node * left;
		Node * right;
		std::vector<T> data;
	}KDNode;

	//! this saves the root of the tree.
	KDNode * root;
	bool constructKDTree(const std::vector<KDNode*>&);
	const unsigned dimension;
	KDNode * newNode(const std::vector<T>&) const;
	KDNode * insert(KDNode * currNode, KDNode * newNode, unsigned int level) const;
	void nearestNeighbor(KDNode* queryPoint, KDNode* currPoint, KDNode& champion, T& closestDistance, unsigned int level) const;
	void helperSerialize(const KDNode *, std::vector<std::string >&) const;
	KDNode* reConstructTree(KDNode* curr, const std::vector<std::string>&, unsigned int& index)const;
	KDNode* getRoot()const;

public:
	KDTree(unsigned int dim);
	~KDTree();
	void insertNewNode(const std::vector<T>& newData);
	void clear();
	bool serialize(const std::string& filename, const std::string& extension, std::string location = "") const;
	bool deSerialize(const std::string& filename, const std::string& location = "");
	bool buildfromFile(const std::string& filename, const std::string& location = "");
	bool kNearestNeighbor(const std::string& queryFileName, const std::string& destinationFileName = "QueriedList", const std::string& ext = ".csv")const;
};


template <typename T>
KDTree<T>::Node::Node(const std::vector<T>& newData) : left(nullptr), right(nullptr), data(newData)
{
}

template <typename T>
KDTree<T>::Node::~Node()
{
	if (left != nullptr)
	{
		left->data.clear();
		delete left;
		left = nullptr;
	}
	if (right != nullptr)
	{
		right->data.clear();
		delete right;
		right = nullptr;
	}
}

template <typename T>
KDTree<T>::KDTree(unsigned dim) : root(nullptr), dimension(dim)
{

}

template <typename T>
KDTree<T>::~KDTree()
{
	if(root!=nullptr)
		clear();
	root = nullptr;
}

/******************************************************************************/
/*!

This insert a new node to the KDTree. It takes a node as a parameter and calls a
helper function "insert" to insert the new node.

*/
/******************************************************************************/

template <typename T>
void KDTree<T>::insertNewNode(const std::vector<T>& newData)
{
	unsigned int level = 0;
	Node * newNodetoInsert = newNode(newData);
	// this function is a helper function which helps to insert the required node
	root = insert(root, newNodetoInsert, level);
}

/******************************************************************************/
/*!

This function returns root of the tree.

*/
/******************************************************************************/
template <typename T>
typename KDTree<T>::KDNode* KDTree<T>::getRoot() const
{
	return root;
}


/******************************************************************************/
/*!

This function is a helper function to find the newarest neighbor to a given node.
-querypoint		 - is the data whose closest neighbor we want to find.
-currPoint		 - is the node that currently being compared to.
-champion		 - is the node that will hold details about the closest node.// I could have used a simple vector but felt more right using a node.
-closestDistance - this saves the closest distance 
-sd				 - this is actually the splitting dimesion of the data. It is used to calculate to access the correct data to compare the distance

*/
/******************************************************************************/
template <typename T>
void KDTree<T>::nearestNeighbor(KDNode* queryPoint, KDNode* currPoint, KDNode& champion, T& closestDistance, unsigned sd) const
{
	if (currPoint == NULL)
		return;
	// distance between the current node that is being considered and the query point is calculated
	T distance = utilities<T>::distance(queryPoint->data, currPoint->data);
	// if the calculcated distance is less than the distance previously calculated we update the new distance and the node data.
	if (distance < closestDistance)
	{
		closestDistance = distance;
		champion.data = currPoint->data; 
	}
	unsigned int index = sd % dimension;
	// this is used to decide whether we need to go to the right or left half of the tree.
	// if the distance from the query point to the splitting edge is less than the current close distance then we explore the other half,
	// as there is a chance that we might have another point closer to the given data
	T distancePointToEdge = static_cast<T>(fabs(queryPoint->data[index] - currPoint->data[index]));

	if (currPoint->data[index] >= queryPoint->data[index])
	{
		nearestNeighbor(queryPoint, currPoint->left, champion, closestDistance, sd + 1);
		if (distancePointToEdge <= closestDistance)
		{
			nearestNeighbor(queryPoint, currPoint->right, champion, closestDistance, sd + 1);
		}
	}
	else
	{
		nearestNeighbor(queryPoint, currPoint->right, champion, closestDistance, sd + 1);
		if (distancePointToEdge<closestDistance)
		{
			nearestNeighbor(queryPoint, currPoint->left, champion, closestDistance, sd + 1);
		}
	}
}


/******************************************************************************/
/*!

This function is responsible for writing the tree state to a file.

filename  - this is the name of the file.
extension - user can save the file in any format
location  - location to save the tree data.

*/
/******************************************************************************/
template <typename T>
bool KDTree<T>::serialize(const std::string& filename, const std::string& extension, const std::string location) const
{

	if (root == nullptr)
	{
		std::cout << "Tree is empty " << std::endl;
		return false;
	}
	std::vector<std::string > data;
	// helper function which will recursively called to access all the nodes.
	helperSerialize(getRoot(), data);
	return FileIO::getInstance().openFiletoWrite(location + filename, extension, data);
}

/******************************************************************************/
/*!

This function reads the tree from a given file.

filename - this is the name of the file which contains the saved tree.
location - this parameter holds the location of the file.  location should always end with "\" or else it will fail to read the file.

*/
/******************************************************************************/
template <typename T>
bool KDTree<T>::deSerialize(const std::string& filename, const std::string& location)
{
	std::vector<std::string> data = FileIO::getInstance().readFile(location + filename);
	if (data.size() == 0)
	{
		std::cout << "invalid file name " << location + filename << std::endl;
		return false;
	}
	unsigned int  index = 0;
	root = reConstructTree(getRoot(), data, index);
	return true;
}

/******************************************************************************/
/*!

This file constructs a KDTree using the data from the file.

*/
/******************************************************************************/
template <typename T>
bool KDTree<T>::buildfromFile(const std::string& fileName, const std::string& location)
{
	std::vector<std::string> listPoints;
	listPoints = FileIO::getInstance().readFile(location + fileName);
	if (listPoints.size() == 0)
	{
		std::cout << "invalid file name " << location + fileName <<  std::endl;
		return false;
	}
	for (unsigned int i = 0; i < listPoints.size(); ++i)
	{
		std::vector<T> data = utilities<T>::stringToData(listPoints[i]);
		insertNewNode(data);
	}
	listPoints.clear();
	return true;
}

/******************************************************************************/
/*!

This creates a file given by the user which contains a list of all the closest neighbor queried in the input file.
queryFilename       -  this is the name of the file which holds the list of data whose nearest neighbor we have to find.
destinationFileName - this is the name of the file which will be used to save all the nearest neighbor.

*/
/******************************************************************************/
template <typename T>
bool KDTree<T>::kNearestNeighbor(const std::string& queryFileName, const std::string& destinationFileName, const std::string& ext) const
{
	std::vector<std::string> source;
	source = FileIO::getInstance().readFile(queryFileName);
	if (source.size() == 0)
	{
		std::cout << "invalid file name " << queryFileName << std::endl;
		return false;
	}
	std::vector<std::string>::const_iterator iter = source.begin();
	std::vector<std::string> result;
	while (iter != source.end())
	{
		std::vector<T> data = utilities<T>::stringToData(*iter);
		Node queryNode(data);
		Node  closestNode(root->data);
		T proximity = std::numeric_limits<T>::max();
		nearestNeighbor(&queryNode, getRoot(), closestNode, proximity, 0);
		std::string sClosestNode = utilities<T>::dataTostring(closestNode.data);
		std::string proximityStr = utilities<T>::dataTostring(proximity);
		result.push_back(sClosestNode + "," + " " + "," + proximityStr);
		++iter;
	}
	return FileIO::getInstance().openFiletoWrite(destinationFileName, ext, result);
}

/******************************************************************************/
/*!

Helper function to construct tree

*/
/******************************************************************************/
template <typename T>
bool KDTree<T>::constructKDTree(const std::vector<KDNode*>& listNodes)
{
	int level = 0;
	for (unsigned int i = 0; i < listNodes.size(); ++i) 
	{
		root = insert(root, listNodes[i], level);
	}
	return true;
}

/******************************************************************************/
/*!

Creates a new Node return.

*/
/******************************************************************************/
template <typename T>
typename KDTree<T>::KDNode* KDTree<T>::newNode(const std::vector<T>& newData) const
{
	KDNode * newNode = new Node(newData);
	return newNode;
}

/******************************************************************************/
/*!

This function inserts node at proper places.

*/
/******************************************************************************/
template <typename T>
typename KDTree<T>::KDNode* KDTree<T>::insert(KDNode* currNode, KDNode* newNode, unsigned level) const
{
	unsigned int index = level % dimension;
	if (currNode == nullptr)
	{
		currNode = newNode;
	}
	else if (currNode->data[index] >= newNode->data[index])
	{
		currNode->left = insert(currNode->left, newNode, level + 1);
	}
	else
	{
		currNode->right = insert(currNode->right, newNode, level + 1);
	}

	return currNode;
}

/******************************************************************************/
/*!

Used to destroy the tree.

*/
/******************************************************************************/
template <typename T>
void KDTree<T>::clear( )
{
	delete root;
	root = nullptr;
}

/******************************************************************************/
/*!

This function is a helper function to serialize the tree to a file.

*/
/******************************************************************************/
template <typename T>
void KDTree<T>::helperSerialize(const KDNode* curr, std::vector<std::string>& vecdata) const
{
	if (curr == nullptr)
	{
		vecdata.push_back("nullptr");
		return;
	}
	std::string convertedData = utilities<T>::dataTostring(curr->data);
	vecdata.push_back(convertedData);
	helperSerialize(curr->left, vecdata);
	// Note: data has been extended with data from curr->left
	helperSerialize(curr->right, vecdata);
}
/******************************************************************************/
/*!

Helper function to Deserialize a tree.

*/
/******************************************************************************/
template <typename T>
typename KDTree<T>::KDNode* KDTree<T>::reConstructTree(KDNode* curr, const std::vector<std::string>& data, unsigned& index) const
{
	std::string line;
	if (index >= data.size())
	{
		return nullptr;
	}
	line = data[index]; 
	++index;			
	if (line == "nullptr") // "nullptr" this is just an indicator that this node is null
	{
		return nullptr;
	}
	std::vector<T> point = utilities<T>::stringToData(line);
	curr = newNode(point);

	curr->left = reConstructTree(curr->left, data, index);
	// NOTE: index is passed by reference and is now different
	curr->right = reConstructTree(curr->right, data, index);
	return curr;
}
