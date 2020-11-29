#include <iostream>
#include "invaders.h"

using namespace std;

int main(int argv, char *argc[])
{
	Invaders main_board;
	main_board.Run();
	
	#ifdef WIN32
	system("pause");
	#endif
	
	return 0;
}
