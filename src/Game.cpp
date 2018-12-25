#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include "Crop.h"
#include "Maps.h"

#define fps 60

using namespace std;


Map map = FARM;
Player player(6, 30);
Crop crop[244];
int numCrops = 9, numSeeds = 9, numFruits = 0;

Item bag[6] = { SEED, WATERING_CAN };
Item _item = SEED;
int itemNo = 1, numItems = 2;

ofstream save;
ifstream load;

int day = 1, month = 1, year = 1;
bool day_end = false;

int temp;
char key, det;
int frame_start, frame_end;

void drawScreen();
void getUserInput();
void menu();
void checkPlayerPosition();
void updateDisplay();
void saveGame();
void loadGame();


int main()
{
	while (true)
	{
		//Main Menu
		cout << "**************\n";
		cout << "FARM SIMULATOR\n";
		cout << "**************\n\n";
		cout << "Choose an option:\n\n";
		cout << "1.New Game\n2.Load Game\n3.Quit\n\n";
		cin >> det;
		system("cls");
		if (det == '1') break;
		if (det == '3') return 0;

		if (det == '2')
		{
			load.open("Saves/Saves");
			if (load)
			{
				loadGame();
				break;
			}

			else
			{
				cout << "No save data found...\n\n";
				system("pause");
				system("cls");
			}
			load.close();
		}
	}

	//Main Loop
	while (true)
	{
		frame_start = (int)time(0);

		drawScreen();
		getUserInput();
		updateDisplay();

		//frame rate cap
		frame_end = (int)time(0);
		if (1000 / fps > frame_end - frame_start)
			Sleep((1000 / fps) - (frame_end - frame_start));

		system("cls");
	}
}



//Functions
void drawFarm()
{
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 40; x++)
		{
			if (player.x == x && player.y == y)
				cout << char(2);

			else if (farm[y][x] == 1)
				cout << "#";

			else if (farm[y][x] == 3)
				cout << "H";

			else if (farm[y][x] == 4)
				cout << "-";

			else if (farm[y][x] == 6)
				cout << "|";

			else if (farm[y][x] == 10)
				cout << "S";

			else if (farm[y][x] == 12)
				cout << ".";

			else if (farm[y][x] == 14)
				cout << "~";

			else if (farm[y][x] == 16)
				cout << "o";

			else if (farm[y][x] == 18)
				cout << "O";

			else if (farm[y][x] == 0 || farm[y][x] == 2 || farm[y][x] == 8 || farm[y][x] == 20)
				cout << " ";
		}

		cout << endl;
	}
}

void drawHouse()
{
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			if (player.x == x && player.y == y)
				cout << char(2);

			else if (house[y][x] == 1)
				cout << "#";

			else if (house[y][x] == 3 || house[y][x] == 4)
				cout << "B";

			else if (house[y][x] == 0 || house[y][x] == 2)
				cout << " ";
		}

		cout << endl;
	}
}

void drawStreet()
{
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			if (player.x == x && player.y == y)
				cout << char(2);

			else if (street[y][x] == 1)
				cout << "#";

			else if (street[y][x] == 3)
				cout << "S";

			else if (street[y][x] == 0 || street[y][x] == 2 || street[y][x] == 4)
				cout << " ";
		}
		cout << endl;
	}
}

void drawScreen()
{
	switch (map)
	{
	case FARM:
		drawFarm();
		break;

	case HOUSE:
		drawHouse();
		break;

	case STREET:
		drawStreet();
		break;
	}

	switch (month)
	{
	case 1: cout << "SPR "; break;
	case 2: cout << "SUM "; break;
	case 3: cout << "AUT "; break;
	case 4: cout << "WIN "; break;
	}

	cout << day << endl;
	cout << "menu: m\t\tItem: ";

	switch (_item)
	{
	case SEED:
		cout << " Seeds (x" << numSeeds << ")\n\n";
		break;

	case WATERING_CAN:
		cout << "Watering can\n\n";
		break;

	case FRUIT:
		cout << "Fruit (x" << numFruits << ")\n\n";
		break;
	}
}

void getUserInput()
{
	cin >> key;

	switch (map)
	{
	case FARM:
		switch (key)
		{
		case 'w':
			if (farm[player.y - 1][player.x] % 2 == 0)
				player.y--;
			break;

		case 's':
			if (farm[player.y + 1][player.x] % 2 == 0)
				player.y++;
			break;

		case 'a':
			if (farm[player.y][player.x - 1] % 2 == 0)
				player.x--;
			break;

		case 'd':
			if (farm[player.y][player.x + 1] % 2 == 0)
				player.x++;
			break;

		case 'e':
			itemNo++;
			if (itemNo > numItems)
				itemNo = 1;

			_item = bag[itemNo - 1];
			break;

		case 'q':
			itemNo--;
			if (itemNo < 1)
				itemNo = numItems;

			_item = bag[itemNo - 1];
			break;

		case 'f':
			switch (_item)
			{
			case SEED:
				if (farm[player.y][player.x] == 8 && numSeeds > 0)
				{
					for (int i = 1; i <= numCrops; i++)
					{
						if (crop[i].is_new)
						{
							crop[i] = player;
							crop[i].is_new = false;
							farm[player.y][player.x] = 12;
							numSeeds--;
							numCrops++;
							break;
						}
					}
				}
				break;

			case WATERING_CAN:
				for (int i = 1; i <= numCrops; i++)
					if (crop[i] == player)
					{
						crop[i].is_watered = true;
						break;
					}
				break;
			}

			break;

		case 'c':
			for (int i = 1; i <= numCrops; i++)
				if (crop[i] == player && crop[i].is_grown)
				{
					crop[i].is_harvested = true;
					crop[i].is_grown = false;
					crop[i].is_watered = false;
					crop[i].is_new = true;
					crop[i].days_watered = 0;
					numFruits++;
					numCrops--;
					if (bag[numItems - 1] != FRUIT)
					{
						bag[numItems] = FRUIT;
						numItems++;
					}
					break;
				}

			if (farm[player.y][player.x] == 10 && _item == FRUIT && numFruits > 0)
			{
				numFruits--;
				player.funds += 30;
				if (numFruits < 1)
				{
					bag[numItems - 1] = NONE;
					numItems--;
					_item = bag[numItems - 1];
				}
			}

			break;

		case 'm':
			system("cls");
			menu();
			break;
		}

		break;

	case HOUSE:
		switch (key)
		{
		case 'w':
			if (house[player.y - 1][player.x] % 2 == 0)
				player.y--;
			break;

		case 's':
			if (house[player.y + 1][player.x] % 2 == 0)
				player.y++;
			break;

		case 'a':
			if (house[player.y][player.x - 1] % 2 == 0)
				player.x--;
			break;

		case 'd':
			if (house[player.y][player.x + 1] % 2 == 0)
				player.x++;
			break;

		case 'e':
			itemNo++;
			if (itemNo > numItems)
				itemNo = 1;

			_item = bag[itemNo - 1];
			break;

		case 'q':
			itemNo--;
			if (itemNo < 1)
				itemNo = 2;

			_item = bag[itemNo - 1];
			break;

		case 'm':
			system("cls");
			menu();
			break;
		}

		break;

	case STREET:
		switch (key)
		{
		case 'w':
			if (street[player.y - 1][player.x] % 2 == 0)
				player.y--;
			break;

		case 's':
			if (street[player.y + 1][player.x] % 2 == 0)
				player.y++;
			break;

		case 'a':
			if (street[player.y][player.x - 1] % 2 == 0)
				player.x--;
			break;

		case 'd':
			if (street[player.y][player.x + 1] % 2 == 0)
				player.x++;
			break;

		case 'e':
			itemNo++;
			if (itemNo > numItems)
				itemNo = 1;

			_item = bag[itemNo - 1];
			break;

		case 'q':
			itemNo--;
			if (itemNo < 1)
				itemNo = 2;

			_item = bag[itemNo - 1];
			break;

		case 'm':
			system("cls");
			menu();
			break;
		}

		break;
	}

	checkPlayerPosition();
}

void menu()
{
	while (true)
	{
		switch (month)
		{
		case 1: cout << "Spring " << day; break;
		case 2: cout << "Summer " << day; break;
		case 3: cout << "Autumm " << day; break;
		case 4: cout << "Winter " << day; break;
		}

		switch (day)
		{
		case 1: cout << "st\n"; break;
		case 2: cout << "nd\n"; break;
		case 3: cout << "rd\n"; break;
		default: cout << "th\n";
		}

		cout << "Funds: $" << player.funds << "\n\n";
		cout << "1.Tips\n2.Controls\n3.Legend\n4.Save Game\n5.Quit Game\n6.Back\n\n";
		cin >> det;
		system("cls");

		switch (det)
		{
		case '1':
			cout << "1.Plant seeds in the fiels and water daily.\n";
			cout << "2.After a few days crop will be fully grown.\n";
			cout << "3.Interact with fully grown crop to harvest it.\n";
			cout << "4.Place crops in the shipment box to earn money.\n";
			cout << "5.Buy more seeds in the shop and plant to your heart's content!\n";
			cout << "6.Farmiliarise yourself with the controls and the legend.\n\n";
			system("pause");
			system("cls");
			break;

		case '2':
			cout << "    Action     |    Key\n";
			cout << "------------------------------\n";
			cout << "     Move      |  w-a-s-d\n";
			cout << "  Switch Item  |    q/e\n";
			cout << "   Use Item    |     f\n";
			cout << "   Interact    |     c\n";
			cout << endl;
			system("pause");
			system("cls");
			break;

		case '3':
			cout << "           |  Symbol  |      Meaning\n";
			cout << "------------------------------------------\n";
			cout << " UNIVERSAL |    " << char(2) << "     |      Player\n";
			cout << "           |    #     |    Border Wall\n";
			cout << "------------------------------------------\n";
			cout << "   HOUSE   |    B     |        Bed\n";
			cout << "------------------------------------------\n";
			cout << "           |    H     |     House wall\n";
			cout << "           |    S     |    Shipment Box\n";
			cout << "   FARM    |    .     |     Seedling\n";
			cout << "           |    ~     |      Sprout\n";
			cout << "           |    o     |    Watered crop\n";
			cout << "           |    O     |   Full grown crop\n";
			cout << "------------------------------------------\n";
			cout << "   STREET  |    S     |     Seed Shop\n";
			cout << endl;
			system("pause");
			system("cls");
			break;

		case '4':
			saveGame();
			return;

		case '5':
			exit(EXIT_SUCCESS);

		case '6':
			return;
		}
	}
}

void shop()
{
	int num = 1;
	int cost = 10;

	while (street[player.y][player.x] == 4)
	{
		cout << "*********\n";
		cout << "SEED SHOP\n";
		cout << "*********\n";
		cout << "Welcome! Would you like to purchase seeds?\n";
		cout << "(Controls: v)\n\n";
		cout << "Funds: $" << player.funds << "\n";
		cout << "In bag: " << numSeeds << "\n\n";
		cout << "Seed x" << num;
		if (num < 10) cout << "  "; else cout << " ";
		cout << " ($" << cost << ")\n\n";
		cin >> det;

		switch (det)
		{
			case 'w':
				if (num < 99) num++;
				else num = 1;
				break;

			case 's':
				if (num > 1) num--;
				else num = 99;
				break;

			case 'a':
				num -= 10;
				if (num <= 0) num += 100;
				break;
			
			case 'd':
				num += 10;
				if (num > 99) num -= 100;
				break;

			case 'c':
				if (cost <= player.funds)
				{
					numSeeds += num;
					numCrops += num;
					player.funds -= cost;
					system("cls");
				}

				else
				{
					cout << "\nYou cant afford that many!\n\n";
					system("pause");
				}

				break;

			case 'b':
				cout << "\nThank you for shopping!\n\n";
				system("pause");
				return;

			case 'v':
				system("cls");
				cout << "  Control  |        Action\n";
				cout << "----------------------------------\n";
				cout << "    w/s    |   Change number by 1\n";
				cout << "    a/d    |   Change number by 1\n";
				cout << "     c     |    Confirm Purchase\n";
				cout << "     b     |      Leave Shop\n\n";
				system("pause");
		}

		system("cls");
		cost = num * 30;
	}
}

void checkPlayerPosition()
{
	switch (map)
	{
	case FARM:
		if (farm[player.y][player.x] == 2)
		{
			map = HOUSE;
			player.y = 8;
			player.x = 11;
		}

		else if (farm[player.y][player.x] == 20)
		{
			map = STREET;
			player.y = 4;
			player.x = 1;
		}
		break;

	case HOUSE:
		if (house[player.y][player.x] == 2)
		{
			map = FARM;
			player.y = 5;
			player.x = 34;
		}

		else while (player.x == 17 && player.y == 6)
		{
			system("cls");
			drawScreen();
			cout << "Would you like to go to sleep now?\n1.Yes\n2.No\n\n";
			cin >> det;

			if (det == '1')
			{
				system("cls");
				day++;

				for (int i = 1; i <= numCrops; i++)
				{
					if (crop[i].is_watered)
					{
						crop[i].days_watered++;
						crop[i].is_watered = false;

						if (crop[i].days_watered == 3)
							crop[i].is_grown = true;
					}
				}

				day_end = true;
				saveGame();
				player.x -= 2;
			}

			else if (det == '2')
				player.x -= 2;
		}

		break;

	case STREET:
		if (street[player.y][player.x] == 2)
		{
			map = FARM;
			player.y = 7;
			player.x = 38;
		}

		else if (street[player.y][player.x] == 4)
		{
			system("cls");
			shop();
			player.y++;
		}

		break;
	}

}

void updateDisplay()
{
	for (int i = 1; i <= numCrops; i++)
	{
		if (crop[i].is_grown)
			farm[crop[i].y][crop[i].x] = 18;

		else if (crop[i].is_watered)
			farm[crop[i].y][crop[i].x] = 16;
		else
			farm[crop[i].y][crop[i].x] = 12;

		if (crop[i].days_watered > 0 && !(crop[i].is_watered || crop[i].is_grown))
			farm[crop[i].y][crop[i].x] = 14;

		if (crop[i].is_harvested)
			farm[crop[i].y][crop[i].x] = 0;
	}

	if (day > 30)
		month++, day = 1;

	if (month > 4)
		year++, month = 1;
}

void saveGame()
{
	save.open("Saves/Saves");
	if (!save.is_open())
	{
		system("cls");
		cout << "Error opening save file...Closing game...\n\n";
		Sleep(2000);
		exit(EXIT_SUCCESS);
	}

	save << day << " " << month << " " << year << endl;
	save << numSeeds << " " << numCrops << " " << numFruits << endl;
	save << day_end << endl;
	save << player.x << " " << player.y << " " << player.funds << endl;

	switch (map)
	{
	case FARM: save << 0; break;
	case HOUSE: save << 1; break;
	case STREET: save << 2;
	}

	save << endl;

	switch (_item)
	{
	case SEED: save << 0; break;
	case WATERING_CAN: save << 1; break;
	case FRUIT: save << 2;
	}

	save << endl << itemNo << " " << numItems << endl;

	for (int i = 0; i < numItems; i++)
	{
		switch (bag[i])
		{
		case SEED: save << 0 << " "; break;
		case WATERING_CAN: save << 1 << " "; break;
		case FRUIT: save << 2 << " "; break;
		}
	}

	save << endl;

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 40; x++)
			save << farm[y][x] << " ";
		save << endl;
	}

	save << endl;

	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 20; x++)
			save << house[y][x] << " ";
		save << endl;
	}

	save << endl;

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 20; x++)
			save << street[y][x] << " ";
		save << endl;
	}

	save << endl;

	for (int i = 1; i <= numCrops; i++)
	{
		save << endl << crop[i].x << " " << crop[i].y << " " << crop[i].is_watered << " " << crop[i].days_watered;
		save << " " << crop[i].is_grown << " " << crop[i].is_harvested;
	}
	save << "\n\n";


	save.close();
	cout << "Game saved!\n\n";
	system("pause");
}

void loadGame()
{
	load >> day >> month >> year;
	load >> numSeeds >> numCrops >> numFruits >> day_end;
	load >> player.x >> player.y >> player.funds;

	load >> temp;
	switch (temp)
	{
	case 0: map = FARM; break;
	case 1: map = HOUSE; break;
	case 2: map = STREET;
	}

	load >> temp;
	switch (temp)
	{
	case 0: _item = SEED; break;
	case 1: _item = WATERING_CAN; break;
	case 2: _item = FRUIT;
	}

	load >> itemNo >> numItems;

	for (int i = 0; i < numItems; i++)
	{
		load >> temp;
		switch (temp)
		{
		case 0: bag[i] = SEED; break;
		case 1: bag[i] = WATERING_CAN; break;
		case 2: bag[i] = FRUIT; break;
		}
	}

	for (int y = 0; y < 20; y++)
		for (int x = 0; x < 40; x++)
			load >> farm[y][x];

	for (int y = 0; y < 10; y++)
		for (int x = 0; x < 20; x++)
			load >> house[y][x];

	for (int i = 1; i <= numCrops; i++)
		load >> crop[i].x >> crop[i].y >> crop[i].is_watered >> crop[i].days_watered >> crop[i].is_grown >> crop[i].is_harvested;

	if (day_end)
	{
		map = HOUSE;
		player.x -= 2;
		day_end = false;
	}
}