

#ifndef B1ActionInitialization_h
#define B1ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

namespace B1
{

  class ActionInitialization : public G4VUserActionInitialization
  {
  public:
    ActionInitialization();
    ~ActionInitialization() override;
    //* 为什么要设置两个成员函数？
    //* 1.BuildFroMaster函数是负责主线程调用，也就是说在内部的设置仅仅运行一次，而且对其他工作线程是通用的。
    //* 2.Build并行线程，负责对单独的线程做特定的动作。
    //* 从语法上来说，这里的const意味着该函数不能修改类（包括基类）的成员变量，只能读取。
    void BuildForMaster() const override;
    void Build() const override;
  };

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
