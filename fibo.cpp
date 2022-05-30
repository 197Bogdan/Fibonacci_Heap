#include <fstream>
#include <vector>
#include <cmath>

using namespace std;


template <class T>
class Node
{
	template <class T> friend class FibonacciHeap;
	Node<T>(T value): value(value), height(0), parent(nullptr), left(nullptr), right(nullptr) {}
private:
	T value;
	int height;				// equals the order of the tree
	vector<Node<T>*> children;
	Node<T>* parent;
	Node<T>* left;				// circular doubly linked list component1
	Node<T>* right;				// and component 2
};

template <class T>
class FibonacciHeap
{
public:
	FibonacciHeap() : max(nullptr), node_count(0) {}
	void insert(T);
	void insert(Node<T>*);
	void merge(FibonacciHeap&);
	int getMax();
	void deleteMax();
private:
	Node<T>* max;
	int node_count;
};

template <class T>
void FibonacciHeap<T>::insert(T value)		// insert by value
{
	if (!max)
	{
		max = new Node<T>(value);			
		max->left = max;
		max->right = max;
	}
	else
	{
		Node<T>* right_of_max = max->right;
		Node<T>* new_node = new Node<T>(value);

		max->right = new_node;
		right_of_max->left = new_node;			// insert between max and the element that was positioned to the right of max
		new_node->left = max;
		new_node->right = right_of_max;

		if (new_node->value > max->value)
			max = new_node;
	}
	node_count++;

}
template<class T>
void FibonacciHeap<T>::insert(Node<T>* n)		// insert an existing node
{
	if (!max)
	{
		max = n;
		max->left = n;
		max->right = n;
	}
	else
	{
		Node<T>* right_of_max = max->right;
		Node<T>* new_node = n;

		max->right = new_node;
		right_of_max->left = new_node;
		new_node->left = max;
		new_node->right = right_of_max;

		if (new_node->value > max->value)
			max = new_node;
	}
	node_count++;
}

template <class T>
void FibonacciHeap<T>::merge(FibonacciHeap& fh)
{
	if (!fh.max)
		return;
	if (!max)
	{
		*this = fh;
		return;
	}
	node_count += fh.node_count;
	fh.node_count = 0;
	Node<T>* this_begin = max->right;			// list1=(x1 x2 x3 x4)    list2=(y1 y2 y3 y4), assume x4 = max and y1 = fh.max
	Node<T>* fh_end = fh.max->left;			// save pointers to x1 (first list begin), y4(second list end)
			
	max->right = fh.max;					// link x4 to y1
	fh.max->left = max;
	this_begin->left = fh_end;				// link x1 to y4
	fh_end->right = this_begin;

	if (fh.max->value > this->max->value)
		this->max = fh.max;
	fh.max = nullptr;


}

template <class T>
int FibonacciHeap<T>::getMax()
{
	return max->value;
}

template <class T>
void FibonacciHeap<T>::deleteMax()
{
	if (!max)		// empty
		return;
	if (max->left == max && max->children.size() == 0)		// heap of 1 node
	{
		delete max;
		max = nullptr;
		node_count = 0;
		return;
	}
	for (auto subtree : max->children)
		this->insert(subtree);						// reinsert children into tree list as roots
	max->right->left = max->left;
	max->left->right = max->right;
	Node<T>* deleted_max_address = max;
	max = max->right;
	delete deleted_max_address;						// and delete max
	node_count--;

	vector<Node<T>*> order_distribution(log2(node_count) + 1, nullptr);		// log2(n) max possible number of trees
	order_distribution[max->height] = max;
	Node<T>* list_iterator = max->right;
	Node<T>* end_point = max;
	while(list_iterator != end_point)		// for all trees in heap
	{
		Node<T>* new_tree = list_iterator;
		while(order_distribution[new_tree->height] != nullptr)				// if there exists a tree of this order, combine them into a (order+1) tree
		{
			Node<T>* other_root = order_distribution[new_tree->height];
			if (new_tree->value >= other_root->value)
			{

				new_tree->children.push_back(other_root);
				other_root->parent = new_tree;
				order_distribution[new_tree->height] = nullptr;
				new_tree->height++;
			}
			else
			{

				other_root->children.push_back(new_tree);
				new_tree->parent = other_root;
				order_distribution[new_tree->height] = nullptr;
				other_root->height++;

				new_tree = other_root;
			}
		}
		order_distribution[new_tree->height] = new_tree;
		list_iterator = list_iterator->right;
	}

	max = nullptr;
	for (auto rebuilt_root : order_distribution)			// create list from built trees, max O(logn) trees
		if (rebuilt_root)
		{
			this->insert(rebuilt_root);
			if (rebuilt_root->value > max->value)
				max = rebuilt_root;
		}

}

int main()
{
	ifstream f("mergeheap.in");
	ofstream g("mergeheap.out");
	int tip_operatie, nr_operatii, nr_heapuri;
	f >> nr_heapuri >> nr_operatii;
	int v1, v2;
	vector<FibonacciHeap<int>> heapuri(nr_heapuri + 1);
	for (int i = 0; i < nr_operatii; i++)
	{
		f >> tip_operatie;
		if (tip_operatie == 1)
		{
			f >> v1 >> v2;
			heapuri[v1].insert(v2);
		}
		else if (tip_operatie == 2)
		{
			f >> v1;
			g << heapuri[v1].getMax() << "\n";
			heapuri[v1].deleteMax();
		}
		else if (tip_operatie == 3)
		{
			f >> v1 >> v2;
			heapuri[v1].merge(heapuri[v2]);
		}

	}





}