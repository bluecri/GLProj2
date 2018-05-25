#include "stdafx.h"

#include "Canvas.h"
#include "Box.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "./src/Control/InputManager.h"
#include "./src/window.h"


std::vector<Canvas*> Canvas::preMadeCanvasVec;


// set focusBox that was clicked.

void Canvas::transferKeyInputToFocusBox(long long inputKey)
{
	// If inputKey == mouse click --> find focus box
	if (GInputManager->controlCheck(inputKey, ENUM_BEHAVIOR::CLICK_L_ONCE))
	{
		double xPos, yPos;
		glfwGetCursorPos(GWindow->_pWindow, &xPos, &yPos);

		printf_s("[LOG] mouse click %lf %lf \n", xPos, yPos);

		refindFocusBoxWithClick(static_cast<int>(xPos), static_cast<int>(yPos));
	}

	if (_focusBox == nullptr)
		return;

	_focusBox->eventKeyInput(inputKey);

}

void Canvas::transferKeyInputToFocusBox(std::string & inputStr)
{
	if (_focusBox == nullptr)
		return;

	_focusBox->eventKeyInput(inputStr);

}

// set focusBox that was clicked.
void Canvas::refindFocusBoxWithClick(int x, int y)
{
	for (auto elem : _boxMap)
	{
		if (elem.second->IsInBox(x, y))
		{
			_focusBox = elem.second->getBoxWithCoord(x, y);
			_focusBox->eventMouseClick();
			return;
		}
	}
}

void Canvas::addBox(Box * box)
{
	_boxMap.insert(std::make_pair(box->_name, box));
}

void Canvas::setBRender(bool bRender)
{
	for (auto elem : _boxMap)
	{
		elem.second->setAllBRender(bRender);
	}
}

void Canvas::update(float deltaTime, float acc)
{
}

void Canvas::initPreMade()
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
		int boxHeightGap = 20;
		int boxWidthGap = 100;
		for (int i = 0; i < MENU_ROW_CNT; i++)
		{
			for (int k = 0; k < MENU_COL_CNT; k++)
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
		}
	}
	{
		// Canvas* gameCanvas = new Canvas();
	}
}
