#include <stdio.h> // for common functions , printf,  fopen...
#include <stdlib.h> // for using of exit function

// change guide mdode make it 0 to disable guide mode
#define GUIDE 1
// some constants for solution result
#define BAD 0
#define GOOD 1
// functions prototype
void fnLoadFile(char *fileName,int *matrix); // load the matrix from a file
void fnSetSolution(_Bool); // determine status of program (good solution or bad solution)
void printMatrix(int*); // to print a matrix in a good form
void fnReadFileName(char*,int); // to read file name
_Bool fnCheckRows(int*); // to check rows and return the status good or bad
_Bool fnCheckColumns(int*); // to check columns and return the status good or bad
_Bool fnCheckBoxes(int*);//to check boxes and return the status good or bad

void main()
{
	// Welcome a user
	printf("Welcome to Sudoku Solution Checker\n");
	
	if(GUIDE)
		printf("You are in Guide mode which will help you to locate the problem\n");
	// Read file name
	char fileName[33];
	fnReadFileName(fileName,32);
	// 2D matrix to store the values in
	int matrix[9][9];
	// now let's read values and put it in matrix , &matrix[0][0] to avoid any kind of warnings
	fnLoadFile(fileName,&matrix[0][0]);
	// let's print it
	printMatrix(&matrix[0][0]);
	/* now let's check it */
	short int iGood=0;
	
	// let's check rows
	iGood += fnCheckRows(&matrix[0][0]);
	// let's check columns
	iGood += fnCheckColumns(&matrix[0][0]);
	// let's check box
	iGood += fnCheckBoxes(&matrix[0][0]);
	
	// now if iGood is not 3 that mean one of checks is not good
	if(iGood == (3*GOOD)) // Good solution
		fnSetSolution(GOOD);
	else // bad solution
		fnSetSolution(BAD);
}
_Bool fnCheckRows(int *matrix)
{
	/*
	 * this function check the rows in the matrix
	 * and return GOOD or BAD
	 */
	// this variable will be used on bit operation we will use first 10 bits as indicator for repeation
	// for example we will take number 2 and put it in 2nd bit in this variable and then if we see this location already 1 then it's repeated if not so continue
	short int iBits=0,iRow,iCol,iValue;
	_Bool bIsValid=GOOD; // this is indicator to see if this solution is bad or not
	for(iRow = 0 ; iRow < 9 ; iRow++)
	{
		// make it 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 , because i will use binary operations on it
		// i used bit operation instead using of array to check within it
		iBits=0; 
		for(iCol = 0 ; iCol < 9 ; iCol++)
		{
			iValue = *(matrix + (iRow*9+iCol)); // this gets the value of element
			if(iValue <=0 || iValue > 9) // that mean not in 1-9 range
			{
				if(GUIDE)
					printf("Guide: The problem is in row %d, Number %d is not in the allowed range (1-9)\n",iRow+1,iValue);

				bIsValid=BAD; // will be used later to know wheather it's good solution or not
				continue;
			}
			if(iBits & (1<<iValue)) // that mean number is repeated
			{
				if(GUIDE)
					printf("Guide: The problem is in row %d, Number %d is repeated\n",iRow+1,iValue);

				bIsValid=BAD;
				continue;
			}
			// that saves it in binary form which will make 1 on value position , we will check it later with & in same position
			iBits |= 1<<iValue;
		}
	}
	return bIsValid; // return the status of matrix , 1=valid , 0=invalid
}
_Bool fnCheckColumns(int *matrix)
{
	/*
	 * this function check the columns in the matrix
	 * and return GOOD or BAD
	 */
	// iBits variable will be used on bit operation we will use first 10 bits as indicator for repeation
	// for example we will take number 2 and put it in 2nd bit in this variable and then if we see this location already 1 then it's repeated if not so continue
	short int iBits=0,iRow,iCol,iValue;
	_Bool bIsValid=GOOD; // this is indicator to see if this solution is bad or not
	for(iCol=0 ; iCol < 9 ; iCol++)
	{
		iBits=0;
		for(iRow = 0 ; iRow < 9 ; iRow++)
		{
			iValue=*(matrix+(iRow*9+iCol)); // get element value in matrix;
			if(iBits & (1<<iValue))
			{
				if(GUIDE)
					printf("Guide: The problem is in Column %d, Number %d is repeated\n",iCol+1,iValue);

				bIsValid=BAD;
				continue;
			}
			iBits |=1 << (iValue);
		}
	}
	return bIsValid; // return the status of matrix , 1=valid , 0=invalid
}
_Bool fnCheckBoxes(int *matrix)
{
	/*
	 * this function check the boxes within the matrix
	 * and return GOOD or BAD
	 */
	short int iOffset=3; // which mean every box takes 3 element in row and 3 elements in column, will be used to skip in between elements
	short int iRelativeRow=0; // this variable is relative in y direction which mean if 00 then 10 then 20 (yx)
	short int iBoxNum=0; // this variable just for guide the user about box number of problem
	short int iCol,iRow,iValue;
	// iBits variable will be used on bit operation we will use first 10 bits as indicator for repeation
	// for example we will take number 2 and put it in 2nd bit in this variable and then if we see this location already 1 then it's repeated if not so continue	
	short int iBits=0;
	_Bool bIsValid=GOOD; // this is indicator to see if this solution is bad or not
	// every iOffset row which mean if we start from 0 then next one will be 3 ..ect
	for(iRow = 0 ; iRow < 9 ; iRow+=iOffset) 
	{
		iBits=0;
		for(iCol=0 ; iCol < 9 ; iCol++)
		{
			
			// this loop will store the numbers from iRow to 3 iRows down relative to it
			for(iRelativeRow=iRow ; iRelativeRow < (iRow+iOffset) ; iRelativeRow++)
			{
				iValue=*(matrix+ (iRelativeRow*9+iCol)); // gets value of element
				if(iBits & (1<<iValue))
				{
					if(GUIDE)
						printf("Guide: The problem in Box number %d, Number %d is repeated\n",iBoxNum+1,iValue);
				bIsValid=BAD;
				continue;
				}
				iBits |= 1 << (iValue);
			}
			// this checks if it's last element in the box
			if(((iCol+1) % iOffset) == 0) // which mean box is done
			{
				// to keep track for box number to be used in Guide mode
				iBoxNum++;
				// to reset it because we done all elements in the box
				iBits=0;
			}
		}
	}
	return bIsValid;// return the status of matrix , 1=valid , 0=invalid
}
void fnSetSolution(_Bool bStatus)
{
	/*
	 * this function only for print good or bad solution
	 * depends in bStatus value GOOD or BAD
	 * 
	 */
	 printf("\n\n\nResult: This is a ");
	 if(bStatus == GOOD)
		printf("Good");
	else if(bStatus == BAD)
		printf("Bad");
	else // how that possible :D but let's print some useful text instead of just stuck there
		printf("Undefined");
	printf(" solution\n\n\n");
	printf("Made by: Abdelhamed Al-Abbady\nID: 11\n");
	getchar(); // to prevent fast exit and user can't see the result
	//exit we don't need the program be continued
	exit(bStatus);
}
void fnLoadFile(char *fileName,int *matrix)
{
	/*
	 *	this function purpose to load file and assign numbers to the matrix 2D array
	 *	
	 * 	and it checks matrix size
	*/
	
	short int iMaxSize = 81; // 9*9 = 81 number
	short int iCurrentPos = 0; // To track current position and also will be used with iMaxSize
	FILE *fp = fopen(fileName,"r"); // open file and mode is reading
	if(fp == NULL) // that mean file is invalid and i can't get FILE pointer
	{
		printf("Sorry , we can't open %s file , make sure that he's exists or read permissions is allowed.",fileName);
		// that mean bad solution
		fnSetSolution(BAD);
	}
	int iValue; // // this variable is temperory for getting characters as number from files in int form
	while(!feof(fp)) // that mean file not ended yet
	{
		// read character from file
		if(fscanf(fp,"%d",&iValue) == -1) // that mean not number, so ignore it
			continue;
		// get represented number from that character will return 0-9 or -1 if it's not number
		// that mean he's a number
		*(matrix + iCurrentPos)=iValue; // assigning the value to matrix
		iCurrentPos++;
	}
	if(iMaxSize != iCurrentPos) // mm.. that mean something was not correct in the file, so let user know about that
	{
		if(GUIDE) // if guide mode is enabled
			printf("Guide: Matrix size is not correct");
		// to let the program restart and let user input the filename again
		fnSetSolution(BAD);
	}
	// it seems everything is OK
	fclose(fp);
}
void printMatrix(int *matrix)
{
	/*
	 * take 2D matrix prints it and boxing it
	 */
	short int iRow,iCol,iOffset=3;
	
	printf("\n\nLoaded the matrix from file:\n\n");
	for(iRow=0;iRow<9;iRow++)
	{
		for(iCol=0;iCol<9;iCol++)
		{
			// iRow*9+iCol will give us the location of that element
			printf("%d  ",*(matrix+(iRow*9+iCol)));
			if(!((iCol+1)%iOffset)) // that mean last element of box in right side
				printf("\t");
		}
		printf("\n");
		if(!((iRow+1)%iOffset)) // that mean last element of box in down side
			printf("\v"); // vertical tab
	}
	printf("\n\n\n\n");
}
void fnReadFileName(char *fileName,int iLength)
{
	/*
	 * this function is used to read the name from user
	 * 
	 */
	// Reading sudoku file name with maximum 32 characters
	printf("\nPlease Enter file location (you can use relative path) (max: %d length): ",iLength);
	char c; // and then to temperory put user input to
	// this section is to read file name
	short int i=0; // used to count character numbers
	scanf("%c",&c);
	while(c != 0xa && i <= iLength) // let him input until enter (0xa) is entered
	{
		*(fileName+i)=c; // assign then increase
		i++;
		scanf("%c",&c); // input again
	}
	*(fileName+i)='\0'; // last character should be null for string
	// now done,  name is stored in fileName string
}
