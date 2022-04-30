#define  _WIN32_WINNT   0x0601
#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>

#include "Workflow.h";
#include "Sorting.h";
#include "Reduce.h";
#include "Map.h";
#include "FileManagement.h";

#include <fstream>

// Begin defining the test case suite here.
BOOST_AUTO_TEST_SUITE(TestingMapReduce)

// Test the Map class. 
// Test one string "test" using the Map class.
// Output should be ("test", 1)
BOOST_AUTO_TEST_CASE(Map_Test)
{
	cout << "*****************Map_Test***************" << std::endl;
	ifstream inputFileStream;
	ofstream intermediateFileStream;
	string inputFile = "input.txt";
	string intermediateFile = "intermediate.txt";

	//Create an object of the FileManagement class
	FileManagement FileStreamSystem;
	FileStreamSystem.clearFile(intermediateFileStream, inputFile);
	string firstData{ "This" };
	Map addToFile(inputFile, firstData);

	//Open the input file and connect to the in stream. Then double check to make sure file is not corrupt
	FileStreamSystem.openFileInstream(inputFileStream, inputFile);
	FileStreamSystem.fileCorrupt(inputFileStream);
	//Clear the contents of the intermediate file which will hold the output of the Map class. This will also close the stream
	FileStreamSystem.clearFile(intermediateFileStream, intermediateFile);
	//Initiate a variable to hold raw data given by the input file
	string data{ "Unknown" };
	//Keep collecting data until the end of file and get a return of "1"
	while (data != "1")
	{
		//Get a line of data from the input file
		FileStreamSystem.readFromFile(inputFileStream, data);
		//Check if data was not the end of file
		if (data != "1")
		{
			//Create a Map class object with the output file and data given.
			//This will add the data to the file given
			Map addToFile(intermediateFile, data);
		}

	}
	FileStreamSystem.closeInputFile(inputFileStream);

	FileStreamSystem.openFileInstream(inputFileStream, intermediateFile);
	FileStreamSystem.readFromFile(inputFileStream, data);

	const char* a = "(\"this\", 1)";
	BOOST_TEST(data == a);

	// inform the user that the test has passed.
	cout << "Map test has been passed." << std::endl;
}

// Test the Sorting class's format method.
BOOST_AUTO_TEST_CASE(Sort_Test)
{
	cout << "*****************Sort_Test***************" << std::endl;
	// Sorting and Reducing
	// declare and initialize local variables.
	string intermediateFilePath = "intermediate.txt"; // intermediate file path
	string* intermediateFilePathPntr = &intermediateFilePath;

	FileManagement FileStreamSystem;

	// initialize local variables
	ofstream outputFileStream;

	// create an instance of the Sorting class.
	Sorting sortingObj(intermediateFilePathPntr);


	// Create local variables. Input file stream object
	ifstream inputFileStreamObj;
	string line;
	string entryString{ NULL };
	string* entryStrPntr{ NULL };

	// create a delimiter to find the following string: ")"
	string closedParenthesis{ ")" };
	string openParenthesis{ "(" };
	size_t openPos{ NULL };
	size_t closedPos{ NULL };

	// format the file.
	sortingObj.format();

	// open the intermediate file
	FileStreamSystem.openFileInstream(inputFileStreamObj, *intermediateFilePathPntr);

	// assign the entry string pointer
	entryStrPntr = &entryString;

	FileStreamSystem.closeInputFile(inputFileStreamObj);

	string data{ "Unknown" };

	FileStreamSystem.openFileInstream(inputFileStreamObj, intermediateFilePath);
	FileStreamSystem.readFromFile(inputFileStreamObj, data);

	const char* a = "(\"this\", [1]), (\"1\", [1])";
	BOOST_TEST(data == a);

	// inform the user that the test has passed.
	cout << "Sort test has been passed." << std::endl;
}

// Test the Reduce class's reduce method.
BOOST_AUTO_TEST_CASE(Reduce_Test)
{
	cout << "*****************Reduce_Test***************" << std::endl;
	Workflow percentCheck;
	// Sorting and Reducing
	// declare and initialize local variables.
	string intermediateFilePath = "intermediate.txt"; // intermediate file path
	string outputFilePath = "output.txt";
	string* intermediateFilePathPntr = &intermediateFilePath;
	string* outputFilePathPntr = &outputFilePath;

	// initialize local variables
	ofstream outputFileStream;

	// open the file for editing.
	FileManagement FileStreamSystem;
	FileStreamSystem.clearFile(outputFileStream, outputFilePath);

	// create an instance of the Sorting class.
	Sorting sortingObj(intermediateFilePathPntr);

	// create an instance of the Reduce class.
	Reduce reduceObj(outputFilePathPntr);

	// Create local variables. Input file stream object
	ifstream inputFileStreamObj;
	string line;
	string entryString{ NULL };
	string* entryStrPntr{ NULL };

	// create a delimiter to find the following string: ")"
	string closedParenthesis{ ")" };
	string openParenthesis{ "(" };
	size_t openPos{ NULL };
	size_t closedPos{ NULL };

	// open the intermediate file
	FileStreamSystem.openFileInstream(inputFileStreamObj, *intermediateFilePathPntr);

	// assign the entry string pointer
	entryStrPntr = &entryString;

	// get the first line from the file.
	while (getline(inputFileStreamObj, line)) {

		// find the position of the first open parenthesis
		openPos = line.find(openParenthesis);

		// offset for the find method of the string class.
		size_t offset{ 0 };

		// while there are words in this line, keep extracting them.
		while (openPos != string::npos) {

			// find the position of the closed parenthesis
			closedPos = line.find(closedParenthesis, offset);

			// if the open and closed parentheses were found, pass the substring to the reduce class.
			if ((openPos != string::npos) && (closedPos != string::npos)) {

				// extract the entry. 
				entryString = line.substr(openPos, (closedPos - openPos) + 1);

				// pass the string to the reduce method from the Reduce class.
				reduceObj.reduce(entryStrPntr);
			}

			// update the offset into the line for the next search.
			offset = closedPos + 1;

			// Find the position of the next open parenthesis. 
			openPos = line.find(openParenthesis, offset);
		}
	}

	FileStreamSystem.closeInputFile(inputFileStreamObj);

	string data{ "Unknown" };

	FileStreamSystem.openFileInstream(inputFileStreamObj, "output.txt");
	FileStreamSystem.readFromFile(inputFileStreamObj, data);

	const char* a = "(\"this\"), 1";
	BOOST_TEST(data == a);

	// inform the user that the test has passed.
	cout << "Reduce test has been passed." << std::endl;
}

// Compare the output of two Workflow objects processing the same input.
// Make sure the results match.
BOOST_AUTO_TEST_CASE(Consistency_Test)
{
	cout << "*****************Consistency_Test***************" << std::endl;
	ifstream inputFileStream1;
	ifstream inputFileStream2;
	ofstream intermediateFileStream1;
	ofstream intermediateFileStream2;
	ifstream outputFileStream1;
	ifstream outputFileStream2;
	string inputFile1 = "Input1.txt";
	string inputFile2 = "Input2.txt";
	string intermediateFile1 = "Intermediate1.txt";
	string intermediateFile2 = "Intermediate2.txt";
	string outputFile1 = "Output1.txt";
	string outputFile2 = "Output2.txt";

	// Create an object of the FileManagement class
	FileManagement fileManagementObj1;
	FileManagement fileManagementObj2;

	// clear the contents of the files.
	fileManagementObj1.clearFile(intermediateFileStream1, inputFile1);
	fileManagementObj2.clearFile(intermediateFileStream2, inputFile2);
	fileManagementObj1.clearFile(intermediateFileStream1, outputFile1);
	fileManagementObj2.clearFile(intermediateFileStream2, outputFile2);

	//Open the input file and connect to the in stream. Then double check to make sure file is not corrupt
	fileManagementObj1.openFileInstream(inputFileStream1, inputFile1);
	fileManagementObj2.openFileInstream(inputFileStream2, inputFile2);

	// create a string to add to the input file.
	string firstData{ "This is a test of the consistency of the Workflow class.\n" };

	// create an instance of the Map class. Add the string to the file
	// using the Map class constructor.
	Map addToFile(inputFile1, firstData);

	// add the same data to the second text file.
	addToFile.tokenize(inputFile2, firstData);

	// add the string to the file 100 more times.
	for (int i = 0; i < 100; i++) {
		addToFile.tokenize(inputFile1, firstData);
		addToFile.tokenize(inputFile2, firstData);
	}

	// Create two instances of the Workflow class.
	Workflow(inputFile1, intermediateFile1, outputFile1);

	cout << "\nPerforming second workflow on same input.\n" << std::endl;

	Workflow(inputFile2, intermediateFile2, outputFile2);

	// Clear the contents of the intermediate file which will hold the output of the Map class. This will also close the stream.
	fileManagementObj1.clearFile(intermediateFileStream1, intermediateFile1);
	fileManagementObj2.clearFile(intermediateFileStream2, intermediateFile2);

	// close the input file streams.
	fileManagementObj1.closeInputFile(inputFileStream1);
	fileManagementObj2.closeInputFile(inputFileStream2);

	// Open the output filestream and connect to the instream.
	fileManagementObj1.openFileInstream(outputFileStream1, outputFile1);
	fileManagementObj2.openFileInstream(outputFileStream2, outputFile2);

	// Initiate a variable to hold raw data given by the input file
	string line1{ "Unknown" };
	string line2{ "Unknown" };

	// Keep collecting data until the end of file and get a return of "1"
	while ((line1 != "1") || (line2 != "1"))
	{
		// Get a line of data from the input file
		fileManagementObj1.readFromFile(outputFileStream1, line1);
		fileManagementObj2.readFromFile(outputFileStream2, line2);

		// Check if data was not the end of file
		if ((line1 != "1") || (line2 != "1"))
		{
			// compare the output
			BOOST_TEST(line1 == line2);
		}
	}

	// inform the user that the test has passed.
	cout << "Consistency test has been passed." << std::endl;
}

// Test the Workflow class's ability to extract a directory name from the input path to a file.
BOOST_AUTO_TEST_CASE(SeparateFileFromDirectory_Test)
{
	cout << "*****************SeparateFileFromDirectory_Test***************" << std::endl;
	Workflow test;
	test.separateOutputPath("C:\\first\\second\\third\\fourth\\file.txt", "output");
	string direcotryPath = test.getOutputFileDirectoryLocation();

	const char* a = "C:\\first\\second\\third\\fourth";
	BOOST_TEST(direcotryPath == a);

	// inform the user that the test has passed.
	cout << "SeparateFileFromDirectory test has been passed." << std::endl;
}

// Test the system in its entirety. Test one word "This".
BOOST_AUTO_TEST_CASE(FullRunFile_Test)
{
	cout << "*****************FullRunFile_Test***************" << std::endl;
	ifstream inputFileStream;
	ofstream intermediateFileStream;
	string inputFile = "input.txt";
	string intermediateFile = "intermediate.txt";
	string outputFile = "output.txt";
	//Create an object of the FileManagement class
	FileManagement FileStreamSystem;
	FileStreamSystem.clearFile(intermediateFileStream, inputFile);
	string firstData{ "This" };
	Map addToFile(inputFile, firstData);

	Workflow(inputFile, intermediateFile, outputFile);

	string data{ "Unknown" };

	FileStreamSystem.openFileInstream(inputFileStream, "output.txt");
	FileStreamSystem.readFromFile(inputFileStream, data);

	const char* a = "(\"this\"), 1";
	BOOST_TEST(data == a);

	// inform the user that the test has passed.
	cout << "RunFullFile test has been passed." << std::endl;
}

// Test the input directory when all user input are text files.
BOOST_AUTO_TEST_CASE(FullRunDirectory_Test)
{
	cout << "*****************FullRunDirectory_Test***************" << std::endl;
	ifstream inputFileStream;
	ofstream intermediateFileStream;
	string inputFile = "C:\\Users\\antho\\OneDrive\\Desktop\\CIS687 OOD\\testFolder\\input.txt";
	string intermediateFile = "C:\\Users\\antho\\OneDrive\\Desktop\\CIS687 OOD\\testFolder\\intermediate.txt";
	string outputFile = "C:\\Users\\antho\\OneDrive\\Desktop\\CIS687 OOD\\testFolder\\output.txt";

	//Create an object of the FileManagement class
	FileManagement FileStreamSystem;
	FileStreamSystem.clearFile(intermediateFileStream, inputFile);
	string firstData{ "This" };
	Map addToFile(inputFile, firstData);

	Workflow(inputFile, intermediateFile, outputFile);

	string data{ "Unknown" };

	FileStreamSystem.openFileInstream(inputFileStream, outputFile);
	FileStreamSystem.readFromFile(inputFileStream, data);

	const char* a = "(\"this\"), 1";
	BOOST_TEST(data == a);

	// inform the user that the test has passed.
	cout << "FullRunDirectory test has been passed." << std::endl;
}

// Check the validity of the user input files.
BOOST_AUTO_TEST_CASE(ValidFileCheck_Test)
{
	cout << "*****************ValidFileCheck_Test***************" << std::endl;
	string inputFile = "input";
	string tempFile = "temp";
	string outputFile = "output";
	Workflow workflowTest;
	workflowTest.checkFilesValid(inputFile, tempFile, outputFile);

	BOOST_CHECK(true == workflowTest.getValidInputFile());
	BOOST_CHECK(true == workflowTest.getValidIntermediateFile());
	BOOST_CHECK(true == workflowTest.getValidOutputFile());

	// inform the user that the test has passed.
	cout << "ValidFileCheck test has been passed." << std::endl;
}

// end of the test suite.
BOOST_AUTO_TEST_SUITE_END()
