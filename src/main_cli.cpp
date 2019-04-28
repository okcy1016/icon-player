#include <windows.h>
#include <commctrl.h>
#include <iostream>
#include <list>
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <sstream>


#define PI 3.14159265

void MoveIcon(HWND, int, POINT);
void CycleIconsOnCircle(POINT, int, int, HWND);
void CycleIconsOnEllipse(POINT, int, int, int, int, HWND);
int SnakeGameMode(HWND, int);
bool fileExists(const std::string);


int main(int argc, char* argv[]) {
  int modeIndex, circleCenterPointX, circleCenterPointY, circleRadius, circleSpinSpeed, ellipseSemiMajorLength, ellipseSemiShortLength, ellipseCenterPointX, ellipseCenterPointY, ellipseSpinSpeed, ellipseRotateAngle, snakeSpeedLevel;
  // exit when the correct parameters are not given or not enough
  if (argc < 3) {
    return -1;
  } else if (std::string(argv[1]) != "-mode" || (std::stoi(argv[2]) != 1 && std::stoi(argv[2]) != 2 && std::stoi(argv[2]) != 3 && std::stoi(argv[2]) != 4 && std::stoi(argv[2]) != 5)) {
    return -1;
  } else {
    modeIndex = std::stoi(argv[2]);
    switch (modeIndex) {
    case 1: {
      if (argc != 7) {
	return -1;
      }
      circleCenterPointX = std::stoi(argv[3]);
      circleCenterPointY = std::stoi(argv[4]);
      circleRadius = std::stoi(argv[5]);
      circleSpinSpeed = std::stoi(argv[6]);
      break;
    }
    case 2: {
      if (argc != 9) {
	return -1;
      }
      ellipseCenterPointX = std::stoi(argv[3]);
      ellipseCenterPointY = std::stoi(argv[4]);
      ellipseSemiMajorLength = std::stoi(argv[5]);
      ellipseSemiShortLength = std::stoi(argv[6]);
      ellipseSpinSpeed = std::stoi(argv[7]);
      ellipseRotateAngle = std::stoi(argv[8]);
      break;
    }
    case 3: {
      if (argc != 4) {
	return -1;
      }
      snakeSpeedLevel = std::stoi(argv[3]);
      break;
    }
    default: {
      break;
    }}
  }
  // get icon list view handler
  HWND desktopHandler = GetDesktopWindow();
  HWND hWorkerWHandler = NULL;
  HWND hShellViewWinHandler = NULL;
  HWND listViewHandler = NULL;
  HWND progManHandler = NULL;
  progManHandler = FindWindowExA(desktopHandler, NULL, "Progman", NULL);
  hShellViewWinHandler = FindWindowExA(progManHandler, NULL, "SHELLDLL_DefView", NULL);
  if (hShellViewWinHandler == NULL) {
    hWorkerWHandler = FindWindowExA(desktopHandler, hWorkerWHandler, "WorkerW", NULL);
    hShellViewWinHandler = FindWindowExA(hWorkerWHandler, NULL, "SHELLDLL_DefView", NULL);
    while (hShellViewWinHandler == NULL && hWorkerWHandler != NULL) {
      hWorkerWHandler = FindWindowExA(desktopHandler, hWorkerWHandler, "WorkerW", NULL);
      hShellViewWinHandler = FindWindowExA(hWorkerWHandler, NULL, "SHELLDLL_DefView", NULL);
    }
  }
  listViewHandler = FindWindowExA(hShellViewWinHandler, NULL, "SysListView32", NULL);

  // detect existence of desktop layout backup file
  if (!fileExists("utils/IconLayouts.ini") && modeIndex != 4) {
    return -2;
  }

  int returnValue = 0;
  switch (modeIndex) {
  case 1: {
    CycleIconsOnCircle(POINT{circleCenterPointX, circleCenterPointY}, circleRadius, circleSpinSpeed, listViewHandler);
    break;
  }
  case 2: {
    CycleIconsOnEllipse(POINT{ellipseCenterPointX, ellipseCenterPointY}, ellipseSemiMajorLength, ellipseSemiShortLength, ellipseSpinSpeed, ellipseRotateAngle, listViewHandler);
    break;
  }
  case 3: {
    returnValue = SnakeGameMode(listViewHandler, snakeSpeedLevel);
    break;
  }
  case 4: {
    // store current desktop layout using ReIcon
    ShellExecute(NULL, "open", "utils\\ReIcon_x64.exe", "/S", NULL, SW_HIDE);
    Sleep(200);
    break;
  }
  case 5: {
    // restore current desktop layout using ReIcon
    ShellExecute(NULL, "open", "utils\\ReIcon_x64.exe", "/R", NULL, SW_HIDE);
    Sleep(200);
    break;
  }
  default: {
    break;
  }}
  return returnValue;
}

void MoveIcon(HWND listViewHandler, int itemIndex, POINT itemPos) {
  ListView_SetItemPosition(listViewHandler, itemIndex, itemPos.x, itemPos.y);
}

void CycleIconsOnCircle(POINT circleCenterPoint, int circleRadius, int spinSpeed, HWND listViewHandler) {
  int iconNum = (int)SendMessage(listViewHandler, LVM_GETITEMCOUNT, 0, 0);
  // +1 to prevent non-closure icon circle
  float angleInterval = 360 / iconNum + 1;
  int xCoordinate, yCoordinate = 0;
  POINT iconCoordinate;
  std::list<POINT> iconCoordinateList;
  // initialize icon position
  for (int i=0; i<iconNum; i++) {
    xCoordinate = circleRadius * cos(i*angleInterval*PI/180);
    yCoordinate = circleRadius * sin(i*angleInterval*PI/180);
    iconCoordinate = {xCoordinate + circleCenterPoint.x, -1*yCoordinate + circleCenterPoint.y};
    iconCoordinateList.push_back(iconCoordinate);
  }
  int i = 0;
  for (POINT iconCoordinate : iconCoordinateList) {
    MoveIcon(listViewHandler, i, iconCoordinate);
    i++;
  }

  int angleOffset = 0;
  while (true) {
    // start continuous moving
    iconCoordinateList.clear();
    for (int i=0; i<iconNum; i++) {
      xCoordinate = circleRadius * cos(((angleOffset+i*angleInterval)*PI)/180);
      yCoordinate = circleRadius * sin(((angleOffset+i*angleInterval)*PI)/180);
      iconCoordinate = {xCoordinate + circleCenterPoint.x, -1*yCoordinate + circleCenterPoint.y};
      iconCoordinateList.push_back(iconCoordinate);
    }
    i = 0;
    for (POINT iconCoordinate : iconCoordinateList) {
      MoveIcon(listViewHandler, i, iconCoordinate);
      i++;
    }
    angleOffset += spinSpeed;
    if (angleOffset >= 360) {
      angleOffset -= 360;
    }
    Sleep(32);
  }
}

bool fileExists(const std::string filePath) {
  std::ifstream fileStream(filePath.c_str());
  return fileStream.good();
}

void CycleIconsOnEllipse(POINT ellipseCenterPoint, int ellipseSemiMajorLength, int ellipseSemiShortLength, int spinSpeed, int rotateAngle, HWND listViewHandler) {
  int iconNum = (int)SendMessage(listViewHandler, LVM_GETITEMCOUNT, 0, 0);
  // +1 to prevent non-closure icon circle
  float angleInterval = 360 / iconNum + 1;
  int xCoordinate, yCoordinate, xCoordinateRotated, yCoordinateRotated;
  POINT iconCoordinate;
  std::list<POINT> iconCoordinateList;
  // initialize icon position
  for (int i=0; i<iconNum; i++) {
    xCoordinate = ellipseSemiMajorLength * cos(i*angleInterval*PI/180);
    yCoordinate = ellipseSemiShortLength * sin(i*angleInterval*PI/180);
    // rotate
    xCoordinateRotated = xCoordinate * cos((rotateAngle*PI)/180) - yCoordinate * sin((rotateAngle*PI/180));
    yCoordinateRotated = xCoordinate * sin((rotateAngle*PI)/180) + yCoordinate * cos((rotateAngle*PI/180));
    iconCoordinate = {xCoordinateRotated + ellipseCenterPoint.x, -1*yCoordinateRotated + ellipseCenterPoint.y};
    iconCoordinateList.push_back(iconCoordinate);
  }
  int i = 0;
  for (POINT iconCoordinate : iconCoordinateList) {
    MoveIcon(listViewHandler, i, iconCoordinate);
    i++;
  }

  int angleOffset = 0;
  while (true) {
    // start continuous moving
    iconCoordinateList.clear();
    for (i=0; i<iconNum; i++) {
      xCoordinate = ellipseSemiMajorLength * cos(((angleOffset+i*angleInterval)*PI)/180);
      yCoordinate = ellipseSemiShortLength * sin(((angleOffset+i*angleInterval)*PI)/180);
      // rotate
      xCoordinateRotated = xCoordinate * cos((rotateAngle*PI)/180) - yCoordinate * sin((rotateAngle*PI/180));
      yCoordinateRotated = xCoordinate * sin((rotateAngle*PI)/180) + yCoordinate * cos((rotateAngle*PI/180));      
      iconCoordinate = {xCoordinateRotated + ellipseCenterPoint.x, -1*yCoordinateRotated + ellipseCenterPoint.y};
      iconCoordinateList.push_back(iconCoordinate);
    }
    i = 0;
    for (POINT iconCoordinate : iconCoordinateList) {
      MoveIcon(listViewHandler, i, iconCoordinate);
      i++;
    }
    angleOffset += spinSpeed;
    if (angleOffset >= 360) {
      angleOffset -= 360;
    }
    Sleep(32);
  }
}

int SnakeGameMode(HWND listViewHandler, int speedLevel) {
  // use RegisterHotkey for detect key press when focus is not on the cmd window
  RegisterHotKey(NULL, 0, MOD_NOREPEAT, VK_UP);
  RegisterHotKey(NULL, 1, MOD_NOREPEAT, VK_DOWN);
  RegisterHotKey(NULL, 2, MOD_NOREPEAT, VK_LEFT);
  RegisterHotKey(NULL, 3, MOD_NOREPEAT, VK_RIGHT);
  
  // hide all icons
  int iconNum = (int)SendMessage(listViewHandler, LVM_GETITEMCOUNT, 0, 0);
  int gridLength = 64;
  if (iconNum < 2) {
    return -3;
  }
  for (int i=0; i<iconNum; i++) {
    MoveIcon(listViewHandler, i, POINT{-100, -100});
  }
  // get desktop resolution
  HWND desktopHandler = GetDesktopWindow();
  RECT desktopRect;
  GetWindowRect(desktopHandler, &desktopRect);
  // divide grid area
  int pseudoXCoordinateMax = desktopRect.right / gridLength;
  int pseudoYCoordinateMax = desktopRect.bottom / gridLength;
  // generate head
  srand(time(NULL));
  POINT pseudoHeadCoordinate = {rand() % pseudoXCoordinateMax, rand() % pseudoYCoordinateMax};
  // generate food
  POINT pseudoFoodCoordinate = {rand() % pseudoXCoordinateMax, rand() % pseudoYCoordinateMax};
  // enter main loop
  bool foodEaten = false;
  char pressedKey;
  std::string headDirection = "right";
  int pseudoNewHeadXCoordinate = pseudoHeadCoordinate.x;
  int pseudoNewHeadYCoordinate = pseudoHeadCoordinate.y;
  // create snake node list
  std::list<POINT> snakeNodeList;
  std::list<POINT>::iterator snakeNodeListIterator;
  snakeNodeList.push_front(pseudoHeadCoordinate);
  // show head and food
  MoveIcon(listViewHandler, 0, POINT{gridLength*pseudoHeadCoordinate.x, gridLength*pseudoHeadCoordinate.y});
  MoveIcon(listViewHandler, snakeNodeList.size(), POINT{gridLength*pseudoFoodCoordinate.x, gridLength*pseudoFoodCoordinate.y});
  while (true) {
    if (foodEaten) {
      foodEaten = false;
      pseudoFoodCoordinate = {rand() % pseudoXCoordinateMax, rand() % pseudoYCoordinateMax};
      // prevent spawn food at snake body
      for (POINT point0 : snakeNodeList) {
	while (pseudoFoodCoordinate.x == point0.x && pseudoFoodCoordinate.y == point0.y) {
	  pseudoFoodCoordinate = {rand() % pseudoXCoordinateMax, rand() % pseudoYCoordinateMax};
	}
      }
      MoveIcon(listViewHandler, snakeNodeList.size(), POINT{gridLength*pseudoFoodCoordinate.x, gridLength*pseudoFoodCoordinate.y});
    }
    // detect key press
    MSG msg0 = {NULL};
    if (PeekMessage(&msg0, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg0);
      DispatchMessage(&msg0);
      if (msg0.message == WM_HOTKEY) {
	switch (msg0.wParam) {
	case 0: {
	  if (headDirection != "down") {
	    headDirection = "up";
	  }
	  break;
	}
	case 1: {
	  if (headDirection != "up") {
	    headDirection = "down";
	  }
	  break;
	}
	case 2: {
	  if (headDirection != "right") {
	    headDirection = "left";
	  }
	  break;
	}
	case 3: {
	  if (headDirection != "left") {
	    headDirection = "right";
	  }
	  break;
	}
	default: {
	  break;
	}}
      }
    }
    //// move snake
    if (headDirection == "up") {
      pseudoNewHeadYCoordinate -= 1;
    } else if (headDirection == "down") {
      pseudoNewHeadYCoordinate += 1;
    } else if (headDirection == "left") {
      pseudoNewHeadXCoordinate -= 1;
    } else {
      pseudoNewHeadXCoordinate += 1;
    }
    // when snake is out of screen, it will enter screen in the opposite boundary
    if (pseudoNewHeadXCoordinate > pseudoXCoordinateMax-1) {
      pseudoNewHeadXCoordinate = 0;
    }
    if (pseudoNewHeadXCoordinate < 0) {
      pseudoNewHeadXCoordinate = pseudoXCoordinateMax-1;
    }
    if (pseudoNewHeadYCoordinate > pseudoYCoordinateMax-1) {
      pseudoNewHeadYCoordinate = 0;
    }
    if (pseudoNewHeadYCoordinate < 0) {
      pseudoNewHeadYCoordinate = pseudoYCoordinateMax-1;
    }
    // eat food
    if (pseudoNewHeadXCoordinate == pseudoFoodCoordinate.x && pseudoNewHeadYCoordinate == pseudoFoodCoordinate.y) {
      foodEaten = true;
    }
    //// move body nodes
    // when haven't eaten food, discard the last node
    if (!foodEaten) {
      snakeNodeList.pop_back();
    }
    // add new node at beginning
    snakeNodeList.push_front(POINT{pseudoNewHeadXCoordinate, pseudoNewHeadYCoordinate});
    // detect body self-collision
    snakeNodeListIterator = snakeNodeList.begin();
    pseudoHeadCoordinate = *snakeNodeListIterator;
    snakeNodeListIterator++;
    for (; snakeNodeListIterator!=snakeNodeList.end(); ++snakeNodeListIterator) {
      if (snakeNodeListIterator->x == pseudoHeadCoordinate.x && snakeNodeListIterator->y == pseudoHeadCoordinate.y) {
	// negative number indicates lose
	return -1*(snakeNodeList.size()-1);
      }
    }
    // update snake
    int i = 0;
    for (POINT point0 : snakeNodeList) {
      MoveIcon(listViewHandler, i, POINT{gridLength*point0.x, gridLength*point0.y});
      i++;
    }
    Sleep(1000/speedLevel);
    // determine if the player wins
    if (snakeNodeList.size() == iconNum || snakeNodeList.size() == pseudoXCoordinateMax*pseudoYCoordinateMax) {
      return snakeNodeList.size()-1;
    }
  }
}
