/*
 * main.cpp
 *
 *  Created on: Oct 11, 2019
 *      Author: YK
 */
#include "Dataset.h"
#include "main.h"
#include "gtest/gtest.h"
#include <memory>
#include <iostream>
using hw3::Dataset;
using std::shared_ptr;
using knn::KdTreeOperation;
using knn::KdTree;
using std::make_shared;
using knn::Point;
using knn::BruteForceOperation;
const int N = 5;
int main(){
	KdTreeOperation op;
	BruteForceOperation bf;
	auto data = op.generateDataset(1000, 3, 100);
	shared_ptr<KdTree> myTree = op.buildTree(data, NULL);
	vector<double> v = {17,99,3};
	shared_ptr<Point> p=make_shared<Point>(v,"NO");


	//Test code for searchNClosestNegativePoints
	/*vector<shared_ptr<KdTree>> closestP;
	op.searchNClosestNegativePoints(myTree, data[0], N, closestP);
	vector<shared_ptr<Point>> rightRzt;
	bf.bfSearchNClosestNegativePoints(data, data[0], N, rightRzt);
	data[0]->print();
	cout<<"My result:\n";*/



	//Test code for searchNearestPositivePoint

	shared_ptr<KdTree> myRzt= op.searchNearestPositivePoint(myTree, data[0]);
	shared_ptr<Point> rightRzt = bf.bfSearchClosestPositivePoint(data, data[0]);
	cout<<"Target Point:\n";
	data[0]->print();
	cout<<"My result:\n";
	myRzt->curPoint_->print();
	cout<<"Right result:\n";
	rightRzt->print();

	//Test code for searchNNearest Points
	/*

	vector<shared_ptr<KdTree>> closestP;
	op.searchNClosestPoints(myTree, p, N, closestP);
	vector<shared_ptr<Point>> rightRzt;
	bf.bfSearchNClosestPoints(data, p, N, rightRzt);
	cout<<"My result:\n";
	for(int i = 0 ; i < N ; i++){
		cout<<closestP[i]->curPoint_->calcDis(p)<<'\t';
		closestP[i]->curPoint_->print();

	}
	cout<<"Right result:\n";
	for(int i = 0 ; i < N ; i++){
		cout<<rightRzt[i]->calcDis(p)<<'\t';
		rightRzt[i]->print();

	}*/



	::testing::InitGoogleTest();

	int r =  RUN_ALL_TESTS();
	system("PAUSE");
	return r;
}
TEST(Knn, searchNearstNegativePoints){
	KdTreeOperation op;
	BruteForceOperation bf;
	for(int i = 0 ;i <10000; i++){
		auto data = op.generateDataset(1000, 3, 100);
		shared_ptr<KdTree> myTree = op.buildTree(data, NULL);
		vector<double> v = {17,99,3};
		shared_ptr<Point> p=make_shared<Point>(v,"NO");


		vector<shared_ptr<KdTree>> closestP;
		op.searchNClosestNegativePoints(myTree, p,DBL_MIN, N, closestP);
		vector<shared_ptr<Point>> rightRzt;
		bf.bfSearchNClosestNegativePoints(data, p, N, rightRzt);

		bool ifPass = true;
		for(int i = 0 ; i < N; i++){
			int j = 0;
			for( ; j < N; j++)
			{
				if(rightRzt[j]->calcDis(p) == closestP[i]->curPoint_->calcDis(p))
					break;
			}
			if(j==N) {
				ifPass = false;
				break;
			}
		}
		ASSERT_EQ(ifPass,true )<<"Predict Function Result is Wrong!";
	}
}
/*TEST(Knn, KdTree){
	KdTreeOperation op;
	BruteForceOperation bf;
	for(int i = 0 ;i <10000; i++){
		auto data = op.generateDataset(1000, 3, 100);
		shared_ptr<KdTree> myTree = op.buildTree(data, NULL);
		vector<double> v = {17,99,3};
		shared_ptr<Point> p=make_shared<Point>(v,"false");
		vector<shared_ptr<KdTree>> closestP;
		op.searchNClosestPoints(myTree, p, DBL_MIN, N, closestP);
		vector<shared_ptr<Point>> rightRzt;
		bf.bfSearchNClosestPoints(data, p, N, rightRzt);
		bool ifPass = true;
		for(int i = 0 ; i < N; i++){
			int j = 0;
			for( ; j < N; j++)
			{
				if(rightRzt[j]->calcDis(p) == closestP[i]->curPoint_->calcDis(p))
					break;
			}
			if(j==N) {
				ifPass = false;
				break;
			}
		}
		ASSERT_EQ(ifPass,true )<<"Predict Function Result is Wrong!";
	}
}
*/

