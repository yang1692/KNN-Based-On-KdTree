/*
 * main.h
 *
 *  Created on: Oct 13, 2019
 *      Author: YK
 */
#include<memory>
#include<vector>
#include "Dataset.h"
using std::shared_ptr;
using hw3::Dataset;
namespace knn{
class Point{
public:
	vector<double> coordinate_;
	string value_;
	double calcDis(shared_ptr<Point> target);
	void print();
	Point(vector<double>v, string value);
};
class KdTree{
public:
	int dimension_;
	shared_ptr<Point> curPoint_;
	shared_ptr<KdTree> parent_;
	shared_ptr<KdTree> leftChild_;
	shared_ptr<KdTree> rightChild_;
	void print(int depth);
	bool isEmpty();
	bool isLeaf();
	bool isRoot();
	bool isLeft();
	bool isRight();
};

class KdTreeOperation{
public:
	vector<shared_ptr<Point>> generateDataset(int num, int d, int range);
	shared_ptr<KdTree> buildTree(vector<shared_ptr<Point>> data,  shared_ptr<KdTree> parent);

	double getMedianValue(vector<shared_ptr<Point>> data, int d);
	int getDimension(vector<shared_ptr<Point>> data);
	double getVar(vector<shared_ptr<Point>> data, int d);
	shared_ptr<KdTree> searchNearestPositivePoint(shared_ptr<KdTree> root, shared_ptr<Point> target );
	shared_ptr<KdTree> searchNearestPositivePoint(shared_ptr<KdTree> root, shared_ptr<Point> target ,double bestDis);

	shared_ptr<KdTree> searchClosestPoint(shared_ptr<KdTree> root, shared_ptr<Point> target,double bestDis);
	shared_ptr<KdTree> searchClosestPoint(shared_ptr<KdTree> root, shared_ptr<Point> target);

	double searchNClosestPoints(shared_ptr<KdTree> root, shared_ptr<Point> target, int n, vector<shared_ptr<KdTree>> &v);
	double searchNClosestPoints(shared_ptr<KdTree> root, shared_ptr<Point> target, double worstDis, int n, vector<shared_ptr<KdTree>> &v);

	double searchNClosestNegativePoints(shared_ptr<KdTree> root, shared_ptr<Point> target, int n, vector<shared_ptr<KdTree>> &v);
	double searchNClosestNegativePoints(shared_ptr<KdTree> root, shared_ptr<Point> target, double worstDis, int n, vector<shared_ptr<KdTree>> &v);

	//vector<double> KdTreeOperation::calcWeight(vector<shared_ptr<Point>> data,  shared_ptr<KdTree> myTree,int n);
};
class BruteForceOperation{
public:
	shared_ptr<Point> bfSearchClosestPositivePoint(vector<shared_ptr<Point>> data, shared_ptr<Point> target);
	shared_ptr<Point> bfSearchClosestPoint(vector<shared_ptr<Point>> data, shared_ptr<Point> target);
	void bfSearchNClosestPoints(vector<shared_ptr<Point>> data, shared_ptr<Point> target,int n, vector<shared_ptr<Point>> &v);
	void bfSearchNClosestNegativePoints(vector<shared_ptr<Point>> data, shared_ptr<Point> target,int n, vector<shared_ptr<Point>> &v);
};
class Typechecker{
public:
	virtual bool check(std::string input)=0;
};
class OptionTypeChecker:public Typechecker{
public:
	int value_=0;
	bool check(std::string input);
};
class UserInput{
	virtual bool input()=0;
};
class CommandLineUserInput:public UserInput{
private:
	std::string input_;
	bool performCheck();
public:
	shared_ptr<Typechecker> checker_;
	bool input();
	void setChecker(shared_ptr<Typechecker> checker);
};
}



