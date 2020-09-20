
#include"main.h"
#include<memory>
#include<float.h>
#include<numeric>
#include<algorithm>
#include<cmath>
#include <stdlib.h>
#include <time.h>
#include <map>
using std::shared_ptr;
using std::make_shared;
using knn::KdTreeOperation;
using knn::KdTree;
using knn::Point;
using std::map;
int KdTreeOperation::getDimension(vector<shared_ptr<Point>> data){
	int totalD = data[0]->coordinate_.size();
	vector<double> var1;
	for(int i = 0 ; i < totalD; i++){
		var1.push_back(this->getVar(data, i));
	}
	int d = 0;
	for(int j = 1 ; j < totalD; j++){
		d = var1[d]>var1[j] ? d:j;
	}
	return d;
}
double KdTreeOperation::getVar(vector<shared_ptr<Point>> data, int dimension){
	vector<double> resultSet;
	int size = data.size();
	for(int i =0; i < size; i++){
		resultSet.push_back(data[i]->coordinate_[dimension]);
	}
	double sum = std::accumulate(std::begin(resultSet), std::end(resultSet), 0.0);
	double mean =  sum / resultSet.size();

	double accum  = 0.0;
	std::for_each (std::begin(resultSet), std::end(resultSet), [&](const double d) {
		accum  += (d-mean)*(d-mean);
	});

	return accum/(resultSet.size()-1);
}
shared_ptr<KdTree> KdTreeOperation::buildTree(vector<shared_ptr<Point>> data,shared_ptr<KdTree> parent ){
	shared_ptr<KdTree> root = make_shared<KdTree>();
	root->parent_ = parent;
	vector<shared_ptr<Point>> leftData, rightData;
	int size = data.size();
	if(size == 0){
		return NULL;
	}
	if(size == 1){
		root->curPoint_ = data[0];
		return root;
	}
	root->dimension_= this->getDimension(data);
	double median = this->getMedianValue(data, root->dimension_);
	for(int i = 0; i <size; i++){
		if(data[i]->coordinate_[root->dimension_] > median){
			leftData.push_back(data[i]);
		}
		else if(data[i]->coordinate_[root->dimension_] < median){
			rightData.push_back(data[i]);
		}
		else if(root->curPoint_ == NULL){
			root->curPoint_ = data[i];
		}
		else if(root->curPoint_ != NULL){
			rightData.push_back(data[i]);
		}
	}
	if(leftData.size() > 0)
		root->leftChild_ = this->buildTree(leftData, root);
	if(rightData.size() > 0)
		root->rightChild_ = this->buildTree(rightData, root);
	return root;
}
vector<shared_ptr<Point>> KdTreeOperation::generateDataset(int num, int d, int range){
	vector<shared_ptr<Point>> rzt;
	srand((unsigned)time(NULL));
	for(int i = 0; i < num;i++ ) {
		vector<double> v;
		string value;
		for(int j = 0; j < d; j++){
			v.push_back(rand()%100);
		}
		value = rand()%2 == 1? "Yes":"NO";
		rzt.push_back(make_shared<Point>(v, value));
	}
	return rzt;
}
shared_ptr<KdTree> KdTreeOperation::searchClosestPoint(shared_ptr<KdTree> root, shared_ptr<Point> target){
	if(root == NULL || root->isEmpty()){
			return NULL;
	}
	double bestDis = DBL_MAX;
	shared_ptr<KdTree> myTree = root;
	shared_ptr<KdTree> bestPoint = root;
	while(!root->isLeaf()){
		if(root->leftChild_ == NULL){
			root = root->rightChild_;
		}
		else if(root->rightChild_ == NULL) {
			root = root->leftChild_;
		}
		else if(target->coordinate_[root->dimension_] >= root->curPoint_->coordinate_[root->dimension_] ){
			root = root->leftChild_;
		}
		else if(target->coordinate_[root->dimension_] < root->curPoint_->coordinate_[root->dimension_]){
			root = root->rightChild_;
		}
		else throw "Error!";
		double curDis = root->curPoint_->calcDis(target);
		if(curDis < bestDis){
			bestDis = curDis;
			bestPoint = root;
		}
	}
	shared_ptr<KdTree> parent = root->parent_;
	while( parent != myTree->parent_ ){
		if(abs(parent->curPoint_->coordinate_[parent->dimension_] - target->coordinate_[parent->dimension_]) < bestDis){
			shared_ptr<KdTree> newBest;
			if( root->isLeft()){
				 newBest = this->searchClosestPoint(parent->rightChild_, target, bestDis);
			}
			else if(root->isRight()){
				 newBest = this->searchClosestPoint( parent->leftChild_, target, bestDis);
			}
			if(newBest != NULL){
				double newBestDis = newBest->curPoint_->calcDis(target);
				if(newBestDis < bestDis){
					bestDis = newBestDis;
					bestPoint = newBest;
				}
			}
		}
		root = parent;
		parent = root->parent_;
	}
	return bestPoint;
}
shared_ptr<KdTree> KdTreeOperation::searchClosestPoint(shared_ptr<KdTree> root, shared_ptr<Point> target, double bestDis){
	if(root == NULL || root->isEmpty()){
		return NULL;
	}
	shared_ptr<KdTree> myTree = root;
	shared_ptr<KdTree> bestPoint = root;
	while(!root->isLeaf()){
		if(root->leftChild_ == NULL){
			root = root->rightChild_;
		}
		else if(root->rightChild_ == NULL) {
			root = root->leftChild_;
		}
		else if(target->coordinate_[root->dimension_] >= root->curPoint_->coordinate_[root->dimension_] ){
			root = root->leftChild_;
		}
		else if(target->coordinate_[root->dimension_] < root->curPoint_->coordinate_[root->dimension_]){
			root = root->rightChild_;
		}
		else throw "Error!";
		double curDis = root->curPoint_->calcDis(target);
		if(curDis < bestDis){
			bestDis = curDis;
			bestPoint = root;
		}
	}
	shared_ptr<KdTree> parent = root->parent_;
	while( parent != myTree->parent_ ){
		if(abs(parent->curPoint_->coordinate_[parent->dimension_] - target->coordinate_[parent->dimension_]) < bestDis){
			shared_ptr<KdTree> newBest;
			if( root->isLeft()){
				 newBest = this->searchClosestPoint(parent->rightChild_, target, bestDis);
			}
			else if(root->isRight()){
				 newBest = this->searchClosestPoint( parent->leftChild_, target, bestDis);
			}
			if(newBest != NULL){
				double newBestDis = newBest->curPoint_->calcDis(target);
				if(newBestDis < bestDis){
					bestDis = newBestDis;
					bestPoint = newBest;
				}
			}
		}
		root = parent;
		parent = root->parent_;
	}
	return bestPoint;
}

shared_ptr<KdTree> KdTreeOperation::searchNearestPositivePoint(shared_ptr<KdTree> root, shared_ptr<Point> target ){
	if(root == NULL || root->isEmpty()){
		shared_ptr<KdTree> sameAsTarget = make_shared<KdTree>();
		sameAsTarget->curPoint_ = target;
		return sameAsTarget;
	}
	double bestDis = DBL_MAX;
	shared_ptr<KdTree> myTree = root;
	shared_ptr<KdTree> bestPoint = make_shared<KdTree>();
	while(!root->isLeaf()){
		if(root->leftChild_ == NULL){
			root = root->rightChild_;
		}
		else if(root->rightChild_ == NULL) {
			root = root->leftChild_;
		}
		else if(target->coordinate_[root->dimension_] >= root->curPoint_->coordinate_[root->dimension_] ){
			root = root->leftChild_;
		}
		else if(target->coordinate_[root->dimension_] < root->curPoint_->coordinate_[root->dimension_]){
			root = root->rightChild_;
		}
		else throw "Error!";
		if(root->curPoint_ != target && target->value_ == root->curPoint_->value_  ){
			double curDis = root->curPoint_->calcDis(target);
			if(curDis < bestDis  ){
				bestDis = curDis;
				bestPoint = root;
				bestPoint->curPoint_->print();
			}
		}
	}
	shared_ptr<KdTree> parent = root->parent_;
	while( parent != myTree->parent_ ){
		if(abs(parent->curPoint_->coordinate_[parent->dimension_] - target->coordinate_[parent->dimension_]) < bestDis){
			shared_ptr<KdTree> newBest;
			if( root->isLeft()){
				 newBest = this->searchNearestPositivePoint(parent->rightChild_, target, bestDis);
			}
			else if(root->isRight()){
				 newBest = this->searchNearestPositivePoint( parent->leftChild_, target, bestDis);
			}
			if(newBest->curPoint_ != target && newBest->curPoint_ != target){
				cout<<"!";
				double newBestDis = newBest->curPoint_->calcDis(target);
				bestDis = newBestDis;
				bestPoint = newBest;
			}
		}
		root = parent;
		parent = root->parent_;
	}
	if(bestPoint->isEmpty()){
		shared_ptr<KdTree> sameAsTarget = make_shared<KdTree>();
		sameAsTarget->curPoint_ = target;
		return sameAsTarget;
	}
	return bestPoint;
}
shared_ptr<KdTree> KdTreeOperation::searchNearestPositivePoint(shared_ptr<KdTree> root, shared_ptr<Point> target ,double bestDis){
	if(root == NULL || root->isEmpty()){
		shared_ptr<KdTree> sameAsTarget = make_shared<KdTree>();
		sameAsTarget->curPoint_ = target;
		return sameAsTarget;
	}
	shared_ptr<KdTree> myTree = root;
	shared_ptr<KdTree> bestPoint = make_shared<KdTree>();
	while(!root->isLeaf()){
		if(root->leftChild_ == NULL){
			root = root->rightChild_;
		}
		else if(root->rightChild_ == NULL) {
			root = root->leftChild_;
		}
		else if(target->coordinate_[root->dimension_] >= root->curPoint_->coordinate_[root->dimension_] ){
			root = root->leftChild_;
		}
		else if(target->coordinate_[root->dimension_] < root->curPoint_->coordinate_[root->dimension_]){
			root = root->rightChild_;
		}
		else throw "Error!";
		if(root->curPoint_ != target && target->value_ == root->curPoint_->value_){
			double curDis = root->curPoint_->calcDis(target);
			if(curDis < bestDis  ){
				bestDis = curDis;
				bestPoint = root;
				bestPoint->curPoint_->print();
			}
		}
	}
	shared_ptr<KdTree> parent = root->parent_;
	while( parent != myTree->parent_ ){
		if(abs(parent->curPoint_->coordinate_[parent->dimension_] - target->coordinate_[parent->dimension_]) < bestDis){
			shared_ptr<KdTree> newBest;
			if( root->isLeft()){
				 newBest = this->searchNearestPositivePoint(parent->rightChild_, target, bestDis);
			}
			else if(root->isRight()){
				 newBest = this->searchNearestPositivePoint( parent->leftChild_, target, bestDis);
			}
			if(newBest->curPoint_ != target){
				cout<<"!";
				double newBestDis = newBest->curPoint_->calcDis(target);
				bestDis = newBestDis;
				bestPoint = newBest;
			}
		}
		root = parent;
		parent = root->parent_;
	}
	if(bestPoint->isEmpty()){
		shared_ptr<KdTree> sameAsTarget = make_shared<KdTree>();
		sameAsTarget->curPoint_ = target;
		return sameAsTarget;
	}
	return bestPoint;
}

double KdTreeOperation::searchNClosestPoints(shared_ptr<KdTree> root, shared_ptr<Point> target, int n, vector<shared_ptr<KdTree>> &v){
	int size = v.size();
	double worstDis = DBL_MIN;
	if(n <= 0){
		throw "Invalid number of points!";
		return -1;
	}
	if(root == NULL || root->isEmpty()){
		if(size < n){
			throw "There are not enough points !";
		}
		return worstDis;
	}
	shared_ptr<KdTree> myTree = root;

	vector<double> dis2Target;
	for(int i = 0 ; i < size; i++ ){
		dis2Target.push_back(v[i]->curPoint_->calcDis(target));
	}
	double curDis = root->curPoint_->calcDis(target);
	if(size < n){
		size ++;
		v.push_back(root);
		dis2Target.push_back(curDis);
		worstDis = worstDis > curDis ? worstDis : curDis;
	}
	else if(curDis < worstDis){
		double newWorstDis = DBL_MIN;
		bool flag = true;
		for(int i = 0; i < n; i++){
			if(dis2Target[i] == worstDis && flag){
				v[i] = root;
				dis2Target[i] = curDis;
				flag = false;
			}
			newWorstDis = dis2Target[i] > newWorstDis ? dis2Target[i] : newWorstDis;
		}
		worstDis = newWorstDis;
	}
	while(!root->isLeaf()){
		if(root->leftChild_ == NULL){
			root = root->rightChild_;
		}
		else if(root->rightChild_ == NULL) {
			root = root->leftChild_;
		}
		else if(target->coordinate_[root->dimension_] >= root->curPoint_->coordinate_[root->dimension_] ){
			root = root->leftChild_;
		}
		else if(target->coordinate_[root->dimension_] < root->curPoint_->coordinate_[root->dimension_]){
			root = root->rightChild_;
		}
		else throw "Error!";
		curDis = root->curPoint_->calcDis(target);
		if(size < n){
			size++;
			v.push_back(root);
			dis2Target.push_back(curDis);
			worstDis = worstDis > curDis ? worstDis : curDis;
		}
		else if(curDis < worstDis){
			double newWorstDis = DBL_MIN;
			for(int i = 0; i < n; i++){
				if(dis2Target[i] == worstDis){
					v[i] = root;
					dis2Target[i] = curDis;
				}
				newWorstDis = dis2Target[i] > newWorstDis ? dis2Target[i] : newWorstDis;
			}
			worstDis = newWorstDis;
		}
	}
	shared_ptr<KdTree> parent = root->parent_;
	while( parent != myTree->parent_ ){
		if(abs(parent->curPoint_->coordinate_[parent->dimension_] - target->coordinate_[parent->dimension_]) < worstDis){
			double newWorstDis = DBL_MAX;
			if( root->isLeft()){
				 newWorstDis = this->searchNClosestPoints(parent->rightChild_, target, worstDis, n, v);
			}
			else if(root->isRight()){
				 newWorstDis = this->searchNClosestPoints( parent->leftChild_, target, worstDis, n, v);
			}
			worstDis = newWorstDis;
		}
		root = parent;
		parent = root->parent_;
	}
	return worstDis;
}
double KdTreeOperation::searchNClosestPoints(shared_ptr<KdTree> root, shared_ptr<Point> target, double worstDis, int n, vector<shared_ptr<KdTree>> &v){
	int size = v.size();
	if(n <= 0){
		throw "Invalid number of points!";
		return -1;
	}
	if(root == NULL || root->isEmpty()){
		if(size < n){
			throw "There are not enough points !";
		}
		return worstDis;
	}
	shared_ptr<KdTree> myTree = root;

	vector<double> dis2Target;
	for(int i = 0 ; i < size; i++ ){
		dis2Target.push_back(v[i]->curPoint_->calcDis(target));
	}
	double curDis = root->curPoint_->calcDis(target);
	if(size < n){
		size ++;
		v.push_back(root);
		dis2Target.push_back(curDis);
		worstDis = worstDis > curDis ? worstDis : curDis;
	}
	else if(curDis < worstDis){
		double newWorstDis = DBL_MIN;
		bool flag = true;
		for(int i = 0; i < n; i++){
			if(dis2Target[i] == worstDis && flag){
				v[i] = root;
				dis2Target[i] = curDis;
				flag = false;
			}
			newWorstDis = dis2Target[i] > newWorstDis ? dis2Target[i] : newWorstDis;
		}
		worstDis = newWorstDis;
	}
	while(!root->isLeaf()){
		if(root->leftChild_ == NULL){
			root = root->rightChild_;
		}
		else if(root->rightChild_ == NULL) {
			root = root->leftChild_;
		}
		else if(target->coordinate_[root->dimension_] >= root->curPoint_->coordinate_[root->dimension_] ){
			root = root->leftChild_;
		}
		else if(target->coordinate_[root->dimension_] < root->curPoint_->coordinate_[root->dimension_]){
			root = root->rightChild_;
		}
		else throw "Error!";
		curDis = root->curPoint_->calcDis(target);
		if(size < n){
			size++;
			v.push_back(root);
			dis2Target.push_back(curDis);
			worstDis = worstDis > curDis ? worstDis : curDis;
		}
		else if(curDis < worstDis){
			double newWorstDis = DBL_MIN;
			for(int i = 0; i < n; i++){
				if(dis2Target[i] == worstDis){
					v[i] = root;
					dis2Target[i] = curDis;
				}
				newWorstDis = dis2Target[i] > newWorstDis ? dis2Target[i] : newWorstDis;
			}
			worstDis = newWorstDis;
		}
	}
	shared_ptr<KdTree> parent = root->parent_;
	while( parent != myTree->parent_ ){
		if(abs(parent->curPoint_->coordinate_[parent->dimension_] - target->coordinate_[parent->dimension_]) < worstDis){
			double newWorstDis = DBL_MAX;
			if( root->isLeft()){
				 newWorstDis = this->searchNClosestPoints(parent->rightChild_, target, worstDis, n, v);
			}
			else if(root->isRight()){
				 newWorstDis = this->searchNClosestPoints( parent->leftChild_, target, worstDis, n, v);
			}
			worstDis = newWorstDis;
		}
		root = parent;
		parent = root->parent_;
	}
	return worstDis;
}

double KdTreeOperation::searchNClosestNegativePoints(shared_ptr<KdTree> root, shared_ptr<Point> target, double worstDis, int n, vector<shared_ptr<KdTree>> &v){
	int size = v.size();
	if(n <= 0){
		throw "Invalid number of points!";
		return -1;
	}
	if(root == NULL || root->isEmpty()){
		return worstDis;
	}
	shared_ptr<KdTree> myTree = root;

	vector<double> dis2Target;
	for(int i = 0 ; i < size; i++ ){
		dis2Target.push_back(v[i]->curPoint_->calcDis(target));
	}
	if(root->curPoint_->value_ != target->value_ ){
		double curDis = root->curPoint_->calcDis(target);
		if(size < n){
			size ++;
			v.push_back(root);
			dis2Target.push_back(curDis);
			worstDis = worstDis > curDis ? worstDis : curDis;
		}
		else if(curDis < worstDis){
			double newWorstDis = DBL_MIN;
			bool flag = true;
			for(int i = 0; i < n; i++){
				if(dis2Target[i] == worstDis && flag){
					v[i] = root;
					dis2Target[i] = curDis;
					flag = false;
				}
				newWorstDis = dis2Target[i] > newWorstDis ? dis2Target[i] : newWorstDis;
			}
			worstDis = newWorstDis;
		}
	}
	while(!root->isLeaf()){
		if(root->leftChild_ == NULL){
			root = root->rightChild_;
		}
		else if(root->rightChild_ == NULL) {
			root = root->leftChild_;
		}
		else if(target->coordinate_[root->dimension_] >= root->curPoint_->coordinate_[root->dimension_] ){
			root = root->leftChild_;
		}
		else if(target->coordinate_[root->dimension_] < root->curPoint_->coordinate_[root->dimension_]){
			root = root->rightChild_;
		}
		else throw "Error!";
		if(root->curPoint_->value_ != target->value_){
			double curDis = root->curPoint_->calcDis(target);
			if(size < n){
				size++;
				v.push_back(root);
				dis2Target.push_back(curDis);
				worstDis = worstDis > curDis ? worstDis : curDis;
			}
			else if(curDis < worstDis){
				double newWorstDis = DBL_MIN;
				for(int i = 0; i < n; i++){
					if(dis2Target[i] == worstDis){
						v[i] = root;
						dis2Target[i] = curDis;
					}
					newWorstDis = dis2Target[i] > newWorstDis ? dis2Target[i] : newWorstDis;
				}
				worstDis = newWorstDis;
			}
		}
	}
	shared_ptr<KdTree> parent = root->parent_;
	while( parent != myTree->parent_ ){
		if(abs(parent->curPoint_->coordinate_[parent->dimension_] - target->coordinate_[parent->dimension_]) < worstDis){
			double newWorstDis = DBL_MAX;
			if( root->isLeft()){
				 newWorstDis = this->searchNClosestNegativePoints(parent->rightChild_, target, worstDis, n, v);
			}
			else if(root->isRight()){
				 newWorstDis = this->searchNClosestNegativePoints( parent->leftChild_, target, worstDis, n, v);
			}
			worstDis = newWorstDis;
		}
		root = parent;
		parent = root->parent_;
	}
	return worstDis;
}
double KdTreeOperation::searchNClosestNegativePoints(shared_ptr<KdTree> root, shared_ptr<Point> target, int n, vector<shared_ptr<KdTree>> &v){
	int size = v.size();
	double worstDis = DBL_MIN;
	if(n <= 0){
		throw "Invalid number of points!";
		return -1;
	}
	if(root == NULL || root->isEmpty()){
		return worstDis;
	}
	shared_ptr<KdTree> myTree = root;

	vector<double> dis2Target;
	for(int i = 0 ; i < size; i++ ){
		dis2Target.push_back(v[i]->curPoint_->calcDis(target));
	}
	if(root->curPoint_->value_ != target->value_ ){
		double curDis = root->curPoint_->calcDis(target);
		if(size < n){
			size ++;
			v.push_back(root);
			dis2Target.push_back(curDis);
			worstDis = worstDis > curDis ? worstDis : curDis;
		}
		else if(curDis < worstDis){
			double newWorstDis = DBL_MIN;
			bool flag = true;
			for(int i = 0; i < n; i++){
				if(dis2Target[i] == worstDis && flag){
					v[i] = root;
					dis2Target[i] = curDis;
					flag = false;
				}
				newWorstDis = dis2Target[i] > newWorstDis ? dis2Target[i] : newWorstDis;
			}
			worstDis = newWorstDis;
		}
	}
	while(!root->isLeaf()){
		if(root->leftChild_ == NULL){
			root = root->rightChild_;
		}
		else if(root->rightChild_ == NULL) {
			root = root->leftChild_;
		}
		else if(target->coordinate_[root->dimension_] >= root->curPoint_->coordinate_[root->dimension_] ){
			root = root->leftChild_;
		}
		else if(target->coordinate_[root->dimension_] < root->curPoint_->coordinate_[root->dimension_]){
			root = root->rightChild_;
		}
		else throw "Error!";
		if(root->curPoint_->value_ != target->value_){
			double curDis = root->curPoint_->calcDis(target);
			if(size < n){
				size++;
				v.push_back(root);
				dis2Target.push_back(curDis);
				worstDis = worstDis > curDis ? worstDis : curDis;
			}
			else if(curDis < worstDis){
				double newWorstDis = DBL_MIN;
				for(int i = 0; i < n; i++){
					if(dis2Target[i] == worstDis){
						v[i] = root;
						dis2Target[i] = curDis;
					}
					newWorstDis = dis2Target[i] > newWorstDis ? dis2Target[i] : newWorstDis;
				}
				worstDis = newWorstDis;
			}
		}
	}
	shared_ptr<KdTree> parent = root->parent_;
	while( parent != myTree->parent_ ){
		if(abs(parent->curPoint_->coordinate_[parent->dimension_] - target->coordinate_[parent->dimension_]) < worstDis){
			double newWorstDis = DBL_MAX;
			if( root->isLeft()){
				 newWorstDis = this->searchNClosestNegativePoints(parent->rightChild_, target, worstDis, n, v);
			}
			else if(root->isRight()){
				 newWorstDis = this->searchNClosestNegativePoints( parent->leftChild_, target, worstDis, n, v);
			}
			worstDis = newWorstDis;
		}
		root = parent;
		parent = root->parent_;
	}
	return worstDis;
}
//This part is still not finished
/*vector<double> KdTreeOperation::calcWeight(vector<shared_ptr<Point>> data, shared_ptr<KdTree> myTree, int n){
	double weight = 0;
	int size = data.size();
	if(size == 0) {
		throw "Empty dataset!";
		return NULL;
	}
	vector<double> v;
	int dim = data[0]->coordinate_.size();
	for(int curIndex = 0; curIndex < size; curIndex++){
		shared_ptr<KdTree> positiveP = this->searchNearestPositivePoint(myTree, data[curIndex]);
		vector<shared_ptr<KdTree>> negativePs ;
		this->searchNClosestNegativePoints(myTree, data[curIndex],n,negativePs);
		for(int i = 0 ; i < size; i++){
			weight += positiveP->curPoint_->coordinate_[i];
			int ngSize = negativePs.size();
			map<string, double> sumHashMap, numHashMap;
			for(int j = 0; j < ngSize; j++){
				if(sumHashMap.find(negativePs[j]->curPoint_->value_)){
					sumHashMap[negativePs[j]->curPoint_->value_] += negativePs[j]->curPoint_->coordinate_[i];
					numHashMap[negativePs[j]->curPoint_->value_] += 1;

				}
				else{
					sumHashMap[negativePs[j]->curPoint_->value_] = negativePs[j]->curPoint_->coordinate_[i];
					numHashMap[negativePs[j]->curPoint_->value_] = 0;
				}
			}
		}
	}
}*/
double KdTreeOperation::getMedianValue(vector<shared_ptr<Point>> data, int d){
	vector<double> dataOfD;
	int size = data.size();
	for(int i =0; i < size; i++){
		dataOfD.push_back(data[i]->coordinate_[d]);
	}
	std::nth_element(dataOfD.begin(), dataOfD.begin() + size/2, dataOfD.end());
	return dataOfD[size/2];
}


