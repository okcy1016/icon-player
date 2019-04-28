#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Multiline_Output.H>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <thread>
#include <FL/fl_ask.H>

void openCircleModeWindow(Fl_Widget*, void*);
void openEllipseModeWindow(Fl_Widget*, void*);
void openSnakeGameModeWindow(Fl_Widget*, void*);
void saveDesktopLayout(Fl_Widget*, void*);
void restoreDesktopLayout(Fl_Widget*, void*);
void circleMode(Fl_Widget*, void*);
void stopMode(Fl_Widget*, void*);
void ellipseMode(Fl_Widget*, void*);
void snakeGameMode(Fl_Widget*, void*);

bool modeStarted = false;

int main(int argc, char **argv) {
  Fl_Window *mainWindow = new Fl_Window(350, 420, "桌面图标把玩器 v0.6");
  Fl_Box *helpLabelBox = new Fl_Box(20, 20, 320, 170, "欢迎使用桌面图标把玩器 v0.6！\n\n在这里，你可以让你的图标组成旋转动画，\n也可以体验图标贪吃蛇 ...\n\n在使用之前请在 \'桌面右键菜单->查看\' 中\n关闭 \'自动排列图标\' 和 \'将图标与网格对齐\'！\n此外，强烈建议先备份当前桌面布局！\n\n请选择游玩模式：");
  Fl_Button *mode0RoundButton = new Fl_Button(150, 210, 60, 30, "圆模式");
  Fl_Button *mode1RoundButton = new Fl_Button(140, 250, 80, 30, "椭圆模式");
  Fl_Button *mode2RoundButton = new Fl_Button(135, 290, 90, 30, "贪吃蛇模式");
  Fl_Button *mode3RoundButton = new Fl_Button(125, 330, 110, 30, "备份桌面布局");
  Fl_Button *mode4RoundButton = new Fl_Button(125, 370, 110, 30, "恢复桌面布局");
  mode0RoundButton->callback(openCircleModeWindow);
  mode1RoundButton->callback(openEllipseModeWindow);
  mode2RoundButton->callback(openSnakeGameModeWindow);
  mode3RoundButton->callback(saveDesktopLayout);
  mode4RoundButton->callback(restoreDesktopLayout);
  mainWindow->end();
  mainWindow->show(argc, argv);
  int returnValue = Fl::run();
  // terminate cli app
  std::system("taskkill /f /im icon_player_cli.exe");
  return returnValue;
}

void openCircleModeWindow(Fl_Widget* w, void* v) {
  Fl_Window *circleModeWindow = new Fl_Window(360, 380, "您正在游玩圆模式");
  Fl_Box *helpBox = new Fl_Box(40, 10, 280, 60, "下列值应根据以屏幕左上角为坐标原点，\n以向右与向下分别为 x、y 轴的正方向，\n单位为像素的直角坐标系来赋予。");
  Fl_Input *input0 = new Fl_Input(130, 80, 140, 30, "圆心 X 坐标");
  Fl_Input *input1 = new Fl_Input(130, 120, 140, 30, "圆心 Y 坐标");
  Fl_Input *input2 = new Fl_Input(130, 160, 140, 30, "圆的半径");
  Fl_Input *input3 = new Fl_Input(130, 200, 140, 30, "旋转速度");
  Fl_Box *helpBox1 = new Fl_Box(130, 225, 140, 30, "推荐值：（1~10）");
  Fl_Button *startButton = new Fl_Button(130, 260, 60, 30, "开始");
  Fl_Button *stopButton = new Fl_Button(210, 260, 60, 30, "终止");
  Fl_Multiline_Output* multilineOutputPointer0 = new Fl_Multiline_Output(40, 300, 280, 60);
  std::list<void*>* pointerToPassList = new std::list<void*>;
  pointerToPassList->push_back((void*)multilineOutputPointer0);
  pointerToPassList->push_back((void*)input0);
  pointerToPassList->push_back((void*)input1);
  pointerToPassList->push_back((void*)input2);
  pointerToPassList->push_back((void*)input3);
  startButton->callback(circleMode, pointerToPassList);
  stopButton->callback(stopMode);
  circleModeWindow->end();
  circleModeWindow->show();
}

void circleMode(Fl_Widget* w, void* v) {
  std::list<void*>::iterator iterator0 = ((std::list<void*>*)v)->begin();
  Fl_Multiline_Output* multilineOutputPointer0 = (Fl_Multiline_Output*)(*iterator0);
  iterator0++;
  std::stringstream cmdStringStream;
  cmdStringStream << "icon_player_cli.exe" << " ";
  cmdStringStream << "-mode 1";
  for (; iterator0 != ((std::list<void*>*)v)->end(); ++iterator0) {
    Fl_Input* inputPointer0 = (Fl_Input*)(*iterator0);
    cmdStringStream << " " << inputPointer0->value();
  }
  if (!modeStarted) {
    modeStarted = true;
    multilineOutputPointer0->value("");
    multilineOutputPointer0->insert("正在执行 ...\n");
    std::thread circleModeThread([] (std::string cmdString, Fl_Multiline_Output* multilineOutputPointer0) {
				   std::cout << cmdString << std::endl;
				   int returnValue = std::system(cmdString.c_str());
				   if (returnValue == -1) {
				     multilineOutputPointer0->insert("参数错误！\n");
				   } else if (returnValue == -2) {
				     multilineOutputPointer0->insert("请先备份桌面布局！\n");
				   }
				   multilineOutputPointer0->insert("已终止。\n");
				   modeStarted = false;
				 }, cmdStringStream.str(), multilineOutputPointer0);
    circleModeThread.detach();
  }
}

void stopMode(Fl_Widget* w, void* v) {
  std::system("taskkill /f /im icon_player_cli.exe");
  modeStarted = false;
}

void openEllipseModeWindow(Fl_Widget* w, void* v) {
  Fl_Window *ellipseModeWindowPointer = new Fl_Window(360, 510, "您正在游玩椭圆模式");
  Fl_Box *helpBox = new Fl_Box(40, 10, 280, 60, "下列值应根据以屏幕左上角为坐标原点，\n以向右与向下分别为 x、y 轴的正方向，\n单位为像素的直角坐标系来赋予。");
  Fl_Input *input0 = new Fl_Input(130, 80, 140, 30, "圆心 X 坐标");
  Fl_Input *input1 = new Fl_Input(130, 120, 140, 30, "圆心 Y 坐标");
  Fl_Input *input2 = new Fl_Input(130, 160, 140, 30, "椭圆的半长轴");
  Fl_Input *input3 = new Fl_Input(130, 200, 140, 30, "椭圆的半短轴");
  Fl_Input *input4 = new Fl_Input(130, 240, 140, 30, "旋转速度");
  Fl_Box *helpBox1 = new Fl_Box(130, 265, 140, 30, "推荐值：（1~10）");
  Fl_Input *input5 = new Fl_Input(130, 290, 140, 30, "自旋角度");
  Fl_Box *helpBox2 = new Fl_Box(130, 315, 140, 30, "（逆时针，0代表不自旋）");
  Fl_Button *startButton = new Fl_Button(130, 355, 60, 30, "开始");
  Fl_Button *stopButton = new Fl_Button(210, 355, 60, 30, "终止");
  Fl_Multiline_Output* multilineOutputPointer0 = new Fl_Multiline_Output(40, 400, 280, 100);
  std::list<void*>* pointerToPassListPointer = new std::list<void*>;
  pointerToPassListPointer->push_back((void*)multilineOutputPointer0);
  pointerToPassListPointer->push_back((void*)input0);
  pointerToPassListPointer->push_back((void*)input1);
  pointerToPassListPointer->push_back((void*)input2);
  pointerToPassListPointer->push_back((void*)input3);
  pointerToPassListPointer->push_back((void*)input4);
  pointerToPassListPointer->push_back((void*)input5);
  startButton->callback(ellipseMode, pointerToPassListPointer);
  stopButton->callback(stopMode);
  ellipseModeWindowPointer->end();
  ellipseModeWindowPointer->show();
}

void ellipseMode(Fl_Widget* w, void* v) {
  std::list<void*>::iterator iterator0 = ((std::list<void*>*)v)->begin();
  Fl_Multiline_Output* multilineOutputPointer0 = (Fl_Multiline_Output*)(*iterator0);
  iterator0++;
  std::stringstream cmdStringStream;
  cmdStringStream << "icon_player_cli.exe" << " ";
  cmdStringStream << "-mode 2";
  for (; iterator0 != ((std::list<void*>*)v)->end(); ++iterator0) {
    Fl_Input* inputPointer0 = (Fl_Input*)(*iterator0);
    cmdStringStream << " " << inputPointer0->value();
  }
  if (!modeStarted) {
    modeStarted = true;
    multilineOutputPointer0->value("");
    multilineOutputPointer0->insert("正在执行 ...\n");
    std::thread ellipseModeThread([] (std::string cmdString, Fl_Multiline_Output* multilineOutputPointer0) {
				   std::cout << cmdString << std::endl;
				   int returnValue = std::system(cmdString.c_str());
				   if (returnValue == -1) {
				     multilineOutputPointer0->insert("参数错误！\n");
				   } else if (returnValue == -2) {
				     multilineOutputPointer0->insert("请先备份桌面布局！\n");
				   }
				   multilineOutputPointer0->insert("已终止。\n");
				   modeStarted = false;
				 }, cmdStringStream.str(), multilineOutputPointer0);
    ellipseModeThread.detach();
  }
}

void openSnakeGameModeWindow(Fl_Widget* w, void* v) {
  Fl_Window* snakeModeWindowPointer = new Fl_Window(240, 250, "贪吃蛇模式");
  Fl_Input* inputPointer = new Fl_Input(80, 20, 120, 30, "难度等级");
  Fl_Box* helpBoxPointer = new Fl_Box(80, 50, 120, 30, "（1~10）");
  Fl_Button* startButton = new Fl_Button(80, 85, 60, 30, "开始");
  Fl_Button* stopButton = new Fl_Button(150, 85, 60, 30, "终止");
  Fl_Multiline_Output* multilineOutputPointer0 = new Fl_Multiline_Output(20, 130, 200, 100);
  std::list<void*>* pointerToPassListPointer = new std::list<void*>;
  pointerToPassListPointer->push_back((void*)multilineOutputPointer0);
  pointerToPassListPointer->push_back((void*)inputPointer);
  startButton->callback(snakeGameMode, pointerToPassListPointer);
  stopButton->callback(stopMode);
  snakeModeWindowPointer->end();
  snakeModeWindowPointer->show();
}

void snakeGameMode(Fl_Widget* w, void* v) {
  std::list<void*>::iterator iterator0 = ((std::list<void*>*)v)->begin();
  Fl_Multiline_Output* multilineOutputPointer0 = (Fl_Multiline_Output*)(*iterator0);
  iterator0++;
  std::stringstream cmdStringStream;
  cmdStringStream << "icon_player_cli.exe" << " ";
  cmdStringStream << "-mode 3";
  for (; iterator0 != ((std::list<void*>*)v)->end(); ++iterator0) {
    Fl_Input* inputPointer0 = (Fl_Input*)(*iterator0);
    cmdStringStream << " " << inputPointer0->value();
  }
  if (!modeStarted) {
    modeStarted = true;
    multilineOutputPointer0->value("");
    multilineOutputPointer0->insert("正在执行 ...\n");
    std::thread snakeGameModeThread([] (std::string cmdString, Fl_Multiline_Output* multilineOutputPointer0) {
				   std::cout << cmdString << std::endl;
				   int returnValue = std::system(cmdString.c_str());
				   if (returnValue == -1) {
				     multilineOutputPointer0->insert("参数错误！\n");
				   } else if (returnValue == -2) {
				     multilineOutputPointer0->insert("请先备份桌面布局！\n");
				   } else if (returnValue == -3) {
				     multilineOutputPointer0->insert("桌面图标数过少！\n");
				   }
				   multilineOutputPointer0->insert("已终止。\n");
				   modeStarted = false;
				 }, cmdStringStream.str(), multilineOutputPointer0);
    snakeGameModeThread.detach();
  }
}

void saveDesktopLayout(Fl_Widget* w, void* v) {
  std::system("icon_player_cli.exe -mode 4");
  fl_alert("已备份。");
}

void restoreDesktopLayout(Fl_Widget* w, void* v) {
  std::system("icon_player_cli.exe -mode 5");
  fl_alert("已恢复桌面布局。");
}
