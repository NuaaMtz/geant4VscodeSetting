

#ifndef B1RunAction_h
#define B1RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

class G4Run;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

namespace B1
{

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    ~RunAction() override;
    //* 这是设置开始模拟的时候要做的事情（每个粒子运行一次）
    void BeginOfRunAction(const G4Run*) override;
    //* 这里设置模拟结束的时候要做的事情（每个粒子运行一次）
    void   EndOfRunAction(const G4Run*) override;
    //* 对每个粒子追踪的值进行处理（这里只是将能量沉淀值乘方）
    void AddEdep (G4double edep);

  private:
    //* 这里记录沉积的能量对应的变量（对每个粒子都通用）
    G4Accumulable<G4double> fEdep = 0.;
    G4Accumulable<G4double> fEdep2 = 0.;
};

}

#endif

