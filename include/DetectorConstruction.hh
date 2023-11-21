

#ifndef B1DetectorConstruction_h
#define B1DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

namespace B1
{
  //* 新建一个类DetctorConstruction，并且继承G4VUserDetectorConstruction的public部分
  //* 注意：作为g4框架使用者，我们使用的每一个类 **都要** 继承自g4框架中的对应类
  class DetectorConstruction : public G4VUserDetectorConstruction
  {
  public:
    // 构造函数,基函数的构造也为缺省。
    // G4VUserDetectorConstruction的构造函数不是虚函数，所以这里不用override
    // 如果不是构造而是普通的成员函数，不用虚函数定义意味着这个函数将能够在其派生类中通用地使用
    DetectorConstruction();
    // G4VUserDetectorConstruction的析构函数为虚函数，所以这里重写其函数的时候要用override
    ~DetectorConstruction() override; //
    // 作为用户接触的主要函数，需要重写的内容都在这里面了
    G4VPhysicalVolume *Construct() override;
    // 用来给外部函数获取我们关心区域的地方
    G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }

  protected:
    // 在construc中用来记录的我们关心的区域，只限于这个类内部使用，而留下了GetScoringVolume函数给外部访问
    G4LogicalVolume *fScoringVolume = nullptr;
  };

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
