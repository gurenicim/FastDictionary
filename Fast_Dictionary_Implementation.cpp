#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <ctime>
#include <ratio>

using namespace std;

bool isPrime( int n )
{
	if ( n == 2 || n == 3 )
		return true;

	if ( n == 1 || n % 2 == 0 )
		return false;

	for ( int i = 3; i * i <= n; i += 2 )
		if ( n % i == 0 )
			return false;

	return true;
}

int nextPrime( int n )
{
	if ( n % 2 == 0 )
		n++;

	for ( ; ! isPrime( n ); n += 2 )
		;

	return n;
}

/*
╔╗ ╔═╗╔╦╗
╠╩╗╚═╗ ║ 
╚═╝╚═╝ ╩ 
*/
using namespace std;
bool isExist = false;
bool isFound = false;

template <class T>
struct TreeNode
{

	T word;
	T translation;
	TreeNode<T> * RNext;
	TreeNode<T> * LNext;

	TreeNode(T element, T translation, TreeNode<T> * rptr, TreeNode<T> * lptr)
	{
		this->word = element;
		this->translation = translation;
		this->RNext = rptr;
		this->LNext = lptr;
	}

	TreeNode() : word(""), translation(""), RNext(nullptr), LNext(nullptr) {}
};

template <class T>
class BST
{
public:
	BST();
	~BST();
	void addleaf(T word, T turkishword);
	TreeNode<T> * createleaf(T eng, T turk);
	TreeNode<T> * search(TreeNode<T> * root, T key);
	void translationadder(TreeNode<T> * ptr, T translation);
	TreeNode<T> * rooter();
private:
	TreeNode<T> * root;
	void addleafpriv(T word, T turkishword, TreeNode<T>* ptr);

};

template <class T>
BST<T>::BST()
{
	root = nullptr;
}

template <class T>
BST<T>::~BST()
{
}

template <class T>
TreeNode<T> * BST<T>::search(TreeNode<T> * root, T key)
{

	// Base Cases: root is null or key is present at root 
	if (root == nullptr || root->word == key) 
		return root; 

	// Key is greater than root's key 
	if (root->word < key) 
		return search(root->RNext, key); 

	// Key is smaller than root's key 
	/*if(root->word > key)*/
	return search(root->LNext, key); 
}

template <class T>
TreeNode<T> * BST<T>::rooter()
{
	return root;
}

template <class T>
void BST<T>::translationadder(TreeNode<T> * ptr, T translation)
{
	ptr->translation = ptr->translation + ", " + translation;
}

template <class T>
TreeNode<T> * BST<T>::createleaf(T eng, T turk)
{
	TreeNode<T> * nodex = new TreeNode<T>;
	nodex->word=eng;
	nodex->translation=turk;
	return nodex;
}

template <class T>
void BST<T>::addleaf(T word, T turkishword)
{
	addleafpriv(word, turkishword, root);
}

template <class T>
void BST<T>::addleafpriv(T word, T turkishword, TreeNode<T> * ptr)
{
	if (root == nullptr)
	{
		root = createleaf(word, turkishword);
	}
	else
	{
		if (word > ptr->word)
		{
			if (ptr->RNext != nullptr)
			{
				addleafpriv(word, turkishword, ptr->RNext);
			}
			else
			{
				ptr->RNext = createleaf(word, turkishword);
			}
		}
		else if (word < ptr->word)
		{
			if (ptr->LNext != nullptr)
			{
				addleafpriv(word, turkishword, ptr->LNext);
			}
			else
			{
				ptr->LNext = createleaf(word, turkishword);
			}
		}
		else if ((string(word) == string(ptr->word)) && string(ptr->translation).find(turkishword) == string::npos)
		{
			ptr->translation = ptr->translation + ", " + turkishword;
		}
	}
}




/*
╦ ╦╔═╗╔═╗╦ ╦
╠═╣╠═╣╚═╗╠═╣
╩ ╩╩ ╩╚═╝╩ ╩
*/

template <class T>
struct HashNode
{
	T engword;
	T trword;
	HashNode<T> * next;

	HashNode(T engword, T translation, HashNode<T> * nxt)
	{
		this->engword = engword;
		this->trword = translation;
		this->next = nxt;
	}

	HashNode()
	{
		this->engword="";
		this->trword="";
		this->next=nullptr;
	}
};


template <class T>
class HashTable
{
public:

	int getItemCount()
	{
		return Item_Count;
	}
	vector <HashNode<T> *> HashT;
	HashTable();
	~HashTable();
	void insert(T eng, T meaning,  vector <HashNode<T> *> & VEC);
	T getTranslation(T dicword);

	//*** to get load factor
	double getLoadFac()
	{
		double itemcount = this->Item_Count, Tabsize = this->Table_Size;
		double result;
		result = (itemcount/Tabsize);
		return result;
	}
	//***

	//*** Rehashing
	void rehash()
	{
		string englishword = "", turkishword = "";
		if (getLoadFac() >= 0.5)
		{
			cout << "rehashed..." << endl;
			int prevsize = Table_Size;
			vector <HashNode<T> *> TMP(nextPrime(2*Table_Size));
			cout << "previous table size: " << Table_Size << ", ";
			//HashT.resize(nextPrime(2*Table_Size));
			int uniqItemc = Item_Count;
			this->Item_Count = 0;
			Table_Size = TMP.size();
			for (int i = 0; i < prevsize; i++)
			{
				HashNode<T> * unnec = HashT[i];
				if (unnec!=nullptr)
				{
					englishword = unnec->engword;
					turkishword = unnec->trword;
					insert(englishword,turkishword, TMP);
					while (unnec->next != nullptr)
					{
						unnec = unnec->next;
						englishword = unnec->engword;
						turkishword = unnec->trword;
						insert(englishword,turkishword, TMP);
					}
				}
			}
			this->HashT=TMP;
			this->Table_Size=TMP.size();
			cout << "new table size: " << Table_Size << ", current unique word count: " << uniqItemc << ", current load factor: " << getLoadFac() << endl;
		}
	}

	//***** Hash Function *****
	int hash(T word)
	{
		int hash = 0;
		int length = word.length();
		for (int i = 0; i < length; i++)
		{
			hash = hash*137 + word.at(i);
			hash= hash % Table_Size;
		}
		return hash;
	}
	//***** End Of Hash Function *****

private:
	int Table_Size;
	int Item_Count;
};

template <class T>
HashTable<T>::HashTable()
{
	Item_Count = 0;
	Table_Size = 53;
	HashT.resize(Table_Size);
}

template <class T>
HashTable<T>::~HashTable()
{
}

template <class T>
void HashTable<T>::insert(T eng, T meaning, vector <HashNode<T> *> & VEC)
{
	if (eng == "train")
	{
		int x = 5;
	}
	rehash();
	//whether the word is already exist;
	bool alreadyExist= false;
	int index = 0;
	HashNode<T> * node = new HashNode<T>(eng,meaning,nullptr);
	index = hash(eng);
	if (VEC[index]==nullptr)
	{
		VEC[index] = node;
		Item_Count++;
	}
	// to make seperated chain
	else
	{
		HashNode<T> * tmp = VEC[index];
		if ((tmp->engword == node->engword) && (tmp->trword == node->trword))
		{
			//cout << "Word already exists, if you want to add new translation please use command 2." << endl;
			alreadyExist = true;
		}
		else if (alreadyExist==false && tmp->engword==node->engword )
		{
			Item_Count--;
		}
		if (alreadyExist == false)
		{

			while (tmp->next != nullptr)
			{
				if (tmp->engword != node->engword)
				{
					tmp = tmp->next;
				}
				else if (tmp->engword == node->engword)
				{
					if (tmp->trword != node->trword)
					{
						tmp = tmp->next;
					}
					if (alreadyExist==false && tmp->engword==node->engword )
					{
						Item_Count--;
					}
					else
					{
						//cout << "Word already exists, if you want to add new translation please use command 2." << endl;
						alreadyExist = true;
					}
				}
			}
			if (alreadyExist==false)
			{
				tmp->next = node;
				Item_Count++;
			}
		}
	}
}

template <class T>
T HashTable<T>::getTranslation(T dicword)
{
	T meaning ="";
	HashNode<T> * ptr;
	int index = hash(dicword);
	ptr = HashT[index];
	int cnt2 = 0;
	if (ptr!=nullptr && ptr->engword == dicword)
	{
		meaning = ptr->trword;
		cnt2++;
	}
	while (ptr->next!=nullptr)
	{
		int cnt = 0;
		ptr= ptr->next;
		if (cnt == 0 && ptr->engword == dicword && cnt2 == 0)
		{
			meaning = ptr->trword;
			cnt++;
		}
		else if (ptr->engword == dicword)
		{
			meaning = meaning + ", " + ptr->trword;
		}
	}
	return meaning;
}


int main()
{
	double BST_time = 0, Hash_time = 0; 

	cout << "Building a binary tree for dict.txt..." << endl <<
		"Building a hash table for dict.txt..." << endl;


	vector <string> treequery;
	vector <string> query;
	ofstream out1;
	out1.open("ht_result.txt");
	ofstream out2;
	out2.open("bst_result.txt");

	HashTable<string> Dictionary;
	int count=0;
	string line="";
	string engword="";
	string allturkish="";
	string t_addition="";
	string filename="dict.txt";
	ifstream input (filename.c_str());
	if (input.is_open())
	{
		while (getline(input,line))
		{
			if (line != "")
			{
				istringstream iss(line);
				iss >> engword;
				if (engword.find("ï»¿") != string::npos)
				{
					engword=engword.substr(3,engword.length());
					count++;
				}
				iss >>allturkish;
				while (iss >> t_addition)
				{
					allturkish = allturkish+ " " + t_addition;
				}
				//inserting comes here
				Dictionary.insert(engword,allturkish, Dictionary.HashT);
			}
		}
		input.close();
		cout << endl << "After preprocessing, the unique word count is " << Dictionary.getItemCount() << ". Current load ratio is " << Dictionary.getLoadFac() << endl;


		cout << endl;
		int qcount=0;
		line="";
		engword="";
		string filename="query1.txt";
		cout << "Running queries in " << filename << "..." << endl << endl;
		ifstream input2 (filename.c_str());
		if (input2.is_open())
		{

			while (getline(input2,line))
			{
				if (line != "")
				{
					istringstream iss(line);
					iss >> engword;
					qcount++;
					if (engword.find("ï»¿") != string::npos)
					{
						engword=engword.substr(3,engword.length());
					}
					query.push_back(engword);					
				}
			}
			auto start = chrono::high_resolution_clock::now();
			for (int i = 0; i < query.size(); i++)
			{
				out1 << query[i] << " " << Dictionary.getTranslation(query[i]) << endl;
			}
			auto time = (chrono::high_resolution_clock::now() - start).count();
			cout<< "***********************************************"<< endl << "Benchmark results for Hash Table:" << endl << "***********************************************"<< endl;
			Hash_time = time;
			cout << "+Elapsed time: " << time << "ns" << endl;
			cout << "+Average query time: " << time/qcount << "ns" << endl;
			cout << endl;
			input2.close();
		}
		out1.close();



	}
	BST<string> Tree;
	line="";
	engword="";
	allturkish="";
	t_addition="";
	filename="dict.txt";
	input.open (filename.c_str());
	if (input.is_open())
	{
		while (getline(input,line))
		{
			if (line != "")
			{
				istringstream iss(line);
				iss >> engword;
				if (engword.find("ï»¿")!=string::npos)
				{
					engword=engword.substr(3,engword.length());
					count++;
				}
				iss >>allturkish;
				while (iss >> t_addition)
				{
					allturkish = allturkish+ " " + t_addition;
				}
				transform(engword.begin(), engword.end(), engword.begin(), ::tolower);
				transform(allturkish.begin(), allturkish.end(), allturkish.begin(), ::tolower);
				Tree.addleaf(engword, allturkish);
			}
		}
		input.close();
	}


	int qcount2 = 0;
	filename = "query1.txt";
	ifstream inputz;
	inputz.open(filename.c_str());
	if (inputz.is_open())
	{
		while (getline(inputz,line))
		{
			if (line != "")
			{
				istringstream iss(line);
				iss >> engword;
				qcount2++;
				if (engword.find("ï»¿") != string::npos)
				{
					engword=engword.substr(3,engword.length());
				}
				treequery.push_back(engword);
				if (Tree.search(Tree.rooter(),engword)==nullptr)
				{
				}
				else
				{
					/*auto StartPoint = chrono::high_resolution_clock::now();
					for (int i = 0; i < treequery.size(); i++)
					{
					out2 << Tree.search(Tree.rooter(),treequery[i])->word + " " + Tree.search(Tree.rooter(),treequery[i])->translation << endl;
					}
					*/
				}
			}
		}
		auto StartPoint = chrono::high_resolution_clock::now();
		for (int i = 0; i < treequery.size(); i++)
		{
			out2 << Tree.search(Tree.rooter(),treequery[i])->word + " " + Tree.search(Tree.rooter(),treequery[i])->translation << endl;
		}
		auto time = (chrono::high_resolution_clock::now() - StartPoint).count();
		BST_time = time;
		cout<< "***********************************************"<< endl << "Benchmark results for Binary Search Tree (BST):" << endl << "***********************************************"<< endl;
		cout << "+ Elapsed time: " << time << " ns." << endl;
		cout << "+ Average query time: " << time/qcount2 << " ns." << endl;
		cout << "+ Speed up: " << (BST_time/Hash_time) << "x" << endl;
		cout << endl;
		//std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		//std::chrono::duration<double> time_span =  std::chrono::duration_cast< std::chrono::duration<double>>(t2 - t1);
		//cout << "+Elapsed time:  " << time_span.count()*1000000000 << " ns." << endl;
		inputz.close();
	}

	out2.close();

	cout << "Time measurements in ns (N, 4096N):" << endl << "*****************************"<< endl << "bst" << endl;
	cout << "N\ttime\n";

	for(int i=1; i<4097; i=i*2){
		auto start3 = std::chrono::high_resolution_clock::now();
		for(int k = 0; k<i; k++)
			for(unsigned int j= 0; j<query.size(); j++)
				Tree.search(Tree.rooter(),query[j]);
		auto time_x = (std::chrono::high_resolution_clock::now() - start3).count();
		cout << i << "\t"<<time_x << endl;
	}

	cout << endl << "ht\nN\ttime\n";
	for(int i=1; i<=4096; i=i*2){
		auto start4 = std::chrono::high_resolution_clock::now();
		for(int g = 0; g<i; g++)
			for(int j= 0; j<treequery.size(); j++)
				Dictionary.getTranslation(treequery[j]);
		auto time4 = (std::chrono::high_resolution_clock::now() - start4).count();
		cout << i << "\t"<<time4 << endl;
	}
	cout << endl;

	return 0;
}