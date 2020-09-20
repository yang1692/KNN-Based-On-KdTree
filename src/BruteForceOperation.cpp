/*
 * BruteForceOperation.cpp
 *
 *  Created on: Nov 15, 2019
 *      Author: YK
 */
#include"main.h"
#include<memory>
#include<float.h>
#include<numeric>
#include<algorithm>
#include<cmath>
#include <stdlib.h>
#include <time.h>
using std::shared_ptr;
using std::make_shared;
using knn::KdTree;
using knn::Point;
using knn::BruteForceOperation;
void BruteForceOperation::bfSearchNClosestPoints(vector<shared_ptr<Point>> data, shared_ptr<Point> target, int n, vector<shared_ptr<Point>> &v){
	int size = data.size();
	if(size < n ) return ;
	double worstDis = DBL_MIN;
	vector<double> dis2Target;
	double curDis;
	for(int i = 0 ; i < n; i++){
		v.push_back(data[i]);
		curDis = data[i]->calcDis(target);
		dis2Target.push_back(curDis);
		worstDis = worstDis > curDis ? worstDis : curDis;
	}
	for(int i = n; i < size ; i++){
		curDis = data[i]->calcDis(target);
		if(curDis < worstDis){
			double newWorstDis = DBL_MIN;
			bool flag = true;
			for(int j = 0; j < n; j++){
				if(dis2Target[j] == worstDis && flag){
					v[j] = data[i];
					dis2Target[j] = curDis;
					flag = false;
				}
				newWorstDis = dis2Target[j] > newWorstDis ? dis2Target[j] : newWorstDis;
			}
			worstDis = newWorstDis;
		}
	}
}
shared_ptr<Point> BruteForceOperation::bfSearchClosestPoint(vector<shared_ptr<Point>> data, shared_ptr<Point> target){
	int size = data.size();
	if(size == 0 ) return NULL;
	double bestDis = data[0]->calcDis(target);
	double curDis ;
	shared_ptr<Point> rzt = data[0];
	for(int i = 1; i < size ; i++){
		curDis = data[i]->calcDis(target);
		if(curDis < bestDis){
			rzt = data[i];
			bestDis = curDis;
		}
	}
	return rzt;
}
shared_ptr<Point> BruteForceOperation::bfSearchClosestPositivePoint(vector<shared_ptr<Point>> data, shared_ptr<Point> target){
	int size = data.size();
	if(size == 0 ) return target;
	double bestDis = DBL_MAX;
	double curDis ;
	shared_ptr<Point> rzt = NULL;
	for(int i = 0; i < size ; i++){
		if(data[i] != target && data[i]->value_ == target->value_ ){
			curDis = data[i]->calcDis(target);
			if(curDis < bestDis){
				rzt = data[i];
				bestDis = curDis;
			}
		}
	}
	return rzt == NULL? target : rzt;
}
void BruteForceOperation::bfSearchNClosestNegativePoints(vector<shared_ptr<Point>> data, shared_ptr<Point> target,int n, vector<shared_ptr<Point>> &v){
	int size = data.size();
	double worstDis = DBL_MIN;
	vector<double> dis2Target;
	double curDis;
	int i = 0;
	for( int j = 0 ; i < size && j < n; i++){
		if(data[i]->value_ != target->value_){
			v.push_back(data[i]);
			curDis = data[i]->calcDis(target);
			dis2Target.push_back(curDis);
			worstDis = worstDis > curDis ? worstDis : curDis;
			j++;
		}
	}
	for(; i < size ; i++){
		if(data[i]->value_ != target->value_){
			curDis = data[i]->calcDis(target);
			if(curDis < worstDis){
				double newWorstDis = DBL_MIN;
				bool flag = true;
				for(int j = 0; j < n; j++){
					if(dis2Target[j] == worstDis && flag){
						v[j] = data[i];
						dis2Target[j] = curDis;
						flag = false;
					}
					newWorstDis = dis2Target[j] > newWorstDis ? dis2Target[j] : newWorstDis;
				}
				worstDis = newWorstDis;
			}
		}
	}
}
