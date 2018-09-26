#include <iostream>
#include <ctime>
#include <GLFW/glfw3.h>
#include <vector>
#include <thread>
#define PI 3.1459

class barrier

{

private:



public:

	double xMax;
	double xMin;
	double yMax;
	double yMin;
	int type;


	void init(double xMax_, double xMin_, double yMax_, double yMin_, int type_)

	{

		xMax = xMax_;
		yMax = yMax_;
		xMin = xMin_;
		yMin = yMin_;
		type = type_;

	}

	void draw()

	{

		if (type == 0)

		{

			glColor3d(0.5, 0.5, 0.5);

		}

		else

		{

			glColor3d(0.5, 0.1, 0.1);

		}

		glBegin(GL_QUADS);
		glVertex2d(xMax, yMax);
		glVertex2d(xMax, yMin);
		glVertex2d(xMin, yMin);
		glVertex2d(xMin, yMax);
		glEnd();


	}

};



class rocket

{

private:

	double dir;
	double size;
	int alive;
	std::vector <barrier>col;

public:

	double timestep;
	double xAcc;
	double yAcc;
	double xVel;
	double yVel;
	double x;
	double y;
	double colLeft;
	double colFront;
	double colRight;
	double force;
	double fitnessVal;
	double aliveTime;
	double startTime;
	double distanceTravelled;
	int killedByTime;
	double objective;
	double distanceFromObjective;
	double endTime;

	void init(std::vector <barrier>col_, double x_, double y_, double xVel_, double yVel_, double xAcc_, double yAcc_, double force_)

	{

		alive = 1;
		col = col_;
		colFront = 0.475;
		colLeft = 0.475;
		colRight = 0.475;
		fitnessVal = 1;
		killedByTime = 0;
		x = x_;
		y = y_;
		xVel = xVel_;
		yVel = yVel_;
		xAcc = xAcc_;
		yAcc = yAcc_;
		//timestep = 0.0001;
		timestep = 0.000005;
		distanceTravelled = 0;
		distanceFromObjective = 10;
		size = 0.05;
		force = force_;
		startTime = clock();

	}

	int getState()

	{

		return alive;

	}

	void kill()

	{

		if (alive == 1)

		{

			killedByTime = 1;

		}

		alive = 0;

	}

	double fitness()

	{
		double vel = (xVel*xVel) + (yVel*yVel);
		aliveTime = (endTime - startTime)*timestep;
		//return (1.01 - killedByTime)*( ((1.0 - killedByTime + objective) / (vel * 10000 + 1))*(1.0 + objective + killedByTime) / (distanceFromObjective*1 + 1.0) + 0.1*aliveTime + (1.0 - killedByTime + objective) / (vel*10000 + 1) + (10.0 + objective)* distanceTravelled + (1.0 + objective) * killedByTime); // + ((1.0 + objective)/(0.5*aliveTime))
		//return ((1.0 + objective) / (distanceFromObjective*0.01 + 1.0) + (1.0 - killedByTime + objective) / (vel + 1) + 0.5*aliveTime + (3.0 + objective) * killedByTime);
		return (objective + 2.0 + killedByTime) / (distanceFromObjective*0.01 + 1.0);

	}

	void draw()

	{

		glColor3d(1.0, 1.0, 1.0);
		glLineWidth(4);
		glBegin(GL_LINES);

		glVertex2d(- size*cos(dir) + x, - size*sin(dir) + y);
		glVertex2d(size*cos(dir)+ x, size*sin(dir) + y);

		glEnd();

		//glBegin(GL_POINTS);
		//glVertex2d(20 * size*cos(dir) / 2.0 + x, 20 * size*sin(dir) / 2.0 + y);
		//glVertex2d(20 * size*cos(dir - PI / 3.0) / 2.0 + x, 20 * size*sin(dir - PI / 3.0) / 2.0 + y);
		//glVertex2d(20 * size*cos(dir + PI / 3.0) / 2.0 + x, 20 * size*sin(dir + PI / 3.0) / 2.0 + y);

		//glEnd();



	}

	void update(double timestep_)

	{

		timestep = timestep_;

		if (alive == 0 || alive == 2)

		{

			return;

		}

		if (xVel == 0)

		{

			dir = 0.0;

		}

		double distanceFromObjectiveTemp = ((1 - y)*(1 - y));

		if (distanceFromObjectiveTemp < distanceFromObjective)

		{

			distanceFromObjective = distanceFromObjectiveTemp;

		}

		alive = collision(x, y);


		dir = atan(yVel / xVel);

		

		x += xVel*timestep;
		y += yVel*timestep;

		distanceTravelled += (xVel * timestep)*(xVel * timestep) + (yVel * timestep)*(yVel * timestep);

		xVel += xAcc*timestep*force;
		yVel += yAcc*timestep*force;

		for (int i = 0; i < 20; i++)

		{

			if (collision(i*size*cos(dir) / 2.0 + x, i*size*sin(dir) / 2.0 + y) == 0)

			{

				colFront = i * size / 2.0;
				

			}

			if (collision(i * size*cos(dir - PI / 3.0) / 2.0 + x, i * size*sin(dir - PI / 3.0) / 2.0 + y) == 0)

			{

				colLeft = i * size / 2.0;

			}

			if (collision(i * size*cos(dir + PI/3.0) / 2.0 + x, i * size*sin(dir + PI / 3.0) / 2.0 + y) == 0)

			{

				colRight = i * size / 2.0;

			}

		}

		//std::cout << "\r Left : " << colLeft << " Right: " << colRight << " Front: " << colFront;


	}

	int collision(double x_, double y_)

	{

		int barrierNumber = col.size();

		if (x_ > 1.0)

		{

			return 0;

		}

		if (x_ < -1.0)

		{

			return 0;

		}

		if (y_ > 1.0)

		{

			return 0;

		}

		if (y_ < -1.0)

		{

			return 0;

		}
		
		for (int i = 0; i < barrierNumber; i++)

		{

			if (x_ < col[i].xMax && x_ > col[i].xMin && y_ < col[i].yMax && y_ > col[i].yMin)

			{

				if (col[i].type == 1)

				{

					endTime = clock();
					objective = 100;
					return 2;

				}

				else

				{

					endTime = clock();
					objective = 0;
					return 0;

				}

			}

		}

		return 1;

	}

};

class neuron

{

private:
	double b;
	double activation;

	double sig(double x)

	{

		return (x / (1 + abs(x)));

	}

public:

	std::vector <double>weights;

	void init(int nodes)

	{

		activation = 0.0;

		
		int num1;
		double num2;
		b = ranNum();
		for (int i = 0; i < nodes; i++)

		{

			num1 = std::rand() % (20000 + 1) - 10000;
			num2 = (double)num1 / 10000;
			weights.push_back(num2);

		}

	}

	void printWeight()

	{

		std::cout << "Weights: ";

		for (int i = 0; i < weights.size(); i++)

		{

			std::cout << " " << weights[i];

		}

		std::cout << " Bias: " << b << std::endl;

	}

	void mutate(double multi)

	{

		int num1;
		double num2;
		b += multi * ranNum();
		for (int i = 0; i < weights.size(); i++)

		{

			num1 = std::rand() % (20000 + 1) - 10000;
			num2 = multi * (double)num1 / 10000;
			//std::cout << num2 << std::endl;
			weights[i] += num2;

		}


	}



	void crossOver(neuron n1, neuron n2)

	{

		for (int i = 0; i < n1.weights.size(); i++)

		{

			weights.push_back((n1.weights[i] + n2.weights[i]) / 2.0);

		}


	}


	double getActivation()

	{

		return activation;

	}

	void process(std::vector <double>inputValues)

	{

		double outputVal = 0.0;

		if (inputValues.size() != weights.size())

		{
			
			std::cout << "\rError Vector Subscript out of Range" << inputValues.size() << " " << weights.size();
			return;

		}


		for (int i = 0; i < inputValues.size(); i++)

		{

			outputVal += inputValues[i] * weights[i];

		}

		activation = sig(outputVal + (-1 * b));
		//std::cout << activation << std::endl;

	}

	void randomVector(std::vector <double>*ptr, int n)

	{

		int num1;
		double num2;
		ptr->clear();

		for (int i = 0; i < n; i++)

		{

			num1 = std::rand() % (20000 + 1) - 10000;
			num2 = (double)num1 / 10000;
			ptr->push_back(num2);

		}

	}

	double ranNum()

	{

		
		int num1;
		double num2;

		num1 = std::rand() % (20000 + 1) - 10000;
		num2 = (double)num1 / 10000;

		return num2;

	}



};

class neuralNetwork

{


private:

	int inputNodes;
	int hiddenNodes;
	int outputNodes;

public:

	std::vector <neuron>hidden;
	std::vector <neuron>output;
	double fitness;

	void init(int inputNodes_, int hiddenNodes_, int outputNodes_)

	{

		inputNodes = inputNodes_;
		hiddenNodes = hiddenNodes_;
		outputNodes = outputNodes_;
		hidden.clear();
		output.clear();

		initWeights();

	}

	void initWeights()

	{

		for (int i = 0; i < hiddenNodes; i++)

		{

			neuron n;
			n.init(inputNodes);
			hidden.push_back(n);
			//std::cout << "Input " << i << " ";
			//n.printWeight();

		}

		for (int i = 0; i < outputNodes; i++)

		{

			neuron n;
			n.init(hiddenNodes);
			output.push_back(n); 
			//std::cout << "Hidden " << i << " ";
			//n.printWeight();

		}


	}

	void update(rocket *rptr)

	{

		//hidden[0].print();

		std::vector <double>inputs = {rptr->x, rptr->y, rptr->xVel, rptr->yVel, rptr->colFront, rptr->colLeft, rptr->colRight, 0.0, 1.0 };
		//std::vector <double>inputs = {rptr->colFront, rptr->colLeft, rptr->colRight, rptr->xVel, rptr->yVel };

		std::vector <double>secInput;
		secInput.clear();

		for (int i = 0; i < hiddenNodes; i++)

		{

			hidden[i].process(inputs);
			secInput.push_back(hidden[i].getActivation());

		}

		//std::cout << secInput[0] << std::endl;

		for (int i = 0; i < outputNodes; i++)

		{

			output[i].process(secInput);

		}

		rptr->xAcc = output[0].getActivation();
		rptr->yAcc = output[1].getActivation();


	}

	void mutate(double multi)

	{

		for (int i = 0; i < hiddenNodes; i++)

		{

			hidden[i].mutate(multi);

		}

		for (int i = 0; i < outputNodes; i++)

		{

			output[i].mutate(multi);

		}

	}

	void crossOver(neuralNetwork n1, neuralNetwork n2, double multi)

	{

		for (int i = 0; i < n1.outputNodes; i++)

		{
			
			hidden[i].crossOver(n1.hidden[i], n2.hidden[i]);
			hidden[i].mutate(multi);

		}

		for (int i = 0; i < n1.outputNodes; i++)


		{

			output[i].crossOver(n1.output[i], n2.output[i]);
			output[i].mutate(multi);

		}



	}


};
