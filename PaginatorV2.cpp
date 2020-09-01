//
// NOTES
//
// Game relevant objective
// So the plan is players collect terms in the exp expansion to approximate answers
// We could use the same dynamic to scoop us cos x as opposed to sin x.
//
// 


#define OLC_PGE_APPLICATION 
#include "olcPixelGameEngine.h"
#include <iostream>
#include <fstream>
#include <string>


class bbPaginator : public olc::PixelGameEngine
{
public:
	bbPaginator()
	{
		sAppName = "Paginator";
	}

private:

	//std::string wtwtch;

	struct TitanQuestion
	{
		std::string RawField;
		std::vector<std::string> Tokens;
		std::string QuestionID;
		std::string MetaQuestionText;
		std::string MetaQuestionMath;
		std::string CommonMistakes;
		std::string AnswerQuantity;
		std::string Units;
		std::string BestTime;
		std::string BestTimeStr;
		std::string NumberOfAttemptsRevision;
	};

	struct sSpaceText
	{
		float x;
		float y;
		float dx;
		float dy;
		int nFontSize;
		std::string str;
	};

	struct Bracket
	{
		// We are calling them Brackets because they are more general and various than just 'variable' brackets
		int nId;
		int nPosOBrac; //Position in StrToPaginate
		int nPosCBrac;
		int nPosOExpBrac;	//Position in StrToPaginate
		int nPosCExpBrac;
		int nDepthOfNesting;	//Exponent brackets will always be nested one deeper
		bool bNewTerm = false;
		int nAggLenAboveLine = 0;
		int nAggLenBelowLine = 0;
		int nAggLenAboveLineExp = 0;
		int nAggLenBelowLineExp = 0;
		int nTotalLength = 0;
		int nTotalHeight = 0;
		std::string sExponentialString; //Special cases {1} do nothing {-1} place 'under line'
		bool bBracIsSurd = false;
		std::vector<sSpaceText> PrePaginatedVar;
		std::vector<sSpaceText> PrePaginatedExp;
		std::vector<sSpaceText> PaginatedText;
		std::vector<int> ContainedBrackets;
		bool bPaginated = false;
		bool bCopied = false;
		bool bEvaluated = false;
		std::vector<float> Terms;
		std::vector<float> TermsInExponent;
		std::vector<float> TermsInVariable;
		float fExponentEvaluation;
		float fVariableEvaluation;
		float fEvaluation;
		sSpaceText stEvaluation;
	};							


	struct Expression
	{
		std::vector<Bracket> vecBrackets;
		std::vector<sSpaceText> vecPaginatedText;
	};


	struct ExponentBracket		//Well this is interesting - not using this presently.
	{
		int nId;
		int nPosOBrac;
		int nPosCBrac;
		int nDepthOfNesting;
	};


	std::string ScreenOutputFlag1 = "";
	std::string ScreenOutputFlag2 = "";
	std::string ScreenOutputFlag3 = "";
	std::string ScreenOutputFlag4 = "";
	std::string ScreenOutputFlag5 = "";
	std::string ScreenOutputFlag6 = "";
	std::string ScreenOutputFlag7 = "";

	int WriteCounter = 0;

	bool SomethingToDisplay = false;
	bool EvaluationMade = false;
	bool bPaused;
	int nNumOpeningBrackets;
	int nNumClosingBrackets;
	int nNumBrackets;
	int nDepthBracketNesting; //Maybe depth of nesting can be counted purely from variable brackets
	int nMaxDepthNesting;
	int LoopCount;
	int AggregateLen;		//We will use an integer for now
	int DummyOffset = 0;	// This is a scaffolding measure.

	std::string StrToPaginate;
	std::string StrToPaginate1;
	std::string StrToPaginate2;
	std::string StrToPaginate3;
	std::string StrToPaginate4;

	std::string StrToEvaluate;
	std::string StrToEvaluate1;
	std::string StrToEvaluate2;
	std::string StrToEvaluate3;
	std::string StrToEvaluate4;


	std::string IntroText;

	std::vector<Bracket> vecBrackets;
	std::vector<sSpaceText> vecPaginatedText;
	std::vector<Expression> vecExpressions;
	//std::vector<std::string> vecInputStr;

	std::vector<TitanQuestion> Questions;
	int NumQuestions = 0;
	int SelectedQuestion = 0;
	bool DisplayQuestion = false;
	bool DisplayValues = false;

	bool Paginating = true;

	//
	//	TextBox
	//
	
	int bbTextBoxFont = 1;
	int bbTextHeight = 0;
	//int bbWordLength = 0;
	//int bbLineLength = 0;
	int bbTextBoxWidth = 400;
	std::vector<std::string> vecbbTextBox;

	std::map<std::string, int> mapOfFunctions;

	bool DisplayDiagnostics = false;
	std::vector<std::string> vecDiagnostics;

public:
	bool OnUserCreate() override
	{
		// We wish to expound on the properties of the exp function
		// Exp : is a function such that the product of two instances of the function
		// governed by different variables is the same as one instance of the function
		// goverened by those two variables summed.

		// One Weber is that flux that linking a circuit of one turn produces an emf of
		// one volt if wachsen or schwinden in ein second.


		std::fstream newfile;
		// So if you write to a file what was in it previously is wiped


		newfile.open("A2_BindingEnergy.txt", std::ios::in); //open a file to perform read operation using file object
		if (newfile.is_open()) {   //checking whether the file is open
			std::string tp;
			while (getline(newfile, tp)) { //read data from file object and put it into string.
			//	wtwtch += tp + " "; //print the data of the string
				TitanQuestion Question;
				//getline(newfile, tp);
				//wtwtch += tp + " ";
				Question.RawField = tp;
				Questions.push_back(Question);
				NumQuestions++;
			}
			newfile.close(); //close the file object.
		}
		ScreenOutputFlag7 = std::to_string(NumQuestions);
		//newfile.open("tpoint.txt", std::ios::out);  // open a file to perform write operation using file object
		//if (newfile.is_open()) //checking whether the file is open
		//{
		//	newfile << wtwtch + "\n";   //inserting text
			//newfile << "3 Lines \n";
		//	newfile.close();    //close the file object
		//}
		for (auto& bbQuestion : Questions)
		{
			RefineQuestion(bbQuestion);
		}


		bPaused = false;

		IntroText = "Welcome Cadet. To escape the Universe we must find it's Null Space.";

		//
		//	Strings to paginate
		//

		//StrToPaginate1 = "[{1}[{1}1]+[{1}x]+[{2}x][{-1}2!]+[{3}x][{-1}3!]+[{4}x][{-1}4!]]";
		StrToPaginate1 = "[{1}[{1}[{1}1]]+[{1}[{1}x]]+[{1}[{2}x][{-1}2!]]+[{1}[{3}x][{-1}3!]]+[{1}[{4}x][{-1}4!]]]";

		//StrToPaginate2 = "[{1}[{[{1}x]}x]+[{[{[{1}x]}x]}x]]";
		//StrToPaginate2 = "[{[{1}[{1}x]+[{1}y]]}x]";
		//StrToPaginate2 = "[{1}[{2}x]]";
		//StrToPaginate2 = "[{[{1}[{1}x]+[{1}y]]}[{1}[{1}z]]]";
		//StrToPaginate2 = "[{[{1}[{1}[{1}1]]+[{1}[{1}x]]+[{1}[{2}x][{-1}2!]]+[{1}[{3}x][{-1}3!]]+[{1}[{4}x][{-1}4!]]]}[{1}x]]";
		StrToPaginate2 = "[{1}[{1}[{1}[{1}1]]+[{1}[{1}x]]+[{1}[{2}x][{-1}2!]]+[{1}[{3}x][{-1}3!]]+[{1}[{4}x][{-1}4!]]]"
						 "[{-1}[{1}[{1}1]]+[{1}[{1}x]]+[{1}[{2}x][{-1}2!]]+[{1}[{3}x][{-1}3!]]+[{1}[{4}x][{-1}4!]]]]";


		//StrToPaginate3 = "[{1}[{[{1}x]}y]+[{[{2}z]}a]]";
		//StrToPaginate3 = "[{1}[{1}x]]";
		//StrToPaginate3 = "[{[{1}[{1}y]]}[{1}x]]";
		//StrToPaginate3 = "[{[{1}[{1}x][{-1}y]]}[{1}[{1}z]]]";
		//StrToPaginate3 = "[{1}[{[{[{1}x]}[{1}x]]}[{1}x]]+[{1}[{1}x]]]";
		//StrToPaginate3 = "[{1}[{1}[{1}x]][{1}[{1}[{1}x]]+[{1}[{1}y]]]]";
		//StrToPaginate3 = "[{1}[{1}[{1}x]]([{1}[{1}[{1}x]]+[{1}[{1}y]]])]";
		StrToPaginate3 = "[{1}[{1}[{1}[{1}x]]][{1}[{1}[{1}x]]+[{1}[{1}y]]]]";

		//StrToPaginate4 = "[{[{1}x]}x]";
		//StrToPaginate4 = "[{[{1}x]}[{1}x]]"
		//StrToPaginate4 = "[{[{1}[{1}x]]}[{1}[{1}x]]]";
		//StrToPaginate4 = "[{[{1}y]}[{1}[{1}x]]]";
		//StrToPaginate4 = "[{[{1}[{1}[{1}x]]+[{1}[{1}y][{-1}z]]]}[{1}[{2}x]]]";
		//StrToPaginate4 = "[{1}[{1}x][{-1}[{[{[{1}x]}[{1}x]]}[{1}x]]]]";
		StrToPaginate4 = "[{1}[{1}[{1}[{1}1]]+[{1}[{1}x]]][{1}[{1}[{1}x]]]]";

		//Initialise 
		StrToPaginate = StrToPaginate1;

		// ANSWER: Only copy once. 

		//StrToPaginate = "[[[][]]]";
		//StrToPaginate = "[[][][]]";

		//If we have the exponents at the front we can find where they are by counting forward

	
		//So the triple bracket is significant
		// [ variable ]
		// [[ term (variables multiplying) ]]
		// [[[ series (set of terms) (terms adding) ]]]

		// [[[[ product of series (series multiplying) ]]]]

		// This would imply that we might want the brackets at the left to inform 
		// paginator what is going on.

		
		//StrToPaginate = "[{1}[{1}[{2}x]]]"; //This begs the question how is this different from
		//StrToPaginate = "[[[x{2}]{1}]{1}]"	//[{1}[{2}[{1}x]]] or [{2}[{1}[{1}x]]]
		//StrToPaginate = "[x{2}]";			//Arguably in this instance there isn't a difference except
											//we could have some policy of (()^2) or (())^2 for appropriate rigour 

		
		//Consider (1+x)x -> [{1}[{1}[{1}[{1}1]]+[{1}[{1}x]]][{1}[{1}[{1}x]]]]
		//				  -> [[[[1{1}]{1}]+[[x{1}]{1}]{1}][[[x{1}]{1}]{1}]{1}]

		//Consider		(1 + x)(2 + y)     + z
		//			[[[[--------------]]]]
		//		 [[[--------------------------]]]

		//Consider		(  1  + x  )  ^   (  2  +  y  )
		//			  [[[----------]] { [[[-----------]]] } ] //This is now backwards



		//
		//	Strings to Evaluate
		//

		// So I think we want to include a vector of terms for each bracket
		// The EvaluateBracket function determines which term the contained bracket is in
		// and multiplies that term by the variable, we then sum all the terms at the end.

		// Note to self, we still have not incorporated minus

		// Also rather that bCopied we use bAggregated

		// Right so, as we allow exponentiation of variables we will allow functions of variables
		// Thus we will need to code this is terminal brackets as well as 'mother' brackets.

		//StrToEvaluate1 = "[{1}[{1}10]]";
		//StrToEvaluate1 = "[{1}[{1}50]+[{1}[{1}40][{1}20]]]";
		StrToEvaluate1 = "[{cos}[{sin}50]+[{tan}[{1}40][{1}20]]]";


		//StrToEvaluate2 = "[{1}[{1}10][{1}10]]";
		//StrToEvaluate2 = "[{exp}[{-1}10][{1}10]]";
		//StrToEvaluate2 = "[{1}[{[{1}[{1}-1]]}[{1}[{1}10]]]]";
		StrToEvaluate2 = "[{[{1}-1]}[{1}10]]";

		//StrToEvaluate3 = "[{1}[{1}[{1}10]]+[{1}[{1}10]]]";
		//StrToPaginate3 = "[{1}[{1}[{1}10]]+[{1}[{1}10]]]";
		//StrToEvaluate3 = "[{1}[{[{1}[{1}1]]}10]]";
		//StrToEvaluate3 = "[{1}[{[{1}[{1}1]]}[{1}10]]]";
		//StrToEvaluate3 = "[{1}[{[{1}[{1}1]]}[{1}[{1}10]]]]";
		//StrToEvaluate3 = "[{1}[{[{1}[{1}-1]]}[{ln}[{1}10]]]]";
		//StrToEvaluate3 = "[{ln}[{1}10]]";
		StrToEvaluate3 = "[{-1}10]";
		

		//StrToEvaluate4 = "[{1}[{-0.5}2]]";
		StrToEvaluate4 = "[{[{1}-1]}[{[{1}[{1}[{1}3]]]}[{1}[{1}[{1}5]]+[{[{1}-1]}[{1}0.1]]]]]";
		//StrToEvaluate4 = "[{[{1}[{1}-1]]}[{1}10]]";
		//StrToEvaluate4 = "[{[{1}-1]}[{1}10]]";

		StrToEvaluate = StrToEvaluate1;

		
		// Inserting data in std::map
		//mapOfFunctions.insert(std::make_pair("moon", 2));
		mapOfFunctions["exp"] = 1;
		mapOfFunctions["ln"]  = 2;
		mapOfFunctions["sin"] = 3;
		mapOfFunctions["cos"] = 4;
		mapOfFunctions["tan"] = 5;
		
		// Iterate through all elements in std::map
		//std::map<std::string, int>::iterator it = mapOfFunctions.begin();
		//while (it != mapOfFunctions.end())
		//{
		//	std::cout << it->first << " :: " << it->second << std::endl;
		//	it++;
		//}


		// Check if insertion is successful or not
		//if (mapOfWords.insert(std::make_pair("earth", 1)).second == false)
		//{
		//	std::cout << "Element with key 'earth' not inserted because already existed" << std::endl;
		//}
		// Searching element in std::map by key.
		//if (mapOfWords.find("sun") != mapOfWords.end())
		//	std::cout << "word 'sun' found" << std::endl;
		//if (mapOfWords.find("mars") == mapOfWords.end())
		//	std::cout << "word 'mars' not found" << std::endl;
		//return 0;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Toggle between Pagination and Evaluation
		if (GetKey(olc::E).bReleased)
		{
			Paginating = !Paginating;
			SomethingToDisplay == false;
			EvaluationMade == false;
			Reset();
		}

		// Display target str
		if (GetKey(olc::L).bReleased)
		{
			if (Paginating)
			{
				DisplayStrToPaginate(StrToPaginate);
			}
			else 
			{
				DisplayStrToPaginate(StrToEvaluate);
			}
		}

		// Launch Paginator
		if (GetKey(olc::K).bReleased)
		{
			if (Paginating)
			{
				Paginator1(StrToPaginate);
			}
			else
			{
				Paginator1(StrToEvaluate);
			}
		}

		//	Display intro
		if (GetKey(olc::H).bReleased)
		{
			DisplayIntro(IntroText);
		}

		//	Display pagination
		if (GetKey(olc::D).bReleased)
		{
			if (Paginating)
			{
				if (SomethingToDisplay == true)
				{
					DisplayPagination();
				}
			}
			else
			{
				if (EvaluationMade == true)
				{
					DisplayEvaluation();
				}
			}
		}

		//	Lets have a go at pausing... wow pausing....
		if (GetKey(olc::P).bReleased)
		{
			bPaused = !bPaused;
		}

		//	Reset - this will allow us to study different strings 
		if (GetKey(olc::C).bReleased)
		{
			SomethingToDisplay = false;
			EvaluationMade = false;
			Reset();
		}

		//	Swap string to paginate
		if (GetKey(olc::K1).bReleased)
		{
			SomethingToDisplay = false;
			EvaluationMade = false;
			Reset();
			if (Paginating)
			{
				StrToPaginate = StrToPaginate1;
			}
			else
			{
				StrToEvaluate = StrToEvaluate1;
			}
		}
		if (GetKey(olc::K2).bReleased)
		{
			SomethingToDisplay = false;
			EvaluationMade = false;
			Reset();
			if (Paginating)
			{
				StrToPaginate = StrToPaginate2;
			}
			else
			{
				StrToEvaluate = StrToEvaluate2;
			}
		}
		if (GetKey(olc::K3).bReleased)
		{
			SomethingToDisplay = false;
			EvaluationMade = false;
			Reset();
			if (Paginating)
			{
				StrToPaginate = StrToPaginate3;
			}
			else
			{
				StrToEvaluate = StrToEvaluate3;
			}
		}
		if (GetKey(olc::K4).bReleased)
		{
			SomethingToDisplay = false;
			EvaluationMade = false;
			Reset();
			if (Paginating)
			{
				StrToPaginate = StrToPaginate4;
			}
			else
			{
				StrToEvaluate = StrToEvaluate4;
			}
		}
		if (GetKey(olc::Z).bReleased)
		{
			ScreenOutputFlag6 = "Length ";
			for (Bracket brac : vecBrackets)
			{
				ScreenOutputFlag6 += std::to_string(brac.nId) +  " " + std::to_string(brac.nTotalLength) + " ";
			}
		}
		if (GetKey(olc::X).bReleased)
		{
			ScreenOutputFlag6 = "Height ";
			for (Bracket brac : vecBrackets)
			{
				ScreenOutputFlag6 += std::to_string(brac.nId) + " " + std::to_string(brac.nTotalHeight) + " ";
			}
		}
		if (GetKey(olc::V).bReleased)
		{
			//ScreenOutputFlag7 += std::to_string(WriteCounter);
			DisplayDiagnostics = !DisplayDiagnostics;
		}

		if (GetKey(olc::Q).bReleased)
		{
			SelectedQuestion++;
			if (SelectedQuestion >= NumQuestions)
			{
				SelectedQuestion = 0;
			}
			
		}

		if (GetKey(olc::U).bReleased)
		{
			DisplayQuestion = !DisplayQuestion;
		}
		//DisplayQuestion = true;

		if (GetKey(olc::T).bReleased)
		{
			if (DisplayValues == true)
			{
				bbTextBoxPopulate(GenerateVariables(Questions[SelectedQuestion].Tokens[1]));
			}
			else
			{
				bbTextBoxPopulate(Questions[SelectedQuestion].Tokens[1]);
			}
			DisplayValues = !DisplayValues;
		} 

		if (GetKey(olc::A).bReleased)
		{
			vecDiagnostics.clear();
		}

		//	Do stuff
		if (bPaused == false)
		{
			// Clear Screen
			Clear(olc::BLACK);


			// Draw Paginated Text
			for (sSpaceText& sText : vecPaginatedText)
			{
				sText.x += sText.dx * fElapsedTime;
				DrawString(sText.x, sText.y, sText.str, olc::WHITE, sText.nFontSize);
			}

			// Delete text after it has left screen
			if (vecPaginatedText.size() > 0)
			{
				auto i = remove_if(vecPaginatedText.begin(), vecPaginatedText.end(),
					[&](sSpaceText o) {return (o.x < -400);});
				if (i != vecPaginatedText.end())
					vecPaginatedText.erase(i, vecPaginatedText.end());
			}

			//ScreenOutputFlag4 = wtwtch;

			// Diagnostics
			DrawString(50, 50, "Number Variable Brackets " + std::to_string(nNumBrackets), olc::WHITE, 2);
			DrawString(50, 100, "Max Depth Variable Nesting " + std::to_string(nMaxDepthNesting), olc::WHITE, 2);
			//DrawString(50, 150, ScreenOutputFlag1, olc::WHITE, 2);
			//DrawString(50, 200, ScreenOutputFlag2, olc::WHITE, 2);
			//DrawString(50, 250, ScreenOutputFlag3, olc::WHITE, 2);
			//DrawString(50, 300, ScreenOutputFlag4, olc::WHITE, 2);
			//DrawString(50, 350, ScreenOutputFlag5, olc::WHITE, 2);
			//DrawString(50, 400, ScreenOutputFlag6, olc::WHITE, 2);
			//DrawString(50, 450, ScreenOutputFlag7, olc::WHITE, 2);
			
			int bbOffset = 0;
			//for (auto Question : Questions)
			//{
			//	DrawString(50, 150 + bbOffset, Question.RawField, olc::WHITE, 2);
			//	bbOffset += 50;
			//}

			//for (auto a : Questions[1].Tokens)
			//{
			//	DrawString(50, 150 + bbOffset, a, olc::WHITE, 2);
			//	bbOffset += 30;
			//}
			
			if (DisplayQuestion == true)
			{
				for (std::string& a : Questions[SelectedQuestion].Tokens)
				{
					DrawString(50, 150 + bbOffset, a, olc::WHITE, 2);
					bbOffset += 30;
				}
			}


			if(DisplayDiagnostics == true)
			{
				bbOffset = 0;
				for (std::string a : vecDiagnostics)
				{
					DrawString(600, 150 + bbOffset, a, olc::WHITE, 1);
					bbOffset += 20;
				}
			}

			//DrawString(50, 150, Questions[SelectedQuestion].QuestionID, olc::WHITE, 2);
			//DrawString(50, 180, Questions[SelectedQuestion].MetaQuestionText, olc::WHITE, 2);
			//DrawString(50, 210, Questions[SelectedQuestion].MetaQuestionMath, olc::WHITE, 2);
			//DrawString(50, 240, Questions[SelectedQuestion].CommonMistakes, olc::WHITE, 2);
			//DrawString(50, 270, Questions[SelectedQuestion].Units, olc::WHITE, 2);
			//DrawString(50, 300, Questions[SelectedQuestion].AnswerQuantity, olc::WHITE, 2);
			//DrawString(50, 330, Questions[SelectedQuestion].BestTime, olc::WHITE, 2);
			//DrawString(50, 360, Questions[SelectedQuestion].BestTimeStr, olc::WHITE, 2);
			//DrawString(50, 390, Questions[SelectedQuestion].NumberOfAttemptsRevision, olc::WHITE, 2);

			bbOffset = 0;
			for (std::string &bbLine : vecbbTextBox)
			{
				DrawString(50, 150 + bbOffset, bbLine, olc::WHITE, 1);
				bbOffset += 20;
			}
		}

		
		return true;
	}

	void Reset()
	{
		vecDiagnostics.clear();
		vecBrackets.clear();
		vecPaginatedText.clear();
		nNumBrackets = 0;
		nMaxDepthNesting = 0;
		ScreenOutputFlag1 = "";
		ScreenOutputFlag2 = "";
		ScreenOutputFlag3 = "";
		ScreenOutputFlag4 = "";
		ScreenOutputFlag5 = "";
		ScreenOutputFlag6 = "";
		ScreenOutputFlag7 = "";
		WriteCounter = 0;
	}


	void DisplayIntro(std::string IntroText)
	{
		std::stringstream SpacePass(IntroText);
		std::string Word;

		// Tokenizing w.r.t. '+'
		int nLenToken = 0;
		int nNumToken = 0;
		int AggregateLen = 0;

		while (getline(SpacePass, Word, ' '))
		{
			for (char a : Word)
				nLenToken++;

			sSpaceText MyText;
			MyText.str = Word;
			MyText.nFontSize = 5;
			MyText.x = ScreenWidth() + (8 * MyText.nFontSize * AggregateLen) + (30 * nNumToken);
			MyText.y = ScreenHeight() / 2;
			MyText.dx = -200;
			vecPaginatedText.push_back(MyText);

			nNumToken++;
			AggregateLen += nLenToken;
			nLenToken = 0;
		}
	}

	// Trial recursive function
	int bbRecursive(int num)
	{
		if (num != 1)
		{
			return num * bbRecursive(num-1);
		}
		else
		{
			return num;
		}
	}

	void DisplayPagination()
	{
		for (sSpaceText PaginatedText : vecBrackets[0].PaginatedText)
		{
			vecPaginatedText.push_back(PaginatedText);
		}
	}

	void DisplayEvaluation()
	{
		sSpaceText MyEvaluation;
		//for (auto a : vecBrackets)
		//{
		//	MyEvaluation.str += " " + std::to_string(a.fEvaluation);
		//}
		MyEvaluation.str = std::to_string(vecBrackets[0].fEvaluation);

		//MyEvaluation.str = "Well Cap'n";
		//ScreenOutputFlag5 = "Well Cap'n";
		MyEvaluation.nFontSize = 2;

		MyEvaluation.x = ScreenWidth();
		MyEvaluation.y = ScreenHeight() / 2;
		MyEvaluation.dx = -200;

		vecPaginatedText.push_back(MyEvaluation);
	}

	std::string GenerateVariables(std::string bbQuestionStr)
	{

		//vecDiagnostics.clear();
		std::string bbQuestionVariablesGenerated;
		bool InVariable = false;
		bool InLowerBound = false;
		bool InUpperBound = false;
		bool InVariableName = false;
		std::string sLowerBound;
		std::string sUpperBound;
		std::string VariableName;
		float fLowerBound;
		float fUpperBound;
		float Value;
		std::string VariableQualifier;

		for (char a : bbQuestionStr)
		{
			if (InVariable == false)
			{
				if (a == '<')
				{
					InVariable = true;
					InLowerBound = true;
				}
				else
				{
					bbQuestionVariablesGenerated += a;
				}

			}
			else
			{
				if (InLowerBound == true)
				{
					if (a != ':')
					{
						sLowerBound += a;
					}
					else
					{
						InLowerBound = false;
						InUpperBound = true;
					}
				}
				else
				{
					if (InUpperBound == true)
					{
						if (a != ':')
						{
							sUpperBound += a;
						}
						else
						{
							InUpperBound = false;
							InVariableName = true;
						}
					}
					else
					{
						//We should be in Variable Name
						if (a != '>')
						{
							VariableName += a;
						}
						else
						{
							//vecDiagnostics.push_back("Hello Captain.");
							//vecDiagnostics.push_back("LowerBound " + sLowerBound);
							VariableQualifier = sLowerBound.at(0);
							sLowerBound.erase(0,1);
							//vecDiagnostics.push_back("LowerBound " + sLowerBound);
							//vecDiagnostics.push_back("UpperBound " + sUpperBound);
							//We need to handle 'normal', integer, hidden and math variables
							if (VariableQualifier == "I")
							{
								int bbInt;
								//bbQuestionVariablesGenerated += " Integer ";
								bbInt = rand() % std::stoi(sUpperBound) + std::stoi(sLowerBound);
								//bbInt = std::stoi(sLowerBound);
								bbQuestionVariablesGenerated += " " + std::to_string(bbInt) + " ";
							}
							else
							{
								if (VariableQualifier == "H")
								{
									bbQuestionVariablesGenerated += " Hidden ";
								}
								else
								{
									if (VariableQualifier == "M")
									{
										bbQuestionVariablesGenerated += " Maths ";
									}
								
									else
									{
										bbQuestionVariablesGenerated += " 3.142 ";
									}
								}
							}
							InVariableName = false;
							InVariable = false;
							sLowerBound = "";
							sUpperBound = "";
						}
					}
				}
			}
		}
		
		return bbQuestionVariablesGenerated;
	}

	void bbTextBoxPopulate(std::string bbText)
	{
		{
			//vecDiagnostics.clear();
			vecbbTextBox.clear();
			std::string bbWord;
			std::string bbLine;
			int bbWordLength = 0;
			int bbLineLength = 0;
			bbWord = "";
			bbLine = "";


			// If there is a word larger than the width of the text box that would probably need to be handled
			for (char a : bbText)
			{
				//if (a != ' ' || a != '#')
				//if (a != ' ' ||  '#')
				//if (a != ' ')
				//if (a != (' ' || '#'))
				if (a != ' ')
				{
					if (a != '#')
					{
						bbWordLength += 8 * bbTextBoxFont;
						if (bbWordLength >= bbTextBoxWidth)
						{
							vecbbTextBox.push_back(bbWord);
							bbWord = a + " ";
							bbWordLength = 2 * 8 * bbTextBoxFont;
						}
						else
						{
							bbWord += a;
						}
					}
					else
					{
						bbLine += bbWord;
						vecbbTextBox.push_back(bbLine);
					}
				}
				else
				{
					bbLineLength += bbWordLength + 8 * bbTextBoxFont;
					//vecDiagnostics.push_back("bbLineLength " + std::to_string(bbLineLength));
					if (bbLineLength <= bbTextBoxWidth)
					{
						bbLine += bbWord + " ";
					}
					else
					{
						vecbbTextBox.push_back(bbLine);
						bbLine = bbWord + " ";
						bbLineLength = bbWordLength + 8 * bbTextBoxFont;
						//vecDiagnostics.push_back("bbLineLength " + std::to_string(bbLineLength));
					}
					bbWord = "";
					bbWordLength = 0;
				}
				//vecDiagnostics.push_back("bbLineLength " + std::to_string(bbLineLength));
			}
		}
	}

	void RefineQuestion(TitanQuestion& Question)
	{
		//std::string StrToRefineRemainder;
		//StrToRefineRemainder = RawFieldStr.substr(1);
		
		std::string Token;
		Token = "";
		int TokenNum = 0;
		for (char &a : Question.RawField)
		{
			Token += a;
			//Token = "CUNT";
			if (a == '#')
			{
				Question.Tokens.push_back(Token);
				Token = "";
				TokenNum++;
			}
		}

		ScreenOutputFlag6 = std::to_string(TokenNum);
		//Question.QuestionID = Question.Tokens[0];
		//Question.MetaQuestionText = Question.Tokens[1];
		//Question.MetaQuestionMath = Question.Tokens[2];
		//Question.CommonMistakes = Question.Tokens[3];
		//Question.AnswerQuantity = Question.Tokens[4];
		//Question.Units = Question.Tokens[5];
		//Question.BestTime = Question.Tokens[6];
		//Question.BestTimeStr = Question.Tokens[7];
		//Question.NumberOfAttemptsRevision = Question.Tokens[8];

	}

	// Another trial recursive function
	int PaginatorR(std::string StrToPaginate, int numReached) //To start let us recursively count the length of the str
	{
		std::string StrToPaginateRemainder;

		StrToPaginateRemainder = StrToPaginate.substr(1);

		//return numReached;
		if (StrToPaginateRemainder != "")
		{
			return PaginatorR(StrToPaginateRemainder, numReached + 1);
		}
		else
		{
			return numReached + 1;
		}
	}

	void Paginator1(std::string StrToPaginate)
	{
		
		nNumOpeningBrackets = 0;
		nNumClosingBrackets = 0;
		nNumBrackets = 0;
		nDepthBracketNesting = 0;
		nMaxDepthNesting = 0;
		int nPositionInStr = 0;

		//vecDiagnostics.clear();

		// Check For Consistency - actually this is more nuanced because there are other ways
		// the brackets might be inconsisten ]][[ for instance...interesting consideration: 
		// with equal numbers of brackets if you allow wrapping round of the string is there a defined
		// outcome for each bracket, one imagines there would be, how do multiple failures manifest
		// if there are unequal numbers of brackets? Anyway.

		for (char a : StrToPaginate)
		{
			if (a == '[')
			{
				Bracket bbBracket;
				bbBracket.nPosOBrac = nPositionInStr;
				bbBracket.nPosOExpBrac = nPositionInStr + 1;
				bbBracket.nDepthOfNesting = nDepthBracketNesting;
				bbBracket.nId = nNumBrackets;
				nNumOpeningBrackets++;
				nNumBrackets++;
				nDepthBracketNesting++;	//Well we have identified some things
				vecBrackets.push_back(bbBracket);
			}
			if (a == ']')
			{
				nNumClosingBrackets++;
				nDepthBracketNesting--;
			}
			if (nDepthBracketNesting > nMaxDepthNesting)
			{
				nMaxDepthNesting = nDepthBracketNesting;
			}
			nPositionInStr++;
		}

		if (nNumOpeningBrackets != nNumClosingBrackets)
		{
			ScreenOutputFlag1 = "Inconsistent in Brackets Cap'n";
			return;
		}

		// Find Closing Brackets
		for (Bracket& brac : vecBrackets)
		{
			FindClosingBracket(brac, StrToPaginate);
		}

		// Find Position Closing Exponential Bracket
		for (Bracket& brac : vecBrackets)
		{
			FindClosingExponentialBracket(brac, StrToPaginate);
		}

		//Find if new term
		int nTermCount = 0;		
		for (Bracket& brac : vecBrackets)
		{
			if (brac.nId != 0)
			{
				if (IsNewTerm(brac, StrToPaginate) == true)
				{
					brac.bNewTerm = true;
					nTermCount++;
				}
			}
		}

		//ScreenOutputFlag5 = "We have " + std::to_string(nTermCount) + " terms Cap'n";

		//Find contained Brackets 
		for (Bracket& bracOuter : vecBrackets)
		{
			// Find enclosedBrackets(brac, StrToPaginate);
			for (Bracket bracInner : vecBrackets)
			{
				if ((bracInner.nPosOBrac > bracOuter.nPosOBrac) and (bracInner.nPosOBrac < bracOuter.nPosCBrac))
					bracOuter.ContainedBrackets.push_back(bracInner.nId);
			}
		}

		ScreenOutputFlag1 = "Brac 0 contains ";
		//ScreenOutputFlag2 = "Brac 1 contains ";

		for (auto a : vecBrackets[0].ContainedBrackets)
			ScreenOutputFlag1 += std::to_string(a) + " ";
		
		ScreenOutputFlag2 = "Pagination order ";	//This is updated in PaginateExpression
		

		// Well this is some spaghetti code
		if(Paginating)
		{ 
			PaginateExpression(vecBrackets[0], StrToPaginate);
			SomethingToDisplay = true;
		}
		else
		{
			EvaluateExpression(vecBrackets[0], StrToPaginate);
			EvaluationMade = true;
			ScreenOutputFlag2 = std::to_string(vecBrackets[0].fEvaluation);
		}

		//Also we need to perform a check on the bracket to see if it is a new term - look left for a '+'.

		// Allow display as there is something to display
		
	}

	void FindClosingBracket(Bracket& ConsideredBracket, std::string StrToPaginate)
	{
		std::string StrToPaginateRemainder;

		StrToPaginateRemainder = StrToPaginate.substr(ConsideredBracket.nPosOBrac);

		nDepthBracketNesting = 0;
		int nPositionInStr = 0;
		// So when we hit a closing brackt and our depth of nesting is zero we have found closing bracket
		for (char a : StrToPaginateRemainder)
		{
			if (a == '[')
			{
				nDepthBracketNesting++;
			}
			if (a == ']')
			{
				nDepthBracketNesting--;
				if (nDepthBracketNesting == 0)
				{
					ConsideredBracket.nPosCBrac = ConsideredBracket.nPosOBrac + nPositionInStr;
					return;
				}
			}
			nPositionInStr++;
		}
	}

	void FindClosingExponentialBracket(Bracket& ConsideredBracket, std::string StrToPaginate)
	{
		std::string StrToPaginateRemainder;

		StrToPaginateRemainder = StrToPaginate.substr(ConsideredBracket.nPosOBrac);

		nDepthBracketNesting = 0;
		int nPositionInStr = 0;
		// So when we hit a closing brackt and our depth of nesting is zero we have found closing bracket
		for (char a : StrToPaginateRemainder)
		{
			if (a == '{')
			{
				nDepthBracketNesting++;
			}
			if (a == '}')
			{
				nDepthBracketNesting--;
				if (nDepthBracketNesting == 0)
				{
					ConsideredBracket.nPosCExpBrac = ConsideredBracket.nPosOBrac + nPositionInStr;
					return;
				}
			}
			nPositionInStr++;
		}
	}
	
	bool IsNewTerm(Bracket& brac, std::string StrToPaginate)
	{
		if (StrToPaginate[brac.nPosOBrac - 1] == '+')
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// We want functions to check if a First Terminal Bracket exists - here we are relying on their being one
	// we also want to check if that bracket is paginated.
	//int FindFirstTerminalBracket(Bracket& brac)
	//{
	//	if(brac.ContainedBrackets.empty() == true)
	//	{
	//		return brac.nId;
	//	}
	//	else
	//	{
	//		return FindFirstTerminalBracket(vecBrackets[brac.ContainedBrackets[0]]);
	//	}
	//}

	//bool  IsBracketInExponential()
	//{
	//
	//	return false;
	//}

	void PaginateBracket(Bracket& brac, std::string StrToPaginate)
	{
		//std::string BracketStr;
		std::string VarString;
		std::string ExponentStr;
		int nLenExpStr;
		int nLenVarStr;

		//Arguably this should be inside the if clause below
		nLenExpStr = brac.nPosCExpBrac - (brac.nPosOExpBrac + 1);
		brac.sExponentialString = StrToPaginate.substr(brac.nPosOExpBrac + 1, nLenExpStr);

		//
		//	Case of a terminal bracket
		//
		if (brac.ContainedBrackets.empty() == true)
		{
			//
			//	Paginate Variable
			//

			nLenVarStr = brac.nPosCBrac - (brac.nPosCExpBrac + 1);
			VarString = StrToPaginate.substr(brac.nPosCExpBrac + 1, nLenVarStr);

			sSpaceText MyVar;
			MyVar.str = VarString;
			MyVar.nFontSize = 2;

			MyVar.x = ScreenWidth();
			MyVar.y = ScreenHeight() / 2;
			MyVar.dx = -200;
			brac.PaginatedText.push_back(MyVar);
			brac.nAggLenAboveLine += (8 * MyVar.nFontSize * nLenVarStr);
			brac.nTotalHeight = 8 * MyVar.nFontSize;
			//
			//
			//	Paginate Exponent
			//

			ExponentStr = brac.sExponentialString;

			// Case of special exponents
			if (ExponentStr != "1")
			{
				if (ExponentStr != "-1")
				{
					sSpaceText MyExp;
					MyExp.str = ExponentStr;
					MyExp.nFontSize = 2;

					MyExp.x = ScreenWidth() + brac.nAggLenAboveLine;
					MyExp.y = (ScreenHeight() / 2) - (brac.nTotalHeight / 2);
					MyExp.dx = -200;
					brac.PaginatedText.push_back(MyExp);
					brac.nAggLenAboveLine += (8 * MyVar.nFontSize * nLenExpStr);
					brac.nTotalHeight += (brac.nTotalHeight / 2);
				}
			}
			brac.nTotalLength = brac.nAggLenAboveLine + 16;	// This should be the case here, will be different for
														// non-terminal brackets
		}

		//
		//	Handle internal paginated brackets
		//
	
		for (int bracId : brac.ContainedBrackets)
		{
			if (vecBrackets[bracId].bCopied == false)
			{
				if (vecBrackets[bracId].nTotalHeight > brac.nTotalHeight)
				{
					brac.nTotalHeight = vecBrackets[bracId].nTotalHeight;
				}

				// Is bracket in Exponent?
				if (vecBrackets[bracId].nPosCBrac < brac.nPosCExpBrac)
				{
					//
					// Pre-Paginate Exponent
					//

					if (vecBrackets[bracId].bNewTerm == true)
					{
						//	Add a plus - ah, at this point we don't know how big the Term is 
						sSpaceText Plus;
						Plus.str = "+";
						Plus.nFontSize = 2;
						Plus.x = ScreenWidth() + brac.nAggLenAboveLineExp;
						Plus.y = (ScreenHeight() / 2); // -vecBrackets[bracId].nTotalHeight / 2;
						Plus.dx = -200;
						brac.PrePaginatedExp.push_back(Plus);
						brac.nAggLenAboveLineExp += 30;
						brac.nAggLenBelowLineExp += 30;
					}

					if (vecBrackets[bracId].sExponentialString != "-1")
					{
						for (sSpaceText PrePaginated : vecBrackets[bracId].PaginatedText)
							{
								PrePaginated.x += brac.nAggLenAboveLineExp;
								brac.PrePaginatedExp.push_back(PrePaginated);
							}
						brac.nAggLenAboveLineExp += vecBrackets[bracId].nTotalLength;
					}
					else
					{
						for (sSpaceText PrePaginated : vecBrackets[bracId].PaginatedText)
						{
							PrePaginated.x += brac.nAggLenBelowLineExp;
							PrePaginated.y += brac.nTotalHeight;
							brac.PrePaginatedExp.push_back(PrePaginated);
						}
						brac.nAggLenBelowLineExp += vecBrackets[bracId].nTotalLength;
					}
				}
				else
				//
				// PrePaginate 'Variable' -- this could actually be a variable or a series of terms.
				//
				{
					if (vecBrackets[bracId].bNewTerm == true)
					{
						//	Add a plus
						sSpaceText Plus;
						Plus.str = "+";
						Plus.nFontSize = 2;
						Plus.x = ScreenWidth() + brac.nAggLenAboveLine;
						Plus.y = (ScreenHeight() / 2); //+ vecBrackets[bracId].nTotalHeight / 2;
						Plus.dx = -200;
						brac.PaginatedText.push_back(Plus);
						brac.nAggLenAboveLine += 30;
						brac.nAggLenBelowLine += 30;
					}

					if (vecBrackets[bracId].sExponentialString != "-1")
					{
						for (sSpaceText PrePaginated : vecBrackets[bracId].PaginatedText)
						{
							PrePaginated.x += brac.nAggLenAboveLine;
							brac.PrePaginatedVar.push_back(PrePaginated);
						}
						brac.nAggLenAboveLine += vecBrackets[bracId].nTotalLength;
					}
					else
					{
						brac.bBracIsSurd = true;
						for (sSpaceText PrePaginated : vecBrackets[bracId].PaginatedText)
						{
							PrePaginated.y += brac.nTotalHeight;
							PrePaginated.x += brac.nAggLenBelowLine;
							brac.PrePaginatedVar.push_back(PrePaginated);
						}
						brac.nAggLenBelowLine += vecBrackets[bracId].nTotalLength;
						brac.nTotalHeight += brac.nTotalHeight + 8;
					}
				}
		
				if (brac.nAggLenAboveLine > brac.nAggLenBelowLine)
				{
					brac.nTotalLength = brac.nAggLenAboveLine;
				}
				else
				{
					brac.nTotalLength = brac.nAggLenBelowLine;
				}

				vecBrackets[bracId].bCopied = true;
			}

		}

		//
		// Paginate variable
		//
		if (brac.bBracIsSurd == false)
		{
			for (sSpaceText PrePaginated : brac.PrePaginatedVar)
			{
				brac.PaginatedText.push_back(PrePaginated);
			}
		}
		else
		{
			for (sSpaceText PrePaginated : brac.PrePaginatedVar)
			{
				PrePaginated.y -= brac.nTotalHeight / 4;
				brac.PaginatedText.push_back(PrePaginated);
			}
		}

		//
		// Paginate Exponent
		//
		for (sSpaceText PrePaginated : brac.PrePaginatedExp)
		{
			WriteCounter++;
			PrePaginated.x += brac.nTotalLength;
			PrePaginated.y -= brac.nTotalHeight / 2;
			brac.nTotalHeight += brac.nTotalHeight / 2;
			brac.PaginatedText.push_back(PrePaginated);
		}

		if (brac.nAggLenAboveLineExp > brac.nAggLenBelowLineExp)
		{
			brac.nTotalLength += brac.nAggLenAboveLineExp;
		}
		else
		{
			brac.nTotalLength += brac.nAggLenBelowLineExp;
		}
	}



	bool PaginateExpression(Bracket& brac, std::string StrToPaginate)
	{
	//ALGORITHM
	// So we call this function, it checks to see if there are any unpaginated brackets inside the bracket
	// If there are it calls itself with the contained bracket; it then calls itself with the original bracket;
	// It returns when no unpaginated brackets remain in the bracket.

	// When considering expressions in exponent we would like to see the order of pagination

		LoopCount++;

		bool ContainedBracketsExhausted = true;

		if (brac.ContainedBrackets.empty() == true)
		{
			//ScreenOutputFlag3 = "Got Here";
			ContainedBracketsExhausted = true;
		}
		else
		{
			for (auto& a : brac.ContainedBrackets)
			{
				
				if (vecBrackets[a].bPaginated == false)
				{
					ContainedBracketsExhausted = false;
				}
			}
		}

		if (ContainedBracketsExhausted == true)
		{
			PaginateBracket(brac, StrToPaginate);
			brac.bPaginated = true;
			ScreenOutputFlag2 += " " + std::to_string(brac.nId);
			return true;
		}
		else
		{
			for (auto& a : brac.ContainedBrackets)
			{
				if (vecBrackets[a].bPaginated == false)
				{
					PaginateExpression(vecBrackets[a], StrToPaginate);
				}
			}
			PaginateExpression(brac, StrToPaginate);
		}
	}

	void EvaluateBracket(Bracket& brac, std::string StrToEvaluate)
	{
		std::string VarString;
		std::string ExponentStr;
		int nLenExpStr;
		int nLenVarStr;

		//Arguably this should be inside the if clause below
		nLenExpStr = brac.nPosCExpBrac - (brac.nPosOExpBrac + 1);
		brac.sExponentialString = StrToEvaluate.substr(brac.nPosOExpBrac + 1, nLenExpStr);

		nLenVarStr = brac.nPosCBrac - (brac.nPosCExpBrac + 1);
		VarString = StrToEvaluate.substr(brac.nPosCExpBrac + 1, nLenVarStr);

		//
		//	Case of a terminal bracket
		//
		brac.fEvaluation = 0;
		brac.fExponentEvaluation = 0;
		brac.fVariableEvaluation = 0;
		brac.TermsInExponent.clear();
		brac.TermsInVariable.clear();

		//vecDiagnostics.push_back(std::to_string(brac.nId));

		if (brac.ContainedBrackets.empty() == true)
		{
			// isdigit() checks if char is 0-9
			// atoi() takes all digits from string and also a starting + or - and casts to int
			brac.fVariableEvaluation = std::stof(VarString);
			
			if (brac.sExponentialString[0] == '-')
			{
				vecDiagnostics.push_back("Got minus Cap'n");
				brac.fExponentEvaluation = std::stof(brac.sExponentialString);
			}
			else
			{
				if (isdigit(brac.sExponentialString[0]))
				{
					vecDiagnostics.push_back("Got digit Cap'n");
					brac.fExponentEvaluation = std::stof(brac.sExponentialString);
				}
			}
			PerformFunction(brac);
		}
		else
		{
			//
			//	Handle internal paginated brackets
			//

			int ExpTermCounter = 0;
			int VarTermCounter = 0;

			brac.TermsInExponent.push_back(1);
			brac.TermsInVariable.push_back(1);

			for (int bracId : brac.ContainedBrackets)
			{

				if (vecBrackets[bracId].bCopied == false)
				{

					//
					//	Brackets in exponent
					//

					if (vecBrackets[bracId].nPosCBrac < brac.nPosCExpBrac)
					{
						//ScreenOutputFlag3 = " We have a bracket in the exponent";
						//int TermCounter = 0;
						//vecBrackets[bracId].Terms.push_back(1);
						//if (vecBrackets[bracId].bCopied == false)
						//{
							//brac.TermsInExponent.push_back(1);
						if (vecBrackets[bracId].bNewTerm == false)
						{
							brac.TermsInExponent[ExpTermCounter] = brac.TermsInExponent[ExpTermCounter] * vecBrackets[bracId].fEvaluation;
						}
						else
						{
							brac.TermsInExponent.push_back(1);
							ExpTermCounter++;
							brac.TermsInExponent[ExpTermCounter] = brac.TermsInExponent[ExpTermCounter] * vecBrackets[bracId].fEvaluation;
						}
						vecBrackets[bracId].bCopied = true;
					//}
					}
					else
					{

						//if (vecBrackets[bracId].bCopied == false)
						//{
							//brac.TermsInVariable.push_back(1);
							//int TermCounter = 0;
						if (vecBrackets[bracId].bNewTerm == false)
						{
							brac.TermsInVariable[VarTermCounter] = brac.TermsInVariable[VarTermCounter] * vecBrackets[bracId].fEvaluation;
						}
						else
						{
							brac.TermsInVariable.push_back(1);
							VarTermCounter++;
							brac.TermsInVariable[VarTermCounter] = brac.TermsInVariable[VarTermCounter] * vecBrackets[bracId].fEvaluation;
						}
						vecBrackets[bracId].bCopied = true;
						
					//}
					}
				}
				//int CheckCounter = 0;
			}

			brac.fEvaluation = 0;
			brac.fExponentEvaluation = 0;
			brac.fVariableEvaluation = 0;

			for (auto a : brac.TermsInExponent)
			{
				brac.fExponentEvaluation = brac.fExponentEvaluation + a;
				if (brac.nId == 0)
				{
					//vecDiagnostics.push_back(" Exp a = " + std::to_string(a));
				}
				//CheckCounter++;
				//ScreenOutputFlag4 = " " + std::to_string(brac.fExponentEvaluation);
				//ScreenOutputFlag5 = " " + std::to_string(a);
			}

			//brac.TermsInExponent.clear();

			for (auto a : brac.TermsInVariable)
			{
				brac.fVariableEvaluation = brac.fVariableEvaluation + a;
				//vecDiagnostics.push_back(" Var a = " + std::to_string(a));
			}

			//brac.TermsInVariable.clear();
			brac.TermsInExponent.clear();
			brac.TermsInVariable.clear();


			if (brac.fExponentEvaluation == 0)
			{
				// Handle Exponent
				brac.fExponentEvaluation = std::stof(brac.sExponentialString);
				//vecDiagnostics.push_back(std::to_string(brac.nId) + brac.sExponentialString);
			}

			//if (brac.fVariableEvaluation == 1)
			//{
				// Handle Exponent
			//	brac.fVariableEvaluation = std::stof(brac.s);
			//} 


			if (brac.nId == 0)
			{
				vecDiagnostics.push_back(" Exp a = " + std::to_string(brac.fExponentEvaluation));
				vecDiagnostics.push_back(" Var a = " + std::to_string(brac.fVariableEvaluation));
			}


			PerformFunction(brac);
		// isdigit() checks if char is 0-9
		// atoi() takes all digits from string and also a starting + or - and casts to int

			
			//vecDiagnostics.push_back(" Exp " + std::to_string(brac.fExponentEvaluation));
			//vecDiagnostics.push_back(" Var " + std::to_string(brac.fVariableEvaluation));
			//vecDiagnostics.push_back(" Eval " + std::to_string(brac.fEvaluation));
			//ScreenOutputFlag4 = " " + std::to_string(brac.fExponentEvaluation);
			//ScreenOutputFlag5 = " " + std::to_string(brac.fVariableEvaluation);

			//ScreenOutputFlag5 = std::to_string(CheckCounter);
		}

	}

	void PerformFunction(Bracket& brac)
	{
		
		//vecDiagnostics.push_back(brac.sExponentialString);
		//if (brac.sExponentialString[0] == '-')
		//{
		//
		//	vecDiagnostics.push_back("Got minus Cap'n");
		//	if (brac.nId == 0)
		//	{
		//		vecDiagnostics.push_back("Got here Cap'n");
		//	}
		//	brac.fEvaluation = pow(brac.fVariableEvaluation, brac.fExponentEvaluation);
		//}
		//else
		//{ 
		//	if (isdigit(brac.sExponentialString[0]))
		//	{
		//		brac.fEvaluation = pow(brac.fVariableEvaluation, brac.fExponentEvaluation);
		//	}
		//	else
		//	{
				switch (mapOfFunctions[brac.sExponentialString]) {
				case 1:
					brac.fEvaluation = exp(brac.fVariableEvaluation);
					break; //optional
				case 2:
					brac.fEvaluation = log(brac.fVariableEvaluation);
					break; //optional
				case 3:
					brac.fEvaluation = sin(brac.fVariableEvaluation);
					break; //optional
				case 4:
					brac.fEvaluation = cos(brac.fVariableEvaluation);
					break; //optional
				case 5:
					brac.fEvaluation = tan(brac.fVariableEvaluation);
					break; //optional
				 // you can have any number of case statements.
				default: //Optional
					brac.fEvaluation = pow(brac.fVariableEvaluation, brac.fExponentEvaluation);
				}
		//	}
		//}
		

		//if (brac.sExponentialString == "sin")
		//{
		//	brac.fEvaluation = sin(brac.fVariableEvaluation);
		//}
		//else
		//{
		//	brac.fEvaluation = pow(brac.fVariableEvaluation, brac.fExponentEvaluation);
		//}
		
	}



	bool EvaluateExpression(Bracket& brac, std::string StrToEvaluate)
	{
		LoopCount++;

		bool ContainedBracketsExhausted = true;

		if (brac.ContainedBrackets.empty() == true)
		{
			//ScreenOutputFlag3 = "Got Here";
			ContainedBracketsExhausted = true;
		}
		else
		{
			for (auto& a : brac.ContainedBrackets)
			{

				if (vecBrackets[a].bEvaluated == false)
				{
					ContainedBracketsExhausted = false;
				}
			}
		}

		if (ContainedBracketsExhausted == true)
		{
			EvaluateBracket(brac, StrToEvaluate);
			brac.bEvaluated = true;
			ScreenOutputFlag2 += " " + std::to_string(brac.nId);
			return true;
		}
		else
		{
			for (auto& a : brac.ContainedBrackets)
			{
				if (vecBrackets[a].bEvaluated == false)
				{
					EvaluateExpression(vecBrackets[a], StrToEvaluate);
				}
			}
			EvaluateExpression(brac, StrToEvaluate);
		}
	}


	void DisplayStrToPaginate(std::string StrToPaginate)
	{
		// stringstream class check1 
		std::stringstream PlusPass(StrToPaginate);
		
		std::string intermediate;
		nNumBrackets = 0;
		nDepthBracketNesting = 0;
		nMaxDepthNesting = 0;

		// Tokenizing w.r.t. '+'
		int nLenToken = 0;
		int nNumToken = 0;
		int AggregateLen = 0;

		while (getline(PlusPass, intermediate, '+'))
		{
			for (char a : intermediate)
				nLenToken++;

			//Right so we are using font size 2 - so that should be 16 pixels
			sSpaceText MyText;
			MyText.str = intermediate;
			MyText.nFontSize = 2;
			MyText.x = ScreenWidth() + (8 * MyText.nFontSize * AggregateLen) + (100 * nNumToken);
			MyText.y = (ScreenHeight() / 2) + 150;
			MyText.dx = -200;
			vecPaginatedText.push_back(MyText);
			 
			sSpaceText MyFormatting;
			MyFormatting.str = std::to_string(nLenToken);
			MyFormatting.nFontSize = 2;
			MyFormatting.x = ScreenWidth() + (8 * MyFormatting.nFontSize * AggregateLen) + (100 * nNumToken);
			MyFormatting.y = (ScreenHeight() / 2) + 200;
			MyFormatting.dx = -200;
			vecPaginatedText.push_back(MyFormatting);

			nNumToken++;
			AggregateLen += nLenToken;
			nLenToken = 0;
		}
	}
};


int main()
{
	bbPaginator demo;
	if (demo.Construct(1200, 640, 1, 1))
		demo.Start();
	return 0;
}