#include "Header.h"

static void keyCallback(GLFWwindow *window, int button, int scancode, int action, int mods);

class program

{

private:

public:

	int height;
	int width;
	int rockets;
	int aliveRockets;
	int noDraw;
	int noDrawUser;
	double startTime;
	double force;
	double mutationRate;
	double timestep;
	double allowedTime;
	int generation;
	std::vector <int>indexFit;
	std::vector <double>genFitness;
	std::vector <rocket>r;
	std::vector <barrier>bar;
	std::vector <neuralNetwork>nN;
	std::vector <neuralNetwork>nextGen;

	void init(int height_, double force_, int rockets_, int generation_, double mutationRate_, double timestep_, double allowedTime_)

	{

		noDraw = 1;
		r.clear();
		bar.clear();

		height = height_;
		width = height_;
		force = force_;
		mutationRate = mutationRate_;
		generation = generation_;
		rockets = rockets_;
		startTime = clock();
		timestep = timestep_;
		allowedTime = allowedTime_;
		barrier b;

		b.init(0.7, -0.7, -0.70, -0.80, 0);
		bar.push_back(b);

		b.init(-0.7, -0.9, 0.7, 0.5, 0);
		bar.push_back(b);

		b.init(0.7, -0.5, 0.3, 0.20, 0);
		bar.push_back(b);

		b.init(0.1, -0.1, 1.0, 0.95, 1);
		bar.push_back(b);


		if (generation == 0)

		{
			
			nN.clear();

		}

		else

		{

			nN = nextGen;


		}

		for (int i = 0; i < rockets; i++)

		{

			rocket rocketInit;
			neuralNetwork networkInit;
			rocketInit.init(bar, 0.0, -0.95, 0.0, 0.0, 0.0, 0.0, force);
			r.push_back(rocketInit);

			if (generation == 0)

			{

				networkInit.init(9, 25, 2);
				nN.push_back(networkInit);

			}

		}

		noDraw = 0;

	}

	void killAllRockets()

	{

		for (int i = 0; i < rockets; i++)

		{

			r[i].kill();

		}


	}

	void draw()

	{


		
		if (noDraw == 0 && noDrawUser == 0)

		{

			for (int i = 0; i < rockets; i++)

			{

				r[i].draw();

			}



			for (int i = 0; i < bar.size(); i++)

			{

				bar[i].draw();

			}

		}

	}

	void update()

	{

		//std::cout << nN.hidden[1].getActivation()  << std::endl;
		aliveRockets = 0;

		if (clock() - startTime > allowedTime)

		{

			killAllRockets();

		}

		//std::cout << "\r" << clock() - startTime;

		for (int i = 0; i < rockets; i++)

		{

			nN[i].update(&r[i]);
			r[i].update(timestep);

			if (r[i].getState() == 1)

			{

				aliveRockets++;
				
			}

		}


		if (aliveRockets == 0)

		{
			genFitness.clear();
			for (int i = 0; i < rockets; i++)

			{

				double fit = r[i].fitness();
				nN[i].fitness = fit;
				genFitness.push_back(fit);

			}

			generation++;
			reproduce();
			init(700, 5000, rockets, generation, mutationRate, timestep, allowedTime);

		}


	}

	void biggest()

	{

		int big;
		int index;

		for (int i = 0; i < nN.size(); i++)

		{
			big = 1;

			for (int j = 0; j < nN.size(); j++)

			{

				if (nN[i].fitness < nN[j].fitness)

				{

					big = 0;

				}

			}

			if (big == 1)

			{

				//std::cout << nN[i].fitness << std::endl;
				indexFit.push_back(i);
				index = i;

			}

		}

		nN[index].fitness = 0;


	}

	void reproduce()

	{

		indexFit.clear();
		nextGen.clear();

		for (int i = 0; i < nN.size(); i++)

		{

			biggest();

		}

		for (int i = 0; i < (nN.size()*0.6) / 2; i++)

		{

			neuralNetwork a = nN[indexFit[i]];
			neuralNetwork b = nN[indexFit[i]];

			a.mutate(mutationRate*3.0);
			b.mutate(mutationRate*3.0);

			nextGen.push_back(a);
			nextGen.push_back(b);


		}

		for (int i = 0; i < (nN.size()*0.2) / 2; i++)

		{

			neuralNetwork a = nN[indexFit[i]];
			neuralNetwork b = nN[indexFit[i]];

			a.mutate(mutationRate);
			b.mutate(mutationRate);

			nextGen.push_back(a);
			nextGen.push_back(b);

		}

		for (int i = 0; i < nN.size()*0.2 / 2; i++)

		{

			int num1 = std::rand() % (nN.size() + 1);

			neuralNetwork a = nN[indexFit[num1]];
			neuralNetwork b = nN[indexFit[num1]];

			a.mutate(mutationRate*50);
			b.mutate(mutationRate*50);

			nextGen.push_back(a);
			nextGen.push_back(b);


		}


		std::cout << "Generation " << generation << " highest fitness: " << genFitness[indexFit[0]] << std::endl;

		/*
		for (int i = 0; i < genFitness.size(); i++)

		{

			std::cout << "Generation " << generation << " " << i << " " << genFitness[indexFit[i]] << std::endl;

		}
		*/

		
	}

	void controlCallback(int val)

	{


		switch (val)

		{

			case (1):

			{
				
				if (noDrawUser == 1)

				{

					noDrawUser = 0;

				}

				else

				{

					noDrawUser = 1;

				}

				break;

			}

			case (2):

			{

				timestep *= 0.5;
				allowedTime *= 2.0;
				break;

			}

			case (3):

			{

				timestep *= 2.0;
				allowedTime *= 0.5;
				break;

			}



		}
		

	}


};


int main()

{

	program p;
	int rockets = 1000;
	srand(21);

	if (!glfwInit())
	
	{

		exit(EXIT_FAILURE);

	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow *window = glfwCreateWindow(700, 700, "Smart Rockets", false, false);

	glfwSetWindowUserPointer(window, &p);

	if (!window)

	{

		glfwTerminate();
		exit(EXIT_FAILURE);

	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	p.init(700, 5000, rockets, 0, 0.04, 0.000005, 60000);

	std::thread calcThread

	{

		[&]()

		{

			while (!glfwWindowShouldClose(window))

			{

				p.update();

			}

		}



	};

	calcThread.detach();

	while (!glfwWindowShouldClose(window))

	{

		glfwGetFramebufferSize(window, &p.width, &p.height);
		glViewport(0, 0, p.width, p.height);
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);


		
		p.draw();

		glfwSetKeyCallback(window, keyCallback);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	return 0;

}

void keyCallback(GLFWwindow *window, int button, int scancode, int action, int mods)

{

	void *ptr = glfwGetWindowUserPointer(window);
	program *pptr = static_cast<program*>(ptr);

	if (button == GLFW_KEY_SPACE && action == GLFW_PRESS)

	{

		pptr->controlCallback(1);

	}

	if (button == GLFW_KEY_LEFT && action == GLFW_PRESS)

	{

		pptr->controlCallback(2);

	}

	if (button == GLFW_KEY_RIGHT && action == GLFW_PRESS)

	{

		pptr->controlCallback(3);

	}

}