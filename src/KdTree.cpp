#include"main.h"
#include<memory>
#include<algorithm>
#include<cmath>
using knn::KdTree;

void KdTree::print(int depth){
	if(this == NULL){
		cout<<'\n';
		return;
	}
	int size = this->curPoint_->coordinate_.size();
	cout<<'(';
	for(int i = 0; i < size ; i++){
		cout<<this->curPoint_->coordinate_[i];
		if(i != size-1) cout<<',';
	}
	cout<<')';
	cout<<"current dimension to split is "<<this->dimension_<<'.';
	cout<<" Its parent is ";
	if(this->parent_ != NULL){
		cout<<'(';
		for(int i = 0; i < size; i++){
			cout<<this->parent_->curPoint_->coordinate_[i];
			if(i != size-1 ) cout<<',';
		}
		cout<<")";
	}
	cout<<" Its value is "+this->curPoint_->value_<<'\n';
	if(this->leftChild_!=NULL){
		for(int i = 0; i < depth+1; i++)
			cout<<'\t';
		cout<<"Left:";
		this->leftChild_->print(depth+1);
	}
	if(this->rightChild_!=NULL){
		for(int i = 0; i < depth+1; i++)
					cout<<'\t';
			cout<<"Right:";
			this->rightChild_->print(depth+1);
	}

}
bool KdTree::isEmpty(){
	return this->curPoint_ == NULL || this == NULL;
}
bool KdTree::isLeaf(){
	return !this->isEmpty() && this->leftChild_==NULL &&this->rightChild_ == NULL;
}
bool KdTree::isRoot(){
	return !this->isEmpty() && this->parent_ == NULL;
}
bool KdTree::isLeft(){
	return this->parent_->leftChild_.get() == this;
}
bool KdTree::isRight(){
	return this->parent_->rightChild_.get() == this;
}
