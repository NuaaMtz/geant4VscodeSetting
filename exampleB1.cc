//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file exampleB1.cc
/// \brief Main program of the B1 example


#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

using namespace B1;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char **argv)
{
  //* 形参argc表示命令行参数的个数
  //! 包括了程序名在内的所有参数
  //* 形参argv是存储命令行参数的字符串数组
  // Detect interactive mode (if no arguments) and define UI session
  //
  //* geant4的UI类。它提供了一个命令行界面，可以用来控制模拟的运行。
  G4UIExecutive *ui = nullptr; // 设置为空指针
  //* 如果命令行参数的个数为1，即只有程序名，那么就创建一个UI类
  //*  这是接口：G4UIExecutive(G4int argc, char** argv, const G4String& type = "");
  if (argc == 1)
  {
    ui = new G4UIExecutive(argc, argv);
  }

  // Optionally: choose a different Random engine...
  // G4Random::setTheEngine(new CLHEP::MTwistEngine);

  // use G4SteppingVerboseWithUnits
  //* 调用G4SteppingVerbose的静态成员函数
  //* 作用的令g4的输出数据只保留小数点后4位
  G4int precision = 4;
  G4SteppingVerbose::UseBestUnit(precision);

  // Construct the default run manager
  //
  //* 创建最大的管理类
  //* 两种产生运行管理对象的方法
  //* 1. G4RunManager* runManager = new G4RunManager;
  //* 2. auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
  //* 两种方法从使用上来说没区别。从g4程序来说灵活性更高，推荐使用第二种放。
  auto *runManager =
      G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  // Set mandatory initialization classes
  //
  // Detector construction
  //* 将探测器交给管理类
  runManager->SetUserInitialization(new DetectorConstruction());

  // Physics list
  //* 物理列表控制你的作用过程
  //* 比如说你关心高能电子和光子的相互作用，则用QGSP_BERT_HP（QBBC）对象，里面就包含了电子和光子可能会发生的物理过程
  //* 物理列表不需要自己写，默认就已经预先定义了足够多的物理列表，你只需要查询使用即可。
  G4VModularPhysicsList *physicsList = new QBBC;
  physicsList->SetVerboseLevel(1);// 控制信息输出详细程度
  runManager->SetUserInitialization(physicsList);

  // User action initialization
  //* 控制行为类
  //* 在前面已经创建好了整个模拟的实体区域和模拟过程的反应过程，但是我们还没告诉程序我们在该模拟过程中关心的事情
  //* 粒子作用可以有无数种状况，但是我们具体关心的是哪些过程，比如说我们只反应后粒子的能量变为1MeV的那些事件
  //* 这样输出的信息都是我们想要的信息，而非在一堆数据里面找`我们想要的。
  //! ActionInitialization类内部调用的RunAction才是我们真正需要写的内容
  //! ！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
  runManager->SetUserInitialization(new ActionInitialization());

  // Initialize visualization
  //* 打开可视化管理器（option）
  G4VisManager *visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager *UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if (!ui)
  {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  }
  else
  {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
