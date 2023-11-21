

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
// #include "Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{
  // add new units for dose
  //
  // 只是创建了单位
  const G4double milligray = 1.e-3*gray;// 毫格瑞
  const G4double microgray = 1.e-6*gray;// 微格瑞
  const G4double nanogray  = 1.e-9*gray;// 纳格瑞
  const G4double picogray  = 1.e-12*gray;// 皮格瑞
  
  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray);
  //* 这样的新的单位的用法
  //! G4double doseInMilligray = 5.0 * milligray;  // 表示5毫戈瑞的剂量
  // Register accumulable to the accumulable manager
  //
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();//* 创建可累积量的管理类
  //* 传入具体的累积量（具体怎么实现累计的暂不清楚）<---- 具体实现应该依赖后面的两个成员函数
  accumulableManager->RegisterAccumulable(fEdep);
  accumulableManager->RegisterAccumulable(fEdep2);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//* 模拟开始前的行为
void RunAction::BeginOfRunAction(const G4Run*)
{
  // inform the runManager to save random number seed
  //* 模拟过程中肯定有随机数产生
  //* 这里设置是否保存随机数种子。
  //* 不保存意味着每次运行的时候产生随机数的初始值都不一样，这样的模拟结果每次肯定不太相同
  //* 保存意味着随机数的初始值相同，每次的模拟结果都基本一样。
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values
  //* 重置我们关心的量，确保一开始的时候是0
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//* 所有粒子模拟结束后的行为
void RunAction::EndOfRunAction(const G4Run* run)
{
  //* 判断是否有模拟成功
  //* 可以开始模拟至少要有1个事件
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge accumulables
  //* 累计之前注册在管理类中的量（因为多线程计算，每个线程都注册一个值）
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Compute dose = total energy deposit in a run and its variance
  //* get函数获取对应的值
  G4double edep  = fEdep.GetValue();
  G4double edep2 = fEdep2.GetValue();
  //* 计算总能量的均方根（平均值）
  G4double rms = edep2 - edep*edep/nofEvents;
  if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;

  const DetectorConstruction* detConstruction
   = static_cast<const DetectorConstruction*>
     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4double mass = detConstruction->GetScoringVolume()->GetMass();
  G4double dose = edep/mass;
  G4double rmsDose = rms/mass;

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const PrimaryGeneratorAction* generatorAction
   = static_cast<const PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }

  // Print
  //
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }

  G4cout
     << G4endl
     << " The run consists of " << nofEvents << " "<< runCondition
     << G4endl
     << " Cumulated dose per run, in scoring volume : "
     << G4BestUnit(dose,"Dose") << " rms = " << G4BestUnit(rmsDose,"Dose")
     << G4endl
     << "------------------------------------------------------------"
     << G4endl
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//! 这里是具体实现累计量的函数
void RunAction::AddEdep(G4double edep)
{
  fEdep  += edep;
  fEdep2 += edep*edep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
