/*Colton Wilson and Anthony Redamonti
*CSE 687- Object Oriented Design
*Syracuse University
*MapReduce - Phase #1
*04/07/22
*This program contains the function definitions to the member functions located in Workflow.h
*/

//Directives
#include "Workflow.h"
#include "FileManagement.h"
#include "Map.h"
#include "Reduce.h"
#include "Sorting.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>


//Namespaces
using std::cout;
using std::cin;
using std::string;
using std::ifstream;
using std::vector;


//default constructor
//TODO:Can to a try-catch to make sure actual string values are submitted
WorkFlow::WorkFlow() {}
//WorkFlow constructor with three parameters
WorkFlow::WorkFlow(string inputFile, string intermediateFile, string outputFile) : inputFileLocation{ inputFile }, intermediateFileLocation{ intermediateFile }, outputFileLocation{ outputFile }
{
	FileManagement inputFileToCheck;
	inputFileToCheck.openFileInstream(inputFile);
	FileManagement intermediateFileToWrite;
	intermediateFileToWrite.openFileOutstream(intermediateFile);
	inputFileToCheck.setData();
	setRawData(inputFileToCheck.getData());
	cout << rawData;


}

//**********Destructor*********
WorkFlow::~WorkFlow() {}

//**********Setters**********
void WorkFlow::setInputFileLocation(const string& userInputFile) { inputFileLocation = userInputFile; }

void WorkFlow::setIntermediateFileLocation(const string& userIntermediateFile) { intermediateFileLocation = userIntermediateFile; }

void WorkFlow::setOutputFileLocation(const string& userOutputFile) { outputFileLocation = userOutputFile; }

void WorkFlow::setRawData(const string& dataTaken)
{
	rawData = dataTaken;
}


//**********Getters**********
const string WorkFlow::getInputFileLocation(void) { return inputFileLocation; }

const string WorkFlow::getIntermediateFileLocation(void) { return intermediateFileLocation; }

const string WorkFlow::getOutputFileLocation(void) { return outputFileLocation; }