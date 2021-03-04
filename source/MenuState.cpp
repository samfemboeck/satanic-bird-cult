#include "MenuState.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include <iostream>
#include <TGUI/TGUI.hpp>
#include "TimerManager.h"
#include "StateManager.h"
#include "ContentManager.h"

using namespace hfm;

void MenuState::init(StateManager* manager, std::shared_ptr<sf::RenderWindow> window) 
{
	State::init(manager, window);
    mWindow = window.get();

    mMenuBuffer = false;
    mMenuTimer = 0;

    mGui.loadWidgetsFromFile("../externals/TGUI-0.8/gui-builder/menu.txt");
    scaleGui(2);
}

void MenuState::exit()
{
    mGamepad1 = nullptr;
    mGamepad2 = nullptr;
    mWindow = nullptr;
    mMenuSwitch = true;
    mMenuBufferTimer = 2.0f;
    mMadeGamepad1 = false;
    mMadeGamepad2 = false;
    mMenuBuffer = false;
    mMenuTimer = 10;
    mGui.removeAllWidgets();
    InputManager::getInstance().clear();
    GameObjectManager::getInstance().clear();
}

void MenuState::update(sf::Time delta)
{
    mMenuBufferTimer -= delta.asSeconds();
    if (!mMenuBuffer && mMenuBufferTimer < 0)
    {
        mMenuBuffer = true;
    }

    mMenuTimer -= delta.asSeconds();
    if (mMenuTimer < 0)
    {
        mMenuTimer = 0.4f;
        mMenuSwitch = true;
    }

    // instance gamepad once, only if connection is available
    if (!mMadeGamepad1 && sf::Joystick::isConnected(0))
    {
        mGamepad1 = InputManager::getInstance().addGamepad();
        mMadeGamepad1 = true;
    }
    if (!mMadeGamepad2 && sf::Joystick::isConnected(1))
    {
        mGamepad2 = InputManager::getInstance().addGamepad();
        mMadeGamepad2 = true;
    }

    // ui menu controll
    tgui::Label::Ptr play = mGui.get<tgui::Label>("play");
    tgui::Label::Ptr info = mGui.get<tgui::Label>("info");
    tgui::Label::Ptr exit = mGui.get<tgui::Label>("exit");

    // show ui connection p1 p2
    tgui::Label::Ptr p1 = mGui.get<tgui::Label>("p1connect");
    tgui::Label::Ptr p2 = mGui.get<tgui::Label>("p2connect");

    if (sf::Joystick::isConnected(0) && sf::Joystick::isConnected(1))
    {
        p1->getRenderer()->setTextColor(tgui::Color(205, 8, 41, 255));
        p2->getRenderer()->setTextColor(tgui::Color(205, 8, 41, 255));
        p1->setText("P1 is connected");
        p2->setText("P2 is connected");
    }
    else if ((sf::Joystick::isConnected(0) && !sf::Joystick::isConnected(1)) 
        || (!sf::Joystick::isConnected(0) && sf::Joystick::isConnected(1)))
    {
        p1->getRenderer()->setTextColor(tgui::Color(205, 8, 41, 255));
        p2->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
        p1->setText("P1 is connected");
        p2->setText("P2 not connected");
    }
    else 
    {
        p1->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
        p2->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
        p1->setText("P1 not connected");
        p2->setText("P2 not connected");
    }

    // menu controlling
    float updateMenuControl = 0;

    if (sf::Joystick::isConnected(0) && mMenuSwitch)
    {
        updateMenuControl += mGamepad1->getAxis(Gamepad::Axis::LStickVertical);
    }
    if (sf::Joystick::isConnected(1) && mMenuSwitch)
    {
        updateMenuControl += mGamepad2->getAxis(Gamepad::Axis::LStickVertical);
    }

    if (mMenuSwitch)
    {
        if (updateMenuControl < -0.3f)
        {
            mMenuControl--;
            mMenuSwitch = false;
        }
        if (updateMenuControl > 0.3f)
        {
            mMenuControl++;
            mMenuSwitch = false;
        }

        if (mMenuControl == 4)
        {
            mMenuControl = 1;
        }
        if (mMenuControl == 0)
        {
            mMenuControl = 3;
        }
    }

    if (mMenuControl == 1 && mMenuBuffer)
    {
        //play
        play->getRenderer()->setTextColor(tgui::Color(205, 8, 41, 255));
        info->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
        exit->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
        if (sf::Joystick::isConnected(0) && sf::Joystick::isConnected(1) 
            && (mGamepad1->isButtonDown(Gamepad::Button::South) || mGamepad2->isButtonDown(Gamepad::Button::South)))
        {
            mMenuBuffer = false;
            mManager->setState("main");
        }
    }
    else if (mMenuControl == 2 && mMenuBuffer)
    {
        // info
        play->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
        info->getRenderer()->setTextColor(tgui::Color(205, 8, 41, 255));
        exit->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
        if ((sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 0))
            || (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 1))
            || (sf::Joystick::isConnected(1) && sf::Joystick::isButtonPressed(1, 0))
            || (sf::Joystick::isConnected(1) && sf::Joystick::isButtonPressed(1, 1)))
        {
            mMenuBuffer = false;
            mManager->setState("info");
        }
    }
    else if (mMenuControl == 3 && mMenuBuffer)
    {
        // exit
        play->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
        info->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
        exit->getRenderer()->setTextColor(tgui::Color(205, 8, 41, 255));
        if ((sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 0))
            || (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 1))
            || (sf::Joystick::isConnected(1) && sf::Joystick::isButtonPressed(1, 0))
            || (sf::Joystick::isConnected(1) && sf::Joystick::isButtonPressed(1, 1)))
        {
            mMenuBuffer = false;
            mWindow->close();
        }
    }
}
