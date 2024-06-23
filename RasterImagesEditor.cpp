#include <iostream>
#include "Session.h"

std::string toLowwer(const std::string& string)
{
	std::string result = string;
	size_t size = string.size();
	for (size_t i = 0; i < size; i++)
	{
		if (result[i] >= 'A' && result[i] <= 'Z')
			result[i] += 'a' - 'A';
	}
	return result;
}

void splitInputs(const std::string& input, std::vector<std::string>& inputs)
{
	inputs.clear();
	std::string currentStr = "";
	size_t inputSize = input.size();
	for (size_t i = 0; i < inputSize; i++)
	{
		if (input[i] == ' ')
		{
			if (currentStr == "") continue;
			inputs.push_back(currentStr);
			currentStr = "";
		}
		else
		{
			currentStr += input[i];
		}
	}

	if (currentStr != "") inputs.push_back(currentStr);
}

void startNewSession(const std::vector<std::string> inputs, std::vector<Session>& sessions)
{
	Session session;
	for (size_t i = 1; i < inputs.size(); i++)
	{
		session.addFile(inputs[i]);
	}
	sessions.push_back(session);
}

bool manageInputss(const std::vector<std::string>& inputs, std::vector<Session>& sessions)
{
	if (!inputs.empty())
	{
		std::string firstCommandTxt = toLowwer(inputs[0]);

		if (firstCommandTxt == "saveas")
		{
			sessions.back().saveas(inputs);
		}
		else if (firstCommandTxt == "add")
		{
			for (size_t i = 1; i < inputs.size(); ++i)
			{
				sessions.back().addFile(inputs[i]);
			}
		}
		else if (firstCommandTxt == "load")
			startNewSession(inputs, sessions);
		else return false;
	}
	return true;
}

bool manageInputs(const std::vector<std::string>& inputs, std::vector<Session>& sessions)
{
	if (inputs.size() == 1)
	{
		std::string commandTxt = toLowwer(inputs[0]);

		if (commandTxt == "grayscale")			sessions.back().addCommand(ImageProcesing::Commands::grayscale);
		else if (commandTxt == "monochrome")	sessions.back().addCommand(ImageProcesing::Commands::monochrome);
		else if (commandTxt == "negative")		sessions.back().addCommand(ImageProcesing::Commands::negative);
		else if (commandTxt == "save")			sessions.back().save();
		else if (commandTxt == "undo")			sessions.back().undo();
		else if (commandTxt == "redo")			sessions.back().redo();
		else if (commandTxt == "exit")			return true;
		else return false;
	}
	else if (inputs.size() == 2)
	{
		std::string firstCommandTxt = toLowwer(inputs[0]);
		std::string secondCommandTxt = toLowwer(inputs[1]);

		if (firstCommandTxt == "rotate" && secondCommandTxt == "left")
			sessions.back().addCommand(ImageProcesing::Commands::rotateLeft);
		else if (firstCommandTxt == "rotate" && secondCommandTxt == "right")
			sessions.back().addCommand(ImageProcesing::Commands::rotateRight);
		else if (firstCommandTxt == "flip" && secondCommandTxt == "top")
			sessions.back().addCommand(ImageProcesing::Commands::flipTop);
		else if (firstCommandTxt == "flip" && secondCommandTxt == "left")
			sessions.back().addCommand(ImageProcesing::Commands::flipLeft);
		else if (firstCommandTxt == "list" && secondCommandTxt == "session")
			sessions.back().listSession();
		else return false;
	}
	else if (inputs.size() == 5)
	{
		std::string firstCommandTxt = toLowwer(inputs[0]);
		std::string secondCommandTxt = toLowwer(inputs[1]);
		std::string thirdCommandTxt = toLowwer(inputs[2]);

		if (firstCommandTxt == "make" && secondCommandTxt == "collage" && thirdCommandTxt == "horizontal")
			sessions.back().makeHorizontalCollage(inputs[3], inputs[4]);
		else if (firstCommandTxt == "make" && secondCommandTxt == "collage" && thirdCommandTxt == "vertical")
			sessions.back().makeVerticalCollage(inputs[3], inputs[4]);
		else if (firstCommandTxt == "crop")
			sessions.back().crop(inputs[1], inputs[2], inputs[3], inputs[4]);
		else return false;
	}
	else return false;
	return true;
}

void startEditor(std::string& input, std::vector<std::string>& inputs)
{
	std::cout << "Please use load to start a session or use exit to close the editor!\n" <<
		"Commands for images: monochrome, graysclae, negative, rotate left, rotate right, flip left, flip top, undo, redo, crop.\n" <<
		"(After crop you can`t use undo for previous commands and for crop!)\n" <<
		"Commands for current session: add, list session, save, saveas, make collage (horizontal or vertical)" << std::endl;

	std::getline(std::cin, input);
	splitInputs(input, inputs);
	while (inputs.empty() || (inputs[0] != "load" && !(inputs[0] == "exit" && inputs.size() == 1)))
	{
		std::cout << "Not a valid command!" << std::endl;
		std::getline(std::cin, input);
		splitInputs(input, inputs);
	}
}

void manageCommands(std::string& input, std::vector<std::string>& inputs, std::vector<Session>& sessions)
{
	do
	{
		std::getline(std::cin, input);
		splitInputs(input, inputs);
		try
		{
			if (!manageInputss(inputs, sessions) && !manageInputs(inputs, sessions))
				std::cout << "Not a valid command!" << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	} while (inputs.empty() || inputs[0] != "exit");
}

void closeEditor(std::vector<Session>& sessions)
{
	for (size_t i = 0; i < sessions.size(); ++i)
	{
		sessions[i].exit();
	}
}

int main()
{
	std::vector<Session> sessions;
	std::string input;
	std::vector<std::string> inputs;

	startEditor(input, inputs);

	if(inputs[0] =="load")
	{
		startNewSession(inputs, sessions);

		manageCommands(input, inputs, sessions);

		closeEditor(sessions);
	}
}