#include "simulation.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <sfml/graphics.hpp>
#include <vector>

//PRELIMINARY iterator
int main() {
    int Number = 3;
    int Steps = 900;
    double DelT = 5;
    int timeParam = 10;
    srand (time(NULL));
    node* root = new node(XX/2,YY/2, 0);
    root->isEmpty = false;
    root->isComposite = true;

    int dep = 0;
    node* N1 = new node(root->cX - XX/pow(2, dep + 2) , root->cY - YY/pow(2, dep + 2) , dep + 1 );
	node* N2 = new node(root->cX + XX/pow(2, dep + 2) , root->cY - YY/pow(2, dep + 2) , dep + 1 );
	node* N3 = new node(root->cX - XX/pow(2, dep + 2) , root->cY + YY/pow(2, dep + 2) , dep + 1 );
	node* N4 = new node(root->cX + XX/pow(2, dep + 2) , root->cY + YY/pow(2, dep + 2) , dep + 1 );

	root->branch1 = N1;
	root->branch2 = N2;
	root->branch3 = N3;
	root->branch4 = N4;

    N1->parent = root;
    N2->parent = root;
    N3->parent = root;
    N4->parent = root;

    node* N;
    std::vector <node *> total;
    int i,j;
    for(i=0; i<Number; i++) {
        //total.push_back(5);
        N = new node();
        N->X = 1 + rand() % (int)XX;
        N->Y = 1 + rand() % (int)YY;
        N->vX = (-1 + (rand() % 100)/50)/40;
        N->vY = (-1 + (rand() % 100)/50)/40;
        N->mass = 2 + rand() % 5;
        N->insertNode(root);
        N->printNode();
        total.push_back(N);
    }

    //total[0]->vX = 0.5;

    std::cout<<"starting";
    sf::RenderWindow win(sf::VideoMode(XX, YY), "SFML Test");
    std::vector <sf::CircleShape> shapes;
    for(i=0; i<Number; i++) {
        double radius = 15/total[i]->mass;
        sf::CircleShape shape;
        shape.setRadius(radius);
        shape.setPosition(total[i]->X, total[i]->Y);
        shape.setFillColor(sf::Color::Green);
        shapes.push_back(shape);
    }
    std::cout<<"outside loop"<<shapes.size();
    //return 1;
    i = 0;
    sf::Time time = sf::milliseconds(timeParam);
    while (win.isOpen() && i<Steps)
    {
        std::cout<<"inside loop"<<i;
        i++;
        sf::Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                win.close();
            }
        }
        root->updateCOM();
        for(j=0; j<Number; j++) {
            total[j]->forceCalc(root);
        }
        for(j=0; j<Number; j++) {
            total[j]->velocityCalc(DelT);
        }
        for(j=0; j<Number; j++) {
            total[j]->mover(DelT);
        }
        //show graphically
        win.clear();
        for(j=0; j<Number; j++) {
            win.draw(shapes[j]);
        }
        win.display();
        for(j=0; j<Number; j++) {
            shapes[j].setPosition(total[j]->X, total[j]->Y);
        }
        for(j=0; j<Number; j++) {
            total[j]->rotateTree(total[j]);
        }
        sf::sleep(time);
    }
    return 1;
}

