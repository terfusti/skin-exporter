

#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <filesystem>

#include "base64.h"
#include "json.hpp"



bool exportSkins()
{
	using nlohmann::json;

	std::ifstream jsonFile{ "launcher_skins.json" };

	if (!jsonFile.good())
	{
		std::cout << "Could not find launcher_skins.json file!\n";
		return false;
	}

	json launcher_json{ json::parse(jsonFile) };


	for (auto it = launcher_json.begin(); it != launcher_json.end(); ++it)
	{


		std::string minecraft_skin{ it.value()["skinImage"] };
		minecraft_skin.erase(0, 22);

		std::vector<BYTE> decodedData = base64_decode(minecraft_skin);
		
		std::string fileName{ "skins\\" + static_cast<std::string>(it.value()["id"]) };
		fileName += ".png";

		if (!std::filesystem::is_directory("skins"))
			std::filesystem::create_directory("skins");

		std::ofstream outfile(fileName, std::ios::binary | std::ios::out);
		outfile.write(reinterpret_cast<const char*>(decodedData.data()), decodedData.size());
		std::cout << fileName << '\n';
		outfile.close();
	}

	return true;
}


void printInstructions()
{

	static bool firstTime{ true };
	std::string firstBar{ "============================================================\n" };

	std::cout << ((!firstTime) ? "\n\n" + firstBar : firstBar) <<
				 "terfusti's minecraft launcher skin exporter!\n" << 
				 "Press numpad 1 to print instructions\n" <<
				 "Press numpad 2 to export skins (read instructions first)\n" <<
				 "Press numpad 0 to print this menu\n" << 
				 "============================================================\n\n";

	if (firstTime)
	{
		firstTime = false;
	}
}

int main()
{
	
	printInstructions();

	while (true)
	{

		if (GetAsyncKeyState(VK_NUMPAD1))
		{
			std::cout << "Instructions: \nPlace this file into your minecraft folder, or copy your launcher_skins.json file into this folder, then export the skins. THey will be exported to a folder called skins.\n\n";
			printInstructions();
		}

		if (GetAsyncKeyState(VK_NUMPAD2))
		{
			std::cout << "Exporting Skins...\n";
			exportSkins();

			printInstructions();
		}

		if (GetAsyncKeyState(VK_NUMPAD0))
		{
			printInstructions();
		}

		Sleep(100);
	}

	return 0;
}




/*


// base64 works 3 characters at a time, if data is not divisble by 3, bytes will be added to the last section to make 3. Added bytes convert to '=' in base64
hey

// convert characters to ascii number
104 101 121

// convert ascii numbers to binary
01101000 01100101 01111001

// split binary into 4 sections
011010 000110 010101 111001

// convert 4 sections to integers
26 6 21 57

// reference base64 table to convert to base64
a G V 5


*/