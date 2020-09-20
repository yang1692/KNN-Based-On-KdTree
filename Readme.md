##KuidTree is a project based on c++ language development. It is based on KdTree data structure and used for Knn algorithm.##  
  
IDE: Eclipse IDE for C/C++ Developers Version: 2019-06 (4.12.0)  
  
Set-up : Windows 10 with Eclipse CDT and MinGW-W64-builds-4.3.5  
  
The package provides the functions for building tree, searching N closed points, test toolkits.  
  
All classes are defined in main.h file.  
  
There are 3 main classes in this project:  
(1) KdTree about the basic structure of the tree  
(2) KdtreeOperation about the operations about a KdTree, such as building a tree,   
(3) BruteForceOperation is a test tookit that uses bruteforce to get the correct result.  
  
  
Main functions:  
(1)KdTreeOperation.buildTree(vector<shared_ptr<Point>> data,  shared_ptr<KdTree> parent);  
  
The second parameter is used for recursively calling the function.  
  
So if you have a dataset and want to build a tree, then you should set the second parameter as NULL;  
  
for example:    
	KdTreeOperation op;  
	auto data = op.generateDataset(1000, 3, 100);  
	shared_ptr<KdTree> myTree = op.buildTree(data, NULL);  
  
(2)searchNClosestPoints(shared_ptr<KdTree> root, shared_ptr<Point> target, int n, vector<shared_ptr<KdTree>> &v);  
  
The first parameter is the root node of the KdTree.   
The second parameter is the target point.   
The third parameter is the number of points you want to search.   
The last parameter is the vector you want to put the result in.  
  
There are also another version of it, but it is only for recursively call. Thus, just use the function above.  
  
for example:  
	vector<shared_ptr<KdTree>> closestP;  
	op.searchNClosestPoints(myTree, p, N, closestP);  