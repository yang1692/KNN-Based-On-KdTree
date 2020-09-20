#include"main.h"
#include<memory>
#include<algorithm>
#include<cmath>
using knn::Point;
Point::Point(vector<double> v, string value){
	this->coordinate_ = v;
	this->value_ = value;
}
double Point::calcDis(shared_ptr<Point> target){
	int size = this->coordinate_.size();
	if(size != target->coordinate_.size()){
		throw "Error!";
	}
	double dis = 0;
	for(int i = 0 ; i < size; i++){
		dis += std::pow((this->coordinate_[i] - target->coordinate_[i]),2);
	}
	dis = sqrt(dis);
	return dis;
}
void Point::print(){
	for(auto iter = this->coordinate_.begin(); iter != this->coordinate_.end(); iter++){
		cout<<*iter<<'\t';
	}
	cout<<this->value_;
	cout<<'\n';
}

