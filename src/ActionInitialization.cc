

#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

namespace B1
{

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  ActionInitialization::ActionInitialization()
  {
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  ActionInitialization::~ActionInitialization()
  {
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //!  该代码的内容相对是固定的，就是调用RunAction然后交给静态函数
  void ActionInitialization::BuildForMaster() const
  {
    //* 这里应该只是告诉runAction那个变量是我们跟踪的变量
    RunAction *runAction = new RunAction;
    SetUserAction(runAction);
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void ActionInitialization::Build() const
  {
    //* 设置初始粒子的种类，能量，位置等（也就是设置粒子源）
    SetUserAction(new PrimaryGeneratorAction);

    //* 设置模拟过程中的行为（我们设置的要收集沉积能量）
    //* 每个事件开始的时候要做的事情
    //* 每个事件结束之后要做的事情（这里设置的是计算）
    RunAction *runAction = new RunAction;
    SetUserAction(runAction); // 相当于启动这个行为

    //* 与每个模拟事件相关联(也就是模拟过程要做的事情)
    EventAction *eventAction = new EventAction(runAction);
    SetUserAction(eventAction);
    //* 设置一个事件每次作用需要做的事情
    SetUserAction(new SteppingAction(eventAction));
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
