
const double theta = 0.5;
const double XX = 600; //screen size
const double YY = 600;

class node {
	public:

	bool isComposite, isEmpty; //isComposite === isInternal
	int mass; //total mass
	double X, Y; //coords of c.o.m. WITH ORIGIN AT TOP-LEFT
	double vX, vY;
	double FX, FY;
	double cX, cY; //coords of center of quadrant to find out the spanning domain. Root has (cX,cY) = (XX/2,YY/2).
	int depth; //Root has depth = 0.
	node* branch1;
	node* branch2;
	node* branch3;
	node* branch4; // north-west, north-east, south-west, south-east
	node* parent;

	node();
	node(double CX, double CY, int D);
	//	MODULE FORCE-CALCULATION ETC:

	/*forceBW2 calculates the simple force between two nodes treating them as point particles.*/
	void forceBW2(node* N);
	/*Runs through the whole tree recursively to calculate the force experienced by it. N is the node to be traversed. N = root when calling the function.*/
	void forceCalc(node* N);

	void velocityCalc(double delT);
	void mover(double delT);

	// MODULE TREE-INSERTION:

	/*Checks if the given body lies in the domain covered by the node center*/
	bool checkIfInDomain(node* center);
	/*If a body is passed onto another solid body, a new internal composite node needs to be made and the bodies need to be sent recursively down the hole*/
	void makeComposite(node* N);

	/*Inserts the body into the tree starting from subbranch N. Use this to fill up the tree putting N = root */
	void insertNode(node* N);

    // MODULE MAINTENANCE:
    void updateCOM();
    //to be called over only massive nodes.
    void rotateTree(node* N);

	// MODULE TESTING:
	void printShort();
	void printNode();
	void printTree(int i );
};

