#pragma once

#include "stdafx.h"
//class Box;
//class TextBox;
//class ImageBox;

#include "TextBox.h"
#include "ImageBox.h"

#define LOBBY_CNT 10
#define ROOM_ROW_CNT 10
#define ROOM_COL_CNT 2
#define PLAYER_ROW_CNT 20
#define PLAYER_COL_CNT 8

enum ENUM_PREMADE_CANVAS
{
	PREMADE_CANVAS_LOGIN,
	PREMADE_CANVAS_ACCOUNTCREATE,
	PREMADE_CANVAS_CHANNEL,
	PREMADE_CANVAS_LOBBY,
	PREMADE_CANVAS_ROOM,
	PREMADE_CANVAS_INGAMEMENU,
	PREMADE_CANVAS_INGAME
};

/* 같은 height의 box depth 미구현. 같은 box height(tree)의 box들은 겹치지 않는 것으로 한다. */
class Canvas
{
public:
	void transferKeyInputToFocusBox(int inputKey);
	// set focusBox that was clicked.
	void transferMouseClickToBox(int x, int y);
	void addBox(Box* box);
	void setBRender(bool bRender)
	{
		for (auto elem : _boxMap)
		{
			elem.second->setAllBRender(bRender);
		}
	}

	static void initPreMade()
	{
		Canvas* loginCanvas = new Canvas();
		preMadeCanvasVec.push_back(loginCanvas);

		Canvas* accountCreateCanvas = new Canvas();
		preMadeCanvasVec.push_back(accountCreateCanvas);

		Canvas* channelCanvas = new Canvas();
		preMadeCanvasVec.push_back(channelCanvas);

		Canvas* lobbyCanvas = new Canvas();
		preMadeCanvasVec.push_back(lobbyCanvas);

		Canvas* roomCanvas = new Canvas();
		preMadeCanvasVec.push_back(roomCanvas);

		Canvas* ingameMenuCanvas = new Canvas();
		preMadeCanvasVec.push_back(ingameMenuCanvas);

		Canvas* gameCanvas = new Canvas();
		preMadeCanvasVec.push_back(gameCanvas);

		// Login
		{
			ImageBox* IDImageBox = ImageBox::preMadeImageBoxesVec[ENUM_PREMADE_IMAGEBOX::PREMADE_IMAGEBOX_BLOCK];
			IDImageBox->setName(std::string("IDImageBox"));
			ImageBox* PWImageBox = ImageBox::preMadeImageBoxesVec[ENUM_PREMADE_IMAGEBOX::PREMADE_IMAGEBOX_BLOCK];
			PWImageBox->setName(std::string("PWImageBox"));
			IDImageBox->modifyImageBoxSize(100, 20);
			PWImageBox->modifyImageBoxSize(100, 20);
			IDImageBox->moveLTPosition(200, 200);
			PWImageBox->moveLTPosition(200, 400);

			TextBox* IDTextBox = TextBox::preMadeTextBoxesVec[ENUM_PREMADE_TEXTBOX::PREMADE_TEXTBOX_INPUT];
			IDImageBox->setName(std::string("IDTextBox"));
			TextBox* PWTextBox = TextBox::preMadeTextBoxesVec[ENUM_PREMADE_TEXTBOX::PREMADE_TEXTBOX_INPUT];
			IDImageBox->setName(std::string("PWTextBox"));
			IDTextBox->moveLTPosition(200, 200);
			PWTextBox->moveLTPosition(200, 400);

			IDImageBox->attachChildUIEntity(IDTextBox);
			PWImageBox->attachChildUIEntity(PWTextBox);

			loginCanvas->addBox(IDImageBox);
			loginCanvas->addBox(PWImageBox);
		}

		// account create
		{
			ImageBox* IDImageBox = ImageBox::preMadeImageBoxesVec[ENUM_PREMADE_IMAGEBOX::PREMADE_IMAGEBOX_BLOCK];
			IDImageBox->setName(std::string("IDImageBox"));
			ImageBox* PWImageBox = ImageBox::preMadeImageBoxesVec[ENUM_PREMADE_IMAGEBOX::PREMADE_IMAGEBOX_BLOCK];
			PWImageBox->setName(std::string("PWImageBox"));
			ImageBox* NicknameImageBox = ImageBox::preMadeImageBoxesVec[ENUM_PREMADE_IMAGEBOX::PREMADE_IMAGEBOX_BLOCK];
			NicknameImageBox->setName(std::string("NicknameImageBox"));
			IDImageBox->modifyImageBoxSize(100, 20);
			PWImageBox->modifyImageBoxSize(100, 20);
			NicknameImageBox->modifyImageBoxSize(100, 20);
			IDImageBox->moveLTPosition(200, 200);
			PWImageBox->moveLTPosition(200, 400);
			NicknameImageBox->moveLTPosition(200, 600);

			TextBox* IDTextBox = TextBox::preMadeTextBoxesVec[ENUM_PREMADE_TEXTBOX::PREMADE_TEXTBOX_INPUT];
			IDTextBox->setName(std::string("IDTextBox"));
			TextBox* PWTextBox = TextBox::preMadeTextBoxesVec[ENUM_PREMADE_TEXTBOX::PREMADE_TEXTBOX_INPUT];
			PWTextBox->setName(std::string("PWTextBox"));
			TextBox* NickNameTextBox = TextBox::preMadeTextBoxesVec[ENUM_PREMADE_TEXTBOX::PREMADE_TEXTBOX_INPUT];
			NickNameTextBox->setName(std::string("NickNameTextBox"));
			IDTextBox->moveLTPosition(200, 200);
			PWTextBox->moveLTPosition(200, 400);
			NickNameTextBox->moveLTPosition(200, 600);

			IDImageBox->attachChildUIEntity(IDTextBox);
			PWImageBox->attachChildUIEntity(PWTextBox);
			NicknameImageBox->attachChildUIEntity(NickNameTextBox);

			accountCreateCanvas->addBox(IDImageBox);
			accountCreateCanvas->addBox(PWImageBox);
			accountCreateCanvas->addBox(NicknameImageBox);
		}

		{
			// channelCanvas
			int boxGap = 40;
			for (int i = 0; i < LOBBY_CNT; i++)
			{
				ImageBox* LobbyImageBox = ImageBox::preMadeImageBoxesVec[ENUM_PREMADE_IMAGEBOX::PREMADE_IMAGEBOX_IRON];
				LobbyImageBox->setName(std::string("LobbyImageBox") + std::to_string(i));
				LobbyImageBox->modifyImageBoxSize(100, 20);
				LobbyImageBox->moveLTPosition(200, 200 + boxGap * i);

				TextBox* LobbyTextBox = TextBox::preMadeTextBoxesVec[ENUM_PREMADE_TEXTBOX::PREMADE_TEXTBOX_INPUT];
				LobbyTextBox->setName(std::string("LobbyTextBox") + std::to_string(i));
				LobbyTextBox->moveLTPosition(200, 200 + boxGap * i);

				LobbyImageBox->attachChildUIEntity(LobbyTextBox);
				channelCanvas->addBox(LobbyImageBox);
				channelCanvas->addBox(LobbyTextBox);
			}
		}
		{
			// Canvas* lobbyCanvas = new Canvas();
			int boxHeightGap = 40;
			int boxWidthGap = 200;
			for (int i = 0; i < ROOM_ROW_CNT; i++)
			{
				for (int k = 0; k < ROOM_COL_CNT; k++)
				{
					ImageBox* RoomImageBox = ImageBox::preMadeImageBoxesVec[ENUM_PREMADE_IMAGEBOX::PREMADE_IMAGEBOX_IRON];
					RoomImageBox->setName(std::string("RoomImageBox") + std::to_string(i));
					RoomImageBox->modifyImageBoxSize(100, 20);
					RoomImageBox->moveLTPosition(200 * boxWidthGap * k, 200 + boxHeightGap * i);

					TextBox* RoomTextBox = TextBox::preMadeTextBoxesVec[ENUM_PREMADE_TEXTBOX::PREMADE_TEXTBOX_INPUT];
					RoomTextBox->setName(std::string("RoomTextBox") + std::to_string(i));
					RoomTextBox->moveLTPosition(200 * boxWidthGap * k, 200 + boxHeightGap * i);

					RoomImageBox->attachChildUIEntity(RoomTextBox);
					lobbyCanvas->addBox(RoomImageBox);
					lobbyCanvas->addBox(RoomTextBox);
				}
			}
		}
		{
			// Canvas* roomCanvas = new Canvas();
			int boxHeightGap = 20;
			int boxWidthGap = 100;
			for (int i = 0; i < PLAYER_ROW_CNT; i++)
			{
				for (int k = 0; k < PLAYER_COL_CNT; k++)
				{
					ImageBox* RoomImageBox = ImageBox::preMadeImageBoxesVec[ENUM_PREMADE_IMAGEBOX::PREMADE_IMAGEBOX_IRON];
					RoomImageBox->setName(std::string("RoomImageBox") + std::to_string(i));
					RoomImageBox->modifyImageBoxSize(100, 20);
					RoomImageBox->moveLTPosition(200 * boxWidthGap * k, 200 + boxHeightGap * i);

					TextBox* RoomTextBox = TextBox::preMadeTextBoxesVec[ENUM_PREMADE_TEXTBOX::PREMADE_TEXTBOX_INPUT];
					RoomTextBox->setName(std::string("RoomTextBox") + std::to_string(i));
					RoomTextBox->moveLTPosition(200 * boxWidthGap * k, 200 + boxHeightGap * i);

					RoomImageBox->attachChildUIEntity(RoomTextBox);
					roomCanvas->addBox(RoomImageBox);
					roomCanvas->addBox(RoomTextBox);
				}
				// 자신의 nickname을 누르면 ready & start. (확인은 texture 교체로)
			}
		}
		{
			// Canvas* ingameMenuCanvas = new Canvas();
			ImageBox* RoomImageBox = ImageBox::preMadeImageBoxesVec[ENUM_PREMADE_IMAGEBOX::PREMADE_IMAGEBOX_IRON];
			RoomImageBox->setName(std::string("RoomImageBox") + std::to_string(i));
			RoomImageBox->modifyImageBoxSize(100, 20);
			RoomImageBox->moveLTPosition(200 * boxWidthGap * k, 200 + boxHeightGap * i);

			TextBox* RoomTextBox = TextBox::preMadeTextBoxesVec[ENUM_PREMADE_TEXTBOX::PREMADE_TEXTBOX_INPUT];
			RoomTextBox->setName(std::string("RoomTextBox") + std::to_string(i));
			RoomTextBox->moveLTPosition(200 * boxWidthGap * k, 200 + boxHeightGap * i);

			RoomImageBox->attachChildUIEntity(RoomTextBox);
			ingameMenuCanvas->addBox(RoomImageBox);
			ingameMenuCanvas->addBox(RoomTextBox);
		}
		{
			// Canvas* gameCanvas = new Canvas();
		}
	}

public:
	std::map<std::string, Box*> _boxMap;
	Box * _focusBox = nullptr;

	static std::vector<Canvas*> preMadeCanvasVec;
	
};