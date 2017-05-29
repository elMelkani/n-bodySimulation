#include "simulation.h"
#include <cstddef>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#define what_is(x) std::cerr << #x << " is " << x << std::endl;

node::node() {
	isComposite = false;
	isEmpty = true;
	mass = 0;
	X = Y = vX = vY = FX = FY = cX = cY = 0;
	depth = 0;
	branch1 = branch2 = branch3 = branch4 = NULL;
	parent = NULL;
}

node::node(double CX, double CY, int D) {
	isComposite = false;
	isEmpty = true;
	mass = 0;
	X = Y = vX = vY = FX = FY = 0;
	cX = CX;
	cY = CY;
	depth = D;
	branch1 = branch2 = branch3 = branch4 = NULL;
	parent = NULL;
}

void node::printNode() {
	what_is(mass);
	what_is(X);
	what_is(Y);
	what_is(cX);
	what_is(cY);
	what_is(depth);
	if(parent == NULL) std::cout<<"null";
	std::cout<<"\n";
}

void node::printShort() {
    if(isEmpty) ;
    std::cout<<"[M:"<<mass<<"|(cX,cY):("<<cX<<","<<cY<<")|(x,y):("<<X<<","<<Y<<")]\n";
    if(parent == NULL) std::cout<<"null\n";
}

void node::printTree(int i = 0) {
    int A[] = {0,0,0,0};
    std::cout<<"U"<<i<<":";
    printShort();
    if(isEmpty)
        std::cout<<"O\n";
    else {
        if(branch1 == NULL)
            std::cout<<"NW-"<<"X\n";
        else {
            i++;
            A[0] = i;
            std::cout<<"NW-U"<<i<<"\n";
        }
        if(branch2 == NULL)
            std::cout<<"NE-"<<"X\n";
        else {
            i++;
            A[1] = i;
            std::cout<<"NE-U"<<i<<"\n";
        }
        if(branch3 == NULL)
            std::cout<<"SW-"<<"X\n";
        else {
            i++;
            A[2] = i;
            std::cout<<"SW-U"<<i<<"\n";
        }
        if(branch4 == NULL)
            std::cout<<"SE-"<<"X\n";
        else {
            i++;
            A[3] = i;
            std::cout<<"SE-U"<<i<<"\n";
        }
        std::cout<<"\n";

        if(A[0] != 0) branch1->printTree(A[0]);
        if(A[1] != 0) branch2->printTree(A[1]);
        if(A[2] != 0) branch3->printTree(A[2]);
        if(A[3] != 0) branch4->printTree(A[3]);
    }
}

void node::forceBW2(node* N) {
	double dist = sqrt( (X - N->X)*(X - N->X) + (Y - N->Y)*(Y - N->Y) );
	if (dist < 1e-5) {
        return;
	}
	double force = (mass*N->mass)/(dist*dist);
	double cos = (X - N->X)/dist;
	double sin = (Y - N->Y)/dist;
	FX = FX  -force*cos;
	FY = FY  -force*sin;
	//return force;
}

void node::forceCalc(node* N) {
	if (N->mass == 0) return;
	if (N == this) return; //self-force
	if(N->isEmpty == true) return;
	if (N->isComposite == false) forceBW2(N); //its an external node

	double dist = sqrt( (X - N->X)*(X - N->X) + (Y - N->Y)*(Y - N->Y) );
	if(1/theta < dist*N->depth) // sufficiently far away
		forceBW2(N); //treat as single body
	else {
    //run recursively over all the branches. depth has increased.
  		forceCalc(N->branch1);
        forceCalc(N->branch2);
        forceCalc(N->branch3);
        forceCalc(N->branch4);
	}
}

void node::velocityCalc(double delT) {
    if(X<11 || X>(XX-11)) {
        vX = -0.5*vX;
    }
    else
        vX = vX + (FX/mass)*delT;
    if(Y<11 || Y>(YY-11)) {
        vY = -0.5*vY;
    }
    else
        vY = vY + (FY/mass)*delT;
    FX = 0;
    FY = 0;
}

void node::mover(double delT) {
    X = X + vX*delT;
    Y = Y + vY*delT;
    if (X<10) {
        X = 10;
    }
    else if (X>XX-10) {
        X = XX-10;
    }
    if (Y<10) {
        Y = 10;
    }
    else if(Y>YY-10) {
        Y = YY-10;
    }
}

bool node::checkIfInDomain(node* center){
	if (X > center->cX + XX/pow(2, center->depth + 1)) return false;
	if (X < center->cX - XX/pow(2, center->depth + 1)) return false;
	if (Y > center->cY + YY/pow(2, center->depth + 1)) return false;
	if (Y < center->cY - YY/pow(2, center->depth + 1)) return false;
	return true;
}

void node::makeComposite(node* N) { //this is the free node to be inserted. N is the solid(non composite) on the tree
	//U is the usurper which takes up the position of N in the tree. Now both N and this are dangling
	node* U;
	U = new node(N->cX, N->cY, N->depth);

	if(N->parent->branch1 == N) N->parent->branch1 = U;
	if(N->parent->branch2 == N) N->parent->branch2 = U;
	if(N->parent->branch3 == N) N->parent->branch3 = U;
	if(N->parent->branch4 == N) N->parent->branch4 = U;
	U->isComposite = true;
	U->isEmpty = false;
	U->mass = mass + N->mass;
	U->parent = N->parent;

    std::cout<<"\nreached make composite"<<X<<","<<Y<<"\nN is:\n";
    N->printNode();
    std::cout<<"this is:\n";
    this->printNode();

	node* N1;
	node* N2;
	node* N3;
	node* N4;
	int dep = N->depth;

	//...TO BE FILLED (OR MAKE A CONSTRUCTOR!!!) ---------------DONE!
	N1 = new node(N->cX - XX/pow(2, dep + 2) , N->cY - YY/pow(2, dep + 2) , dep + 1 );
	N2 = new node(N->cX + XX/pow(2, dep + 2) , N->cY - YY/pow(2, dep + 2) , dep + 1 );
	N3 = new node(N->cX - XX/pow(2, dep + 2) , N->cY + YY/pow(2, dep + 2) , dep + 1 );
	N4 = new node(N->cX + XX/pow(2, dep + 2) , N->cY + YY/pow(2, dep + 2) , dep + 1 );

	N1->parent = U;
	N2->parent = U;
	N3->parent = U;
	N4->parent = U;

	U->branch1 = N1;
	U->branch2 = N2;
	U->branch3 = N3;
	U->branch4 = N4;

	//node* Ni;
	//check in which sub-domain the center of mass of N lies so that we can pass the solid into that.
	if (N->checkIfInDomain(N1)) {
        //std::cout<<"in domain N1"<<N1->cX<<N1<<cY;
		U->branch1 = N;
		N->cX = N1->cX;
		N->cY = N1->cY;
		N->depth = N1->depth;
		delete N1;
		N1 = N;
	}
	else if (N->checkIfInDomain(N2)) {
	    //std::cout<<"in domain N2"<<N2->cX<<"/"<<N2->cY<<"?";
		U->branch2 = N;
		N->cX = N2->cX;
		N->cY = N2->cY;
		N->depth = N2->depth;
		delete N2;
		N2 = N;
	}
	else if (N->checkIfInDomain(N3)) {
	    //std::cout<<"in domain N3";
		U->branch3 = N;
		N->cX = N3->cX;
		N->cY = N3->cY;
		N->depth = N3->depth;
		delete N3;
		N3 = N;
	}
	else if (N->checkIfInDomain(N4)){
	    //std::cout<<"in domain N4";
		U->branch4 = N;
		N->cX = N4->cX;
		N->cY = N4->cY;
		N->depth = N4->depth;
		delete N4;
		N4 = N;
	}
	N->parent = U;
	N->isEmpty = false;

	if (this->checkIfInDomain(N)) {
        std::cout<<"eternity"<<N->cX<<"/"<<N->cY<<"?";
        if((N->X - X)*(N->X - X) + (N->Y - Y)*(N->Y - Y) < 10) {
            std::cout<<"TOO CLOSE!!!";
            exit(1);
        }
            //std::cout<<"route to eternity";
		this->makeComposite(N);
		return;
	}
	//good they have separate domains assigned.
	else if (this->checkIfInDomain(N1)){
		this->cX = N1->cX;
		this->cY = N1->cY;
		this->depth = N1->depth;
		U->branch1 = this;
		delete N1;
	}
	else if (this->checkIfInDomain(N2)){
		this->cX = N2->cX;
		this->cY = N2->cY;
		this->depth = N2->depth;
		U->branch2 = this;
		delete N2;
	}
    else if (this->checkIfInDomain(N3)){
		this->cX = N3->cX;
		this->cY = N3->cY;
		this->depth = N3->depth;
		U->branch3 = this;
		delete N3;
	}
    else if (this->checkIfInDomain(N4)){
		this->cX = N4->cX;
		this->cY = N4->cY;
		this->depth = N4->depth;
		U->branch4 = this;
		delete N4;
	}
	this->parent = U;
	this->isEmpty = false;
}

void node::insertNode(node* N) { //this is more of a insert body into existing structure of nodes.
	if(N->isEmpty == true) { //put body here, delete the empty node.
		this->cX = N->cX;
		this->cY = N->cY;
		this->depth = N->depth;
		this->isEmpty = false;
		this->parent = N->parent;
		if(N->parent->branch1 == N) N->parent->branch1 = this;
        if(N->parent->branch2 == N) N->parent->branch2 = this;
        if(N->parent->branch3 == N) N->parent->branch3 = this;
        if(N->parent->branch4 == N) N->parent->branch4 = this;
        delete N;
		return;
	}
	if(N->isEmpty == false && N->isComposite == true) {
		//route to proper spatial region
		std::cout<<"route"<<X<<","<<Y<<"\n";
		//this->printNode();
		N->mass = N->mass + this->mass;
		if (X <= N->cX && Y <= N->cY) {
            this->insertNode(N->branch1);
            return;
		}
		if (X >= N->cX && Y <= N->cY) {
            this->insertNode(N->branch2);
            return;
		}
		if (X <= N->cX && Y >= N->cY) {
            this->insertNode(N->branch3);
            return;
		}
		if (X >= N->cX && Y >= N->cY) {
            this->insertNode(N->branch4);
            return;
		}
	}
	if(N->isEmpty == false && N->isComposite == false) {
		//Oops! routed to a solid body! need to make new nodes:
		//std::cout<<"composite";
		std::cout<<"before composite";
		this->makeComposite(N);
		return;
	}
}

void node::updateCOM() {
    if (branch1->isComposite == true) branch1->updateCOM();
    if (branch2->isComposite == true) branch2->updateCOM();
    if (branch3->isComposite == true) branch3->updateCOM();
    if (branch4->isComposite == true) branch4->updateCOM();
    //If you have reached here all your branches are up to date. You can now use their positions.
    X = (!branch1->isEmpty)*(branch1->X)*(branch1->mass) + (!branch2->isEmpty)*(branch2->X)*(branch2->mass) + (!branch3->isEmpty)*(branch3->X)*(branch3->mass) + (!branch4->isEmpty)*(branch4->X)*(branch4->mass);
    X = X/mass;
    Y = (!branch1->isEmpty)*(branch1->Y)*(branch1->mass) + (!branch2->isEmpty)*(branch2->Y)*(branch2->mass) + (!branch3->isEmpty)*(branch3->Y)*(branch3->mass) + (!branch4->isEmpty)*(branch4->Y)*(branch4->mass);
    Y = Y/mass;
}

void node::rotateTree(node* N) {
    std::cout<<"rotate\n";
    N->printNode();
    if (isComposite || isEmpty) return;
    if(checkIfInDomain(N)) { //N is by default  the this node.
        // 2 cases if N is this nothing needs to be done, if not insert into.
        std::cout<<"check yes\n";
        if(N == this) return;
        else {
            //node* newEmpty;
            node* T = N->parent;
            //loop over all the forefathers to delete their mass too?
            /*while (T!= NULL) {
                T->mass = T->mass + mass;
                T = T->parent;
            }*/
            std::cout<<"insert from rotate\n";
            insertNode(N);
        }
    }
    else {
        //delete from here and carry to forefathers for further examination.
        //this->isEmpty = true;
        //N4 = new node(N->cX + XX/pow(2, dep + 2) , N->cY + YY/pow(2, dep + 2) , dep + 1 );
        bool emptySiblings = (N->parent->branch1->isEmpty && N->parent->branch2->isEmpty && N->parent->branch3->isEmpty);
        emptySiblings = emptySiblings || (N->parent->branch1->isEmpty && N->parent->branch2->isEmpty && N->parent->branch4->isEmpty);
        emptySiblings = emptySiblings || (N->parent->branch1->isEmpty && N->parent->branch4->isEmpty && N->parent->branch3->isEmpty);
        emptySiblings = emptySiblings || (N->parent->branch4->isEmpty && N->parent->branch2->isEmpty && N->parent->branch3->isEmpty);
        std::cout<<"\nEmpty siblings:"<<emptySiblings<<"\n";
        if(N==this && !emptySiblings) {
            std::cout<<" opposite of empty siblings\n";
            //an empty node will replace this
            node* E;
            E = new node(cX, cY, depth);
            if(parent->branch1 == this) parent->branch1 = E;
            if(parent->branch2 == this) parent->branch2 = E;
            if(parent->branch3 == this) parent->branch3 = E;
            if(parent->branch4 == this) parent->branch4 = E;
            E->parent = parent;
            //N->mass = N->mass - mass;
            node* T;
            T = N->parent;
            //loop over all the forefathers to delete their mass too?
            while (T!= NULL) {
                T->mass = T->mass - mass;
                T = T->parent;
            }
            rotateTree(N->parent);
        }
        else if(N == this & emptySiblings){
            std::cout<<"empty siblings\n";
            //if all its siblings are empty we need to collapse the redundant branches
            if(parent->branch1 != this) delete parent->branch1;
            if(parent->branch2 != this) delete parent->branch2;
            if(parent->branch3 != this) delete parent->branch3;
            if(parent->branch4 != this) delete parent->branch4;

            this->cX = parent->cX;
            this->cY = parent->cY;
            this->depth = parent->depth;
            if(parent->parent->branch1 == parent) parent->parent->branch1 = this;
            if(parent->parent->branch2 == parent) parent->parent->branch2 = this;
            if(parent->parent->branch3 == parent) parent->parent->branch3 = this;
            if(parent->parent->branch4 == parent) parent->parent->branch4 = this;

            node* temp;
            temp = parent;
            this->parent = parent->parent;
            delete temp;

            rotateTree(this);
        }
        else {
            std::cout<<"simple";
            rotateTree(N->parent);
        }
    }
}

