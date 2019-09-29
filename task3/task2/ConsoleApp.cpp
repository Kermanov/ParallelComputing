#include "ConsoleApp.h"

void ConsoleApp::printLine()
{
	for (size_t i = 0; i < 50; ++i)
	{
		cout << '=';
	}
	cout << endl;
}

void ConsoleApp::help()
{
	for (const Function& method : functions)
	{
		cout << setw(10) << right << method.title << " - ";
		cout << method.descr << endl;
	}

	cout << setw(10) << right << "help";
	cout << " - displays info about functions" << endl;
	cout << setw(10) << right << "clear";
	cout << " - clears the console" << endl;
	cout << setw(10) << right << "exit";
	cout << " - closes the app" << endl;
}

void ConsoleApp::printHead()
{
	cout << title << endl;
	cout << "Enter \"help\" for functions description." << endl;
	printLine();
}

ConsoleApp::ConsoleApp(string _title, vector<Function> _methods) :
	title(_title),
	functions(_methods)
{}

void ConsoleApp::run()
{
	printHead();
	string command;
	while (true)
	{
		getline(cin, command);
		for (const Function& method : functions)
		{
			if (method.title == command)
			{
				printLine();
				try
				{
					method.function();
				}
				catch (const exception& exc)
				{
					cout << "Error: " << exc.what() << endl;
				}
				printLine();

				command.clear();
				break;
			}
		}

		if (command == "exit")
		{
			break;
		}
		else if (command == "help")
		{
			printLine();
			help();
			printLine();
		}
		else if (command == "clear")
		{
			system("cls");
			printHead();
		}
		else if (!command.empty())
		{
			cout << "Unknown command." << endl;
		}
	}
}