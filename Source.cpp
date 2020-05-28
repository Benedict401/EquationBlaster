#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// APPLICATION STARTS HERE

// Plan
//
// We want Bombs these will have a vecModel and also bounding boxes for the purpopses of collision detection
// What we want know is if a non-regular convex polygon will work with our collision detection
// Also we want to drop a bomb! - Implement the display as a space object for now

class EquationBlaster : public olc::PixelGameEngine
{
public:
	EquationBlaster()
	{
		sAppName = "EquationBlaster";
	}

	//int bbLevel = 1;

private:
	olc::Sprite* sprDemo = nullptr;

	struct Variable
	{
		std::string Name;
		int Power;
	};

	struct Term
	{
		std::vector<Variable> vecVariable;
	};

	struct SideOfEquation
	{
		std::vector<Term> vecTerm;
		std::string SideOfEquationStr;
	};

	struct Equation
	{
		SideOfEquation LHS;
		SideOfEquation RHS;
	};

	struct Logarithm
	{
		long Base;
		long Log;
		long AntiLog;
		boolean Achieved;
	};

	struct sSpaceObject
	{
		float x;
		float y;
		float dx;
		float dy;
		int nSize;
		float angle;
		// For now we will put it in here, not an awful thing to do
		std::string OpStr;
	};

	//A little bit of hacking here

	struct vec2d
	{
		float x;
		float y;
	};

	struct polygon
	{
		std::vector<vec2d> p;	// Transformed Points
		vec2d pos;				// Position of shape
		float angle;			// Direction of shape
		std::vector<vec2d> o;	// "Model" of shape							
		bool overlap = false;	// Flag to indicate if overlap has occurred
	};


	std::string ScreenOutputFlag1 = "Not there yet";
	std::string ScreenOutputFlag2 = "Not there yet";
	std::string ScreenOutputFlag3 = "Not there yet";
	std::string ScreenOutputFlag4 = "Not there yet";
	std::string ScreenOutputFlag5 = "Not there yet";
	std::string ScreenOutputFlag6 = "Not there yet";

	bool DoingLogarithms = false;

	Equation bbEquation;
	Equation bbEquationSolution;
	Logarithm TriangleLogarithm;
	int bbAsteroidLevel = 1;
	int bbEquationLevel = 1;
	int bbLogarithmLevel = 1;
	std::vector<std::string> vecAppropriateVariables;
	std::vector<Logarithm> vecAppropriateLogarithms; //This probably wants to be called something else
	std::vector<double> vecLogs;
	std::string sLevelInstruction = "";
	std::string sCongratulations = "";
	bool LevelComplete = false;
	bool BetweenEquationLevels = false;	// Question of wether we want separate variables for different level types.
	std::string bbTestStr = "123456789112345678921234567893123456789412345678951234567896";
	int bbStrLen = 0;
	float bbRnd = 0;
	std::string bbOpStr = "";
	int bbi = 0;
	int AvailableBoosts = 0;
	int AvailableSpaceAnchors = 0;

	std::vector<sSpaceObject> vecAsteroids;
	std::vector<sSpaceObject> vecBullets;
	std::vector<sSpaceObject> vecOperations;
	std::vector<sSpaceObject> vecSpaceAnchors;
	std::vector<sSpaceObject> vecTicks;
	std::vector<sSpaceObject> vecBombs;

	sSpaceObject player;
	int nScore = 0;
	bool bDead = false;
	float fScale = 3;
	float fScaleLogarithmTriangle = 10;
	float AsteroidSpeed = 20;
	int nScale = 3;
	float fOperationStrOffsetx = 10;	//We can do this precisely now we know the dimensions of strs
	float fOperationStrOffsety = 5;
	float Pi = 3.14159265359;
	std::string PresentOperation = "";
	std::string PresentOperationVariable = "";
	float fAccruedTime;

	std::vector<std::pair<float, float>> vecModelShip;
	std::vector<std::pair<float, float>> vecModelAsteroid;
	std::vector<std::pair<float, float>> vecModelSpaceAnchor;
	std::vector<std::pair<float, float>> vecModelTick;
	std::vector<std::pair<float, float>> vecModelLogarithmTriangle;
	std::vector<std::pair<float, float>> vecModelBomb;
	std::vector<std::pair<float, float>> vecBoundingBoxBomb;

	polygon BoundingBoxBomb;
	polygon Ship;

public:
	bool OnUserCreate() override
	{
		//sprDemo = new olc::Sprite("APicture.png");

		vecAsteroids.push_back({ 20.0f, 20.0f, 8.0f, -6.0f, (int)16, 0.0f });

		//Initialise Player Position
		player.x = ScreenWidth() / 2.0f;
		player.y = ScreenHeight() / 2.0f;
		player.dx = 0.0f;
		player.dy = 0.0f;
		player.angle = 0.0f;

		vecModelShip =
		{
			{fScale * 0.0f, fScale * -5.0f},
			{fScale * -2.5f, fScale * +2.5f},
			{fScale * +2.5f, fScale * +2.5f}
			//{0.0f, -15.0f},
			//{-7.5f, +7.5f},
			//{+7.5f, +7.5f}

		}; // A simple Isoceles Triangle 

		vec2d Initialiser = { 10,10 };
		Ship.p.push_back(Initialiser);

		// Construct Asteroid
		int verts = 20;
		for (int i = 0; i < verts; i++)
		{
			//float radius = 1.0f;
			//float radius = (float)rand() / (float)RAND_MAX * 0.4f + 0.8f;
			float radius = (float)rand() / (float)RAND_MAX * fScale * 0.4f + fScale * 0.8f;
			float a = ((float)i / (float)verts) * 6.28318f;
			vecModelAsteroid.push_back(std::make_pair(radius * sinf(a), radius * cosf(a)));
		}


		vecModelSpaceAnchor =
		{
			{ -0.5f,  2.0f},
			{ -0.75f, 1.75f},
			{ -0.5f,  1.5f},
			{ -0.5f, 1.0f },
			{ -0.75, 1.0f},
			{ -0.75, 0.5f},
			{ -0.5f, 0.5f},
			{ -0.5f, -2.0f},
			{ -0.75f, -1.75f},
			{ -1.0f, -1.0f},
			{ -1.25f, -1.25f},
			{ -1.0f, -2.25f},
			{ -0.5f, -2.5f},
			{  0.5f, -2.5f},
			{  1.0f, -2.25f},
			{ 1.25f, -1.25f},
			{ 1.0f, -1.0f},
			{ 0.75f, -1.75f},
			{ 0.5f, -2.0f},
			{ 0.5f, 0.5f},
			{ 0.75f, 0.5f },
			{ 0.75f, 1.0f},
			{ 0.5f, 1.0f},
			{ 0.5f, 1.5f},
			{ 0.75f, 1.75f},
			{ 0.5f, 2.0f}
		};

		vecModelTick =
		{
			{2,-2},
			{(2 + 1/sqrt(2)), -(1 + 1/sqrt(2))},
			{-1, 2},
			{-(2 + 1/sqrt(2)), 1/sqrt(2)},
			{-2,0},
			{-1,1}
		};

		vecModelBomb =
		{
			{1.0f,-2.0f},
			{1.0f,-1.8f},
			{0.8f,-1.8f},
			{0.8f,-1.6f},
			{1.0f,-1.6f},
			{1.0f, 1.6f},
			{(float) 2/3,1.8f},
			{0.25f,2.0f},
			{-0.25f,2.f}, 
			{(float) -2/3,1.8f},
			{-1.0f,1.6f},
			{-1.0f,-1.6f},
			{-0.8f,-1.6f},
			{-0.8f,-1.8f},
			{-1.0f,-1.8f},
			{-1.0f,-2.0f}
		};

		vecBoundingBoxBomb =
		{
			{1,-2},
			{1, 2},
			{-1, 2},
			{-1, -2}
		};

		vecModelLogarithmTriangle =
		{
			{fScaleLogarithmTriangle * 0.0f, fScaleLogarithmTriangle * -sqrt(3)/2},
			{fScaleLogarithmTriangle * -1.0f, fScaleLogarithmTriangle * sqrt(3)/2},
			{fScaleLogarithmTriangle * 1.0f, fScaleLogarithmTriangle * sqrt(3)/2}
		};

		ResetGame();

		return true;

	}


	bool IsPointInsideCircle(float cx, float cy, float radius, float x, float y)
	{
		return sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) < fScale * radius;
	}

	void LoadAsteroidLevel(int CurrentAsteroidLevel)
	{

		//Initialise Player Position
		player.x = ScreenWidth() / 2.0f;
		player.y = ScreenHeight() / 2.0f;
		player.dx = 0.0f;
		player.dy = 0.0f;
		player.angle = 0.0f;

		//Clear bullets - maybe also clear operations - tomorrow implement asteroid levels and Equation Level
		vecBullets.clear();

		//bbEquation.LHS.vecTerm.push_back(Term1);
		//bbEquation.LHS.vecTerm.clear();

		//int bbInt = bbEquation.LHS.vecTerm.capacity();

		switch (CurrentAsteroidLevel)
		{
		case 1:
			AsteroidSpeed = 20;
			vecAsteroids.push_back({ 220.0f, 220.0f, 8.0f, -6.0f, (int)16, 0.0f });
			vecAsteroids.push_back({ 900.0f, 300.0f, -5.0f, 3.0f, (int)16, 0.0f });
			break;
		case 2:
			AsteroidSpeed = 30;
			vecAsteroids.push_back({ 150.0f, 600.0f, 8.0f, -6.0f, (int)16, 0.0f });
			vecAsteroids.push_back({ 260.0f, 120.0f, -5.0f, 3.0f, (int)16, 0.0f });
			vecAsteroids.push_back({ 500.0f, 220.0f, -7.0f, 7.0f, (int)16, 0.0f });
			break;
		case 3:
			AsteroidSpeed = 40;
			vecAsteroids.push_back({ 150.0f, 600.0f, 8.0f, -6.0f, (int)16, 0.0f });
			vecAsteroids.push_back({ 260.0f, 120.0f, -5.0f, 3.0f, (int)16, 0.0f });
			vecAsteroids.push_back({ 500.0f, 220.0f, -7.0f, 7.0f, (int)16, 0.0f });
			break;
		default:
			AsteroidSpeed = 10;
			vecAsteroids.push_back({ 220.0f, 220.0f, 8.0f, -6.0f, (int)16, 0.0f });
			vecAsteroids.push_back({ 900.0f, 300.0f, -5.0f, 3.0f, (int)16, 0.0f });
			;
		}
	}

	void LoadEquationLevel(int CurrentEquationLevel)
	{

		//Initialise
		vecOperations.clear();
		vecTicks.clear();
		LevelComplete = false;
		BetweenEquationLevels = false;

		//So here we load in starting state of Equation and also EquationSolution
		bbEquation.LHS.vecTerm.clear();
		bbEquation.RHS.vecTerm.clear();

		bbEquationSolution.LHS.vecTerm.clear();
		bbEquationSolution.RHS.vecTerm.clear();

		vecAppropriateVariables.clear();

		Term Term1;	//We shall look at the behaviour of this re scope seems to generate a new instance
					//when pushed back onto a vector which is gratifying
		//Term Term2;
		//Term Term3;

		switch (CurrentEquationLevel)
		{
		case 1:
			sLevelInstruction = "Find m";
			sCongratulations = "m Found!";
			//bbEquation
			Term1.vecVariable.push_back({ "D", 1 });
			bbEquation.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.push_back({ "m", 1 });
			Term1.vecVariable.push_back({ "V", -1 });
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			//bbEquationSolution
			Term1.vecVariable.push_back({ "D", 1 });
			Term1.vecVariable.push_back({ "V", 1 });
			bbEquationSolution.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.push_back({ "m", 1 });
			bbEquationSolution.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			//AppropriateVariables
			//vecAppropriateVariables.push_back({ "D"});	// Make it easier to start with
			//vecAppropriateVariables.push_back({ "m"});
			vecAppropriateVariables.push_back({ "V"});
			break;
		case 2:
			sLevelInstruction = "Find F";
			sCongratulations = "F Found!";
			Term1.vecVariable.push_back({ "P", 1 });
			bbEquation.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.push_back({ "F", 1 });
			Term1.vecVariable.push_back({ "A", -1 });
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			//bbEquationSolution
			Term1.vecVariable.push_back({ "P", 1 });
			Term1.vecVariable.push_back({ "A", 1 });
			bbEquationSolution.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.push_back({ "F", 1 });
			bbEquationSolution.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			//AppropriateVariables
			vecAppropriateVariables.push_back({ "P"});
			//vecAppropriateVariables.push_back({ "F"});
			vecAppropriateVariables.push_back({ "A",});
			break;
		case 3:
			sLevelInstruction = "Find d";
			sCongratulations = "d Found!";
			Term1.vecVariable.push_back({ "s", 1 });
			bbEquation.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.push_back({ "d", 1 });
			Term1.vecVariable.push_back({ "t", -1 });
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			//bbEquationSolution
			Term1.vecVariable.push_back({ "s", 1 });
			Term1.vecVariable.push_back({ "t", 1 });
			bbEquationSolution.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.push_back({ "d", 1 });
			bbEquationSolution.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			//AppropriateVariables
			vecAppropriateVariables.push_back({ "s"});
			vecAppropriateVariables.push_back({ "t"});
			vecAppropriateVariables.push_back({ "d"});
			break;
		default:
			bbEquationLevel = 1;
			sLevelInstruction = "Find m";
			sCongratulations = "m Found!";
			//We need a suitable way to end the game
			Term1.vecVariable.push_back({ "D", 1 });
			bbEquation.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.push_back({ "m", 1 });
			Term1.vecVariable.push_back({ "V", -1 });
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			//bbEquationSolution
			Term1.vecVariable.push_back({ "D", 1 });
			Term1.vecVariable.push_back({ "V", 1 });
			bbEquationSolution.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.push_back({ "m", 1 });
			bbEquationSolution.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			//AppropriateVariables
			vecAppropriateVariables.push_back({ "D"});
			vecAppropriateVariables.push_back({ "m"});
			vecAppropriateVariables.push_back({ "V"});
			;
		}
	}

	void LoadLogarithmLevel(int CurrentEquationLevel)
	{

		//Initialise
		vecOperations.clear();
		vecTicks.clear();
		LevelComplete = false;
		BetweenEquationLevels = false;

		vecAppropriateLogarithms.clear();
		TriangleLogarithm.Base = NULL;
		TriangleLogarithm.Log = NULL;
		TriangleLogarithm.AntiLog = NULL;


		switch (CurrentEquationLevel)
		{
		case 1:
			GenLogs(2, 1, 5);
			for (int a : vecLogs)
			{
				vecAppropriateLogarithms.push_back({ 2, a, (long)pow(2, a), false });
			}
			break;
		case 2:
			GenLogs(3, 1, 5);
			for (int a : vecLogs)
			{
				vecAppropriateLogarithms.push_back({ 3, a, (long)pow(3, a), false });
			}
			break;
		case 3:
			GenLogs(2, 1, 2);
			for (int a : vecLogs)
			{
				vecAppropriateLogarithms.push_back({ 2, a, (long)pow(2, a), false });
			}
			GenLogs(3, 1, 3);
			for (int a : vecLogs)
			{
				vecAppropriateLogarithms.push_back({ 3, a, (long)pow(3, a) , false});
			}
			break;
		case 4:
			GenLogs(2, 1, 1);
			for (int a : vecLogs)
			{
				vecAppropriateLogarithms.push_back({ 2, a, (long)pow(2, a) , false});
			}
			GenLogs(5, 1, 2);
			for (int a : vecLogs)
			{
				vecAppropriateLogarithms.push_back({ 5, a, (long)pow(5, a) , false});
			}
			GenLogs(3, 1, 1);
			for (int a : vecLogs)
			{
				vecAppropriateLogarithms.push_back({ 3, a, (long)pow(3, a) , false});
			}
			GenLogs(5, 1, 1);
			for (int a : vecLogs)
			{
				vecAppropriateLogarithms.push_back({ 5, a, (long)pow(5, a) , false});
			}
			break;
		default:
			GenLogs(2, 1, 5);
			for (int a : vecLogs)
			{
				vecAppropriateLogarithms.push_back({ 2, a, (long)pow(2, a), false});
			}
		}
	}

	void GenLogs(int base, long Floor, int NumLogs)
	{
		long bbNumber;
		int bbCounter;
		bool AllPositive = false;

		while (!AllPositive)
		{
			bbNumber = Floor;
			bbCounter = 0;
			vecLogs.clear();

			while (bbCounter < NumLogs)
			{
				bbRnd = (float)rand() / (float)RAND_MAX;
				if (bbRnd < 0.3)
				{
					vecLogs.push_back(bbNumber);
					bbCounter++;
				}
				bbNumber++;
			}
			if ((long) pow(base, bbNumber) > 0)
				AllPositive = true;
		}
	}

	std::string GenerateOpStr()
	{
		//In the first instance we are only going to generate xVariable and %Variable

		float MaxRnd = 0;
		std::string OpStr = "";

		if (!DoingLogarithms)
		{
			// Generate a random number for each appropriate variable
			for (std::string bbVar : vecAppropriateVariables)
			{
				bbRnd = (float)rand() / (float)RAND_MAX;
				if (bbRnd > MaxRnd)
				{
					MaxRnd = bbRnd;
					OpStr = bbVar;
				}
			}

			// Chose variable with highest random number - then choose multiply or divide


			bbRnd = (float)rand() / (float)RAND_MAX;
			if (bbRnd < 0.5)
				return "x" + OpStr;
			else
				return "%" + OpStr;
		}
		else
		{
			bbRnd = (float)rand() / (float)RAND_MAX;
			if (bbRnd < 0.5)
			{
				//Now arguably we are only going to use bases of 2,3 and 5
				MaxRnd = 0;
				int bbBase;
				for (int i = 1; i < 4; i++)
				{
					bbRnd = (float)rand() / (float)RAND_MAX;
					if (bbRnd > MaxRnd)
					{
						MaxRnd = bbRnd;
						bbBase = i;
					}
				}
				switch (bbBase)
				{
				case 1:
					return "b2";
					break;
				case 2:
					return "b3";
					break;
				case 3:
					return "b5";
					break;
				default:
					return "b2";
				}
			}
			else
			{
				MaxRnd = 0;
				//Generate a random number for each appropriate log
				for (Logarithm bbLog : vecAppropriateLogarithms)
				{
					bbRnd = (float)rand() / (float)RAND_MAX;
					if (bbRnd > MaxRnd)
					{
						MaxRnd = bbRnd;
						OpStr = std::to_string(bbLog.Log);
					}
				}
				return "L" + OpStr;
			}
		}

	}

	void PerformOperationControl(std::string opStr)
	{
		PresentOperation = opStr.substr(0, 1);
		PresentOperationVariable = opStr.substr(1, opStr.length());

		if (PresentOperation == "x")
			//ScreenOutputFlag1 = "Making Progress";
			OperationMultiplication(PresentOperationVariable);

		if (PresentOperation == "%")
			OperationDivision(PresentOperationVariable);

		if (PresentOperation == "+")
			OperationAddition(PresentOperationVariable);

		if (PresentOperation == "-")
			OperationSubtraction(PresentOperationVariable);

		if (PresentOperation == "b")
			OperationPlaceBase(PresentOperationVariable);

		if (PresentOperation == "L")
			OperationPlaceLog(PresentOperationVariable);


		CheckIfSolutionAchieved(); //Also handles situation if solution achieved

	}

	void OperationMultiplication(std::string varStr)
	{

		int bbCounter = 0;

		bool VariableFound = false;
		//Go through all terms in bbEquation, if variable names match increase power by 1
		//We should make a vector of equation sides but that is tomorrow's business
		for (Term &bbTerm : bbEquation.LHS.vecTerm)
		{
			for (Variable &bbVar : bbTerm.vecVariable)
			{
				if (bbVar.Name == varStr)
				{
					bbVar.Power++;
					VariableFound = true;
					if (bbVar.Power == 0)
					{
						bbTerm.vecVariable.erase(bbTerm.vecVariable.begin() + bbCounter);
						//bbTerm.vecVariable.erase(1);
					}
				}
				bbCounter++;
			}
			if (VariableFound == false) 
			{
				ScreenOutputFlag1 = "In here now";
				bbTerm.vecVariable.push_back({ varStr, 1 });
			}
			VariableFound = false;
			bbCounter = 0;
		}


		for (Term &bbTerm : bbEquation.RHS.vecTerm)
		{
			for (Variable &bbVar : bbTerm.vecVariable)
			{
				if (bbVar.Name == varStr)
				{
					bbVar.Power++;
					//bbVar.Power = bbVar.Power + 1;
					//ScreenOutputFlag1 = std::to_string(bbVar.Power);
					VariableFound = true;
					if (bbVar.Power == 0)
					{
						bbTerm.vecVariable.erase(bbTerm.vecVariable.begin() + bbCounter);
						//bbTerm.vecVariable.erase(1);
					}
				}
				bbCounter++;
			}
			if (VariableFound == false)
			{
				bbTerm.vecVariable.push_back({ varStr, 1 });
			}
			VariableFound = false;
			bbCounter = 0;
		}
	}

	void OperationDivision(std::string varStr)
	{

		int bbCounter = 0;
		bool VariableFound = false;
		//Go through all terms in bbEquation, if variable names match increase power by 1
		//We should make a vector of equation sides but that is tomorrow's business
		for (Term& bbTerm : bbEquation.LHS.vecTerm)
		{
			for (Variable& bbVar : bbTerm.vecVariable)
			{
				if (bbVar.Name == varStr)
				{
					bbVar.Power--;
					VariableFound = true;
					if (bbVar.Power == 0)
					{
						bbTerm.vecVariable.erase(bbTerm.vecVariable.begin() + bbCounter);
						//bbTerm.vecVariable.erase(1);
					}
				}
				bbCounter++;
			}
			if (VariableFound == false)
			{
				ScreenOutputFlag1 = "In here now";
				bbTerm.vecVariable.push_back({ varStr, -1 });
			}
			VariableFound = false;
			bbCounter = 0;
		}


		for (Term& bbTerm : bbEquation.RHS.vecTerm)
		{
			for (Variable& bbVar : bbTerm.vecVariable)
			{
				if (bbVar.Name == varStr)
				{
					//bbVar.Power++;
					bbVar.Power--;
					//ScreenOutputFlag1 = std::to_string(bbVar.Power);
					VariableFound = true;
					if (bbVar.Power == 0)
					{
						bbTerm.vecVariable.erase(bbTerm.vecVariable.begin() + bbCounter);
						//bbTerm.vecVariable.erase(1);
					}
				}
				bbCounter++;
			}
			if (VariableFound == false)
			{
				bbTerm.vecVariable.push_back({ varStr, -1 });
			}
			VariableFound = false;
			bbCounter = 0;
		}
	}

	void OperationAddition(std::string varStr)
	{

	}

	void OperationSubtraction(std::string varStr)
	{

	}

	void OperationPlaceBase(std::string bbBase)
	{
		//float bbBase = (float) bbBase;
		float fBase = std::stof(bbBase);
		int iBase = std::stoi(bbBase);
		long lBase = std::stol(bbBase);

		TriangleLogarithm.Base = lBase;
	}

	void OperationPlaceLog(std::string bbLog)
	{
		long lLog = std::stol(bbLog);

		TriangleLogarithm.Log = lLog; 
	}

	void OperationPlaceAntiLog(float bbAntilog)
	{

	}

	void ClearTriangleLogarithm()
	{
		TriangleLogarithm.Base = NULL;
		TriangleLogarithm.Log = NULL;
		TriangleLogarithm.AntiLog = NULL;
	}

	void CheckIfSolutionAchieved()
	{
		//Here we might want a EquationSolution to go with Equation data type.
		//We want to call this after every operation is performed
		//Increments bbEquationLevel if this function returns true

		// Check LHS against LHS and RHS against RHS then swap and check
		// LHS against RHS and RHS against LHS

		// This is a bit spaghetti code but there are justifications 
		if (!DoingLogarithms)
		{
			bool SolutionAchieved = false;

			if ((SidesEquivalent(bbEquation.LHS, bbEquationSolution.LHS) and SidesEquivalent(bbEquation.RHS, bbEquationSolution.RHS))
				or (SidesEquivalent(bbEquation.LHS, bbEquationSolution.RHS) and SidesEquivalent(bbEquation.RHS, bbEquationSolution.LHS)))
				SolutionAchieved = true;

			if (SolutionAchieved)
			{
				ChangeOperationsToTicks();
			}

			BetweenEquationLevels = SolutionAchieved;
		}
		else
		{
			// If Triangle Logarithm is the first instance of an 'appropriate' logarithm
			// Then that Log is flagged as achieved (and coloured blue)
			// If the Triangle Logarithm is not in the list or is a second instance
			// Release  bomb wave

			//Logarithm& TargetLog = vecAppropriateLogarithms.front();
			
			if ((std::to_string(TriangleLogarithm.Base) != "0") and (std::to_string(TriangleLogarithm.Log) != "0"))
				if ((long)pow(TriangleLogarithm.Base, TriangleLogarithm.Log) > 0)
				{	
					boolean BombsNeeded= true;
					TriangleLogarithm.AntiLog = (long)pow(TriangleLogarithm.Base, TriangleLogarithm.Log);
					for (Logarithm& bbLog : vecAppropriateLogarithms)
					{
						if (bbLog.AntiLog == TriangleLogarithm.AntiLog)
						{
							BombsNeeded = false;
							if (bbLog.Achieved == false)
							{
								bbLog.Achieved = true;
							}
							else
							{
								BombsNeeded = true;
							}
						}
					}
					if (BombsNeeded == true)
						BombWave(0, 0);
				}
				else
				{
					ClearTriangleLogarithm();
				}
		}
	}

	bool SidesEquivalent(SideOfEquation Side1, SideOfEquation Side2)
	{
		//Assuming any gathering has taken place

		// Check first number of terms is equal
		// If there are the same number of terms and all the terms have a match then
		// SidesEquivalent is true
		
		bool TermMatch = false;
		int nNumTerm1 = 0;
		int nNumTerm2 = 0;

		for (Term Term1 : Side1.vecTerm)
			nNumTerm1++;

		for (Term Term2 : Side2.vecTerm)
			nNumTerm2++;

		if (nNumTerm1 != nNumTerm2)
			return false;

		for (Term Term1 : Side1.vecTerm)
		{
			TermMatch = false;
			for (Term Term2 : Side2.vecTerm)
			{
				if (TermsEquivalent(Term1, Term2))
					TermMatch = true;
			}
			if (TermMatch == false)
				return false;
		}

		return true;

	}

	bool TermsEquivalent(Term Term1, Term Term2)
	{
		bool VariableMatch = false;
		int nNumVar1 = 0;
		int nNumVar2 = 0;

		// Check first for same number of variables - we will implement coefficients later
		// If there are the same number of variables and all the variables have a match then
		// TermsEquivalent is true.

		for (Variable Var1 : Term1.vecVariable)
			nNumVar1++;

		for (Variable Var2 : Term2.vecVariable)
			nNumVar2++;

		if (nNumVar1 != nNumVar2)
			return false;

		for (Variable Var1 : Term1.vecVariable)
		{
			VariableMatch = false;
			for (Variable Var2 : Term2.vecVariable)
			{
				if (Var1.Name == Var2.Name)
					if (Var1.Power == Var2.Power)
						VariableMatch = true;
			}
			if (VariableMatch == false)
				return false;
		}

		return true;
	}

	void ResetGame()
	{
		if (!DoingLogarithms)
		{

			BetweenEquationLevels = false;

			vecAsteroids.clear();
			vecBullets.clear();
			vecOperations.clear();
			vecSpaceAnchors.clear();
			vecTicks.clear();

			bbAsteroidLevel = 1;
			bbEquationLevel = 1;

			LoadAsteroidLevel(bbAsteroidLevel);
			LoadEquationLevel(bbEquationLevel);

			AvailableSpaceAnchors = 0;

		}
		else
		{

			BetweenEquationLevels = false;

			vecAsteroids.clear();
			vecBullets.clear();
			vecOperations.clear();
			vecSpaceAnchors.clear();
			vecTicks.clear();

			bbAsteroidLevel = 1;
			bbLogarithmLevel = 1;

			LoadAsteroidLevel(bbAsteroidLevel);
			LoadLogarithmLevel(bbLogarithmLevel);

			;
		}

		//bDead = -false;
		//nScore = 0;
	}

	void ChangeOperationsToTicks()
	{
		for (auto& a : vecOperations)
			vecTicks.push_back({ a.x, a.y, a.dx, a.dy, 8, 0.0f });

		vecOperations.clear();
	}

	void BombWave(float BombAngle, float fElapsedTime)
	{
		//Toward South
		for (int i=0; i <= 11; i++)
		{
			vecBombs.push_back({ (float) (i * 100) + 50, -50, 0, 50, 12, BombAngle });
		}

		//Toward East
		for (int i = 0; i <= 5; i++)
		{
			vecBombs.push_back({ -50, (float)(i * 100) + 50, 50, 0, 12, 3 * Pi /2 });
		}

		//Toward SouthEast
		for (int i = 0; i <= 5; i++)
		{
			vecBombs.push_back({-90 + (float)(i * sqrt(1250)), 90 - (float)(i * sqrt(1250)), -50 * sinf(7 * Pi / 4), 50 * cosf(7 * Pi / 4), 12, 7 * Pi / 4 });
		}

		//Toward North
		for (int i = 0; i <= 11; i++)
		{
			vecBombs.push_back({ (float)(i * 100) + 50, (float) (ScreenHeight() + 50), 0, -50, 12, Pi });
		}

		//Toward West
		for (int i = 0; i <= 5; i++)
		{
			vecBombs.push_back({ (float) (ScreenWidth() + 50), (float)(i * 100) + 50, -50, 0, 12, Pi / 2 });
		}

		//Toward NorthWest
		for (int i = 0; i <= 5; i++)
		{
			vecBombs.push_back({ -90 + ScreenWidth() + (float)(i * sqrt(1250)), 90 + ScreenHeight() - (float)(i * sqrt(1250)) , -50 * sinf(3 * Pi / 4), 50 * cosf(3 * Pi / 4), 12, 3 * Pi / 4 });
		}
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		if (bDead)
		{	
			ResetGame();
			nScore = 0;
			bDead = -false;
		}


		// Clear Screen
		//Fill(0, 0, ScreenWidth(), ScreenHeight(), olc::PIXEL_SOLID, FG_BLACK);
		Clear(olc::BLACK);

		//Toggle between Equations and Logarithms
		if (GetKey(olc::L).bReleased)
		{
			DoingLogarithms = !DoingLogarithms;
			// For now
			ResetGame();
		}

		//Advance Level
		if (GetKey(olc::I).bReleased)
		{
			if (DoingLogarithms)
			{
				if (bbLogarithmLevel < 4)
				{
					bbLogarithmLevel++;
				}
				else
				{
					bbLogarithmLevel = 1;
				}
				LoadLogarithmLevel(bbLogarithmLevel);
			}
			else
			{
				if (bbEquationLevel < 3)
				{
					bbEquationLevel++;
				}
				else
				{
					bbEquationLevel = 1;
				}
				//Between = false;
				LoadEquationLevel(bbEquationLevel);
			}
		}

		//Bomb Wave
		if (GetKey(olc::B).bReleased)
		{
			BombWave(0.0f, fElapsedTime);
		}

		//Steer
		//if(GetKey(olc::Key::LEFT).bHeld)
		if (GetKey(olc::A).bHeld) //[VK_LEFT].bHeld)
			player.angle -= 5.0f * fElapsedTime;
		if (GetKey(olc::D).bHeld)
			player.angle += 5.0f * fElapsedTime;

		//Thrust
		if (GetKey(olc::W).bHeld)
		{
			//ACCELERATION changes VELOCITY (with respect to time)
			player.dx += sin(player.angle) * 40.0f * fElapsedTime;
			player.dy += -cos(player.angle) * 40.0f * fElapsedTime;
		}

		//Deploy Space Anchor
		if (GetKey(olc::Key::SPACE).bReleased)
		{
			if (AvailableSpaceAnchors > 0)
			{	
				AvailableSpaceAnchors--;
				//player.dx = 0 ;
				//player.dy = 0 ;
				player.dx = player.dx / 2;
				player.dy = player.dy / 2 ;
			}
		}

		// VELOCITY changes POSITION (with respect to time)
		player.x += player.dx * fElapsedTime;
		player.y += player.dy * fElapsedTime;

		//Keep ship in gamespace
		WrapCoordinates(player.x, player.y, player.x, player.y);

		//Check for collisions with Asteroids
		for (auto& a : vecAsteroids)
			if (IsPointInsideCircle(a.x, a.y, a.nSize, player.x, player.y))
				bDead = true; // Uh oh....
	
		//Check for collisions with Bombs
		ReturnTransformedCoordinates(Ship, vecModelShip, player.x, player.y, player.angle, 1);
		for (auto& a : vecBombs)
			if (IsPointInsideCircle(a.x, a.y, a.nSize, player.x, player.y))
			{
				//Draw bounding box
				//Use DrawWireFrame model now
				DrawWireFrameModel(vecBoundingBoxBomb, a.x, a.y, a.angle, a.nSize, olc::RED, 0xFFFFFFFF);
				ReturnTransformedCoordinates(BoundingBoxBomb, vecBoundingBoxBomb, a.x, a.y, a.angle, a.nSize);
		
				//if (ShapeOverlap_DIAGS(Ship, BoundingBoxBomb) == true)
				if (ShapeOverlap_DIAGS(BoundingBoxBomb, Ship) == true)
				{
					ScreenOutputFlag1 = "Should work.";
					bDead = true;
				}
			}
				//bDead = true; // This will be poor.


		//Bullet in direction of player
		if (GetKey(olc::ENTER).bReleased)
			vecBullets.push_back({ player.x, player.y, 100.0f * sinf(player.angle), -100.0f * cosf(player.angle), 0, 0 });
		//			vecBullets.push_back({ player.x, player.y, 50.0f * cosf(player.angle), -50.0f * sinf(player.angle), 0, 0 });


		//Update and draw asteroids
		for (auto& a : vecAsteroids)
		{
			a.x += a.dx * fElapsedTime;
			a.y += a.dy * fElapsedTime;
			a.angle += 0.5f * fElapsedTime;
			WrapCoordinates(a.x, a.y, a.x, a.y);

			DrawWireFrameModel(vecModelAsteroid, a.x, a.y, a.angle, a.nSize, olc::WHITE, 0xFFFFFFFF);
				//			for (int x = 0; x < a.nSize; x++) 
				//				for (int y = 0; y < a.nSize; y++)
				//					Draw(a.x + x, a.y + y, PIXEL_SOLID, FG_WHITE);
		}
		
		//Update and draw operations
		for (auto& a : vecOperations)
		{
			a.x += a.dx * fElapsedTime;
			a.y += a.dy * fElapsedTime;
			a.angle += 0.5f * fElapsedTime;
			WrapCoordinates(a.x, a.y, a.x, a.y);

			DrawString(a.x, a.y, a.OpStr, olc::WHITE, 5);
		}

		//Check for scooping up Operations
		int bbCounter = 0;
		std::string CarryOpStr = "";
		for (auto& a : vecOperations)
		{
			if (IsPointInsideCircle(a.x + (fScale * fOperationStrOffsetx), a.y + (fScale * fOperationStrOffsety), a.nSize, player.x, player.y))
			{
				CarryOpStr = a.OpStr;
				a.x = -100;		// Send it off the screen  // second thoughts kill the fucker
				vecOperations.erase(vecOperations.begin() + bbCounter);
				//Now here we are working from the position that all OpStr are two characters
				PerformOperationControl(CarryOpStr);
				//LevelComplete = true;
			}
			bbCounter++;
		}
		//Update and draw Space Anchors
		for (auto& a : vecSpaceAnchors)
		{
			a.x += a.dx * fElapsedTime;
			a.y += a.dy * fElapsedTime;
			a.angle += 2.5f * fElapsedTime;
			WrapCoordinates(a.x, a.y, a.x, a.y);

			DrawWireFrameModel(vecModelSpaceAnchor, a.x, a.y, a.angle, a.nSize, olc::WHITE, 0xFFFFFFFF);
	
		}

		//Check for scooping up Space Anchors
		for (auto& a : vecSpaceAnchors)
			if (IsPointInsideCircle(a.x, a.y, a.nSize, player.x, player.y))
			{
				AvailableSpaceAnchors++;
				a.x = -100;		// Send it off the screen
			}

		//Update and draw Ticks
		for (auto& a : vecTicks)
		{
			a.x += a.dx * fElapsedTime;
			a.y += a.dy * fElapsedTime;
			a.angle = 0;
			WrapCoordinates(a.x, a.y, a.x, a.y);

			DrawWireFrameModel(vecModelTick, a.x, a.y, a.angle, a.nSize, olc::WHITE, 0xFFFFFFFF);
		}

		//Check for scooping up Ticks
		for (auto& a : vecTicks)
			if (IsPointInsideCircle(a.x, a.y, a.nSize, player.x, player.y))
			{
				vecTicks.clear();
				bbEquationLevel++;
				LoadEquationLevel(bbEquationLevel);
				BetweenEquationLevels = false;
				//a.x = -100;		// Send it off the screen
			}

		fAccruedTime += fElapsedTime;

		// Update and draw Bombs
		for (auto& a : vecBombs)
		{
			//a.dx = 50 * sinf(fAccruedTime);
			a.x += a.dx * fElapsedTime;
			a.y += a.dy * fElapsedTime;

			DrawWireFrameModel(vecModelBomb, a.x, a.y, a.angle, a.nSize, olc::WHITE, 0xFFFFFFFF);
		}

		std::vector<sSpaceObject> newAsteroids;
		//std::vector<sSpaceObject> newOperations;



		//Update and draw bullets
		for (auto& b : vecBullets)
		{
			b.x += b.dx * fElapsedTime;
			b.y += b.dy * fElapsedTime;
			WrapCoordinates(b.x, b.y, b.x, b.y);
			Draw(b.x, b.y);

			// Check collision with asteroids
			for (auto& a : vecAsteroids)
			{
				if (IsPointInsideCircle(a.x, a.y, a.nSize, b.x, b.y))
				{
					// Asteroid Hit
					b.x = -100;
					
					if (a.nSize > 4)
					{
						// Create three child asteroids
						float angle1 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
						float angle2 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
						float angle3 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
						
						newAsteroids.push_back({ a.x, a.y, AsteroidSpeed * sinf(angle1), -AsteroidSpeed * cosf(angle1), (int)a.nSize >> 1, 0.0f });
						newAsteroids.push_back({ a.x, a.y, AsteroidSpeed * sinf(angle2), -AsteroidSpeed * cosf(angle2), (int)a.nSize >> 1, 0.0f });
						newAsteroids.push_back({ a.x, a.y, AsteroidSpeed * sinf(angle3), -AsteroidSpeed * cosf(angle3), (int)a.nSize >> 1, 0.0f });
						//newAsteroids.push_back({ a.x, a.y, 20.0f * sinf(angle2), -20.0f * cosf(angle3), (int)a.nSize >> 1, 0.0f });
						//Create an operation
						
					}

					//if (!DoingLogarithms)
					//{
					if (BetweenEquationLevels == false) //We might change this to BetweenLessonLevels
					{
						//Just playing around really
						bbRnd = (float)rand() / (float)RAND_MAX;

						//Drop Operations or Space Achors
						if (bbRnd < 0.5)
						{

							bbOpStr = GenerateOpStr();

							// Seems we need count our vectors; they don't have a count method.

							bbi = 0;
							for (auto& c : vecOperations)
							{
								bbi++;
							}

							float angle4 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
							if (bbi <= 2)
								//vecOperations.push_back({ a.x, a.y, 10.0f * sinf(angle4), -10.0f * cosf(angle4),  (int)fScale * a.nSize >> 1, 0.0f, bbOpStr });
								vecOperations.push_back({ a.x, a.y, 10.0f * sinf(angle4), -10.0f * cosf(angle4),  8, 0.0f, bbOpStr });

							else
							{
								vecOperations.erase(vecOperations.begin());

								bbi = 0;
								for (auto& c : vecOperations)
								{
									bbi++;
								}
								if (bbi <= 2)
									vecOperations.push_back({ a.x, a.y, 10.0f * sinf(angle4), -10.0f * cosf(angle4), 8, 0.0f, bbOpStr });
							}
						}
						else
						{
							bbi = 0;
							for (auto& c : vecSpaceAnchors)
							{
								bbi++;
							}
							float angle5 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
							if (bbi <= 2)
								vecSpaceAnchors.push_back({ a.x, a.y, 10.0f * sinf(angle5), -10.0f * cosf(angle5),  8, 0.0f });
							else
							{
								vecSpaceAnchors.erase(vecSpaceAnchors.begin());

								bbi = 0;
								for (auto& c : vecSpaceAnchors)
								{
									bbi++;
								}
								if (bbi <= 2)
									vecSpaceAnchors.push_back({ a.x, a.y, 10.0f * sinf(angle5), -10.0f * cosf(angle5), 8, 0.0f });
							}
						}
					}
					else
					{
						bbi = 0;
						for (auto& c : vecTicks)
						{
							bbi++;
						}
						if (bbi <= 6)
						{
							float angle6 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
							vecTicks.push_back({ a.x, a.y, 10.0f * sinf(angle6), -10.0f * cosf(angle6), 8, 0.0f });
						}
					}

					a.x = -100;
					nScore += 100;
		
				}
			}
		}

		// Append new asteroids to existing vector
		for (auto a : newAsteroids)
			vecAsteroids.push_back(a);

		// Append new operation to existing vector
		//for (auto a : newOperations)
		//	vecOperations.push_back(a);

		//Remove off screen bullets
		if (vecBullets.size() > 0)
		{
			auto i = remove_if(vecBullets.begin(), vecBullets.end(),
				[&](sSpaceObject o) {return (o.x <= 1 || o.y <= 1 || o.x >= ScreenWidth() - 1 || o.y >= ScreenHeight() - 1);});
			if (i != vecBullets.end()) 
				vecBullets.erase(i, vecBullets.end());
		}

		//Remove off screen asteroids
		if (vecAsteroids.size() > 0)
		{
			auto i = remove_if(vecAsteroids.begin(), vecAsteroids.end(),
				[&](sSpaceObject o) {return (o.x < -50);});
			if (i != vecAsteroids.end())
				vecAsteroids.erase(i,vecAsteroids.end());
		}

		//Remove off screen operations
		if (vecAsteroids.size() > 0)
		{
			auto i = remove_if(vecOperations.begin(), vecOperations.end(),
				[&](sSpaceObject o) {return (o.x < -50);});
			if (i != vecOperations.end())
				vecOperations.erase(i, vecOperations.end());
		}

		//Remove off screen space anchors
		if (vecSpaceAnchors.size() > 0)
		{
			auto i = remove_if(vecSpaceAnchors.begin(), vecSpaceAnchors.end(),
				[&](sSpaceObject o) {return (o.x < -50);});
			if (i != vecSpaceAnchors.end())
				vecSpaceAnchors.erase(i, vecSpaceAnchors.end());
		}

		//Remove off screen ticks
		if (vecTicks.size() > 0)
		{
			auto i = remove_if(vecTicks.begin(), vecTicks.end(),
				[&](sSpaceObject o) {return (o.x < -50);});
			if (i != vecTicks.end())
				vecTicks.erase(i, vecTicks.end());
		}

		//Remove off screen bombs - do this in two bites
		if (vecBombs.size() > 0)
		{
			auto i = remove_if(vecBombs.begin(), vecBombs.end(),
				[&](sSpaceObject o) {return (o.y > (ScreenHeight() + 100));});
			if (i != vecBombs.end())
				vecBombs.erase(i, vecBombs.end());
		}

		if (vecBombs.size() > 0)
		{
			auto i = remove_if(vecBombs.begin(), vecBombs.end(),
				[&](sSpaceObject o) {return (o.y < (-100));});
			if (i != vecBombs.end())
				vecBombs.erase(i, vecBombs.end());
		}


		if (vecBombs.size() > 0)
		{
			auto i = remove_if(vecBombs.begin(), vecBombs.end(),
				[&](sSpaceObject o) {return (o.x > (ScreenWidth() + 100));});
			if (i != vecBombs.end())
				vecBombs.erase(i, vecBombs.end());
		}

		if (vecBombs.size() > 0)
		{
			auto i = remove_if(vecBombs.begin(), vecBombs.end(),
				[&](sSpaceObject o) {return (o.x < (-100));});
			if (i != vecBombs.end())
				vecBombs.erase(i, vecBombs.end());
		}


		//Handle running out of asteroids.
		if (vecAsteroids.empty())
		{
			nScore += 1000;
			// Here we are going to have to work on a repopulate asteroids based on level.
			bbAsteroidLevel++;
			if (bbAsteroidLevel == 4)
				bbAsteroidLevel = 1;

			LoadAsteroidLevel(bbAsteroidLevel);
			//Add new asteroids
			//vecAsteroids.push_back({ 30.0f * sinf(player.angle - 3.14159f / 2.0f),
			//						30.0f * cosf(player.angle - 3.14159f / 2.0f),
			//						10.0f * sinf(player.angle),
			//						10.0f * cosf(player.angle),
			//						(int)16, 0.0f });
			//
			//vecAsteroids.push_back({ 30.0f * sinf(player.angle + 3.14159f / 2.0f),
			//						30.0f * cosf(player.angle + 3.14159f / 2.0f),
			//						10.0f * sinf(-player.angle),
			//						10.0f * cosf(-player.angle),
			//						(int)16, 0.0f });
		}

		// Draw Ship
		DrawWireFrameModel(vecModelShip, player.x, player.y, player.angle, 1.0f);

		std::string bbStr = "Score: ";

		// Draw Score
		//DrawString(5, 10, bbStr + std::to_string(nScore), olc::WHITE, 10);
		//DrawString(5, 10, "Level" + std::to_string(bbAsteroidLevel), olc::WHITE, 10);
		//DrawString(5, 10,  "vec.size " + std::to_string(vecOperations.size()), olc::WHITE, 10);
		//DrawString(5, 128, "vec.capacity " + std::to_string(vecOperations.capacity()), olc::WHITE, 10);
		//DrawString(5, 256, "vec.rend " + std::to_string(vecOperations.rend), olc::WHITE, 10);
		//DrawString(5, 10, PresentOperation + PresentOperationVariable, olc::WHITE, 10);
		//DrawString(5, 100, ScreenOutputFlag1, olc::WHITE, 5);
		//DrawString(5, 150, ScreenOutputFlag2, olc::WHITE, 5);
		//DrawString(5, 200, ScreenOutputFlag3, olc::WHITE, 5);
		//DrawString(5, 250, ScreenOutputFlag4, olc::WHITE, 5);
		//DrawString(5, 300, ScreenOutputFlag5, olc::WHITE, 5);
		//DrawString(5, 350, ScreenOutputFlag6, olc::WHITE, 5);

		//DrawString(5, 10, std::to_string(BetweenEquationLevels), olc::WHITE, 10);


		//Draw Equation
		//DrawString((int)(ScreenWidth() / 2), (int)(ScreenHeight() / 2), L"Equation!");
		//DrawString(5, 20, L"Equation ");

		// Get mouse location this frame
		//olc::vf2d vMouse = { (float)GetMouseX(), (float)GetMouseY() };

		// Draw a string
		//DrawString(64, 64, "Equation", olc::WHITE, 10);

		//Draw Sprite
		//DrawSprite(vMouse.x, vMouse.y, sprDemo);
		//DrawSprite(vMouse.x, vMouse.y, sprDemo);

		//Draw Equation
		if (!DoingLogarithms)
		{
			if (BetweenEquationLevels)
			{
				DrawString(5, 10, sCongratulations, olc::WHITE, 10);
			}
			else
			{
				DrawString(5, 10, sLevelInstruction, olc::WHITE, 10);
			}

			bbEquation.LHS.SideOfEquationStr = "";
			for (auto& a : bbEquation.LHS.vecTerm)
				for (auto& b : a.vecVariable)
					bbEquation.LHS.SideOfEquationStr = bbEquation.LHS.SideOfEquationStr + "[" + b.Name + "{" + std::to_string(b.Power) + "}]";

			if (bbEquation.LHS.SideOfEquationStr == "")
				bbEquation.LHS.SideOfEquationStr = "[1]";

			bbStrLen = 0;

			for (char a : bbEquation.LHS.SideOfEquationStr)
				bbStrLen++;

			//We have the issue of the scaling, we probably need to concrete a certain scale - this one

			//DrawString((ScreenWidth() / 2.0f) - ((bbStrLen * 24) + 24), 5 * ScreenHeight() / 6, bbEquation.LHS.SideOfEquationStr + std::to_string(bbStrLen), olc::WHITE, 3);
			DrawString((ScreenWidth() / 2.0f) - ((bbStrLen * 24) + 24), 5 * ScreenHeight() / 6, bbEquation.LHS.SideOfEquationStr, olc::WHITE, 3);

			DrawString((ScreenWidth() / 2) - 12, 5 * ScreenHeight() / 6, "=", olc::WHITE, 3);

			if (bbEquation.RHS.SideOfEquationStr == "")
				bbEquation.RHS.SideOfEquationStr = "[1]";

			bbStrLen = 0;

			for (char a : bbEquation.RHS.SideOfEquationStr)
				bbStrLen++;

			bbEquation.RHS.SideOfEquationStr = "";
			for (auto& a : bbEquation.RHS.vecTerm)
				for (auto& b : a.vecVariable)
					bbEquation.RHS.SideOfEquationStr = bbEquation.RHS.SideOfEquationStr + "[" + b.Name + "{" + std::to_string(b.Power) + "}]";

			//DrawString((ScreenWidth() / 2.0f) + 24, 5 * ScreenHeight() / 6, bbEquation.RHS.SideOfEquationStr + std::to_string(bbStrLen), olc::WHITE, 3);
			DrawString((ScreenWidth() / 2.0f) + 24, 5 * ScreenHeight() / 6, bbEquation.RHS.SideOfEquationStr, olc::WHITE, 3);
		}
		else
		{
			int i = 0;
			int j = 0;
			for (Logarithm a : vecAppropriateLogarithms)
			{
				j++;
				i = i + 32;
				if (a.Achieved == true)
				{
					DrawString(50, 64 + i, std::to_string(a.AntiLog), olc::BLUE, 4);
				}
				else
				{ 
					DrawString(50, 64 + i, std::to_string(a.AntiLog), olc::WHITE, 4);
				}
				
			}

			//DrawString(5, 10, "Doing Logarithms", olc::WHITE, 10);
			DrawWireFrameModel(vecModelLogarithmTriangle, ScreenWidth() / 2, ScreenHeight() / 2, 0.0f, 20.0f, olc::BLUE);
		
			//DrawWireFrameModel(vecModelBomb, ScreenWidth() / 4, ScreenHeight() / 2, 0.0f, 10.0f, olc::WHITE);


			//Locate triangle corners
			//Logarithm TargetLog = vecAppropriateLogarithms[j-1];

			bbStrLen = 0;
			for (char a : std::to_string(TriangleLogarithm.Log))
				bbStrLen++;

			if (std::to_string(TriangleLogarithm.Log) != "0")
				DrawString((ScreenWidth() / 2) - (16*bbStrLen), (ScreenHeight() / 5) - 24, std::to_string(TriangleLogarithm.Log), olc::WHITE, 4);
			
			bbStrLen = 0;
			for (char a : std::to_string(TriangleLogarithm.Base))
				bbStrLen++;
			
			if (std::to_string(TriangleLogarithm.Base) != "0")
				DrawString((ScreenWidth() / 3) - (16*bbStrLen), 4 * ScreenHeight() / 5, std::to_string(TriangleLogarithm.Base), olc::WHITE, 4);
			
			bbStrLen = 0;
			for (char a : std::to_string(TriangleLogarithm.AntiLog))
				bbStrLen++;

			if (std::to_string(TriangleLogarithm.AntiLog) != "0")
				DrawString((2 * ScreenWidth()/ 3) - (16 * bbStrLen), 4 * ScreenHeight() / 5, std::to_string(TriangleLogarithm.AntiLog), olc::WHITE, 4);

		}
		//Investigate string parameters


		//Size 1 - 8 pixels
		//DrawString(0.0f, 1 * ScreenHeight() / 6, bbTestStr, olc::BLUE, 1);

		//Size 2 - 16 pixels
		//DrawString(0.0f, 2 * ScreenHeight() / 6, bbTestStr, olc::GREEN, 2);

		//Size 3 - 24 pixels
		//DrawString(0.0f, 1 * ScreenHeight() / 6, bbTestStr, olc::WHITE, 3);

		//Size 4 - 32 Pixels
		//DrawString(0.0f, 2 * ScreenHeight() / 6, bbTestStr, olc::WHITE, 4);

		//Size 5 - 40 Pixels
		//DrawString(0.0f, 3 * ScreenHeight() / 6, bbTestStr, olc::WHITE, 5);

		// Display Number of Space Anchors
		DrawString(1120, 80, std::to_string(AvailableSpaceAnchors), olc::WHITE, 5);


		return true;
	
	}

	void DrawWireFrameModel(const std::vector<std::pair<float, float>>& vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, olc::Pixel p = olc::WHITE,  uint32_t pattern = 0xFFFFFFFF)
	{
		// pair.first = x coordinate
		// pair.second = y coordinate

		// Create translated model vector of coordiante pairs
		std::vector<std::pair<float, float>> vecTransformedCoordinates;
		int verts = vecModelCoordinates.size();
		vecTransformedCoordinates.resize(verts);


		// Rotate
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
			vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
		}

		// Scale
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
		}

		// Translate
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;

		}

		// Draw Closed Polygon
		for (int i = 0; i < verts; i++)
		{
			int j = (i + 1);
			//DrawLine(vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second, vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second, olc::WHITE, pattern);
			DrawLine(vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second, vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second, p, pattern);

		}


	}

	void ReturnTransformedCoordinates(polygon &polyref, const std::vector<std::pair<float, float>>& vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f)
	{

		polyref.pos.x = x;
		polyref.pos.y = y;
		polyref.angle = r;
		
		polyref.p.clear();
		//vec2d tempVec2d;

		
		// Create translated model vector of coordiante pairs
		std::vector<std::pair<float, float>> vecTransformedCoordinates;
		int verts = vecModelCoordinates.size();
		vecTransformedCoordinates.resize(verts);
		//ScreenOutputFlag1 = "In the damn function Cap'n";
		// Rotate
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
			vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
		}

		// Scale
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
		}

		// Translate
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
		}

		//Copy to Polygon
		for (int i = 0; i < verts; i++)
		{

			vec2d tempVec2d;
			tempVec2d.x = vecTransformedCoordinates[i].first;
			tempVec2d.y = vecTransformedCoordinates[i].second;

			polyref.p.push_back(tempVec2d);
		}

		//ScreenOutputFlag1 = std::to_string(polyref.pos.x) + " pos " + std::to_string(polyref.pos.y);
		//ScreenOutputFlag2 = std::to_string(polyref.p[0].x) + " p[0] " + std::to_string(polyref.p[0].y);
		//ScreenOutputFlag3 = std::to_string(polyref.p[1].x) + " p[1] " + std::to_string(polyref.p[1].y);
		//ScreenOutputFlag4 = std::to_string(polyref.p[2].x) + " p[2] " + std::to_string(polyref.p[2].y);
		//ScreenOutputFlag5 = std::to_string(vecModelCoordinates[1].first) + " vmc[1] " + std::to_string(vecModelCoordinates[1].second);
		//ScreenOutputFlag6 = std::to_string(vecModelCoordinates[2].first) + " vmc[2] " + std::to_string(vecModelCoordinates[2].second);
		//ScreenOutputFlag6 = std::to_string(s) + " s : r" + std::to_string(r);

	}

	bool ShapeOverlap_SAT(polygon& r1, polygon& r2)
	{

		polygon* poly1 = &r1;
		polygon* poly2 = &r2;

		ScreenOutputFlag1 = std::to_string(poly1->p[2].x) + " " + std::to_string(poly2->p[2].x);

		for (int shape = 0; shape < 2; shape++)
		{
			if (shape == 1)
			{
				poly1 = &r2;
				poly2 = &r1;
			}

			for (int a = 0; a < poly1->p.size(); a++)
			{
				int b = (a + 1) % poly1->p.size();
				vec2d axisProj = { -(poly1->p[b].y - poly1->p[a].y), poly1->p[b].x - poly1->p[a].x };
				float d = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
				axisProj = { axisProj.x / d, axisProj.y / d };

				// Work out min and max 1D points for r1
				//float min_r1 = INFINITY, max_r1 = -INFINITY;
				float min_r1 = 1000000000, max_r1 = -1000000000;
				for (int p = 0; p < poly1->p.size(); p++)
				{
					float q = (poly1->p[p].x * axisProj.x + poly1->p[p].y * axisProj.y);
					//min_r1 = std::min(min_r1, q);
					min_r1 = (min_r1 < q) ? min_r1 : q;
					//max_r1 = std::max(max_r1, q);
					max_r1 = (max_r1 > q) ? max_r1 : q;
				}

				// Work out min and max 1D points for r2
				float min_r2 = 1000000000, max_r2 = -1000000000;
				for (int p = 0; p < poly2->p.size(); p++)
				{
					float q = (poly2->p[p].x * axisProj.x + poly2->p[p].y * axisProj.y);
					//min_r2 = std::min(min_r2, q);
					min_r2 = (min_r2 < q) ? min_r2 : q;
					//max_r2 = std::max(max_r2, q);
					max_r2 = (max_r2 > q) ? max_r2 : q;
				}

				if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
					return false;
			}
		}

		return true;
	}

	bool ShapeOverlap_DIAGS(polygon& r1, polygon& r2)
	{
		polygon* poly1 = &r1;
		polygon* poly2 = &r2;

		//ScreenOutputFlag1 = std::to_string(poly1->p[2].x) + " " + std::to_string(poly2->p[2].x);
		//ScreenOutputFlag1 = std::to_string(poly1->pos.x) + " " + std::to_string(poly2->pos.x);

		for (int shape = 0; shape < 2; shape++)
		{
			if (shape == 1)
			{
				poly1 = &r2;
				poly2 = &r1;
			}

			// Check diagonals of polygon...
			for (int p = 0; p < poly1->p.size(); p++)
			{
				vec2d line_r1s = poly1->pos;
				//line_r1s.x = poly1->pos.x;
				//line_r1s.y = poly1->pos.y;
				vec2d line_r1e = poly1->p[p];
				//line_r1e.x = poly1->p[p].x;
				//line_r1e.y = poly1->p[p].y;

				// ...against edges of the other
				for (int q = 0; q < poly2->p.size(); q++)
				{
					vec2d line_r2s = poly2->p[q];
					vec2d line_r2e = poly2->p[(q + 1) % poly2->p.size()];
					
					//ScreenOutputFlag3 = std::to_string(line_r1s.x) + " r1s " + std::to_string(line_r1s.y);
					//ScreenOutputFlag4 = std::to_string(line_r1e.x) + " r1e " + std::to_string(line_r1e.y);
					//ScreenOutputFlag5 = std::to_string(line_r2s.x) + " r2s " + std::to_string(line_r2s.y);
					//ScreenOutputFlag6 = std::to_string(line_r2e.x) + " r2e " + std::to_string(line_r2e.y);

					// Standard "off the shelf" line segment intersection
					float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
					float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
					float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

					//ScreenOutputFlag5 = std::to_string(t1) + " " + std::to_string(t2);
					//ScreenOutputFlag5 = std::to_string(poly1->p.size());
					//ScreenOutputFlag6 = std::to_string(poly2->p.size());

					//ScreenOutputFlag1 = std::to_string(poly1->pos.x) + " p1 pos " + std::to_string(poly1->pos.y);
					//ScreenOutputFlag2 = std::to_string(poly2->pos.x) + " p2 pos " + std::to_string(poly2->pos.y);

					if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	void WrapCoordinates(float ix, float iy, float& ox, float& oy)
	{
		ox = ix;
		oy = iy;
		if (ix < 0.0f) ox = ix + (float)ScreenWidth();
		if (ix > (float)ScreenWidth()) ox = ix - (float)ScreenWidth();
		if (iy < 0.0f) oy = iy + (float)ScreenHeight();
		if (iy > (float)ScreenHeight()) oy = iy - (float)ScreenHeight();
	}

	virtual void Draw(int x, int y, short c = 0x2588, short col = 0x000F)
	{
		float fx, fy;
		WrapCoordinates(x, y, fx, fy);

		olc::vi2d bbPos;
		bbPos.x = int(fx);
		bbPos.y = int(fy);

		PixelGameEngine::Draw(bbPos, olc::WHITE);
		//olcConsoleGameEngine::Draw(fx, fy, c, col);
		//Draw(const olc::vi2d & pos, Pixel p = olc::WHITE)
	}
};

int main()
{
	EquationBlaster demo;
	//So we will work of a World Area of this plus 100 all round so -100-1300, -100-740.
	if (demo.Construct(1200, 640, 1, 1))
		demo.Start();
	return 0;
}
