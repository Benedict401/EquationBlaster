// So the plan is players collect terms in the exp expansion to approximate answers
// We could use the same dynamic to scoop us cos x as opposed to sin x.

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class bbPaginator : public olc::PixelGameEngine
{
public:
	bbPaginator()
	{
		sAppName = "Paginator";
	}

private:

	struct sSpaceText
	{
		float x;
		float y;
		float dx;
		float dy;
		int nFontSize;
		std::string str;
	};

	//struct sContainedBracket
	//{
	//
	//};

	struct Bracket
	{
		// We are calling them Brackets because they are more general and various than just 'variable' brackets
		int nId;
		int nPositionOpeningBracket;
		int nPositionClosingBracket;
		int nPositionOfOpeningExponentialBracket;
		int nPositionOfClosingExponentialBracket;
		int nDepthOfNesting;	//Exponent brackets will always be nested one deeper
		bool bNewTerm = false;
		int AggregateLengthAboveLine = 0;
		int AggregateLengthBelowLine = 0;
		std::string sExponentialString; //Special cases {1} do nothing {-1} place 'under line'
		std::vector<sSpaceText> PaginatedText;
		std::vector<int> ContainedBrackets;
		bool bPaginated = false;
		bool bCopied = false;
	};							//At this point we don't know exactly how to handle nesting

	struct ExponentBracket
	{
		int nId;
		int nPositionOpeningBracket;
		int nPositionClosingBracket;
		int nDepthOfNesting;
	};


	std::string ScreenOutputFlag1 = "";
	std::string ScreenOutputFlag2 = "";
	std::string ScreenOutputFlag3 = "";
	std::string ScreenOutputFlag4 = "";
	std::string ScreenOutputFlag5 = "";

	bool SomethingToDisplay = false;
	bool bPaused;
	//OK some backwards engineering!
	//Well that didn't work, it is something to do with the non standard function keys on my laptop.
	//boolean KeyFound = false;
	int nNumOpeningBrackets;
	int nNumClosingBrackets;
	int nNumBrackets;
	int nDepthBracketNesting; //Maybe depth of nesting cane be counted purely from variable brackets
	int nMaxDepthNesting;
	int LoopCount;
	int AggregateLen;		//We will use an integer for now

	std::string StrToPaginate;
	std::string IntroText;

	std::vector<Bracket> vecBrackets; 
	std::vector<sSpaceText> vecPaginatedText;
	std::vector<std::string> vecInputStr;

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		//Right here we are not looking for an ultimately recursive general function
		//We wish to expound on the properties of the exp function
		// Exp : is a function such that the product of two instances of the function
		// governed by different variables is the same as one instance of the function
		// goverened by those two variables summed.

		//One Weber is that flux that linking a circuit of one turn produces an emf of
		// one volt if wachsen or schwinden in ein second.

		bPaused = false;

		IntroText = "Welcome Cadet. To escape the Universe we must find it's Null Space.";
		
		//StrToPaginate = "[[[1{1}]{1}]+[[x{1}]{1}]+[[x{2}][2!{-1}]{1}]+[[x{3}][3!{-1}]{1}]+[[x{4}][4!{-1}]{1}]{1}]";
		
		StrToPaginate = "[{1}[{1}[{1}1]]+[{1}[{1}x]]+[{1}[{2}x][{-1}2!]]+[{1}[{3}x][{-1}3!]]+[{1}[{4}x][{-1}4!]]]";

		//StrToPaginate = "[{1}[{1}1]+[{1}x]+[{2}x][{-1}2!]+[{3}x][{-1}3!]+[{4}x][{-1}4!]]";

		//StrToPaginate = "[{1}[{1}[{1}[{1}[{1}[{1}x]]]]]]"; //We want some way of counting two layers
		// We want to search to see mutual enclosed brackets. ANSWER: Only copy once. 

		//StrToPaginate = "[x{2}]";

		//StrToPaginate = "[[x{2}][y{3}]{2}]";
	
		//StrToPaginate = "[[[][]]]";
		//StrToPaginate = "[[][][]]";

		// So this is a [[[ ]]] series

		// More brackets needed 
		//				"+[x{5}][5!{-1}]+[x{6}][6!{-1}]+[x{7}][7!{-1}]"
		//				"+[x{8}][8!{-1}]+[x{9}][9!{-1}]+[x{10}][10!{-1}]";

		//StrToPaginate = "[1{1}]+[ln(2){1}][x{1}]+[ln(2){2}][x{2}][2!{-1}]"
		//				"+[ln(2){3}}[x{3}][3!{-1}]+[ln(2){4}][x{4}][4!{-1}]"
		//				"+[ln(2){5}}[x{5}][5!{-1}]+[ln(2){6}][x{6}][6!{-1}]"
		//				"+[ln(2){7}}[x{7}][7!{-1}]+[ln(2){8}][x{8}][8!{-1}]"
		//				"+[ln(2){9}}[x{9}][9!{-1}]+[ln(2){10}][x{10}][10!{-1}]";

		//StrToPaginate = "[[[1{1}]{1}]+[[1{1}][[[1{1}]{1}]+[[1{1}][x{-1}]{1}]{-1}]{1}]{1}]";
	
		//Start simple physics boy
		//StrToPaginate = "[[[1{1}]{1}] + [[x{1}]{1}]{1}]"
		// or

		//StrToPaginate = "[{1}[{1}[{1}1]]+[{1}[{1}x]]]";

		//If we have the exponents at the front we can find where they are by counting forward

		//consider
		//StrToPaginate = "[{[{1}[{1}[{1}x]]]}[{1}[{1}[{1}1]] + [{1}[{1}x]]]" // 

		//So the triple bracket is significant
		// [ variable ]
		// [[ term (variables multiplying) ]]
		// [[[ series (set of terms) (terms adding) ]]]

		// [[[[ product of series (series multiplying) ]]]]

		// This would imply that we would want the brackets at the left to inform 
		// paginator what is going on.

		//Simpler physics boy
		//StrToPaginate = "[{1}[{1}[{2}x]]]"; //This begs the question how is this different from
		//StrToPaginate = "[[[x{2}]{1}]{1}]"	//[{1}[{2}[{1}x]]] or [{2}[{1}[{1}x]]]
		//StrToPaginate = "[x{2}]";			//Arguably in this instance there isn't a difference except
											//we could have some policy of (()^2) or (())^2 for appropriate rigour 

		//StrToPaginate = "[A{2}]";

		//Consider (1+x)x -> [{1}[{1}[{1}[{1}1]]+[{1}[{1}x]]][{1}[{1}[{1}x]]]]
		//				  -> [[[[1{1}]{1}]+[[x{1}]{1}]{1}][[[x{1}]{1}]{1}]{1}]

		//Consider		(1 + x)(2 + y)     + z
		//			[[[[--------------]]]]
		//		 [[[--------------------------]]]

		//Consider		(  1  + x  )  ^   (  2  +  y  )
		//			  [[[----------]] { [[[-----------]]] } ]



		//for (int i = 0; i < 10; i++)
		///	vecInputStr.push_back("K" + std::to_string(i));

		//std::string line = "GeeksForGeeks is a must try";

		// Vector of string to save tokens 
		//vector <string> tokens;


		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		//for (int x = 0; x < ScreenWidth(); x++)
		//	for (int y = 0; y < ScreenHeight(); y++)
		//		Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand() % 255));

		
		//Launch Pagination
		if (GetKey(olc::L).bReleased)
		{
			//LaunchPagination(StrToPaginate);
			LaunchPagination(StrToPaginate);
		}

		if (GetKey(olc::K).bReleased)
		{
			Paginator1(StrToPaginate);
		}

		if (GetKey(olc::J).bReleased)
		{
			LaunchPagination2(StrToPaginate);
		}

		if (GetKey(olc::H).bReleased)
		{
			DisplayIntro(IntroText);
		}

		if (GetKey(olc::R).bReleased)
		{
			ScreenOutputFlag2 = std::to_string(bbRecursive(5));
		}

		if (GetKey(olc::T).bReleased)
		{
			ScreenOutputFlag2 = std::to_string(PaginatorR(StrToPaginate, 0));
		}

		if (GetKey(olc::D).bReleased)
		{
			if (SomethingToDisplay == true)
			{
				DisplayPagination();
			}
		}

		//Lets have a go at pausing... wow pausing....
		if (GetKey(olc::P).bReleased)
		{
			bPaused = !bPaused;
		}

		//Investigating user Input - this olc::SHIFT is just the number 55. Hope!
		//if (GetKey(olc::SHIFT).bHeld)
		//	if(GetKey(olc::B).bReleased)
		//if (GetKey(olc::173).bReleased);

		//if (GetKey(olc::LEFT_SQUARE_BRACKET).bReleased)
		//	KeyFound = true;

		//if (GetKey(olc::HASH).bReleased)
		//	KeyFound = true;

		//if (GetKey(olc::C).bReleased)
		//	KeyFound = false;

		if (bPaused == false)
		{
			// Clear Screen
			Clear(olc::BLACK);

			//if (KeyFound == true)
			//	Clear(olc::BLUE);

			//Draw Paginated Text
			for (sSpaceText& sText : vecPaginatedText)
			{
				sText.x += sText.dx * fElapsedTime;
				DrawString(sText.x, sText.y, sText.str, olc::WHITE, sText.nFontSize);
			}

			//Delete text after it has scrolled across screen
			if (vecPaginatedText.size() > 0)
			{
				auto i = remove_if(vecPaginatedText.begin(), vecPaginatedText.end(),
					[&](sSpaceText o) {return (o.x < -400);});
				if (i != vecPaginatedText.end())
					vecPaginatedText.erase(i, vecPaginatedText.end());
			}

			//Draw Pagination Parameters
			DrawString(50, 50, "Number Variable Brackets " + std::to_string(nNumBrackets), olc::WHITE, 2);
			DrawString(50, 100, "Max Depth Variable Nesting " + std::to_string(nMaxDepthNesting), olc::WHITE, 2);
			DrawString(50, 150, ScreenOutputFlag1, olc::WHITE, 2);
			DrawString(50, 200, ScreenOutputFlag2, olc::WHITE, 2);
			DrawString(50, 250, ScreenOutputFlag3, olc::WHITE, 2);
			DrawString(50, 300, ScreenOutputFlag4, olc::WHITE, 2);
			DrawString(50, 350, ScreenOutputFlag5, olc::WHITE, 2);
		}


		return true;
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


			//Right so we are using font size 2 - so that should be 16 pixels
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

	//	Right as we are presently flying blind let us have a display function
	void DisplayPagination()
	{
		//for (Bracket brac : vecBrackets)
		//{
		//	for (sSpaceText PaginatedText : brac.PaginatedText)
		//	{
		//		vecPaginatedText.push_back(PaginatedText);
		//	}
		//}
		//if (vecBrackets[0].PaginatedText.empty() == false)
		//{
			for (sSpaceText PaginatedText : vecBrackets[0].PaginatedText)
			{
				vecPaginatedText.push_back(PaginatedText);
			}
		//}
	}

	// Right the overall paginate function is going to call itself while there is string left
	// Now we want to find all the closing brackets.


	int PaginatorR(std::string StrToPaginate, int numReached) //To start let us recursively count the length of the str
	{
		std::string StrToPaginateRemainder;

		StrToPaginateRemainder = StrToPaginate.substr(1);

		//if (numReached == 0)
		//	ScreenOutputFlag3 = StrToPaginateRemainder;

		//if (numReached == 1)
		//	ScreenOutputFlag4 = StrToPaginateRemainder;

		//if (numReached == 2)
		//	ScreenOutputFlag5 = StrToPaginateRemainder;


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
		//std::string intermediate;
		//std::string Parsed;	//as s stands for struct we might just leave str identified by lack of identifier

		nNumOpeningBrackets = 0;
		nNumClosingBrackets = 0;
		nNumBrackets = 0;
		nDepthBracketNesting = 0;
		nMaxDepthNesting = 0;
		int nPositionInStr = 0;

		//std::string bbVariableItIsAnX;
		//std::string bbPowerItIsA2;


		//Parsed = "";

		//Check For Consistency - actually this is more nuanced because there are other ways
		//The brackets might be inconsisten ]][[ for instance...interesting consideration: 
		//with equal numbers of brackets if you allow wrapping round of the string is there a defined
		//outcome for each bracket, one immagines there would be, how do multiple failures manifest
		//if there are unequal numbers of brackets? Anyway.

		for (char a : StrToPaginate)
		{
			if (a == '[')
			{
				Bracket bbBracket;

				bbBracket.nPositionOpeningBracket = nPositionInStr;
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

		//Find Closing Brackets
		for (Bracket& brac : vecBrackets)
		{
			FindClosingBracket(brac, StrToPaginate);
		}

		//Find if new term
		int nTermCount = 0;		// This is just to check it works
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

		ScreenOutputFlag5 = "We have " + std::to_string(nTermCount) + " terms Cap'n";

		//Find contained Brackets - arguably this cound be done as part of finding closing bracket
		for (Bracket& bracOuter : vecBrackets)
		{
			//FindEnclosedBrackets(brac, StrToPaginate);
			for (Bracket bracInner : vecBrackets)
			{
				if ((bracInner.nPositionOpeningBracket > bracOuter.nPositionOpeningBracket) and (bracInner.nPositionOpeningBracket < bracOuter.nPositionClosingBracket))
					bracOuter.ContainedBrackets.push_back(bracInner.nId);
			}
		}

		ScreenOutputFlag1 = "Brac 0 contains ";
		//ScreenOutputFlag2 = "Brac 1 contains ";

		for (auto a : vecBrackets[0].ContainedBrackets)
			ScreenOutputFlag1 += std::to_string(a) + " ";
		
		//for (auto a : vecBrackets[1].ContainedBrackets)
		//	ScreenOutputFlag2 += std::to_string(a) + " ";

		//Right now we want to go to the first bracket; then go to the first bracket it contains recursively
		//If the bracket does not contain anything we want to paginate it and return to the next bracket up

		//ScreenOutputFlag3 = std::to_string(FindFirstTerminalBracket(vecBrackets[0]));
		//PaginateBracket(vecBrackets[FindFirstTerminalBracket(vecBrackets[0])], StrToPaginate);

		PaginateExpression(vecBrackets[0], StrToPaginate);
#
		ScreenOutputFlag2 = "Loop count " + std::to_string(LoopCount);

		//Here is a question, will everything be enclosed in an World/Expression Bracket? Might make it simpler.
		//Then vecBrackets[0] would be the Expression Bracket.

		//Also we need to perform a check on the bracket to see if it is a new term - look left for a '+'.


		// 
		SomethingToDisplay = true;



		//for (Bracket& brac: vecBrackets)
		//{
		//	ScreenOutputFlag1 += "Brac " + std::to_string(brac.nId) + " at "
		//		+ std::to_string(brac.nPositionOpeningBracket) + " to " 
		//		+ std::to_string(brac.nPositionClosingBracket) + " ";
		//}

		//Rightso; the aim of the game here is to add brackets to vecBrackets
		//What we wish to do is to find the closing bracket for every opening bracket
		//This looks like it would be a factorial time problem?

		//Just get the x and the 2 and paginate them bellend.
		//bbVariableItIsAnX = StrToPaginate.substr(1, 1);
		//bbPowerItIsA2 = StrToPaginate.substr(3, 1);

		//Looks like we want to parse all least deeply nested '+' to a tagged value so we can then
		// tokenize - and we can only use one character at a time.... also this brings up negatives
		// and other dimensions like i IJK; 

		//Actually a vector of pointers to the appropriate '+' will allow us to cut it up ourselves
		//Also looks like we need more brackets - if everything is bracketed properly then once
		//we get a lump of anything we should always be able to put it into our bracket data structure.

		//So the aim is to work from the leftmost deepest nested bracket until that is the outer bracket
		//and we are finished
	}

	void FindClosingBracket(Bracket& ConsideredBracket, std::string StrToPaginate)
	{
		std::string StrToPaginateRemainder;

		StrToPaginateRemainder = StrToPaginate.substr(ConsideredBracket.nPositionOpeningBracket);

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
					ConsideredBracket.nPositionClosingBracket = ConsideredBracket.nPositionOpeningBracket + nPositionInStr;
					return;
				}
			}
			nPositionInStr++;
		}
	
	}

	void FindEnclosedBrackets(Bracket ConsideredBracket, std::string StrToPaginate)
	{
		//Actually this str information contains no refernce to the enclosed brackets.
		std::string EnclosedStr;

		int LenEnclosedStr = ConsideredBracket.nPositionClosingBracket - ConsideredBracket.nPositionOpeningBracket;

		EnclosedStr = StrToPaginate.substr(ConsideredBracket.nPositionOpeningBracket +1 , LenEnclosedStr -1);

		if (ConsideredBracket.nId == 0)
			ScreenOutputFlag2 = EnclosedStr;

		if (ConsideredBracket.nId == 1)
			ScreenOutputFlag3 = EnclosedStr;

		if (ConsideredBracket.nId == 2)
			ScreenOutputFlag4 = EnclosedStr;

		return;
	}

	bool IsNewTerm(Bracket& brac, std::string StrToPaginate)
	{
		if (StrToPaginate[brac.nPositionOpeningBracket - 1] == '+')
		{
			//brac.bNewTerm = true;	// This is an interrogator function so it should nto make changes
			//ScreenOutputFlag5 += " Cap'n " + StrToPaginate[brac.nPositionOpeningBracket - 1];
			return true;
		}
		else
		{
			//ScreenOutputFlag5 = ScreenOutputFlag5 + " False Cap'n " + StrToPaginate[brac.nPositionOpeningBracket - 1];
			return false;
		}
	}


	// We want functions to check if a First Terminal Bracket exists - here we are relying on their being one
	// we also want to check if that bracket is paginated.

	
	int FindFirstTerminalBracket(Bracket& brac)
	{
		if(brac.ContainedBrackets.empty() == true)
		{
			return brac.nId;
		}
		else
		{
			return FindFirstTerminalBracket(vecBrackets[brac.ContainedBrackets[0]]);
		}
	}

	void PaginateBracket(Bracket& brac, std::string StrToPaginate)
	{
		 
		//This wants to look at the bracs it contains, due to our recursive function they should all be
		//Paginated if we are attempting to paginate the bracket
		//We can also check if the paginated brackets are in the bracket's expontent or not
		//CHECK: if paginated brackets are in exponent - this will be done by finding location of exponent
		//brackets {} we can either count backwards or we can put the exponent at the front of the str.

		//Lets just do something
		std::string BracketStr;

		int bbCounter = 0;
		//int nExponentBrackets = 0;
		//int nStartOfExponent;
		bool EndExponentFound = false;
		int nEndOfExponent;

		std::string VarString;
		std::string ExponentStr;

		BracketStr = StrToPaginate.substr(brac.nPositionOpeningBracket, brac.nPositionClosingBracket - brac.nPositionOpeningBracket);

		// Right we are going ot be working forwards

		for (char a : BracketStr)
		{
			if (a == '}')
			{
				if (EndExponentFound == false)
				{
					nEndOfExponent = bbCounter;
					EndExponentFound = true;
				}
				
			}
			bbCounter++;
		}

		//for (char a : BracketStr)
		//{
		//	if (a == '{')
		//	{
		//		nExponentBrackets++;
		//		nStartOfExponent = bbCounter;	//If this works there should only be one instance of '{' 
		//										//in the string.
		//	}
		//	bbCounter++;
		//}

		//brac.sExponentialString = BracketStr.substr(nStartOfExponent + 1, (bbCounter - nStartOfExponent) - 2);
		brac.sExponentialString = BracketStr.substr(2, nEndOfExponent-2);

		if (brac.ContainedBrackets.empty() == true)	//Case it is a terminal bracket
		{

			//Right for now we want to change the behaviour if we encounter more than one '{'
			//This highlights a thorny issue or how we apply this term exponent.
			//So here what we want to do is store pagination information in paginated brackets
			//Then when we have an outer bracket it goes back and looks at this information and 
			//acts appropriately.

			//ScreenOutputFlag4 += " " + std::to_string(nEndOfExponent);
			ScreenOutputFlag4 += " " + BracketStr.substr(2, nEndOfExponent - 2);

			//if (nExponentBrackets == 1) // This is the case for a terminal bracket
										
			//{
				//VarString = BracketStr.substr(1, nStartOfExponent - 1);
				VarString = BracketStr.substr(nEndOfExponent+1, bbCounter);
				//ExponentStr = BracketStr.substr(nStartOfExponent + 1, (bbCounter - nStartOfExponent) - 2);
				ExponentStr = BracketStr.substr(2, nEndOfExponent - 2);

				//This is one of the most important variables
				//int AggregateLen = 0;

				sSpaceText MyVar;
				MyVar.str = VarString;
				MyVar.nFontSize = 5;

				if (brac.sExponentialString != "-1")
				{
					MyVar.x = ScreenWidth() + brac.AggregateLengthAboveLine;
					MyVar.y = ScreenHeight() / 2;
					MyVar.dx = -200;
					//vecPaginatedText.push_back(MyVar);
					brac.AggregateLengthAboveLine += (4 * MyVar.nFontSize * (bbCounter - nEndOfExponent));
				}
				else
				{
					MyVar.x = ScreenWidth() + brac.AggregateLengthBelowLine;
					MyVar.y = (ScreenHeight() / 2) +100;
					MyVar.dx = -200;
					brac.AggregateLengthBelowLine += (4 * MyVar.nFontSize * (bbCounter - nEndOfExponent));
				}
				brac.PaginatedText.push_back(MyVar);
				//Botch for now
				//AggregateLen += (4 * MyVar.nFontSize * (bbCounter - nEndOfExponent));

				//While engaged in useful botching - we only want to do this if the exponent is other than 1
				if (ExponentStr != "1")
				{
					sSpaceText MyExp;
					MyExp.str = ExponentStr;
					MyExp.nFontSize = 4;
					if (ExponentStr != "-1")
					{
						MyExp.x = ScreenWidth() + brac.AggregateLengthAboveLine;
						MyExp.y = (ScreenHeight() / 2) - 16;
						MyExp.dx = -200;
						//vecPaginatedText.push_back(MyExp);
						brac.AggregateLengthAboveLine += (4 * 4) * nEndOfExponent;
					}
					else
					{
						MyExp.x = ScreenWidth() + brac.AggregateLengthBelowLine;
						MyExp.y = (ScreenHeight() / 2) - 16;
						MyExp.dx = -200;
						//vecPaginatedText.push_back(MyExp);
						brac.AggregateLengthBelowLine += (4 * 4) * nEndOfExponent;
					}
					brac.PaginatedText.push_back(MyExp);
				}
					
				//brac.AggregateLengthAboveLine += (4 * 4) * 5;
				//brac.AggregateLengthBelowLine += (4 * 4) * 5;

			//}
			// Set Bracket Pagination to true - OK we seem to be doing this twice
			brac.bPaginated = true;
		}
		else
		{
			//Handle internal paginated brackets
			//Right, plan is to only handle the exponents in the surrounding brackets
			//Right there is probably more to this but the plan of attack is to transfer
			// all the paginated text in the contained brackets to the the containing bracket
			// and add more formatting - under the line action!
			// OK we only want ot put things under the line if their exponent is -1

			//Right here is the nub of the issue; we only want to add the text once
			//Triple brackets and higher give multiple instances of the same text

			for (int bracId : brac.ContainedBrackets)
			{
				if(vecBrackets[bracId].bCopied == false)
				{
					if (vecBrackets[bracId].sExponentialString == "-1")
					{
						for (sSpaceText PrePaginated : vecBrackets[bracId].PaginatedText)
						{
							PrePaginated.x += brac.AggregateLengthBelowLine;
							brac.PaginatedText.push_back(PrePaginated);
						}
						brac.AggregateLengthBelowLine += vecBrackets[bracId].AggregateLengthBelowLine;
					}
					else
					{
						for (sSpaceText PrePaginated : vecBrackets[bracId].PaginatedText)
						{
							PrePaginated.x += brac.AggregateLengthAboveLine;
							brac.PaginatedText.push_back(PrePaginated);
						}
						brac.AggregateLengthAboveLine += vecBrackets[bracId].AggregateLengthAboveLine;
					}
					vecBrackets[bracId].bCopied = true;
					//ScreenOutputFlag1 += " Copied ";
				}
			}

			//ExponentStr = BracketStr.substr(nStartOfExponent + 1, (bbCounter - nStartOfExponent) - 2);
			ExponentStr = BracketStr.substr(2, nEndOfExponent - 2);

			ScreenOutputFlag4 += " " + BracketStr.substr(0, nEndOfExponent);

			if (ExponentStr != "1")
			{
				sSpaceText MyExp;
				MyExp.str = ExponentStr;
				MyExp.nFontSize = 4;
				MyExp.x = ScreenWidth() + AggregateLen;
				MyExp.y = (ScreenHeight() / 2) - 16;
				MyExp.dx = -200;
				//vecPaginatedText.push_back(MyExp);
				brac.PaginatedText.push_back(MyExp);
			}
			brac.bPaginated = true;
		}

	}

	//bool DoubleCounted(Bracket bracOuter, int nIdbracInner)
	//{
	//	bool CountedDouble = false;
	//
	//	for()
	//}

	bool PaginateExpression(Bracket& brac, std::string StrToPaginate)
	{
	//ALGORITHM
	// So we call this function, it checks to see if there are any unpaginated brackets inside the bracket
	// If there are it calls itself with the contained bracket; it then calls itself with the original bracket;
	// It returns when no unpaginated brackets remain in the bracket.
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

	void LaunchPagination2(std::string StrToPaginate)
	{
		std::stringstream StrToChunk(StrToPaginate);

		std::string Chunk;

		int nNumChunks = 0;
		int nLenChunk = 0;
		int nAggregateLenChunks = 0;
		int bloodyCounter = 0;

		while (getline(StrToChunk, Chunk, '+'))
		{
			for (char a : Chunk)
				nLenChunk++;

			sSpaceText MyChunk;
			MyChunk.str = Chunk;
			MyChunk.nFontSize = 2;
			//MyChunk.x = ScreenWidth() + (nNumChunks * 500);
			MyChunk.x = ScreenWidth() + (nAggregateLenChunks * MyChunk.nFontSize * 8) + (nNumChunks * 100);
			//if (bloodyCounter == 0)
			//	ScreenOutputFlag1 = std::to_string(MyChunk.x);
			//if (bloodyCounter == 1)
			//	ScreenOutputFlag2 = std::to_string(MyChunk.x);
			//if (bloodyCounter == 2)
			//	ScreenOutputFlag3 = std::to_string(MyChunk.x);
			//if (bloodyCounter == 3)
			//	ScreenOutputFlag4 = std::to_string(MyChunk.x);
			//if (bloodyCounter == 4)
			//	ScreenOutputFlag5 = std::to_string(MyChunk.x);


			MyChunk.y = ScreenHeight() / 2;
			MyChunk.dx = -200;
			vecPaginatedText.push_back(MyChunk);

			nAggregateLenChunks += nLenChunk;
			nNumChunks++;
			nLenChunk = 0;
			bloodyCounter++;
		}
	}

	void LaunchPagination(std::string StrToPaginate)
	{
		// stringstream class check1 
		std::stringstream PlusPass(StrToPaginate);
		//Here we see something - we are looking for Plusses of the same level of nesting
		//Here for instance the least nested.
		//So here we see some recursion - first we pass paginate the whole thing;
		//Then it chops it up into the least nested terms
		//Then we pass paginate each of those terms in turn, all of which it handles recursively.

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
			MyText.y = ScreenHeight() / 2;
			MyText.dx = -200;
			vecPaginatedText.push_back(MyText);
			 
			sSpaceText MyFormatting;
			MyFormatting.str = std::to_string(nLenToken);
			MyFormatting.nFontSize = 2;
			MyFormatting.x = ScreenWidth() + (8 * MyFormatting.nFontSize * AggregateLen) + (100 * nNumToken);
			MyFormatting.y = (ScreenHeight() / 2) + 50;
			MyFormatting.dx = -200;
			vecPaginatedText.push_back(MyFormatting);

			nNumToken++;
			AggregateLen += nLenToken;
			nLenToken = 0;
			//nDepthVariabeNesting;
			//Right so it is leftmost deepest bracket time again - Paginate this with offsets and scales
			//Then Work outwards.
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